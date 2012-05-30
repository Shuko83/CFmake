/**
@file MainWindow.cpp
@brief Fenetre principale
@author F.Bighelli
*/
#include <QPrinter>
#include <QPrintDialog>
#include <QFileInfo>
#include <QSettings>
#include "MainWindow.h" 
#include "_SwGuiPluginsFrontEnd.h" 
#include "GraphicsResources.h" 
#include "Arranger.h" 
#include "WidgetFactory.h"
#include "ViewNavigator.h"
#include "LogView.h"
#include "ISwPluginOverview.h"
#include "SwApplication.h"
#include "ISwEditionService.h"
#include "MenuManager.h"
#include <qDebug>

static int nbWindows=0;

#define MAX_HISTORY_FILE 4
/** @brief qmap */
static QMap<SwComponent_Class *,MainWindow *> _editors; 

/** @brief Constructor */
MainWindow::MainWindow():QMainWindow(),_streamControler(0) {
	_streamSourceOpener=0;
	setWindowTitle("StreamWorkEditor V2");
	setTabPosition(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea,QTabWidget::North);
	setMinimumSize(400,300);
	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
	nbWindows++;

	//Definition du wizard
	//_wizard=new SwWizardFrontEnd(this);
	
	QSettings settings;

	_history = settings.value("history").toStringList();

	//Acces Stream
	_streamMenu=menuBar()->addMenu("Streams");
	_streamMenu->addAction("New",this,SLOT(onNewStream()),Qt::CTRL + Qt::Key_N);
	_streamMenu->addAction("Load",this,SLOT(onLoadStream()),Qt::CTRL + Qt::Key_L);
	_streamMenu->addAction("Save",this,SLOT(onSaveStream()),Qt::CTRL + Qt::Key_S);
	_streamMenu->addAction("Save As",this,SLOT(onSaveAsStream()));
	_streamMenu->addSeparator();
	_streamMenu->addAction("New Editor",this,SLOT(onNewWindow()));
	_streamMenu->addAction("Print",this,SLOT(onPrint()),Qt::CTRL + Qt::Key_P);
	_streamMenu->addAction("SwGenerator",this,SLOT(onLaunchSwGenerator()),Qt::CTRL + Qt::Key_G);

	_streamMenu->addSeparator();
	for(int i = 0; i< MAX_HISTORY_FILE ; i++)
	{
		QAction *action = new QAction(this);
		action->setText("");
		action->setData("");
		action->setVisible(false);
		action->setShortcut(Qt::CTRL + Qt::Key_1+i);
		_streamMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(onLoadStreamf()));
		_listOfActions.append(action);
		_streamMenu->addAction(action);

	}
	
	if(_history.count() != 0)
	{
		int index = 1;
		foreach(QString absF, _history)
		{
			QFileInfo *test = new QFileInfo(absF);
			if(test->exists())
			{
				_listOfActions.at(_history.indexOf(absF))->setText(QString::number(index)+") "+test->fileName());
				_listOfActions.at(_history.indexOf(absF))->setData(test->absoluteFilePath());
				_listOfActions.at(_history.indexOf(absF))->setVisible(true);
				index++;
			}
		}
	}
	//_streamMenu->addAction(&_wizard->GetAction());
	_streamMenu->addSeparator();
	_streamMenu->addAction("Quit",this,SLOT(onQuit()),Qt::CTRL + Qt::Key_Q);

	//Acces Edition
	QMenu * editionMenu=menuBar()->addMenu("Edition");
	editionMenu->addAction("Arrange",this,SLOT(onRearrange()));

	//Acces about
	QMenu * aboutMenu=menuBar()->addMenu("About");
	aboutMenu->addAction("Plugins List",this,SLOT(onDisplayPluginList()));

	//Log
	menuBar()->addAction("Show Log",LogView::getInstance(),SLOT(show()));

	//Definition du widget central
	_streamView=new StreamView(this);
	setCentralWidget(_streamView);

	//Ajout dock widget
	QDockWidget *pdock = new QDockWidget(tr("Plugins"), this);
	pdock->setObjectName("DockPlugins");
	pdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	pdock->setWidget(WidgetFactory::getInstance()->buildPluginsBankView(true));
	addDockWidget(Qt::RightDockWidgetArea, pdock);

	QDockWidget *ddock = new QDockWidget(tr("Doc"), this);
	ddock->setObjectName("DockDoc");
	ddock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	QWidget * docWid = WidgetFactory::getInstance()->buildDocBlankView();
	connect(this,SIGNAL(selection(QString&,QString&)),docWid,SLOT(onSelection(QString&,QString&)));
	ddock->setWidget(docWid);
	addDockWidget(Qt::RightDockWidgetArea, ddock);

	QDockWidget * stdock = new QDockWidget(tr("Stream Tree"), this);
	stdock->setObjectName("DockStreamTree");
	stdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_streamTreeModel=new QStreamTreeModel(this);
	QTreeView * treew=(QTreeView *)WidgetFactory::getInstance()->buildStreamTreeView(_streamTreeModel);
	stdock->setWidget(treew);
	connect(treew,SIGNAL(clicked ( const QModelIndex &  )),_streamTreeModel,SLOT(onSelectItem ( const QModelIndex &  )));
	addDockWidget(Qt::RightDockWidgetArea, stdock);

	tabifyDockWidget(pdock,stdock);

	QDockWidget * iadock = new QDockWidget(tr("Interest Areas"), this);
	iadock->setObjectName("DockInterestArea");
	iadock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tabifyDockWidget(stdock,iadock);
	treew=new QTreeView();
	treew->setAlternatingRowColors(true);
	_iaTreeModel=new QInterestAreaTreeModel(this);
	treew->setModel(_iaTreeModel);
	connect(treew,SIGNAL(clicked ( const QModelIndex &  )),_iaTreeModel,SLOT(onSelectItem ( const QModelIndex &  )));
	iadock->setWidget(treew);

	tabifyDockWidget(iadock,stdock);
	tabifyDockWidget(stdock,pdock);


	QDockWidget * propdock = new QDockWidget(tr("Component Properties"), this);
	propdock->setObjectName("DockComponentProperties");
	propdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	_propertyWidget=WidgetFactory::getInstance()->buildPropertiesWidget();
	propdock->setWidget(_propertyWidget);
	addDockWidget(Qt::RightDockWidgetArea, propdock);


	QDockWidget * legendDock = new QDockWidget(tr("Color legend"), this);
	legendDock->setObjectName("DockColorLegend");
	legendDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	legendDock->setWidget(WidgetFactory::getInstance()->buildLegendView());
	addDockWidget(Qt::RightDockWidgetArea, legendDock);

	QDockWidget * navdock = new QDockWidget(tr("Navigator"), this);
	navdock->setObjectName("DockNavigator");
	navdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	navdock->setWidget(new ViewNavigator(_streamView,this));
	addDockWidget(Qt::RightDockWidgetArea, navdock);

	_statusWidget=new QLineEdit(this);
	_statusWidget->setReadOnly(true);
	statusBar()->addPermanentWidget(_statusWidget,100);
	_statusWidget->setText("Ready");

	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

    QStringList liste_arg=QCoreApplication::instance()->arguments();
    int nb_args=liste_arg.count();
    for(int i=1;i+1<nb_args;i++) {
        QString test=liste_arg[i];
        //aide
        if (liste_arg[i]=="-stream" ) {
            QFileInfo fi(liste_arg[i+1]);
            if (fi.exists()) {
                if (_streamControler!=0) {
                    _streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
                    _streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
                    _streamTreeModel->setStreamControler(0);
                    _iaTreeModel->setStreamControler(0);
                    delete _streamControler;
                }
                _streamControler=new StreamControler(_propertyWidget);
                _streamControler->setView(_streamView);
                _streamControler->loadStream(liste_arg[i+1]);
                _streamTreeModel->setStreamControler(_streamControler);
                _iaTreeModel->setStreamControler(_streamControler);
				_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
                setWindowTitle(fi.fileName()+ " - " + fi.filePath());
                _editors.insert(_streamControler->getRootItem(),this);
                _streamSourceOpener=0;
                _streamControler->getRootItem()->OnDestroy.iconnect(*this,&MainWindow::internalClose);
            }

        }
    }

}
/** @brief sur new stream */
void MainWindow::onNewStream() {
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
	if (_streamControler!=0) {
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
		delete _streamControler;
	}
	_streamControler=new StreamControler(_propertyWidget);
	_streamControler->setView(_streamView);
	_streamTreeModel->setStreamControler(_streamControler);
	_iaTreeModel->setStreamControler(_streamControler);
	_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	setWindowTitle("StreamWorkEditor V2");
	_editors.insert(_streamControler->getRootItem(),this);
	_streamSourceOpener=0;
	_streamControler->getRootItem()->OnDestroy.iconnect(*this,&MainWindow::internalClose);
}
/** @brief sur load stream */
void MainWindow::onLoadStream(){
	QSettings settings;
	QString currentDir="";
	if(settings.contains("EditorDirectory")) {
		currentDir=settings.value("EditorDirectory").toString();
	}
	//Get filename
	QStringList files = QFileDialog::getOpenFileNames(
		NULL,
		"Select a stream to open",
		currentDir,
		"Stream (*.xml)");
	QStringList list = files;
	QStringList::Iterator it = list.begin();
	if (it != list.end()) {
		QFileInfo fi(*it);
		settings.setValue("EditorDirectory",QVariant(fi.filePath()));
		if (_streamControler!=0) 
		{
			_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
			_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
			_streamTreeModel->setStreamControler(0);
			_iaTreeModel->setStreamControler(0);
			delete _streamControler;
		}

		manageHistory(fi);

		_streamControler=new StreamControler(_propertyWidget);
		_streamControler->setView(_streamView);
		_streamControler->loadStream(*it);
		_streamTreeModel->setStreamControler(_streamControler);
		_iaTreeModel->setStreamControler(_streamControler);
		_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		setWindowTitle(fi.fileName()+ " - " + fi.filePath());
		_editors.insert(_streamControler->getRootItem(),this);
		_streamSourceOpener=0;
		_streamControler->getRootItem()->OnDestroy.iconnect(*this,&MainWindow::internalClose);
	}
}

