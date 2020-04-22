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
#include "ISwServiceConfiguration.h"
#include "ISwServiceShortcuts.h"
#include "SwMacros.h"
#include "GlobalConf.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

static int nbWindows = 0;

#define MAX_HISTORY_FILE 4
/** @brief qmap */
static QMap<SwComponent_Class *, MainWindow *> _editors;

//---------------------------------------------------------------------------------
MainWindow::MainWindow(QString streamFile)
	: QMainWindow()
	, _streamControler(nullptr)
	, _streamSourceOpener(nullptr)
{
	setWindowTitle("StreamWorkEditor");
	setObjectName("StreamWorkEditor");
	setTabPosition(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea, QTabWidget::North);
	setMinimumSize(400, 300);
	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
	nbWindows++;
	_isMasterWindow = false;

	//Definition du wizard
	//_wizard=new SwWizardFrontEnd(this);

	QSettings settings;

	_history = settings.value("history").toStringList();

	//Acces Stream
	_streamMenu = menuBar()->addMenu("Streams");
	_streamMenu->addAction("New", this, SLOT(onNewStream()), Qt::CTRL + Qt::Key_N);
	_streamMenu->addAction("Load", this, SLOT(onLoadStream()), Qt::CTRL + Qt::Key_L);
	_streamMenu->addAction("Save", this, SLOT(onSaveStream()), Qt::CTRL + Qt::Key_S);
	_streamMenu->addAction("Save As", this, SLOT(onSaveAsStream()));
	_streamMenu->addSeparator();
	_streamMenu->addAction("New Editor", this, SLOT(onNewWindow()));
	_streamMenu->addAction("Print", this, SLOT(onPrint()), Qt::CTRL + Qt::Key_P);
	_streamMenu->addAction("SwGenerator", this, SLOT(onLaunchSwGenerator()), Qt::CTRL + Qt::Key_G);
	_streamMenu->addSeparator();

	for (int i = 0; i < MAX_HISTORY_FILE; i++)
	{
		QAction *action = new QAction(this);
		action->setText("");
		action->setData("");
		action->setVisible(false);
		action->setShortcut(Qt::CTRL + Qt::Key_1 + i);
		_streamMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(onLoadStreamf()));
		_listOfActions.append(action);
		_streamMenu->addAction(action);

	}

	if (_history.count() != 0)
	{
		int index = 1;
		for (QString absF : _history)
		{
			QFileInfo file(absF);
			if (file.exists())
			{
				_listOfActions.at(_history.indexOf(absF))->setText(QString::number(index) + ") " + file.fileName());
				_listOfActions.at(_history.indexOf(absF))->setData(file.absoluteFilePath());
				_listOfActions.at(_history.indexOf(absF))->setVisible(true);
				index++;
			}
		}
	}
	//_streamMenu->addAction(&_wizard->GetAction());
	_streamMenu->addSeparator();
	_streamMenu->addAction("Quit", this, SLOT(onQuit()), Qt::CTRL + Qt::Key_Q);

	//Acces Edition
	QMenu * editionMenu = menuBar()->addMenu("Edition");
	editionMenu->addAction("Arrange", this, SLOT(onRearrange()));

	//Acces about
	QMenu * aboutMenu = menuBar()->addMenu("About");
	aboutMenu->addAction("Plugins List", this, SLOT(onDisplayPluginList()));

	QMenu * optionMenu = menuBar()->addMenu("Options");
	QAction * acc = optionMenu->addAction("Link Spline");
	acc->setCheckable(true);
	if (GlobalConf::instance()->isSplineLink())
		acc->setChecked(true);
	else
		acc->setChecked(false);

	connect(acc, &QAction::toggled, this, [acc, this]() {
		GlobalConf::instance()->setSplineLink(acc->isChecked());
		for (ComponentGraphicItem* item : this->_streamControler->getScene()->getAllComponentGraphicItems())
			item->updateAttributs();
	});

	//Log
	menuBar()->addAction("Show Log", LogView::getInstance(), SLOT(show()));

	//Definition du widget central
	_streamView = new StreamView(this);
	setCentralWidget(_streamView);

	//Ajout dock widget
	QDockWidget *pdock = new QDockWidget(tr("Plugins"), this);
	pdock->setObjectName("DockPlugins");
	pdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	pdock->setWidget(WidgetFactory::getInstance()->buildPluginsBankView(true));
	addDockWidget(Qt::RightDockWidgetArea, pdock);

	QDockWidget * stdock = new QDockWidget(tr("Stream Tree"), this);
	stdock->setObjectName("DockStreamTree");
	stdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_streamTreeModel = new QStreamTreeModel(this);
	QTreeView * treew = (QTreeView *)WidgetFactory::getInstance()->buildStreamTreeView(_streamTreeModel);
	stdock->setWidget(treew);
	connect(treew, SIGNAL(clicked(const QModelIndex &)), _streamTreeModel, SLOT(onSelectItem(const QModelIndex &)));
	addDockWidget(Qt::RightDockWidgetArea, stdock);

	tabifyDockWidget(pdock, stdock);

	QDockWidget * iadock = new QDockWidget(tr("Interest Areas"), this);
	iadock->setObjectName("DockInterestArea");
	iadock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tabifyDockWidget(stdock, iadock);
	treew = new QTreeView();
	treew->setAlternatingRowColors(true);
	_iaTreeModel = new QInterestAreaTreeModel(this);
	treew->setModel(_iaTreeModel);
	connect(treew, SIGNAL(clicked(const QModelIndex &)), _iaTreeModel, SLOT(onSelectItem(const QModelIndex &)));
	iadock->setWidget(treew);

	tabifyDockWidget(iadock, stdock);
	tabifyDockWidget(stdock, pdock);


	QDockWidget * propdock = new QDockWidget(tr("Component Properties"), this);
	propdock->setObjectName("DockComponentProperties");
	propdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	_propertyWidget = WidgetFactory::getInstance()->buildPropertiesWidget();
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
	navdock->setWidget(new ViewNavigator(_streamView, this));
	addDockWidget(Qt::RightDockWidgetArea, navdock);
	_streamView->show();

	_statusWidget = new QLineEdit(this);
	_statusWidget->setReadOnly(true);
	statusBar()->addPermanentWidget(_statusWidget, 100);
	_statusWidget->setText("Ready");

	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

	_streamControler = new StreamControler(_propertyWidget);
	_streamControler->setView(_streamView);

	QFileInfo fi(streamFile);
	if (!streamFile.isEmpty() && fi.exists())
	{
		_streamControler->loadStream(streamFile);
		setWindowTitle(fi.fileName() + " - " + fi.filePath());
	}
	else
	{
		setWindowTitle("StreamWorkEditor");
	}

	_streamTreeModel->setStreamControler(_streamControler);
	_iaTreeModel->setStreamControler(_streamControler);
	_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
}

