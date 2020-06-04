/*!
 \file _SwGuiCompListDockWidget.cpp
 \brief Implementation of the Class _SwGuiCompListDockWidget generant une list de SwDockWidget
 \version 1.0
 \date
 \author
*/

//#include <SwApplication.h>
//#include <SwMacros.h>
#include "_SwGuiCompListDockWidget.h"
#include "ISwDockWidget.h"

#define WIDGET_INTERFACE_NAME "Widget_%1"

/*! \brief Constructeur */
_SwGuiCompListDockWidget::_SwGuiCompListDockWidget() :
    SwAssistedComponent(),
    _widgetNumber( 0 )
{
    setPropertyServiceAvaibility( true );
    setConsumerServiceAvaibility( true );
    setProviderServiceAvaibility( true );
}

/*! \brief Destructeur */
_SwGuiCompListDockWidget::~_SwGuiCompListDockWidget()
{
    //---------------------------------------------------
    //              Gestion des Interfaces
    //---------------------------------------------------
    unprovideInterface( "ISwListDockWidget" );
}

void _SwGuiCompListDockWidget::initializeComponent() throw( SwException )
{
    //---------------------------------------------------
    //              Gestion des Interfaces
    //---------------------------------------------------
    provideInterface<ISwListDockWidget>( "ISwListDockWidget", ( ISwListDockWidget * )this );
    
    //---------------------------------------------------
    //              Gestion des proprietes
    //---------------------------------------------------
    createPropertiesForThisObject( "DockWidget" );
}

void _SwGuiCompListDockWidget::interfaceAvailable( QString interfaceName )
{
    //DockWidget
    QMap<QString, SwDockWidget_DockWidget *>::iterator dockwidget_it = _dockwidgets.find( interfaceName );
    if( dockwidget_it != _dockwidgets.end() && dockwidget_it.value() )
    {
		QWidget * widget = getInterface<QWidget>( interfaceName );
        if( widget )
        {
            dockwidget_it.value()->setWidget( widget );
            
            //Notification
			for ( ISwListDockWidgetListener * listener: _listListener )
                listener->addDockWidget( dockwidget_it.value(), _name );
        }
    }
}


void _SwGuiCompListDockWidget::interfaceUnavailable( QString interfaceName )
{
    //DockWidget
    
    QMap<QString, SwDockWidget_DockWidget *>::iterator dockwidget_it = _dockwidgets.find( interfaceName );
    if( dockwidget_it != _dockwidgets.end() && dockwidget_it.value() )
    {
        SwDockWidget_DockWidget * dock = dockwidget_it.value();
        if( dock )
        {
            //Notification
			for ( ISwListDockWidgetListener * listener: _listListener )
                listener->removeDockWidget( dock );
        }
        dockwidget_it.value()->setWidget( nullptr );
        return;
    }
}

void _SwGuiCompListDockWidget::setWidgetNumber( uint nb )
{
    QString interface_name;
    if( _widgetNumber != nb )
    {
        //S'il faut supprimer des interfaces
        if( nb < _widgetNumber )
        {
            for( uint i = nb; i < _widgetNumber; i++ )
            {
                interface_name = QString( WIDGET_INTERFACE_NAME ).arg( i );
                
                QMap<QString, SwDockWidget_DockWidget *>::const_iterator iDock = _dockwidgets.find( interface_name );
                if( iDock != _dockwidgets.end() && iDock.key() == interface_name )
                {
                    getPropertiesService().DestroyPropertiesBeginWith( interface_name );
                    
                    delete iDock.value();
                    _dockwidgets.remove( interface_name );
                }
                unconsumeInterface( interface_name );
            }
        }
        
        //S'il faut ajouter des interfaces
        else if( nb > _widgetNumber )
        {
            for( uint i = _widgetNumber; i < nb; i++ )
            {
                interface_name = QString( WIDGET_INTERFACE_NAME ).arg( i );
                SwDockWidget_DockWidget * dock = new SwDockWidget_DockWidget();
                createPropertiesForQObject( dock, interface_name, true );
                _dockwidgets.insert( interface_name, dock );
                consummeInterface<QWidget>( interface_name );
            }
        }
        
        _widgetNumber = nb;
    }
}

uint _SwGuiCompListDockWidget::getWidgetNumber()
{
    return _widgetNumber;
}

void _SwGuiCompListDockWidget::setName( QString name )
{
    if( name != _name )
        _name = name;
}

//----------------------------------------------------------------------------
//Interface ISwListDockWidget
//----------------------------------------------------------------------------
QList<SwDockWidget_DockWidget *> _SwGuiCompListDockWidget::GetListDockWidget()
{
    return _dockwidgets.values();
}

void _SwGuiCompListDockWidget::addDockWidgetListener( ISwListDockWidgetListener * observer )
{
    if( observer && !_listListener.contains( observer ) )
        _listListener.push_back( observer );
}

void _SwGuiCompListDockWidget::removeDockWidgetListener( ISwListDockWidgetListener * observer )
{
    if( observer && _listListener.contains( observer ) )
        _listListener.removeOne( observer );
}

QString _SwGuiCompListDockWidget::getDockWidgetName()
{
    return _name;
}