//-----------------------------------------------------------------------
void MainWindow::onLoadStreamf()
{
	QAction * lsender = qobject_cast<QAction*>(sender());

	if(!lsender)
		return;

	QFileInfo file(lsender->data().toString());

	if (file.exists()) 
	{
		if (_streamControler!=0) 
		{
			_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
			_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
			_streamTreeModel->setStreamControler(0);
			_iaTreeModel->setStreamControler(0);
			delete _streamControler;
		}
		manageHistory(file);
		_streamControler=new StreamControler(_propertyWidget);
		_streamControler->setView(_streamView);
		_streamControler->loadStream(file.absoluteFilePath());
		_streamTreeModel->setStreamControler(_streamControler);
		_iaTreeModel->setStreamControler(_streamControler);
		_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		setWindowTitle(file.fileName()+ " - " + file.filePath());
		_editors.insert(_streamControler->getRootItem(),this);
		_streamSourceOpener=0;
		_streamControler->getRootItem()->OnDestroy.iconnect(*this,&MainWindow::internalClose);
	}
}

/** @brief sur load stream existant */
void MainWindow::onLoadExistingStream(SwComponent_Class * aStream,QString path,SwComponent_Class * aStreamSource) {
	QFileInfo fi(path);

	if (_streamControler!=0) {
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
		_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		_streamTreeModel->setStreamControler(0);
		_iaTreeModel->setStreamControler(0);
		delete _streamControler;
	}
	manageHistory(fi);

	_streamControler=new StreamControler(_propertyWidget);
	_streamControler->setView(_streamView);
	_streamControler->loadExistingStream(path,aStream);
	_streamTreeModel->setStreamControler(_streamControler);
	_iaTreeModel->setStreamControler(_streamControler);
	_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	setWindowTitle(fi.fileName()+ " - " + fi.filePath());
	_editors.insert(_streamControler->getRootItem(),this);
	_streamSourceOpener=aStreamSource;
	_streamControler->getRootItem()->OnDestroy.iconnect(*this,&MainWindow::internalClose);

}

