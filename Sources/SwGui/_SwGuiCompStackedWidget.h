/*!
 \file _SwGuiCompStackedWidget.h
 \brief Implementation of the Class _SwGuiCompStackedWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SWGUICOMPSTACKEDWIDGET_H
#define __SWGUICOMPSTACKEDWIDGET_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QStackedWidget>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwLayout.h"
#include "ISwWidget2.h"
#include "ISwWidget2_Observer.h"
#include "ISwStackedWidget_Controler.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
    \class _SwGuiCompStackedWidget
    \brief _SwGuiCompStackedWidget generant un QWidget
*/
class _SwGuiCompStackedWidget : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwWidget2, public ISwWidget2_Observer, public ISwStackedWidget_Controler
{
protected:
    /* menu */
    QStackedWidget * _stackedWidget;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    
    // --- Widgets ---
    
    /* nombre de stackedwidget*/
    uint _stackedWidgets_nb;
    /* propriété nombre de widgets */
    ISwProperty * _stackedWidgets_nb_property;
    
    uint _stackedWidgets_page_index;
    /* Sauvegarde de la page courrante */
    ISwProperty * _stackedWidgets_current_index_page;
    
    /* map des interfaces widgets*/
    QMap<QString, ISwWidget *> _widgets;
    
    /* handle temporaire d'interface widget*/
    ISwWidget * _tmp_handle_widget;
    
    /* liste des observers */
    QList<ISwWidget2_Observer *> _wObservers;
    QList<ISwStackedWidget_Observer *> _wStackedWidgetsObservers;
    
public:
    /*! \brief Constructeur */
    _SwGuiCompStackedWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompStackedWidget();
    
    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw( SwException );
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange( ISwProperty * property );
    
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    
    //---------------------------------------------------------------------
    // Interface ISwWidget2
    //---------------------------------------------------------------------
    /*! \brief Renvoie le widget
    \return le widget */
    virtual QWidget * GetWidget();
    /** @brief Enregistrement de l'observer */
    virtual void RegisterISwWidgetObserver( ISwWidget2_Observer * o );
    /** @brief Desregistrement de l'observer */
    virtual void UnregisterISwWidgetObserver( ISwWidget2_Observer * o );
    
    //---------------------------------------------------------------------
    // InterfaceISwWidget2_Observer
    //---------------------------------------------------------------------
    /** @brief mettre en avant le widget si c'est possible */
    virtual void OnBringToFrontRequest( ISwWidget * w );
    
    //---------------------------------------------------------------------
    // ISwStackedWidget_Controler
    //---------------------------------------------------------------------
    /*! \brief Renvoie le nombre de page disponible
    /*  \return un int définissant le nombre de page (0 étant la première)*/
    virtual int getNbPage();
    
    /*! \brief Permet de mettre le StackedWidget sur la page de l'index voulu
     *! \param int pageIndex  : L'index de la page voulue*/
    virtual void setCurrentIndex( int pageIndex );
    
    /*! \brief Renvoie l'index courrant
    /*  \return l'index courrant du QStackedWidget*/
    virtual int getCurrentIndex();
    
    /*! \brief Renvoie le nom du widget pour une page donnée
    /*  \param l'index de la page contenant le widget
    /*  \return le nom du widget
    */
    virtual QString getWidgetName( int pageIndex );
    
    /*! \brief Renvoie l'icon du widget pour une page donnée
    /*  \param l'index de la page contenant le widget
    /*  \return l'icon du widget
    */
    virtual QIcon getWidgetIcon( int pageIndex );
    
    
    
    /*! \brief Permet d'enregistrer un observer
    *! \param pointeur sur l'observer
     */
    virtual void registerStackedWidgetObserver( ISwStackedWidget_Observer * o );
    
    /*! \brief Permet de désenregistrer un observer
    *! \param pointeur sur l'observer
     */
    virtual void unRegisterStackedWidgetObserver( ISwStackedWidget_Observer * o );
    
    /*! \brief Permet de notifier les observers */
    void notifyObserver();
};
#endif
