/**
@file MainWindow.cpp
@brief Fenetre principale
@author F.Bighelli
 */

#include <QFileInfo>
#include <QSettings>
#include "MainWindow.h" 
#include "_SwGuiPluginsFrontEnd.h" 
#include "GraphicsResources.h" 
#include "Arranger.h" 
#include "WidgetFactory.h"
#include "ViewNavigator.h"

static int nbWindows=0;

/** @brief Constructor */
MainWindow::MainWindow():QMainWindow(),_streamControler(0) {
    setWindowTitle("StreamWorkEditor V2");
    setTabPosition(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea,QTabWidget::North);
    setMinimumSize(400,300);
    nbWindows++;

    //Definition du wizard
    _wizard=new SwWizardFrontEnd(this);

    //Acces Stream
    QMenu * streamMenu=menuBar()->addMenu("Streams");
    streamMenu->addAction("New",this,SLOT(onNewStream()),Qt::CTRL + Qt::Key_N);
    streamMenu->addAction("Load",this,SLOT(onLoadStream()),Qt::CTRL + Qt::Key_L);
    streamMenu->addAction("Save",this,SLOT(onSaveStream()),Qt::CTRL + Qt::Key_S);
    streamMenu->addAction("Save As",this,SLOT(onSaveAsStream()));
    streamMenu->addSeparator();
    streamMenu->addAction("New Editor",this,SLOT(onNewWindow()));
    streamMenu->addAction(&_wizard->GetAction());
    //streamMenu->addSeparator();
    //streamMenu->addAction("Quit",this,SLOT(onQuit()),Qt::CTRL + Qt::Key_Q);

    //Acces Edition
    QMenu * editionMenu=menuBar()->addMenu("Edition");
    editionMenu->addAction("Arrange",this,SLOT(onRearrange()));

    //Acces about
    QMenu * aboutMenu=menuBar()->addMenu("About");
    aboutMenu->addAction("Plugins List",this,SLOT(onDisplayPluginList()));

    //Definition du widget central
    _streamView=new StreamView(this);
    setCentralWidget(_streamView);

    //Ajout dock widget
    QDockWidget *pdock = new QDockWidget(tr("Plugins"), this);
    pdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pdock->setWidget(WidgetFactory::getInstance()->buildPluginsBankView(true));
    addDockWidget(Qt::RightDockWidgetArea, pdock);


   
    QDockWidget * stdock = new QDockWidget(tr("Stream Tree"), this);
    stdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _streamTreeModel=new QStreamTreeModel(this);
    QTreeView * treew=(QTreeView *)WidgetFactory::getInstance()->buildStreamTreeView(_streamTreeModel);
    stdock->setWidget(treew);
    connect(treew,SIGNAL(clicked ( const QModelIndex &  )),_streamTreeModel,SLOT(onSelectItem ( const QModelIndex &  )));
    addDockWidget(Qt::RightDockWidgetArea, stdock);

    tabifyDockWidget(pdock,stdock);

    QDockWidget * iadock = new QDockWidget(tr("Interest Areas"), this);
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
    propdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    _propertyWidget=WidgetFactory::getInstance()->buildPropertiesWidget();
    propdock->setWidget(_propertyWidget);
    addDockWidget(Qt::RightDockWidgetArea, propdock);


    QDockWidget * navdock = new QDockWidget(tr("Navigator"), this);
    navdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    navdock->setWidget(new ViewNavigator(_streamView,this));
    addDockWidget(Qt::RightDockWidgetArea, navdock);
}
/** @brief sur new stream */
void MainWindow::onNewStream() {
    _streamTreeModel->setStreamControler(0);
    _iaTreeModel->setStreamControler(0);
    if (_streamControler!=0) {
        delete _streamControler;
    }
    _streamControler=new StreamControler(_propertyWidget);
    _streamControler->setView(_streamView);
    _streamTreeModel->setStreamControler(_streamControler);
    _iaTreeModel->setStreamControler(_streamControler);
    setWindowTitle("StreamWorkEditor V2");
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
        if (_streamControler!=0) {
            _streamTreeModel->setStreamControler(0);
            _iaTreeModel->setStreamControler(0);
           delete _streamControler;
        }
        _streamControler=new StreamControler(_propertyWidget);
        _streamControler->setView(_streamView);
        _streamControler->loadStream(*it);
        _streamTreeModel->setStreamControler(_streamControler);
        _iaTreeModel->setStreamControler(_streamControler);
        setWindowTitle(fi.fileName()+ " - " + fi.filePath());
    }
}
/** @brief sur save stream */
void MainWindow::onSaveStream(){
    if (_streamControler->getStreamFileName().isEmpty()) {
        onSaveAsStream();
    } else {
        _streamControler->saveStream();
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
    settings.setValue("EditorDirectory",QVariant(fi.filePath()));
    _streamControler->saveStreamAs(fileName);
    setWindowTitle(fi.fileName()+ " - " + fi.filePath());
}
/** @brief sur wizard */
void MainWindow::onWizard(){
}
///** @brief sur quit */
//void MainWindow::onQuit(){
//    delete _streamControler;
//    qApp->disconnect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(onQuit()));
//    this->deleteLater();
//    nbWindows--;
//    if (nbWindows==0)
//        qApp->exit(0);
//}
/** @brief sur close event */
void MainWindow::closeEvent(QCloseEvent *event) {   
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
