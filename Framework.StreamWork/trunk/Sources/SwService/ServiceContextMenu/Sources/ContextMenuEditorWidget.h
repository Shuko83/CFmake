#ifndef CONTEXTMENUEDITORWIDGET_H
#define CONTEXTMENUEDITORWIDGET_H

#include <QDialog>
#include "ui_ContextMenuEditorWidget.h"

static const QString actionPrefix = "Action#";

static const QString separatorMark = "------------------";

static const QString submenuBeginMark = "SubMenuBegin#";
static const QString submenuEndMark = "SubMenuEnd";
static const QString subMenuIndent = "     ";

static const QString titleMark = "Title#";

class ContextMenuEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ContextMenuEditorWidget(QWidget *parent = 0);
	~ContextMenuEditorWidget();

	void addLineToTextEdit(const QString &text);
	QStringList getLines();
	
signals:
	void okPushed();
	void cancelPushed();
	void applyPushed();

private:
	Ui::ContextMenuEditorWidget ui;

	void insertSeparator();	
	void insertSubMenu();
	void insertTitle();
};

#endif // CONTEXTMENUEDITORWIDGET_H
