/*
    _SwGuiCssDialog.cpp

	TODO
	
	Améliorer le principe de recherche et de surlignement
	Gérer le "dé higlighting"
	Faire en sorte que la fonction recherche soit une qtoolbar


*/

#include "_SwGuiCssDialog.h"
#include <iostream>

/*****************************************************************************/
_SwGuiCssDialog::_SwGuiCssDialog(QDialog *parent):QDialog(parent),_StyleSheet(""),_isFinded(false),_savedPos(0),_isModified(false)
{
    ui.setupUi(this);

	setWindowFlags(Qt::WindowMinimizeButtonHint| Qt::WindowMaximizeButtonHint| Qt::WindowCloseButtonHint);

	resize(1000,700);
	ui.B_Update->hide();

	_CSSHighLighter = new _SwGuiCssHighlighter(ui.textEdit->document());
	//_CSSWatcher = new QFileSystemWatcher(this);
	_searchShortcut = new QShortcut(QKeySequence(tr("Ctrl+f", "Search")),this);
	_f3Shortcut = new QShortcut(QKeySequence(tr("F3", "SearchF3")),this);

	connect(ui.textEdit, SIGNAL(undoAvailable(bool)),this, SLOT(documentWasModified(bool)));
	connect(ui.B_Save, SIGNAL(clicked()),this, SLOT(saveDoc()));
	connect(_searchShortcut,SIGNAL(activated()),this,SLOT(setFocusOnSearchLE()));
	connect(_f3Shortcut,SIGNAL(activated()),this,SLOT(searchNext()));
	connect(ui.PB_Next,SIGNAL(clicked()),this,SLOT(searchNext()));
	connect(ui.LE_Search, SIGNAL(returnPressed()),this, SLOT(searchOnCss()));
	connect(ui.PB_StopSearch, SIGNAL(clicked()),this, SLOT(stopSearch()));
	
	//connect(myCssWatcher,SIGNAL(fileChanged(QString)),this,SLOT(documentChangedFromOutside(QString)));
	
	//disabled checkB_Regx because not yet work 
	ui.checkB_Regx->hide();

	//Hide before use
	ui.GB_Search->hide();
}

/*****************************************************************************/
_SwGuiCssDialog::~_SwGuiCssDialog()
{	

	if(_CSSHighLighter)
	{
		delete _CSSHighLighter;
		_CSSHighLighter = NULL;
	}
	
// 	if(_CSSWatcher)
// 	{
// 		delete _CSSWatcher;
// 		_CSSWatcher = NULL;
// 	}
// 	
	if(_searchShortcut)
	{
		delete _searchShortcut;
		_searchShortcut = NULL;
	}
	
	if(_f3Shortcut)
	{
		delete _f3Shortcut;
		_f3Shortcut = NULL;
	}
	

}


/*****************************************************************************/
void _SwGuiCssDialog::setMyStyleSheet(const QString& styleSheet)
{
	_StyleSheet = styleSheet;

// 	if(_CSSWatcher->files().isEmpty())
// 	{
// 		_CSSWatcher->addPath(_StyleSheet);
// 	}
}


/*****************************************************************************/
void _SwGuiCssDialog::documentChangedFromOutside(QString path)
{
	QMessageBox msgBox;
	msgBox.setText(tr("Le fichier a été modifié à l'extérieur."));
	msgBox.setInformativeText(tr("Voulez vous le recharger ?"));
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();

	switch (ret) {
	   case QMessageBox::Ok:
		   {
			   QFile file(_StyleSheet);
			   if(file.open(QFile::ReadOnly))
			   {
				   QString styleSheet = QLatin1String(file.readAll());
				   ui.textEdit->setText(styleSheet);
			   }
			   file.close();
		   }
		   break;
	   default:
		   break;
	}

}

/*****************************************************************************/
void _SwGuiCssDialog::showEvent(QShowEvent *event)
{
	QFile file(_StyleSheet);
	if(file.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(file.readAll());
		ui.textEdit->setText(styleSheet);
		_CSSHighLighter->rehighlight();
	}
	else
	{
		ui.textEdit->append("<font color='red'>"+tr("Impossible d'ouvrir le fichier ")+_StyleSheet+"</font>");
	}
	file.close();
	
	event->accept();
	documentWasModified(false);
}

