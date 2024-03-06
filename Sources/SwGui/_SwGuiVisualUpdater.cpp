/**
@file _SwGuiVisualUpdater.cpp
@brief Update pour streamwork
@author F.Bighelli
 */

#include "_SwGuiVisualUpdater.h"
#include "Main/Plugin/ISwPluginsBank.h"
#include "Main/SwMacros.h"
#include "Main/SwApplication.h"

#define UPDATE_DIR "_update"

/** @brief Constructor */
_SwGuiVisualUpdater::_SwGuiVisualUpdater() {
	_handleHttpServer=0;
    _consumer_service=NULL;
    QFile page(":/SwGui/update.html");
    page.open(QIODevice::ReadOnly);
    _updateHtmlBody=page.readAll();
    page.close();
    QFile pagelock(":/SwGui/updatelock.html");
    pagelock.open(QIODevice::ReadOnly);
    _updateLockHtmlBody=pagelock.readAll();
    pagelock.close();
    QFile pageprogress(":/SwGui/updateprogress.html");
    pageprogress.open(QIODevice::ReadOnly);
    _updateInProcessHtmlBody=pageprogress.readAll();
    pageprogress.close();
    _isLocked=false;
    _updateDirectory=QCoreApplication::applicationDirPath()+QDir::separator()+UPDATE_DIR;
    QDir dir(_updateDirectory);
    if (!dir.exists()) {
        dir.mkpath(_updateDirectory);
    }
    _trayIcon=new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon(":/SwGui/waiting0.png"));
    _trayIcon->setVisible(false);
    _trayIconTimer=new QTimer(this);
    connect(_trayIconTimer,SIGNAL(timeout()),this,SLOT(onUpdateTrayIcon()));
    _trayIconCpt=0;
    _isUpdateLocked=false;
    _updateTimer=new QTimer(this);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(onUpdate()));
    _updateTimer->start(500);
}
/** @brief Destructeur */
_SwGuiVisualUpdater::~_SwGuiVisualUpdater(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());

    //Destruction des services
    delete _consumer_service;

    _consumer_service=NULL;
}
/** 
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void _SwGuiVisualUpdater::InitializeResources() throw(SwException) {
    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;

    //Enregistrement des services
    this->RegisterService(_consumer_service);

    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------
    //Importation de l'interface QWidget
    _consumer_service->RegisterConsumedInterface<ISwHttpServer>("HttpServer",&_handleHttpServer);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void _SwGuiVisualUpdater::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (_handleHttpServer!=0) {
        _handleHttpServer->unregisterHttpPart(this);
    }
}
/** @brief Apres changement de la disponibilité de l'interface */
void _SwGuiVisualUpdater::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (_handleHttpServer!=0) {
        _handleHttpServer->registerHttpPart(this);
    }

}

//---------------------------------------------------------------------
// Interface ISwHttpPart
//---------------------------------------------------------------------
/*! @brief Renvoie le path de la partie  */
QString  _SwGuiVisualUpdater::getPath() {
    return "/update";
}
/*! @brief process de la requete */
void _SwGuiVisualUpdater::processRequest(/*QHttpRequestHeader * request,*/
                            QMap<QString,QString> * parameters,
                            QByteArray * body,
                            QIODevice * device) 
{
    /*if (request->path().startsWith("/update")) {
        if (_isUpdateLocked) {
            QHttpResponseHeader httpreponse(200,"Ok",1,1);
            httpreponse.setContentType("text/html");
            httpreponse.setContentLength(_updateLockHtmlBody.size());
            device->write(_updateLockHtmlBody);
            return;
        }
        if (parameters->size()==1) {
            QMap<QString,QString>::iterator it=parameters->find("action"); 
            if (it!=parameters->end()) {
                if (!_isLocked && it.value()=="lock") {
                    lockOn();
                } 
                if (_isLocked && it.value()=="unlock") {
                    lockOff();
                }
                if (it.value()=="status") {
                    sendStatus(request,device);
                    return;
                }
            }
        }
        if (_isLocked && body->size()>0) {
            decodeData(body);
        }
        QHttpResponseHeader httpreponse(200,"Ok",1,1);
        httpreponse.setContentType("text/html");
        QByteArray bodyresponse;
        if (_isLocked) {
            bodyresponse=buildResponse().toUtf8();
            httpreponse.setContentLength(bodyresponse.size());
        } else {
            httpreponse.setContentLength(_updateHtmlBody.size());
        }
        device->write(httpreponse.toString().toUtf8());
        if (_isLocked) {
            device->write(bodyresponse);
        } else {
            device->write(_updateHtmlBody);
        }
        return;
    }*/

}
/*! @breif  sur mise a jour du tray icon */
void _SwGuiVisualUpdater::onUpdateTrayIcon() {
    _trayIconCpt++;
    if (_trayIconCpt>5) {
        _trayIconCpt=0;
    }
    QString iconame=QString(":/SwGui/waiting%1.png").arg(_trayIconCpt);
    _trayIcon->setIcon(QIcon(iconame));
}
/*! @brief lock */
void _SwGuiVisualUpdater::lockOn() {
    QFile file(_updateDirectory+QDir::separator()+"lock.lock");
    if (file.exists()) 
        return;
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.close();
    _trayIcon->setToolTip(QApplication::applicationName()+" download update in progress...");
    _isLocked=true;
    _trayIconTimer->start(100);
    _trayIcon->setVisible(true);
}
/*! @brief unlock */
void _SwGuiVisualUpdater::lockOff() {
    QFile file(_updateDirectory+QDir::separator()+"lock.lock");
    if (file.exists()) {
        file.remove();
    }
    _isLocked=false;
    _trayIcon->setVisible(false);
    _loadedFiles.clear();
}
/*! @brief build response */
QString _SwGuiVisualUpdater::buildResponse() {
    if (_loadedFiles.count()==0) {
        return QString(_updateLockHtmlBody).arg("");
    } else {
        QString listText="<ul>\n";
        for(int i=0;i<_loadedFiles.count();i++) {
            listText+="<li>";
            listText+=_loadedFiles[i];
            listText+="</li>";
        }
        listText+="</ul>\n";
        return QString(_updateLockHtmlBody).arg(listText);
    }
}
 /*! @brief decode des donnees au format multipart/form-data */