//---------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
	_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
    
	delete _iaTreeModel;
	delete _streamControler;
    delete _streamView;
}


//---------------------------------------------------------------------------------
void MainWindow::onNewStream()
{
	// Clear old stream
	_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
	_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
	delete _streamControler;

	clearServices();

	// Init New Stream
	_streamControler = new StreamControler(_propertyWidget);
	_streamControler->setView(_streamView);
	_streamTreeModel->setStreamControler(_streamControler);
	_iaTreeModel->setStreamControler(_streamControler);
	_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	setWindowTitle("StreamWorkEditor");
	_editors.insert(_streamControler->getRootItem(), this);
	_streamSourceOpener = 0;
	_streamControler->getRootItem()->OnDestroy.iconnect(*this, &MainWindow::internalClose);
}

//---------------------------------------------------------------------------------
void MainWindow::onLoadStream()
{
	QSettings settings;
	QString currentDir = "";
	if (settings.contains("EditorDirectory"))
	{
		currentDir = settings.value("EditorDirectory").toString();
	}
	//Get filename
	QStringList files = QFileDialog::getOpenFileNames(
		NULL,
		"Select a stream to open",
		currentDir,
		"Stream (*.xml)");
	QStringList list = files;
	QStringList::Iterator it = list.begin();
	if (it != list.end())
	{
		QFileInfo fi(*it);
		settings.setValue("EditorDirectory", QVariant(fi.filePath()));
		
		if (_streamControler)
		{
			_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
			_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
			_streamTreeModel->setStreamControler(0);
			_iaTreeModel->setStreamControler(0);
			delete _streamControler;

			clearServices();
		}

		manageHistory(fi);

		settings.setValue("history", _history);
		_streamControler = new StreamControler(_propertyWidget);
		_streamControler->setView(_streamView);
		_streamControler->loadStream(*it);
		_streamTreeModel->setStreamControler(_streamControler);
		_iaTreeModel->setStreamControler(_streamControler);
		_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		setWindowTitle(fi.fileName() + " - " + fi.filePath());
		_editors.insert(_streamControler->getRootItem(), this);
		_streamSourceOpener = 0;
		_streamControler->getRootItem()->OnDestroy.iconnect(*this, &MainWindow::internalClose);

	}
}