/** @brief sur save stream */
void MainWindow::onSaveStream(){
	if (_streamControler->getStreamFileName().isEmpty()) {
		onSaveAsStream();
	} else {
		GET_SW_EDITION_SERVICE->setEditorSavingInProgress(true);
		_streamControler->saveStream();
		GET_SW_EDITION_SERVICE->setEditorSavingInProgress(false);
	}
}
/** @brief sur save as stream */
void MainWindow::onSaveAsStream() {
	QSettings settings;
	QString currentDir="";
	if(settings.contains("EditorDirectory")) {
		currentDir=settings.value("EditorDirectory").toString();
	}
	QString fileName = QFileDialog::getSaveFileName(NULL,"Save stream as",
		currentDir,
		"Stream (*.xml)");
	if (fileName.isEmpty())
		return;
	QFileInfo fi(fileName);
	manageHistory(fi);
	settings.setValue("EditorDirectory",QVariant(fi.filePath()));
	GET_SW_EDITION_SERVICE->setEditorSavingInProgress(true);
	_streamControler->saveStreamAs(fileName);
	GET_SW_EDITION_SERVICE->setEditorSavingInProgress(false);
	setWindowTitle(fi.fileName()+ " - " + fi.filePath());
}
/** @brief sur wizard */
void MainWindow::onWizard(){
}
/** @brief sur quit */
void MainWindow::onQuit()
{
	if (_streamControler->getRootItem()->_getReferencesNb()>0) {
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
	}
	_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
	delete _streamControler;

	qApp->disconnect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(onQuit()));
	this->deleteLater();
	nbWindows--;
	if (nbWindows==0)
		qApp->exit(0);
}
/** @brief sur close event */
void MainWindow::closeEvent(QCloseEvent *event) {   
	QSettings settings;
	if (_streamControler->getRootItem()->_getReferencesNb()>0) {
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this,&MainWindow::internalClose);
	}
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	settings.setValue("history",_history);
	_editors.remove(_streamControler->getRootItem());
	_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
	delete _streamControler;
	event->accept();

	this->deleteLater();
	nbWindows--;
	if (nbWindows==0)
		qApp->exit(0);
}
/** @brief sur affichage de la liste des plugins */
void MainWindow::onDisplayPluginList() {
	_SwGuiPluginsFrontEnd * dialog=new _SwGuiPluginsFrontEnd(NULL);
	dialog->exec();
	delete dialog;
}
/** @brief rearrange */
void MainWindow::onRearrange() {
	QList<ComponentGraphicItem *> mainItems=_streamControler->getScene()->getAllComponentGraphicItems();
	Arranger arranger;
	arranger.arrangeConnectors(&mainItems);
}
/** @brief sur nouvelle fenetre */
void MainWindow::onNewWindow() {
	MainWindow *window=new MainWindow;
	//qApp->connect(qApp, SIGNAL(lastWindowClosed()), window, SLOT(onQuit()));
	window->onNewStream();
	window->showMaximized();
}
/** @brief sur print */
void MainWindow::onPrint() {

	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
		QPainter painter(&printer);
		painter.setRenderHint(QPainter::Antialiasing);
		_streamControler->getScene()->render(&painter);
	}
}
/** @brief sur selection */
void MainWindow::setSelection(QList<StreamWork::SwCore::SwComponent_Class *> & sel) 
{
	if (sel.count()!=1)
	{
		emit selection(QString(""),QString(""));
		return;
	}

	//qDebug(sel[0]->GetFactoryComponentName().toLatin1().data());

	_statusWidget->setText(sel[0]->GetFactoryComponentName());

	ISwService * service=sel[0]->QueryService(CG_SW_SERVICE_PLUGIN_OVERVIEW);
	if (service!=NULL && dynamic_cast<ISwPluginOverview *>(service)!=NULL) 
	{
		ISwPluginOverview * poverview=dynamic_cast<ISwPluginOverview *>(service);
		QString texte=sel[0]->GetFactoryComponentName()+ " [Factory: "+poverview->GetPath()+"/"+poverview->GetPluginName()+ "] [Version:"+ poverview->GetPluginVersion() +"]";
		_statusWidget->setText(texte);
		_statusWidget->repaint();

		emit selection(sel[0]->GetFactoryComponentName(),poverview->GetPluginName());
	}
}

