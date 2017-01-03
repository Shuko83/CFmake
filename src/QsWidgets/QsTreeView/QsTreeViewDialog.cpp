#include "QsTreeViewDialog.h"

#include <QHeaderView>
#include <QAbstractItemModel>

//---------------------------------------------------------------------------------
QsTreeViewDialog::QsTreeViewDialog(QWidget* parent, QHeaderView* header)
	: QDialog(parent), _header(header)
{
	_ui.setupUi(this);

	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	connect(_ui.resetButton, &QAbstractButton::clicked, this, &QsTreeViewDialog::resetHiddenColumns);
}

//---------------------------------------------------------------------------------
QsTreeViewDialog::~QsTreeViewDialog()
{

}

void QsTreeViewDialog::done(int r)
{
	// si le model n'a pas changé entre temps
	if (r == QDialog::Accepted && _ui.columnList->count() == _header->count())
	{
		for (int i = 0; i < _ui.columnList->count(); i++)
		{
			Qt::CheckState check = _ui.columnList->item(i)->checkState();
			_header->setSectionHidden(i, check == Qt::Unchecked);
		}
	}
	  
	QDialog::done(r);
}

//---------------------------------------------------------------------------------
void QsTreeViewDialog::showEvent(QShowEvent *e)
{
	initHiddenColumns();
	QDialog::showEvent(e);
}

//---------------------------------------------------------------------------------
void QsTreeViewDialog::initHiddenColumns()
{
	_ui.columnList->clear();

	if (!_header)
		return;

	QAbstractItemModel* model = _header->model();

	for (int i = 0; i < _header->count(); ++i)
	{
		QString columnName = model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
		QListWidgetItem* item = new QListWidgetItem(columnName);

		Qt::CheckState check = _header->isSectionHidden(i) ? Qt::Unchecked : Qt::Checked;
		item->setCheckState(check);

		item->setData(Qt::UserRole, i);
		_ui.columnList->addItem(item);
	}
}

//---------------------------------------------------------------------------------
void QsTreeViewDialog::resetHiddenColumns()
{
	for (int i = 0; i < _ui.columnList->count(); i++)
	{
		_ui.columnList->item(i)->setCheckState(Qt::Checked);
	}
}
