#pragma once

#include <QDialog>
#include "ui_QsTreeViewDialog.h"

class QHeaderView;
class QAbstractItemModel;

class QsTreeViewDialog : public QDialog
{
public:
	explicit QsTreeViewDialog(QWidget* parent, QHeaderView* header);
	~QsTreeViewDialog();

	void done(int) override;

private:
	void showEvent(QShowEvent *) override;

private slots:
	void initHiddenColumns();
	void resetHiddenColumns();

private:
	Ui::QsTreeViewDialog _ui;

	QHeaderView * _header;
};