/** @brief getEditors */
QMap<SwComponent_Class *,MainWindow *> * MainWindow::getEditors() {
	return & _editors;
}
/** @brief permet de voir l'editeur du stream a l'origine de l'ouverture de nous meme */
void MainWindow::displayStreamSourceOpener() {
	if (_streamSourceOpener==0) {
		return;
	}
	MainWindow *window=_editors.value(_streamSourceOpener);
	if(window!=0) {
		if (window->isMinimized()) {
			window->showMaximized();
		}
		window->activateWindow();
	}
}
/** @brief fermeture interne */
void MainWindow::internalClose() {
	close();
}
/** @brief sur close event */
void MainWindow::focusInEvent ( QFocusEvent * event ) {
	QMainWindow::focusInEvent(event);
	MenuManager::getInstance()->setControler(_streamControler);
}

//-------------------------------------------------------------------------
void MainWindow::onLaunchSwGenerator()
{
	if(!QFile::exists(qApp->applicationDirPath()+QDir::separator()+"libctemplated.dll") && !QFile::exists(qApp->applicationDirPath()+QDir::separator()+"libctemplated.dll"))
	{
		if(QFile::exists(qApp->applicationDirPath()+QDir::separator()+"../../../../import_dependencies.bat"))
		{
			QProcess testP;
			testP.setWorkingDirectory(qApp->applicationDirPath()+QDir::separator()+"../../../../");
			testP.start(qApp->applicationDirPath()+QDir::separator()+"../../../../import_dependencies.bat");
			testP.waitForFinished();
		}
		else
		{
			QMessageBox::critical(this,"Dependencies copy fails! ","Unable to launch script import_dependencies.bat.");
		}
	}

	if(QFile::exists(qApp->applicationDirPath()+QDir::separator()+"SwGeneratord.exe"))
		QProcess::startDetached(qApp->applicationDirPath()+QDir::separator()+"SwGeneratord.exe");
	else if(QFile::exists(qApp->applicationDirPath()+QDir::separator()+"SwGenerator.exe"))
		QProcess::startDetached(qApp->applicationDirPath()+QDir::separator()+"SwGenerator.exe");
	else
		QMessageBox::critical(this,"SwGenerator Errors! ","Unable to find the executable SwGenerator.exe");
}

//-----------------------------------------------------------------------
void MainWindow::manageHistory( QFileInfo fi )
{
	if(_history.count() >= MAX_HISTORY_FILE && !_history.contains(fi.absoluteFilePath()))
		_history.removeLast();

	if(!_history.contains(fi.absoluteFilePath()))
	{
		_history.prepend(fi.absoluteFilePath());
	}
	else
	{
		_history.swap(0,_history.indexOf(fi.absoluteFilePath()));
		
	}
	//on actualise la liste
	for(int i = 0; i < _history.count(); i++)
	{
		QFileInfo fi(_history.at(i));
		_listOfActions.at(i)->setData(fi.absoluteFilePath());
		_listOfActions.at(i)->setText(QString::number(i+1)+") "+fi.fileName());
		_listOfActions.at(i)->setVisible(true);
	}
}