/*****************************************************************************/
void _SwGuiCssDialog::documentWasModified(bool isModified)
{
	if(isModified)
	{
		isModified=true;
		if(!windowTitle().contains(QRegularExpression("\\*")))
			this->setWindowTitle(windowTitle()+" *");
	}
	else
	{
		isModified=false;
		this->setWindowTitle(windowTitle().mid(0,windowTitle().indexOf("*")));
	}
	
}

/*****************************************************************************/
void _SwGuiCssDialog::closeEvent(QCloseEvent *event)
{
	if(_isModified)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Le fichier a été modifié."));
		msgBox.setInformativeText(tr("Voulez vous sauvegarder vos modifications ?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
	   case QMessageBox::Save:
		    saveDoc();
			ui.B_Update->click();
		   event->accept();
		   break;
	   case QMessageBox::Discard:
		    event->accept();
		   break;
	   case QMessageBox::Cancel:
		    event->ignore();
		   break;
	   default:
		   event->ignore();
		   break;
		}
	}
	else
	{
		ui.B_Update->click();	
		event->accept();
	}
}

/*****************************************************************************/
void _SwGuiCssDialog::saveDoc()
{
	QFile file(_StyleSheet);
	QTextStream _out(&file);
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		_out << ui.textEdit->toPlainText();
	}
	file.close();
	emit saveCSS();
	documentWasModified(false);
	ui.B_Update->click();	

}

/*****************************************************************************/
void _SwGuiCssDialog::setFocusOnSearchLE()
{
	ui.GB_Search->show();
	ui.LE_Search->setFocus(Qt::ShortcutFocusReason);
}


/*****************************************************************************/
void _SwGuiCssDialog::searchOnCss()
{

	_CSSHighLighter->isMatchCase = ui.checkB_Case->isChecked();
	_CSSHighLighter->isRegx = ui.checkB_Regx->isChecked();
	_CSSHighLighter->isMatchWord = ui.checkB_WO->isChecked();
	_CSSHighLighter->pattern = ui.LE_Search->text();
	_CSSHighLighter->rehighlight();
	//ui.LE_Search->setFocus();
	searchNext();
}

/*****************************************************************************/
void _SwGuiCssDialog::searchNext()
{
	ui.textEdit->setFocus();
	
	//Get all the css if can't find the text
	if(!_isFinded)
	{
		_allCSS = ui.textEdit->toPlainText();
		_savedPos = 0;
	}
	
	Qt::CaseSensitivity sensi = Qt::CaseInsensitive;

	if(ui.checkB_Case->isChecked())
		sensi = Qt::CaseSensitive;
	
	int pos=-1;
	
	if(ui.checkB_Regx->isChecked() || ui.checkB_WO->isChecked()) 
	{
		QString pat = ui.LE_Search->text();
		if(ui.checkB_WO->isChecked() && !ui.checkB_Regx->isChecked())
			pat = "\\b"+ui.LE_Search->text()+"\\b";
		pos= _allCSS.indexOf(QRegularExpression(pat));
	}
	else
	{
		pos= _allCSS.indexOf(ui.LE_Search->text(),0,sensi);
	}
	
	if(pos != -1) // that's mean we find the text
	{
		QTextCursor ttot = ui.textEdit->textCursor();
		ttot.setPosition(pos+_savedPos);
		ui.textEdit->setTextCursor(ttot);
		_isFinded = true;
		_allCSS = _allCSS.mid(pos+ui.LE_Search->text().size());
		_savedPos += pos+ui.LE_Search->text().size();


	}
	else
	{
		if(!_isFinded)
			QMessageBox::warning(this,"Text not found","\""+ui.LE_Search->text()+"\" not found");
		_isFinded = false;
	}
}

/*****************************************************************************/
void _SwGuiCssDialog::stopSearch()
{
	ui.GB_Search->hide();
	_CSSHighLighter->pattern = "";
	_CSSHighLighter->rehighlight();


}
