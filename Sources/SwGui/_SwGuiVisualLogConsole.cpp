/**
 * @file _SwGuiVisualLogConsole.cpp
 * @brief Log Console
 * @version 1.0
 * @date Mon Oct 20 15:04:05 CEST 2008
 * @author AuthorOfPlugin
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include <QVBoxLayout>
#include "_SwGuiVisualLogConsole.h"
#include <QColor>
#include <QFile>

using namespace StreamWork::SwCore;

/** @brief Constructeur */
_SwGuiVisualLogConsole::_SwGuiVisualLogConsole(): SwComponent_Class()
{
    _properties_service = NULL;
    _provider_service = NULL;
    _consumer_service = NULL;
	_mainWidget = 0;
    _handleHttpServer = 0;
    QFile image( ":/SwGui/console.jpg" );
    image.open( QIODevice::ReadOnly );
    _favicon = image.readAll();
    image.close();
    QFile page( ":/SwGui/console.html" );
    page.open( QIODevice::ReadOnly );
    _consoleHtmlBody = page.readAll();
    page.close();
    _backgroundColor = QColor( "black" );
    _foregroundColor = QColor( "white" );
    _maxLine = 500;
    
    //--------------------------------------
    //Autres
    //--------------------------------------
	_mainWidget = _SwConsoleWidget::buildScrollableConsole( 0, &logWidget );
    
}
/** @brief Destructeur */
_SwGuiVisualLogConsole::~_SwGuiVisualLogConsole()
{
    SW_APP->Logger().DetachLogRecorder( this );
    
    //Destruction properties automatiques
    //Desenregistrement automatiques des interfaces
    //Destruction Pins
    
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    
    //DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    delete _mainWidget;
}
/**
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void _SwGuiVisualLogConsole::InitializeResources() throw( SwException )
{
    QVariant tmp;
    
    //Creation des services
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    
    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    _provider_service->RegisterProvidedInterface<QWidget>( "logConsole", _mainWidget);
    
    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------
    //Importation de l'interface QWidget
    _consumer_service->RegisterConsumedInterface<ISwHttpServer>( "HttpServer", &_handleHttpServer );
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //--------------------------------------
    //Definition Pins
    //--------------------------------------
    
    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    _properties_service->CreatePropertiesForQObject( this, QString() );
    
    SW_APP->Logger().AttachLogRecorder( this );
}

/** @brief Callback sur les changements de propriétés*/
void _SwGuiVisualLogConsole::OnPropertyChange( ISwProperty * property )
{
}
QColor _SwGuiVisualLogConsole::backgroundColor()
{
    return _backgroundColor;
}
void _SwGuiVisualLogConsole::setBackgroundColor( QColor color )
{
    _backgroundColor = color;
	_mainWidget->setStyleSheet( QString( "color: %1;background-color: %2;" ).arg( _foregroundColor.name() ).arg( _backgroundColor.name() ) );
}
QColor _SwGuiVisualLogConsole::foregroundColor()
{
    return _foregroundColor;
}
void _SwGuiVisualLogConsole::setForegroundColor( QColor color )
{
    _foregroundColor = color;
	_mainWidget->setStyleSheet( QString( "color: %1;background-color: %2;" ).arg( _foregroundColor.name() ).arg( _backgroundColor.name() ) );
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void _SwGuiVisualLogConsole::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    if( _handleHttpServer != 0 )
    {
        _handleHttpServer->unregisterHttpPart( this );
    }
    
}
/** @brief Apres changement de la disponibilité de l'interface */
void _SwGuiVisualLogConsole::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    if( _handleHttpServer != 0 )
    {
        _handleHttpServer->registerHttpPart( this );
    }
}
//---------------------------------------------------------------------
// Interface ISwLogRecorder
//---------------------------------------------------------------------
/*! \brief Methode de log */
void _SwGuiVisualLogConsole::RecordLog( TSw_Log_Level level, QString text )
{
    QString message = "";
    switch( level )
    {
        case LogLvl_Debug:
            break;
        case LogLvl_Info:
            message += "<b class=\"styleI\">I:</b>"; break;
        case LogLvl_Warning:
            message += "<b class=\"styleW\">W:</b>"; break;
        case LogLvl_Critical:
            message += "<b class=\"styleC\">C:</b>"; break;
        case LogLvl_Emergency:
            message += "<b class=\"styleE\">E:</b>"; break;
        default:
            message += "<b class=\"styleU\">U:</b>"; break;
            
            //      case LogLvl_Info:
            //          message+="<b style=\"color:#2086ee\">I:</b>";break;
            //      case LogLvl_Warning:
            //          message+="<b style=\"color:#ee8120\">W:</b>";break;
            //      case LogLvl_Critical:
            //          message+="<b style=\"color:#ce0606\">C:</b>";break;
            //      case LogLvl_Emergency:
            //          message+="<b style=\"color:#8d1698\">E:</b>";break;
            //      default:
    }
    message += text;
    _mutex.lock();
    _liste.append( message );
    if( _liste.size() > _maxLine )
    {
        _liste.clear();
    }
    _mutex.unlock();
    if( _mainWidget->parentWidget() == 0 )
        return;
    message = "";
    switch( level )
    {
        case LogLvl_Debug:
            break;
        case LogLvl_Info:
            message += "I:"; break;
        case LogLvl_Warning:
            message += "W:"; break;
        case LogLvl_Critical:
            message += "C:"; break;
        case LogLvl_Emergency:
            message += "E:"; break;
        default:
            message += "U:"; break;
    }
    message += text;
    
    logWidget->addMessage( message );
}
//---------------------------------------------------------------------
// Interface ISwHttpPart
//---------------------------------------------------------------------
/*! @brief Renvoie le path de la partie  */
QString  _SwGuiVisualLogConsole::getPath()
{
    return "/_console";
}
/*! @brief process de la requete */
void _SwGuiVisualLogConsole::processRequest( /*QHttpRequestHeader * request,*/
    QMap<QString, QString> * parameters,
    QByteArray * body,
    QIODevice * device )
{
    /* if (request->path()=="/_console.xml") {
         QHttpResponseHeader httpreponse(200,"Ok",1,1);
         httpreponse.setContentType("text/xml");
         QString result="";
         _mutex.lock();
         result+=_liste.join("<br/>");
         _liste.clear();
         _mutex.unlock();
         QByteArray resultArray=result.toUtf8();
         httpreponse.setContentLength(resultArray.size());
         device->write(httpreponse.toString().toUtf8());
         device->write(resultArray);
         return;
     }
     if (request->path()=="/_console.jpg") {
         QHttpResponseHeader httpreponse(200,"Ok",1,1);
         httpreponse.setContentType("image/jpg");
         httpreponse.setContentLength(_favicon.size());
         device->write(httpreponse.toString().toUtf8());
         device->write(_favicon);
         return;
     }
     if (request->path()=="/_console" || request->path()=="/_console.html") {
         QHttpResponseHeader httpreponse(200,"Ok",1,1);
         httpreponse.setContentType("text/html");
         httpreponse.setContentLength(_consoleHtmlBody.size());
         device->write(httpreponse.toString().toUtf8());
         device->write(_consoleHtmlBody);
         return;
     }*/
    
}

//-------------------------------------------------------------------------
void _SwGuiVisualLogConsole::setMaxLine( int val )
{
    _maxLine = val;
    if( logWidget )
        logWidget->setMaxMessages( val );
}
