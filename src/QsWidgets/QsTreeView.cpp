#include "QsTreeView.h"

#include <QtGui>
#include "QItemSelectionProxyModel.h"
#include "QsTreeViewHeader.h"
#include "QsTreeViewProxy.h"
#include "QsTreeViewDialog.h"

#define SingleClicked AllEditTriggers + 1

//---------------------------------------------------------------------------------
QsTreeView::QsTreeView(QWidget *parent /*= 0*/)
	: QTreeView(parent), _columnDialog(nullptr), _selection(nullptr)
{
	setIndentation(0);
	setEditTriggers(NoEditTriggers);
 
 	_header = new QsTreeViewHeader;
 	_proxy = new QsTreeViewProxy;

 	connect(_header, &QsTreeViewHeader::columnFilterTextChanged, _proxy, &QsTreeViewProxy::setSectionFilter);
 	connect(_header, &QsTreeViewHeader::cornerButtonClicked, this, &QsTreeView::showColumnDialog);

 	setHeader(_header);
 	QTreeView::setModel(_proxy);

	connect(_proxy, &QAbstractItemModel::modelReset, this, &QsTreeView::onModelReset);
	connect(_proxy, &QAbstractItemModel::columnsInserted, this, &QsTreeView::onColumnsInserted);
	connect(_proxy, &QAbstractItemModel::columnsMoved, this, &QsTreeView::onColumnsMoved);
	connect(_proxy, &QAbstractItemModel::columnsRemoved, this, &QsTreeView::onColumnsRemoved);
}

//---------------------------------------------------------------------------------
QsTreeView::~QsTreeView()
{
 	delete _selection;
 	delete _proxy;
}

//---------------------------------------------------------------------------------
void QsTreeView::setSelectionModel(QItemSelectionModel * selectionModel)
{
#ifdef _DEBUG
	QLibrary qsCore("QsCored");
#else
	QLibrary qsCore("QsCore");
#endif

	//récupération dynamique afin de ne pas linker QtDesigner avec la bibliothèque
	typedef QItemSelectionProxyModel* (*SelectionConstructor) (QAbstractProxyModel*);
	SelectionConstructor createQItemSelectionProxyModel = (SelectionConstructor)qsCore.resolve("createQItemSelectionProxyModel");

	if (createQItemSelectionProxyModel)
	{
		// _selection = new QItemSelectionProxyModel(_proxy);
		_selection = createQItemSelectionProxyModel(_proxy);
		_selection->setSourceSelectionModel(selectionModel);
		QTreeView::setSelectionModel(_selection);
	}
	else
	{
		qWarning() << "Could not load dynamic library QsCore";
		QTreeView::setSelectionModel(selectionModel);
	}
}

//---------------------------------------------------------------------------------
void QsTreeView::setModel(QAbstractItemModel * model)
{
	_proxy->setSourceModel(model);
	QTreeView::setModel(_proxy);
}

//---------------------------------------------------------------------------------
QString QsTreeView::getClassCSS() const
{
	return _classCSS;
}

//---------------------------------------------------------------------------------
void QsTreeView::setClassCSS(QString val)
{
	_classCSS = val;
}

