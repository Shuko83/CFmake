/*!
 \file _SwGuiCompMainWindow.h
 \brief Implementation of the Class _SwGuiMainWindow generant une SwDockWidget_MainWindow
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef ___SwGuiCompMainWindow_H
#define ___SwGuiCompMainWindow_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <Qt>

#include "SwGuiConstantes.h"
#include "SwAssistedComponent.h"
#include <Component.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <SwEnum.h>
#include "ISwMenu.h"
#include "ISwToolBar.h"
#include "ISwListDockWidget.h"
#include "ISwListDockWidgetListener.h"

/*
  * INCLUDES LOCAUX
  */
#include "ISwMainWindow.h"
#include "ISwEvent.h"
#include "ISwEventObserver.h"
#include "SwDockWidget_DockWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwFoundation;

/*!
    \class _SwGuiCompMainWindow
    \brief _SwGuiMainWindow generant une QMainWindow
*/
class SWGUI_EXPORT _SwGuiCompMainWindow : public SwAssistedComponent,
    public ISwMainWindow,
    public ISwEvent,
    public ISwFinalizer,
    public ISwListDockWidgetListener
{
    Q_OBJECT
    
public :
    enum PathType
    {
        UserHomeDir = 0,
        ApplicationDir = 1,
        Fixed = 2
    };
    
    Q_ENUMS( PathType );
    Q_PROPERTY( QString relativePath READ getRelativePath WRITE setRelativePath );
    Q_PROPERTY( QString absolutePath READ getAbsolutePath WRITE setAbsolutePath );
    Q_PROPERTY( PathType pathType READ getPathType WRITE setPathType );
    Q_PROPERTY( bool quitOnClose READ getQuitOnClose WRITE setQuitOnClose );
    
protected:
    /* fenetre principale */
    SwDockWidget_MainWindow * _mainWindow;
    
    /* show */
    SwEnum _show_mode;
    ISwProperty * _show_property;
    
    //Parametre de sauvegarde automatique de la configuraiton
    unsigned int _saveAutoPeriod;
    ISwProperty * _saveAutoPeriod_property;
    
    // --- Menus ---
    /* nombre de menus */
    uint _menus_nb;
    /* propriété nombre de menu */
    ISwProperty * _menus_nb_property;
    /* map des interfaces menus*/
    QMap<QString, ISwMenu *> _menus;
    
    // --- Actions ---
    /* nombre d'actions */
    uint _actions_nb;
    /* propriété nombre d'actions*/
    ISwProperty * _actions_nb_property;
    /* map des interfaces actions*/
    QMap<QString, QAction *> _actions;
    
    // --- ToolBars ---
    /* default toolbar position */
    SwEnum _default_toolbar_position;
    /* nombre de toolbars */
    uint _toolbars_nb;
    /* propriété nombre de toolbars */
    ISwProperty * _toolbars_nb_property;
    /* map des interfaces toolbars*/
    QMap<QString, ISwToolBar *> _toolbars;
    /* map des properties positions toolbars*/
    QMap<QString, ISwProperty *> _toolbar_positions;
    
    // --- DockWidgets ---
    /* nombre de dockwidgets */
    uint _dockwidgets_nb;
    /* propriété nombre de dockwidgets */
    ISwProperty * _dockwidgets_nb_property;
    /* map des interfaces dockwidgets*/
    QMap<QString, SwDockWidget_DockWidget *> _dockwidgets;
    /* nombre de listes de dockwidgets */
    uint _listdockwidgets_nb;
    /* propriété nombre de listes de dockwidgets */
    ISwProperty * _listdockwidgets_nb_property;
    /* map des liste des docks widgets*/
    QMap<QString, ISwListDockWidget *> _listdockwidgets;
    
    // --- Central Widget ---
    /* choix du type d'interface Widget ou MainWindow*/
    bool  _useAsWidget;
    /* propriété nombre d'actions*/
    ISwProperty * _use_aswidget_property;
    
    //Propriete
    ISwProperty * _path_property;
    
    QList<ISwEventObserver *> _iSwEvent;
    bool _finalized;
    
    QString _relativePath;
    QString _absolutePath;
    PathType _pathType;
    bool _quitOnClose;
    
    // --- Fichier de configuration --
    //Nom du fichier de configuration
    QString _configurationFileName;
    
    
    
public:
    /*! \brief Constructeur */
    _SwGuiCompMainWindow();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompMainWindow();
    
    /*! \brief Initialisation du composant */
    virtual void initializeComponent() throw( SwException );
    /*! \brief Callback sur les changements de propriétés*/
    void eventPropertyChange( ISwProperty * property );
    
    void saveConfiguration();
    void restoreConfiguration();
    
    //---------------------------------------------------------------------
    // Interface ISwQMainWindow
    //---------------------------------------------------------------------
    /*! \brief Renvoie le nom du service
    \return le nom du service */
    virtual SwDockWidget_MainWindow & getMainWindow();
    
    //---------------------------------------------------------------------
    // Interface ISwEvent
    //---------------------------------------------------------------------
    void addObserver( ISwEventObserver * obs );
    void removeObserver( ISwEventObserver * obs );
    void notify( QEvent * event );
    
    //---------------------------------------------------------------------
    // Interface ISwFinalizer
    //---------------------------------------------------------------------
    virtual bool Finalize( quint64 historic_index );
    
    //----------------------------------------------------------------------------
    // Interface ISwListDockWidgetListener
    //----------------------------------------------------------------------------
    virtual void addDockWidget( SwDockWidget_DockWidget * widget, QString menuName );
    virtual void removeDockWidget( SwDockWidget_DockWidget * widget );
    
    //---------------------------------------------------------------------
    // Gestion des Propriétés
    //---------------------------------------------------------------------
    /**
     * @brief    : Callback appelée lors de la disponibilité de l'interface
     * @param    : QString interfaceName - Nom de l'interface
     */
    virtual void interfaceAvailable( QString interfaceName );
    
    /**
     * @brief    : Callback appelée lors de l'indisponibilité de l'interface
     * @param    : QString interfaceName - Nom de l'interface
     */
    virtual void interfaceUnavailable( QString interfaceName );
    
    
    //----------------------------------------------------
    // Accesseur
    //----------------------------------------------------
    
    /** @brief Property value relativePath */
    QString getRelativePath();
    void setRelativePath( QString val );
    
    /** @brief Property value absolutePath */
    QString getAbsolutePath();
    void setAbsolutePath( QString val );
    
    /** @brief Property value pathType */
    PathType getPathType();
    void setPathType( PathType val );
    
    /** @brief Property value Quit On close */
    bool getQuitOnClose();
    void setQuitOnClose( bool val );
    
private:
    void showChanged();
    
    /**
     * @brief : getFilePath() récupère le path du fichier de sauvegarde d'une configuration
     */
    QString getFilePath();
    
    /**
     * @brief : checkDirectory() vérifie l'existence des répertoires dans le path entré
     *          pour les fichiers de sauvegarde de la conf. Les créé s'ils n'existent pas
     * @param : QString : inDirectoryBasePath, type de path choisi par l'utilisateur (absolu, relatif, fixe)
     */
    void checkDirectory( QString inDirectoryBasePath );
    
protected:
    void closeEvent( QCloseEvent * event );
};

#endif
