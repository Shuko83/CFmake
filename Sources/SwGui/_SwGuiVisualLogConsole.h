/**
 * @file _SwGuiVisualLogConsole.h
 * @brief Log Console
 * @version 1.0
 * @date Mon Oct 20 15:04:05 CEST 2008
 * @author AuthorOfPlugin
 */

#ifndef __SwGuiVisualLogConsole_H
#define __SwGuiVisualLogConsole_H

/*
  * INCLUDES LOCAUX
  */
#include "Types/SwEnum.h"
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Component/Services/ServiceImpl/SwPins_Manager_Class.h"
#include "Properties/ISwProperty.h"
#include "Component/Interfaces/ISwPin_Listener.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Tools/Log/ISwLogRecorder.h"
#include <QTextEdit>
#include "ISwHttpServer.h"
#include "ISwHttpPart.h"
#include <QStringList>
#include <QMutex>
#include <QColor>
#include "_SwConsoleWidget.h"

//Check namespace needed
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/**
 *  @class _SwGuiVisualLogConsole
 *  @brief Log Console
 */
class _SwGuiVisualLogConsole : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver,
    public ISwLogRecorder,
    public ISwHttpPart
{
    Q_OBJECT
    Q_PROPERTY( QColor backgroundColor READ backgroundColor WRITE setBackgroundColor )
    Q_PROPERTY( QColor foregroundColor READ foregroundColor WRITE setForegroundColor )
    Q_PROPERTY( int    maxLine         READ getMaxLine      WRITE setMaxLine )

protected:
    //--------------------------------------------------------------
    //Services
    //--------------------------------------------------------------
    /** @brief service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /** @brief service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /** @brief service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    //--------------------------------------------------------------
    //Widget
    //--------------------------------------------------------------
    //WidgetCentral
    QWidget * _mainWidget;
    //LogWidget
    _SwConsoleWidget * logWidget;
    //--------------------------------------------------------------
    // Handle serveur http
    //--------------------------------------------------------------
    ISwHttpServer * _handleHttpServer;
    QByteArray _favicon;
    QByteArray _consoleHtmlBody;
    //--------------------------------------------------------------
    // Autre
    //--------------------------------------------------------------
    QMutex _mutex;
    QStringList _liste;
    QColor _backgroundColor;
    QColor _foregroundColor;
    int _maxLine;
    
public:
    /** @brief Constructeur */
    _SwGuiVisualLogConsole();
    /** @brief Destructeur */
    virtual ~_SwGuiVisualLogConsole();
    /**
     * @brief Initialisation des ressources
     * @note tous les services du composants doivent être déclarés dans cette methodes
     */
    virtual void InitializeResources() throw( SwException );
    /** @brief Callback sur les changements de propriétés*/
    void OnPropertyChange( ISwProperty * property );
    
    QColor backgroundColor();
    void setBackgroundColor( QColor color );
    QColor foregroundColor();
    void setForegroundColor( QColor color );
    int getMaxLine() const { return _maxLine; }
    void setMaxLine( int val );
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /** @brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    /** @brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    //---------------------------------------------------------------------
    // Interface ISwLogRecorder
    //---------------------------------------------------------------------
    /*! \brief Methode de log */
    virtual void RecordLog( TSw_Log_Level level, QString text );
    //---------------------------------------------------------------------
    // Interface ISwHttpPart
    //---------------------------------------------------------------------
    /*! @brief Renvoie le path de la partie  */
    virtual QString  getPath();
    /*! @brief process de la requete */
    virtual void processRequest( /*QHttpRequestHeader * request,*/
        QMap<QString, QString> * parameters,
        QByteArray * body,
        QIODevice * device );
};

#endif