void _SwGuiVisualUpdater::decodeData(QByteArray * data) {
    int index=data->indexOf("\r\n");
    int next_index;
    if (index<0)
        return;
    index+=2;
    //Definition du separateur
    QByteArray separator;
    for(int i=0;i<index;i++) {
        separator.push_back((*data)[i]);
    }
    //Extraction des parties
    next_index=data->indexOf(separator,index);
    while (next_index>0) {
        QByteArray part=data->mid(index,next_index-index);
        decodePart(&part);
        index=next_index+separator.size();
        next_index=data->indexOf(separator,index);
    }
}
/*! @brief decode des donnees ai format form-data */
void _SwGuiVisualUpdater::decodePart(QByteArray * data) {
    QByteArray real_data;
    //Recuperation de l'entete
    int index=data->indexOf("\r\n\r\n");
    if (index<0)
        return;
    index+=4;
    QString header=QString(data->mid(0,index));
    //Pour debug
    QString s="<<<";
    s+=header;
    s+=">>>";
    qDebug("part header:\r\n%s",s.toLatin1().data());
    //Fin pour debug
    //Extraction données d'entete
    QMap<QString,QString> headerData;
    extractPartHeader(&headerData,header);
    //Enregistrement d'un fichier si c'est un fichier
    QString filename=headerData.value("filename");
    if (!filename.isEmpty()) {
        real_data=data->mid(index,data->size()-index-2);
        saveFile(filename,headerData.value("content-type"),real_data);
    } else {
        qDebug("Part is not a file");
    }

}
/*! @brief decode des donnees d'entete au format form-data*/
void _SwGuiVisualUpdater::extractPartHeader(QMap<QString,QString> * headerData,QString header) {
    QStringList lines=header.split("\r\n",QString::SkipEmptyParts);
    QStringList keyValues;
    QStringList pair;
    for(int i=0;i<lines.count();i++) {
        keyValues+=lines[i].split(";");
    }
    for(int i=0;i<keyValues.count();i++) {
        pair.clear();
        if (keyValues[i].contains('=')) {
            pair=keyValues[i].split('=');
            pair[1].remove('"');
        } else if (keyValues[i].contains(':')) {
            pair=keyValues[i].split(':');
            pair[1].remove(' ');
        } 
        if (pair.count()==2) {
            pair[0].remove(' ');
            headerData->insert(pair[0].toLower(),pair[1]);
        }
    }

}
/*! @brief Sauvegarde d'un fichier recu*/
void _SwGuiVisualUpdater::saveFile(QString filename,QString contentType,QByteArray & realData) {
    //if (contentType=="application/octet-stream") {
        qDebug("Writing %s",filename.toLatin1().data());
        QFile file(_updateDirectory+QDir::separator()+filename);
        if (!file.open(QIODevice::WriteOnly))
            return;
        file.write(realData);
        file.close();
        _loadedFiles.push_back(filename);
        _trayIcon->showMessage("Updating",filename+" downloaded",QSystemTrayIcon::Information,4000);
    //}
}
/*! @brief envoie du status */
void _SwGuiVisualUpdater::sendStatus(/*QHttpRequestHeader * request,*/QIODevice *device) {
    /*QHttpResponseHeader httpreponse(200,"Ok",1,1);
    httpreponse.setContentType("text/xml");
    device->write(buildStatus().toUtf8());*/
}
/*! @brief construction du status */
QString _SwGuiVisualUpdater::buildStatus() {
    QString result;
    result=QString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n<Status lock=\"%1\">\r\n").arg(_isLocked?"on":"off");
    QMap<QString,bool> paths=SW_APP->ComponentsBank().GetPathList();
    QMap<QString,bool>::iterator it=paths.begin();
    while(it!=paths.end()) {
        QDir d=QDir(it.key());
        if (d.exists()) {
            QFileInfoList fiList=d.entryInfoList();
            for(int i=0;i<fiList.count();i++) {
                if (fiList[i].isFile()) {
                    result+=QString("    <File name=\"%1\" created=\"%2\"/>\r\n")
                                .arg(fiList[i].fileName())
                                .arg(fiList[i].created().toTime_t());
                }
            }
        }
        it++;
    }
    result+="</Status>";
    return result;
}
//-----------------------------------------------------------------------------
// Update phase
//-----------------------------------------------------------------------------
/*! @breif  sur update a faire*/
void _SwGuiVisualUpdater::onUpdate() {
    //Check de l'update a faire
    QFile file(_updateDirectory+QDir::separator()+"lock.lock");
    if (file.exists()) 
        return;
    QDir updateDir(_updateDirectory);
    updateDir.refresh();
    if (updateDir.entryInfoList(QDir::Files).count()==0)
        return;
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.close();
    //Update en preparation
    _isUpdateLocked=true;
    _trayIcon->setToolTip(QApplication::applicationName()+" update in progress...");
    _trayIconTimer->start(100);
    _trayIcon->setVisible(true);
    //---------------------------------------------------------
    //Update 
    //---------------------------------------------------------
    QMap<QString,QString> existantFiles; 
    //Evaluation de l'existant
    QMap<QString,bool> paths=SW_APP->ComponentsBank().GetPathList();
    QMap<QString,bool>::iterator it=paths.begin();
    while(it!=paths.end()) {
        QDir d=QDir(it.key());
        if (d.exists()) {
            QFileInfoList fiList=d.entryInfoList();
            for(int i=0;i<fiList.count();i++) {
                if (fiList[i].isFile()) {
                    existantFiles.insert(fiList[i].fileName(),fiList[i].absolutePath());
                }
            }
        }
        it++;
    }
    //Deplacement et renommage des fichiers
    QFileInfoList fiUpdateList=updateDir.entryInfoList();
    for(int i=0;i<fiUpdateList.count();i++) {
        if (fiUpdateList[i].isFile() && fiUpdateList[i].fileName()!="lock.lock") {
            QFile f(fiUpdateList[i].absoluteFilePath()); 
            QMap<QString,QString>::iterator itex=existantFiles.find(fiUpdateList[i].fileName());
            QString target;
            if (itex!=existantFiles.end()) {
                target=itex.value()+QDir::separator()+fiUpdateList[i].fileName()+".new";
            } else {
                target=QCoreApplication::applicationDirPath()+QDir::separator()+fiUpdateList[i].fileName()+".new";
            }
            QFile fnew(target);
            if (fnew.exists()) {
                f.remove();
            }
            f.rename(target);
        }
    }
    //---------------------------------------------------------
    //Fin Update
    //---------------------------------------------------------
    QFile filel(_updateDirectory+QDir::separator()+"lock.lock");
    if (filel.exists()) {
        filel.remove();
    }
    _isUpdateLocked=false;
    _trayIcon->setVisible(false);
    _loadedFiles.clear();
    //Restart asking
    int ret = QMessageBox::warning(0, QString("Update done"),
                           QString("Would you restart now ?"),
                           QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
    if (ret == QMessageBox::Yes) {
        //Restart
        QStringList args=qApp->arguments();
        for(int i=0;i<args.count()-1;i++) {
            QString arg=args[i];
            //aide
            if (arg=="-restart") {
                args.removeAt(i+1); 
                args.removeAt(i); 
            }
        }
        args.push_back("-restart");
        args.push_back(QString("%1").arg(QCoreApplication::applicationPid()));
        QProcess::startDetached(QCoreApplication::applicationFilePath(),args,SW_APP->GetWorkingPath());
        //End
        QApplication::closeAllWindows();
    }

}