//-----------------------------------------------------------------------
void MainWindow::onLoadStreamf()
{
	QAction * lsender = qobject_cast<QAction*>(sender());

	if (!lsender)
		return;

	QFileInfo file(lsender->data().toString());

	if (file.exists())
	{
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
		_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		_streamTreeModel->setStreamControler(0);
		_iaTreeModel->setStreamControler(0);
		delete _streamControler;

		clearServices();
		
		manageHistory(file);
		_streamControler = new StreamControler(_propertyWidget);
		_streamControler->setView(_streamView);
		_streamControler->loadStream(file.absoluteFilePath());
		_streamTreeModel->setStreamControler(_streamControler);
		_iaTreeModel->setStreamControler(_streamControler);
		_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
		setWindowTitle(file.fileName() + " - " + file.filePath());
		_editors.insert(_streamControler->getRootItem(), this);
		_streamSourceOpener = 0;
		_streamControler->getRootItem()->OnDestroy.iconnect(*this, &MainWindow::internalClose);
	}
}

//---------------------------------------------------------------------------------
void MainWindow::onLoadExistingStream(SwComponent_Class * aStream, QString path, SwComponent_Class * aStreamSource)
{
	QFileInfo fi(path);

	_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
	_streamControler->removeSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	_streamTreeModel->setStreamControler(0);
	_iaTreeModel->setStreamControler(0);
	delete _streamControler;
	
	manageHistory(fi);

	_streamControler = new StreamControler(_propertyWidget);
	_streamControler->setView(_streamView);
	_streamControler->loadExistingStream(path, aStream);
	_streamTreeModel->setStreamControler(_streamControler);
	_iaTreeModel->setStreamControler(_streamControler);
	_streamControler->addSelectionObserver(dynamic_cast<ISelectionObserver *>(this));
	setWindowTitle(fi.fileName() + " - " + fi.filePath());
	_editors.insert(_streamControler->getRootItem(), this);
	_streamSourceOpener = aStreamSource;
	_streamControler->getRootItem()->OnDestroy.iconnect(*this, &MainWindow::internalClose);

}

//---------------------------------------------------------------------------------
void MainWindow::onSaveStream()
{
	if (_streamControler->getStreamFileName().isEmpty())
	{
		onSaveAsStream();
	}
	else
	{
		GET_SW_EDITION_SERVICE->setEditorSavingInProgress(true);
		_streamControler->saveStream();
		GET_SW_EDITION_SERVICE->setEditorSavingInProgress(false);
	}
}

//---------------------------------------------------------------------------------
void MainWindow::onSaveAsStream()
{
	QSettings settings;
	QString currentDir = "";
	if (settings.contains("EditorDirectory"))
	{
		currentDir = settings.value("EditorDirectory").toString();
	}
	QString fileName = QFileDialog::getSaveFileName(NULL, "Save stream as",
													currentDir,
													"Stream (*.xml)");
	if (fileName.isEmpty())
		return;
	QFileInfo fi(fileName);
	manageHistory(fi);
	settings.setValue("EditorDirectory", QVariant(fi.filePath()));
	GET_SW_EDITION_SERVICE->setEditorSavingInProgress(true);
	_streamControler->saveStreamAs(fileName);
	GET_SW_EDITION_SERVICE->setEditorSavingInProgress(false);
	setWindowTitle(fi.fileName() + " - " + fi.filePath());
}

//---------------------------------------------------------------------------------
void MainWindow::onWizard()
{
}

//---------------------------------------------------------------------------------
void MainWindow::onQuit()
{
	if (_streamControler->getRootItem()->_getReferencesNb() > 0)
	{
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);
	}

	qApp->disconnect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(onQuit()));
	this->deleteLater();
	nbWindows--;
	if (nbWindows == 0)
		qApp->exit(0);
}

//---------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
	//Si on est le master, on ferme les autres editors avant de se fermer
	if (_isMasterWindow)
	{
		for (auto * main : _editors.values())
			main->close();
	}


	QSettings settings;
	if (_streamControler->getRootItem()->_getReferencesNb() > 0)
		_streamControler->getRootItem()->OnDestroy.idisconnect(*this, &MainWindow::internalClose);

	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	settings.setValue("history", _history);
	_editors.remove(_streamControler->getRootItem());

	event->accept();

	this->deleteLater();
	nbWindows--;
	if (nbWindows == 0)
		qApp->exit(0);

}

//---------------------------------------------------------------------------------
void MainWindow::onDisplayPluginList()
{
	_SwGuiPluginsFrontEnd * dialog = new _SwGuiPluginsFrontEnd(NULL);
	dialog->exec();
	delete dialog;
}

//---------------------------------------------------------------------------------
void MainWindow::onRearrange()
{
	QList<ComponentGraphicItem *> mainItems = _streamControler->getScene()->getAllComponentGraphicItems();
	Arranger arranger;
	arranger.arrangeConnectors(&mainItems);
}

//---------------------------------------------------------------------------------
void MainWindow::onNewWindow()
{
	MainWindow *window = new MainWindow(false);
	window->onNewStream();
	window->showMaximized();
}

