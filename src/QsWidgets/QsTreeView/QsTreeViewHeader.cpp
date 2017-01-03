#include "QsTreeViewHeader.h"

#include <QAbstractProxyModel>
#include <QLineEdit>
#include <QComboBox>

//---------------------------------------------------------------------------------
QsTreeViewHeader::QsTreeViewHeader(QWidget * parent /*= 0*/)
	: QHeaderView(Qt::Horizontal, parent), _comboModel(nullptr), _sectionsFilterable(false), _sectionsHideable(false)
{
	setDefaultAlignment(Qt::AlignTop);

	_comboModel = model();

	_settingsButton = new QPushButton(this);
	connect(_settingsButton, &QAbstractButton::clicked, this, &QsTreeViewHeader::cornerButtonClicked);
	//_settingsButton->setIcon(QIcon(":/QsLog/settingsBlack.png"));
	//_settingsButton->setFlat(true);
	updateSettingsButtonGeometry();

	connect(this, &QHeaderView::sectionResized, this, &QsTreeViewHeader::onSectionResized);
	connect(this, &QHeaderView::sectionMoved, this, &QsTreeViewHeader::onSectionMoved);
}

//---------------------------------------------------------------------------------
QsTreeViewHeader::~QsTreeViewHeader()
{
	qDeleteAll(_filters);
}

