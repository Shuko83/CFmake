#ifndef CONTEXTMENUEDITORWIDGET_H
#define CONTEXTMENUEDITORWIDGET_H

#include <QDialog>
#include "ui_ContextMenuEditorWidget.h"

static const QString actionPrefix = "Action#";

static const QString separatorMark = "------------------";

static const QString submenuBeginMark = "SubMenuBegin#";
static const QString submenuEndMark = "SubMenuEnd";
static const QString subMenuIndent = "\t";

static const QString categoryBeginMark = "CategoryBegin#";
static const QString categoryEndMark = "CategoryEnd";

class ContextMenuEditorWidget : public QDialog
{
	Q_OBJECT

public:
	ContextMenuEditorWidget(QWidget *parent = 0);
	~ContextMenuEditorWidget();

	void addLineToTextEdit(const QString &text);
	QStringList getLines();

private:
	Ui::ContextMenuEditorWidget ui;

	void insertSeparator();
	void insertCategory();
	void insertSubMenu();
};

#endif // CONTEXTMENUEDITORWIDGET_H
