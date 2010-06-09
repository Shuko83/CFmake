/*!
 \file _SwGuiCompHttpServer.cpp
 \brief Implementation of the Class _SwGuiCompHttpServer generant une QMenu
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QFile>
#include <QByteArray>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompHttpServer.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

#define DEFAULT_SERVER_PORT 1234

/*! \brief Constructeur */
_SwGuiCompHttpServer::_SwGuiCompHttpServer(): SwComponent_Class(){
    _provider_service=NULL;
    _properties_service=NULL;
    _portServer=DEFAULT_SERVER_PORT;
    _portServer_property=NULL;
    _server=new QTcpServer();
    _isStarted=false;
}
/*! \brief Destructeur */
_SwGuiCompHttpServer::~_SwGuiCompHttpServer(){
    //Desenregistrement des services
    this->UnregisterService(this->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _provider_service;
    delete _properties_service;

    if (_isStarted) {
        _server->close();
    }
    delete _server;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompHttpServer::InitializeResources() throw(SwException) {
    //Creation des service
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_provider_service);
    this->RegisterService(this); //Execution

    //Exportation de l'interface ISwHttpServer
    _provider_service->RegisterProvidedInterface<ISwHttpServer>("ISwHttpServer",(ISwHttpServer *)this);
   

    //Enregistrement des propriétés

    //Gestion du port
    _portServer_property=_properties_service->CreateProperty<uint>("HttpServerPort");
    if (_portServer_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register HttpServerPort property\n"));
    }
    _portServer_property->SetDescription("Define which port is used par http server");  
    _portServer_property->SetValue(QVariant(_portServer));
    _portServer_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompHttpServer::OnPropertyChange);


    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiCompHttpServer done\n"));

    //Callback server
    connect(_server, SIGNAL(newConnection()), this, SLOT(httpRequestReceived()));

    //Enregistrement about
    registerHttpPart(this);

    if(_server->listen(QHostAddress::Any,_portServer)) {
        _isStarted=true;
    }

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompHttpServer::OnPropertyChange(ISwProperty * property) {

    if (_portServer_property==property) {
        _portServer=property->GetValue().toUInt();
        if (_portServer>65535 || _portServer<=1024) {
            _portServer=DEFAULT_SERVER_PORT;
            _portServer_property->SetValue(QVariant(_portServer));
        }
        if (_isStarted) {
            _server->close();
            if(_server->listen(QHostAddress::Any,_portServer)) {
                _isStarted=true;
            }
        }
    }
}

//---------------------------------------------------------------------
// Interface ISwHttpServer
//---------------------------------------------------------------------
/*! \brief Permet d'enregistrer une partie de serveur http */
void _SwGuiCompHttpServer::registerHttpPart(ISwHttpPart *part){
    QSet<ISwHttpPart *>::iterator it=_parts.find(part);
    if (it==_parts.end())
        _parts.insert(part);
}
/*! \brief Permet de desenregistrer une partie de serveur http */
void _SwGuiCompHttpServer::unregisterHttpPart(ISwHttpPart *part){
    QSet<ISwHttpPart *>::iterator it=_parts.find(part);
    if (it!=_parts.end())
        _parts.erase(it);
}
//----------------------------------------------------
// Interface ISwExecutable_Service
//----------------------------------------------------
/** @brief Initialisation */
void _SwGuiCompHttpServer::Initialize(double start_time,StreamWork::SwExecution::ISwExecution_Service * executor) 
    throw (StreamWork::SwCore::SwException)
{
    //Nothing to do
}            
/** @brief Demarrage */
void _SwGuiCompHttpServer::Start(double current_time) throw (StreamWork::SwCore::SwException){
}            
/** @brief Execution */
void _SwGuiCompHttpServer::Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException){
    //Nothing to do
}            
/** @brief Execution */
void _SwGuiCompHttpServer::Stop(double current_time){
    
}            
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/** @brief Est appele uniquement par le service manager aupres duquel le service est enregistr?
lorsque ce premier se detruit ou une operation de desenregistrement du service est r?alis?e*/
void _SwGuiCompHttpServer::Liberate(){
    //Nothing to do
}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/** @brief acces a son composant hote */
StreamWork::SwCore::SwComponent_Class * _SwGuiCompHttpServer::GetHostComponent(){
    return this;
}            
//---------------------------------------------------------------------
// Interface Reception d'une requete HTTP
//---------------------------------------------------------------------
void _SwGuiCompHttpServer::httpRequestReceived() {
    //Recuperation de la socket client
    QTcpSocket *clientConnection = _server->nextPendingConnection();


    //Autodestruction de la socket sur deconnection
    QObject::connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    //Recuperation des donnees de la requete
    QByteArray data;
    while(clientConnection->waitForReadyRead(50)) {
        data+=clientConnection->readAll();
    }

    //Separation header and body
    int sep=data.indexOf("\r\n\r\n")+4;
    QByteArray header=data;
    QByteArray body;
    if (sep<data.size()-1) {
        header.remove(sep,data.size()-sep);
        body=data;
        body.remove(0,sep);
    } 
    
    //Construction de l'objet requete qt
    QHttpRequestHeader httpr=QHttpRequestHeader(QString(header));
    
    //Definition des parametres
    QMap<QString,QString> parameters;

    int sepparam=httpr.path().indexOf('?');
    if (sepparam>0) {
        QString params=httpr.path();
        params.remove(0,sepparam+1);
        QStringList liste=params.split("&");
        QStringList pair;
        for(int i=0;i<liste.count();i++) {
            pair=liste[i].split("=");
            if (pair.size()==2) {
                parameters.insert(QByteArray::fromPercentEncoding(pair[0].toUtf8()),
                                  QByteArray::fromPercentEncoding(pair[1].toUtf8()));
                qDebug("key %s ",pair[0].toLatin1().data());
                qDebug("value %s ",pair[1].toLatin1().data());
            }
        }
    }

    //Recherche de la partie qui convient
    QSet<ISwHttpPart *>::iterator it=_parts.begin();
    while(it!=_parts.end() && !httpr.path().startsWith((*it)->getPath())) {
        it++;
    }
    if (it!=_parts.end()) {
        //Traitement de la part
        (*it)->processRequest(&httpr,&parameters,&body,clientConnection);
    } else {
        //Si non trouve, renvoie erreur 404
        QHttpResponseHeader httpreponse(404,"Resource not found",1,1);
        httpreponse.setContentType("text/html");
        httpreponse.setValue("charset","utf-8");
        //httpreponse.setContentLength(310);

        QTextStream os(clientConnection);
        os << httpreponse.toString() << "<h1>Sorry resource not found</h1>Path:";
        os << httpr.path();
        os.flush();
    }

    clientConnection->close();
    clientConnection->disconnectFromHost();

}
//---------------------------------------------------------------------
// Interface ISwHttpPart
//---------------------------------------------------------------------            
/*! @brief Renvoie le path de la partie  */
QString _SwGuiCompHttpServer::getPath() {
    return "/_about";
}

/*! @brief process de la requete */
void _SwGuiCompHttpServer::processRequest(QHttpRequestHeader * request,
                                            QMap<QString,QString> * parameters,
                                            QByteArray * body,
                                            QIODevice * device) {
    if (request->path()=="/_about.jpg") {
        QFile image(":/SwGui/about.jpg");
        image.open(QIODevice::ReadOnly);
        QByteArray data=image.readAll();
        image.close();
        QHttpResponseHeader httpreponse(200,"Ok",1,1);
        httpreponse.setContentType("image/jpeg");
        httpreponse.setContentLength(data.size());
        device->write(httpreponse.toString().toUtf8());
        device->write(data);
        return;
    }
    //Si non trouve, renvoie erreur 404
    QHttpResponseHeader httpreponse(200,"Ok",1,1);
    httpreponse.setContentType("text/html");
    httpreponse.setValue("charset","utf-8");

    QTextStream os(device);
    os << httpreponse.toString() << "<link rel=\"icon\" type=\"image/jpg\" href=\"/_about.jpg\" /><center><img src=\"/_about.jpg\"><h1> Tiny Streamwork HttpServer version "<< CG_STREAMWORK_VERSION <<" </h1><img src=\"/_about.jpg\"></center>";
    os.flush();
}