//---------------------------------------------------------------------------------
bool QsTreeViewHeader::sectionsFilterable()
{
	return _sectionsFilterable;
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::setSectionsFilterable(bool filterable)
{
	_sectionsFilterable = filterable;
	emit geometriesChanged();
	updateFiltersGeometry();
}

//---------------------------------------------------------------------------------
bool QsTreeViewHeader::sectionsHideable()
{
	return _sectionsHideable;
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::setSectionsHideable(bool hideable)
{
	_sectionsHideable = hideable;
	updateSettingsButtonGeometry();
}

//---------------------------------------------------------------------------------
bool QsTreeViewHeader::sectionComboFiltered(int logicalIndex)
{
	return _sectionsComboFiltered.at(logicalIndex);
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::setSectionComboFiltered(int logicalIndex, bool comboFiltered)
{
	_sectionsComboFiltered[logicalIndex] = comboFiltered;
	delete _filters[logicalIndex];
	if (comboFiltered)
	{
		QComboBox* cb = new QComboBox(this);
		cb->addItem("");
		_filters[logicalIndex] = cb;
		connect(cb, &QComboBox::currentTextChanged, this, &QsTreeViewHeader::onTextChanged);
	}
	else
	{
		QLineEdit* le = new QLineEdit(this);
		_filters[logicalIndex] = le;
		connect(le, &QLineEdit::textChanged, this, &QsTreeViewHeader::onTextChanged);
	}
	updateComboFilters();
	updateFiltersGeometry();
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::onSectionMoved(int, int, int)
{
	updateFiltersGeometry();
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::onSectionResized(int, int, int)
{
	updateFiltersGeometry();
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::onTextChanged(const QString & text)
{
	QWidget* w = dynamic_cast<QWidget*>(sender());
	int columnIndex = _filters.indexOf(w);
	emit columnFilterTextChanged(columnIndex, text);
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::initFilters()
{
	qDeleteAll(_filters);
	_filters.clear();
	_sectionsComboFiltered.clear();

	for (int i = 0; i < count(); i++)
	{
		QLineEdit* le = new QLineEdit(this);
		_filters.append(le);
		_sectionsComboFiltered.append(false);
		connect(le, &QLineEdit::textChanged, this, &QsTreeViewHeader::onTextChanged);
	}
	updateFiltersGeometry();
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::updateFiltersCount()
{
	if (model())
	{
		while (_filters.size() > model()->columnCount())
		{
			delete _filters.last();
			_filters.removeLast();
			_sectionsComboFiltered.removeLast();
		}
		while (_filters.size() < model()->columnCount())
		{
			QLineEdit* le = new QLineEdit(this);
			_filters.append(le);
			_sectionsComboFiltered.append(false);
			connect(le, &QLineEdit::textChanged, this, &QsTreeViewHeader::onTextChanged);
		}
	}
	else
	{
		_filters.clear();
	}
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::updateFiltersGeometry()
{
	if (_filters.size() != count())
		return updateFiltersCount();

	for (int i = 0; i < count(); i++)
	{
		if (!_sectionsFilterable || sectionSize(i) == 0)
		{
			QLineEdit* le = dynamic_cast<QLineEdit*>(_filters[i]);
			QComboBox* cb = dynamic_cast<QComboBox*>(_filters[i]);
			if (le)
			{
				le->clear();
			}
			if (cb)
			{
				cb->setCurrentText("");
			}
		}
		_filters[i]->setGeometry(sectionViewportPosition(i) + 2, QHeaderView::height() - 20 - 3, sectionSize(i) - 5, 20);
		_filters[i]->setVisible(isVisible() && _sectionsFilterable);
	}
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::updateComboFilters()
{
	for (int col = 0; col < count(); col++)
	{
		if (_sectionsComboFiltered[col])
		{
			QSet<QString> items;
			for (int i = 0; i < _comboModel->rowCount(); i++)
			{
				QString s = _comboModel->data(_comboModel->index(i, col)).toString();
				if (!s.isEmpty())
					items.insert(s);
			}

			QComboBox* cb = dynamic_cast<QComboBox*>(_filters[col]);
			for (int i = 1; i < cb->count(); i++)
			{
				if (items.contains(cb->itemText(i)))
					items.remove(cb->itemText(i));
				else
					cb->removeItem(i);
			}

			cb->addItems(items.toList());
		}
	}
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::updateSettingsButtonGeometry()
{
	_settingsButton->setVisible(_sectionsHideable);

	int size = QHeaderView::sizeHint().height();
	_settingsButton->setGeometry(viewport()->width() - size + 1, -1, size, size);

	_settingsButton->raise();
}

//---------------------------------------------------------------------------------
QSize QsTreeViewHeader::sizeHint() const
{
	if (!_sectionsFilterable)
		return QHeaderView::sizeHint();
	else
		return QSize(QHeaderView::sizeHint().width(), QHeaderView::sizeHint().height() + 22);
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::setModel(QAbstractItemModel *newModel)
{
	if (_comboModel)
	{
		disconnect(_comboModel, &QAbstractItemModel::rowsInserted, this, &QsTreeViewHeader::updateComboFilters);
		disconnect(_comboModel, &QAbstractItemModel::rowsRemoved, this, &QsTreeViewHeader::updateComboFilters);
	}
	if (model())
	{
		disconnect(model(), &QAbstractItemModel::modelReset, this, &QsTreeViewHeader::updateFiltersCount);
		disconnect(model(), &QAbstractItemModel::columnsInserted, this, &QsTreeViewHeader::updateFiltersCount);
		disconnect(model(), &QAbstractItemModel::columnsRemoved, this, &QsTreeViewHeader::updateFiltersCount);
	}

	QHeaderView::setModel(newModel);

	QAbstractProxyModel * proxyModel = dynamic_cast<QAbstractProxyModel *>(newModel);
	if (proxyModel)
		_comboModel = proxyModel->sourceModel();
	else
		_comboModel = newModel;

	if (_comboModel)
	{
		connect(_comboModel, &QAbstractItemModel::rowsInserted, this, &QsTreeViewHeader::updateComboFilters);
		connect(_comboModel, &QAbstractItemModel::rowsRemoved, this, &QsTreeViewHeader::updateComboFilters);
	}
	if (model())
	{
		connect(model(), &QAbstractItemModel::modelReset, this, &QsTreeViewHeader::updateFiltersCount);
		connect(model(), &QAbstractItemModel::columnsInserted, this, &QsTreeViewHeader::updateFiltersCount);
		connect(model(), &QAbstractItemModel::columnsRemoved, this, &QsTreeViewHeader::updateFiltersCount);
		initFilters();
	}
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::showEvent(QShowEvent *e)
{
	QHeaderView::showEvent(e);
	updateFiltersGeometry();
	updateSettingsButtonGeometry();
}

//---------------------------------------------------------------------------------
void QsTreeViewHeader::resizeEvent(QResizeEvent *e)
{
	QHeaderView::resizeEvent(e);
	updateSettingsButtonGeometry();
}
