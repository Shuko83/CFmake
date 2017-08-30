/*
    _SwGuiCssDialog.h
*/

#ifndef _SwGuiCssDialog_H
#define _SwGuiCssDialog_H

#pragma warning(disable: 4311 4312 4482 4615 ) //  disable warning QT
#include <QDialog>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "ui__SwGuiCssDialog.h"
#pragma warning(default: 4311 4312 4482 4615 )

/*CSS Highlighter*/
#include "_SwGuiCssHighlighter.h"

/*CSS Watcher*/
#include <QFileSystemWatcher>

class _SwGuiCssDialog : public QDialog
{
	Q_OBJECT

public:
    _SwGuiCssDialog(QDialog *parent = 0);
	~_SwGuiCssDialog();

	void setMyStyleSheet(const QString& styleSheet);

public slots:
	void showEvent(QShowEvent *event);
	void documentWasModified(bool isModified);
	void saveDoc();
	void closeEvent(QCloseEvent *event);
	void documentChangedFromOutside(QString path);
	void setFocusOnSearchLE();
	void searchNext();
	void searchOnCss();
	void stopSearch();

signals:
	void saveCSS();
private:
	Ui::_SwGuiCssDialogUI ui;
	QString _StyleSheet;
	bool _isModified;
	_SwGuiCssHighlighter* _CSSHighLighter;
	//QFileSystemWatcher *_CSSWatcher;
	QShortcut *_searchShortcut;
	QShortcut *_f3Shortcut;
	QString _allCSS;
	bool _isFinded;
	int _savedPos;
};

#endif
