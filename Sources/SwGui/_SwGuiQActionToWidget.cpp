/**
 * @file _SwGuiQActionToWidget.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Mon Mar 14 16:59:33 CET 2011
 * @author ATN
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiQActionToWidget.h"
#include <QDebug>

#include <QApplication>
#include <QDesktopWidget>




//-----------------------------------------------------------------------
_SwGuiQActionToWidget::_SwGuiQActionToWidget() : Component(), _action( 0 )
{
    //RESET Interfaces
    _i_Widget = NULL; /* Remplacer null par le bon handle pour les interfaces fournies */
    _widget = NULL;
    _hostWidget = new _SwContainerCloseableWidget( 0 );
    connect( _hostWidget, SIGNAL( onClose() ), this, SLOT( ManageWidget() ) );
    //RESET Pins
    
    //RESET Properties
    //INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE
    _isVisible = false;
    _stayOnTop = false;
    _showName = "Default visible";
    _hiddenName = "Default hidden";
    
    _action.setCheckable( true ); //rend l'action checkable (du coup utiliser le SIGNAL cganged() sinon triggered()
    if( !connect( &_action, SIGNAL( changed() ), this, SLOT( ManageAction() ) ) )
    {
        qDebug() << "QObject::connect(_action)" << "\t" << "failed";
    }
    
    isMoved = false;
}

//-----------------------------------------------------------------------
_SwGuiQActionToWidget::~_SwGuiQActionToWidget()
{

    getIConsumerService().UnregisterConsumedInterface( "Widget" );
    getIProviderService().UnregisterProvidedInterface( "Action" );
    
    delete _hostWidget;
}


//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::initializeComponent() throw( SwException )
{
    QVariant tmp;
    
    
    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    getIProviderService().RegisterProvidedInterface<ISwAction>( "Action", ( ISwAction * )this );
    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------
    getIConsumerService().RegisterConsumedInterface<ISwWidget>( "Widget", &_i_Widget );
    
    //--------------------------------------
    //Definition Pins
    //--------------------------------------
    
    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    
    getPropertiesService().CreatePropertiesForQObject( this, "", true );
    getPropertiesService().CreatePropertiesForQObject( _hostWidget, "HostWidget", true );
    
    //--------------------------------------
    //Autres
    //--------------------------------------
    
    
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventPropertyChange( ISwProperty * property )
{
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventBeforeInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
    if( interface_name == "Widget" && _i_Widget )
    {
        _widget = _i_Widget->GetWidget();
        _hostWidget->setContainedWidget( _widget );
    }
    else
    {
        if( _i_Widget == nullptr )
            _widget = nullptr;
            
        _hostWidget->setContainedWidget( nullptr );
        _hostWidget->hide();
    }
}

//-----------------------------------------------------------------------
QAction & _SwGuiQActionToWidget::GetAction()
{
    return _action;
}


//-----------------------------------------------------------------------
QString _SwGuiQActionToWidget::getShowName()
{
    return _showName;
}


//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::setShowName( QString name )
{
    if( name != "" )
    {
        _showName = name;
        _action.setText( _showName );
    }
}


//-----------------------------------------------------------------------
QString _SwGuiQActionToWidget::getHiddenName()
{
    return _hiddenName;
}


//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::setHiddenName( QString name )
{
    if( name != "" )
    {
        _hiddenName = name;
        _action.setText( _hiddenName );
    }
}


//-----------------------------------------------------------------------
_SwGuiQActionToWidget::WindowFlag _SwGuiQActionToWidget::getFlag()
{
    if( _hostWidget )
    {
        int flag = ( _hostWidget->windowFlags() & 0xFFF );
        return ( _SwGuiQActionToWidget::WindowFlag )flag;
    }
    else
    {
        return WIDGET;
    }
}


//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::setFlag( _SwGuiQActionToWidget::WindowFlag flag )
{
    if( _hostWidget )
    {
        _hostWidget->setWindowFlags( ( Qt::WindowFlags )flag );
    }
}

//-----------------------------------------------------------------------
bool _SwGuiQActionToWidget::getStayOnTop() const
{
    return _stayOnTop;
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::setStayOnTop( bool val )
{
    bool visible = _hostWidget->isVisible();
    _stayOnTop = val;
    if( _stayOnTop )
    {
        if( _hostWidget->windowFlags() /*Qt::WindowFlags(_flags_mode.ToInt())*/ == Qt::FramelessWindowHint )
            _hostWidget->setWindowFlags( _hostWidget->windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool );
        else
            _hostWidget->setWindowFlags( _hostWidget->windowFlags() | Qt::WindowStaysOnTopHint );
    }
    else
    {
        if( _hostWidget->windowFlags() == Qt::FramelessWindowHint )
            _hostWidget->setWindowFlags( _hostWidget->windowFlags() | Qt::Tool );
        else
            _hostWidget->setWindowFlags( _hostWidget->windowFlags() );
    }
    
    _hostWidget->setVisible( visible );
}



//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::ManageAction()
{
    if( _widget != 0 )
    {
        bool isChecked = _action.isChecked();//isChecked =  fenetre visible sauf si l'user clique sur la croix de la fenetre
        _hostWidget->setVisible( isChecked );
        _widget->setVisible( isChecked ); //force l'affichage du sous widget
        
        if( !isMoved )
        {
            //centre la widget si c'est le 1er affichage
            MoveCenter();
            isMoved = true;
        }
        
        if( isChecked )
        {
            _hostWidget->setWindowTitle( _showName ); //affecte le titre de l'action à la fenetre
            setShowName( _showName ); //change le nom du menu
        }
        else
        {
            setHiddenName( _hiddenName ); //change le nom du menu
        }
    }
}


//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::ManageWidget()
{
    _isVisible = false;
    _action.setChecked( _isVisible );
}



//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::MoveCenter()
{
    //centre la widget
    QDesktopWidget * desktop = QApplication::desktop();
    QRect r = _hostWidget->frameGeometry();
    QRect rd = desktop->availableGeometry();
    QPoint pCenter = rd.center(); //centre de l'écran
    r.moveCenter( pCenter );
    _hostWidget->move( r.topLeft() );
}