//---------------------------------------------------------------------------------
void MainWindow::onPrint()
{

	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted)
	{
		QPainter painter(&printer);
		painter.setRenderHint(QPainter::Antialiasing);
		_streamControler->getScene()->render(&painter);
	}
}

//---------------------------------------------------------------------------------
void MainWindow::setSelection(QList<StreamWork::SwCore::SwComponent_Class *> & sel)
{
	if (sel.count() != 1)
	{
		emit selection(QString(""), QString(""));
		return;
	}

	_statusWidget->setText(sel[0]->GetFactoryComponentName());

	ISwService * service = sel[0]->QueryService(CG_SW_SERVICE_PLUGIN_OVERVIEW);
	if (service != NULL && dynamic_cast<ISwPluginOverview *>(service) != NULL)
	{
		ISwPluginOverview * poverview = dynamic_cast<ISwPluginOverview *>(service);
		QString texte = sel[0]->GetFactoryComponentName() + " [Factory: " + poverview->GetPath() + "/" + poverview->GetPluginName() + "] [Version:" + poverview->GetPluginVersion() + "]";
		_statusWidget->setText(texte);
		_statusWidget->repaint();

		emit selection(sel[0]->GetFactoryComponentName(), poverview->GetPluginName());
	}
}

//---------------------------------------------------------------------------------
QMap<SwComponent_Class *, MainWindow *> * MainWindow::getEditors()
{
	return &_editors;
}

//---------------------------------------------------------------------------------
void MainWindow::displayStreamSourceOpener()
{
	if (!_streamSourceOpener)
		return;

	MainWindow *window = _editors.value(_streamSourceOpener);
	if (window != 0)
	{
		if (window->isMinimized())
		{
			window->showMaximized();
		}
		window->activateWindow();
	}
}

//---------------------------------------------------------------------------------
void MainWindow::internalClose()
{
	close();
}

//---------------------------------------------------------------------------------
void MainWindow::focusInEvent(QFocusEvent * event)
{
	QMainWindow::focusInEvent(event);
	MenuManager::getInstance()->setControler(_streamControler);
}

//-------------------------------------------------------------------------
void MainWindow::onLaunchSwGenerator()
{
	if (!QFile::exists(qApp->applicationDirPath() + QDir::separator() + "libctemplated.dll") && !QFile::exists(qApp->applicationDirPath() + QDir::separator() + "libctemplated.dll"))
	{
		QMessageBox::critical(this, "Dependencies fails! ", "libctemplated.dll");
	}

	if (QFile::exists(qApp->applicationDirPath() + QDir::separator() + "SwGeneratord.exe"))
		QProcess::startDetached(qApp->applicationDirPath() + QDir::separator() + "SwGeneratord.exe");
	else if (QFile::exists(qApp->applicationDirPath() + QDir::separator() + "SwGenerator.exe"))
		QProcess::startDetached(qApp->applicationDirPath() + QDir::separator() + "SwGenerator.exe");
	else
		QMessageBox::critical(this, "SwGenerator Errors! ", "Unable to find the executable SwGenerator.exe");
}

//-----------------------------------------------------------------------
void MainWindow::manageHistory(QFileInfo fi)
{
	if (_history.count() >= MAX_HISTORY_FILE && !_history.contains(fi.absoluteFilePath()))
		_history.removeLast();

	_history.removeOne(fi.absoluteFilePath());

	_history.prepend(fi.absoluteFilePath());

	//on actualise la liste
	for (int i = 0; i < _history.count(); i++)
	{
		QFileInfo file(_history.at(i));
		_listOfActions.at(i)->setData(file.absoluteFilePath());
		_listOfActions.at(i)->setText(QString::number(i + 1) + ") " + file.fileName());
		_listOfActions.at(i)->setVisible(true);
	}
}

//-------------------------------------------------------------------------
void MainWindow::clearServices()
{
	//clear Service conf
	ISwServiceConfiguration *_serviceSaveConfiguration = dynamic_cast<ISwServiceConfiguration *>(SW_APP->QueryService(CG_SW_SERVICE_SAVECONFIGURATION));
	if (_serviceSaveConfiguration)
	{
		ISwAdminConfiguration *adminConfig = _serviceSaveConfiguration->getAdmin();

		if (adminConfig)
			adminConfig->clearConfService();
	}

	// clear Service shortcuts
	ISwServiceShortcuts *_serviceShortcuts = dynamic_cast<ISwServiceShortcuts *>(SW_APP->QueryService(CG_SW_SERVICE_SHORTCUTS));
	if (_serviceShortcuts)
	{
		_serviceShortcuts->clearShortcutsService();
	}
}

//-----------------------------------------------------------------------
void MainWindow::setIsMasterWindow(bool val)
{
	_isMasterWindow = val;
}

