#include "ContextMenuEditorWidget.h"

#include <QDebug>

ContextMenuEditorWidget::ContextMenuEditorWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	connect(ui.pb_separator, &QPushButton::pressed, this, &ContextMenuEditorWidget::insertSeparator);
	connect(ui.pb_subMenu, &QPushButton::pressed, this, &ContextMenuEditorWidget::insertSubMenu);

	//Not coded yet, are categories useful?
	//connect(ui.pb_category, &QPushButton::pressed, this, &ContextMenuEditorWidget::insertCategory);
	ui.pb_category->setVisible(false);
}

ContextMenuEditorWidget::~ContextMenuEditorWidget()
{	
}

//---------------------------------------------------------------------------------
void ContextMenuEditorWidget::addLineToTextEdit(const QString &text)
{
	ui.textEdit->append(text);
}

//---------------------------------------------------------------------------------
QStringList ContextMenuEditorWidget::getLines()
{
	QString plainText = ui.textEdit->document()->toPlainText();

	QStringList lines = plainText.split('\n');

	return lines;
}

//---------------------------------------------------------------------------------
void ContextMenuEditorWidget::insertSeparator()
{
	QTextCursor textCursor = ui.textEdit->textCursor();

	textCursor.insertText(separatorMark);
}

//---------------------------------------------------------------------------------
void ContextMenuEditorWidget::insertCategory()
{
	QTextCursor textCursor = ui.textEdit->textCursor();

	textCursor.insertText(categoryBeginMark + "defaultCategory\n");
	textCursor.insertText(categoryEndMark);
}

//---------------------------------------------------------------------------------
void ContextMenuEditorWidget::insertSubMenu()
{
	QTextCursor textCursor = ui.textEdit->textCursor();

	QString testBegin = "/*";
	QString testEnd = "*/";

	if (!textCursor.hasSelection())
	{
		textCursor.insertText(submenuBeginMark + "defaultSubMenuName\n");
		textCursor.insertText(submenuEndMark);		
	}
	else
	{	
		//TODO indenter les action selectionnées quand on push "New Submenu"

		QString defaultSubMenuName = "defaultSubMenuName\n";
		int selectionEndPosition = textCursor.selectionEnd() + submenuBeginMark.length() + defaultSubMenuName.length();

		textCursor.setPosition(textCursor.selectionStart());
		textCursor.insertText(submenuBeginMark + "defaultSubMenuName\n");

		textCursor.setPosition(selectionEndPosition);
		textCursor.insertText("\n" + submenuEndMark);
	}
}