//---------------------------------------------------------------------------------
QAbstractItemView::EditTriggers QsTreeView::sectionEditTrigger(int section)
{
	return _editTriggers[section];
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionEditTrigger(int section, QAbstractItemView::EditTriggers trigger)
{
	_editTriggers[section] = trigger;
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionEditTriggerSingleClick(int section, bool triggerOnSingleClick)
{
	if (triggerOnSingleClick)
	{
		_editTriggers[section] |= SingleClicked;
	}
	else
	{
		_editTriggers[section] ^= SingleClicked;
	}
}

//---------------------------------------------------------------------------------
bool QsTreeView::sectionsMovable()
{
	return _header->sectionsMovable();
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionsMovable(bool movable)
{
	_header->setSectionsMovable(movable);
}

//---------------------------------------------------------------------------------
bool QsTreeView::sectionsFilterable()
{
	return _header->sectionsFilterable();
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionsFilterable(bool filterable)
{
	_header->setSectionsFilterable(filterable);
}

//---------------------------------------------------------------------------------
bool QsTreeView::sectionsHideable()
{
	return  _header->sectionsHideable();
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionsHideable(bool hideable)
{
	_header->setSectionsHideable(hideable);
}

//---------------------------------------------------------------------------------
bool QsTreeView::sectionComboFiltered(int index)
{
	return _header->sectionComboFiltered(index);
}

//---------------------------------------------------------------------------------
void QsTreeView::setSectionComboFiltered(int index, bool comboFiltered)
{
	_header->setSectionComboFiltered(index, comboFiltered);
}

//---------------------------------------------------------------------------------
void QsTreeView::hideEvent(QHideEvent * event)
{
	if (_columnDialog)
	{
		_columnDialog->reject();
	}

	QTreeView::hideEvent(event);
}

//---------------------------------------------------------------------------------
void QsTreeView::resizeEvent(QResizeEvent * event)
{
	if (_columnDialog)
		_columnDialog->resize(this->width() - 40, this->height() - 40);

	QTreeView::resizeEvent(event);
}

//---------------------------------------------------------------------------------
bool QsTreeView::edit(const QModelIndex & index, EditTrigger trigger, QEvent * event)
{
	if (!index.isValid())
		return QTreeView::edit(index, trigger, event);

	EditTriggers lastTriggers = editTriggers();

	int columnTriggers = _editTriggers[index.column()] ^ SingleClicked;

	setEditTriggers(static_cast<EditTriggers>(columnTriggers));

	bool retVal = QTreeView::edit(index, trigger, event);

	setEditTriggers(lastTriggers);

	return retVal;
}

//---------------------------------------------------------------------------------
void QsTreeView::showColumnDialog()
{
	QWidget::setEnabled(false);

	_columnDialog = new QsTreeViewDialog(this, _header);

	_columnDialog->resize(this->width() - 40, this->height() - 40);

	const QPoint global = this->mapToGlobal(rect().center());
	_columnDialog->move(global.x() - _columnDialog->width() / 2, global.y() - _columnDialog->height() / 2);

	connect(_columnDialog, &QDialog::finished, this, &QsTreeView::hideColumnDialog);

	_columnDialog->show();
}

//---------------------------------------------------------------------------------
void QsTreeView::hideColumnDialog()
{
	_columnDialog->deleteLater();
	_columnDialog = 0;
	QWidget::setEnabled(true);
}

//---------------------------------------------------------------------------------
void QsTreeView::onModelReset()
{
	while (_editTriggers.size() > model()->columnCount())
	{
		_editTriggers.removeLast();
	}
	while (_editTriggers.size() < model()->columnCount())
	{
		_editTriggers.append(QAbstractItemView::DoubleClicked);
	}
}

//---------------------------------------------------------------------------------
void QsTreeView::onColumnsInserted(const QModelIndex &, int first, int last)
{
	for (int i = first; i <= last; i++)
		_editTriggers.insert(i, QAbstractItemView::DoubleClicked);
}

//---------------------------------------------------------------------------------
void QsTreeView::onColumnsMoved(const QModelIndex &, int start, int end, const QModelIndex &, int column)
{
	for (int i = 0; i <= end - start; i++)
		_editTriggers.move(i + start, i + column);
}

//---------------------------------------------------------------------------------
void QsTreeView::onColumnsRemoved(const QModelIndex &, int first, int last)
{
	for (int i = first; i <= last; i++)
		_editTriggers.removeAt(i);
}

//---------------------------------------------------------------------------------
void QsTreeView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		QModelIndex index = indexAt(event->pos());
		if (index.isValid())
		{
			if (_editTriggers[index.column()] & SingleClicked)
			{
				QTreeView::edit(index);
			}
		}
	}
	QTreeView::mousePressEvent(event);
}
