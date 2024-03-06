/**
 * @file _SwGuiQActionToWidget.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Mon Mar 14 16:59:33 CET 2011
 * @author ATN
 */

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiQActionToWidget.h"
#include <QDebug>

#include <QApplication>
#include <QDesktopWidget>

const QString INTERFACE_WIDGET = QStringLiteral("Widget");
const QString INTERFACE_ACTION = QStringLiteral("Action");
//-----------------------------------------------------------------------
_SwGuiQActionToWidget::_SwGuiQActionToWidget() 
	: Component()
	, _widget(nullptr)
	, _hostWidget(new _SwContainerCloseableWidget(nullptr))
	, _isVisible(false)
	, _stayOnTop(false)
	, _showName(QStringLiteral("Default visible"))
	, _hiddenName(QStringLiteral("Default hidden"))
{
    //RESET Interfaces
    connect( _hostWidget, &_SwContainerCloseableWidget::onClose, this, &_SwGuiQActionToWidget::ManageWidget);

    //RESET Properties
    
    _action.setCheckable( true ); //rend l'action checkable (du coup utiliser le SIGNAL cganged() sinon triggered()
    if( !connect( &_action, &QAction::changed, this, &_SwGuiQActionToWidget::ManageAction))
    {
        qDebug() << "QObject::connect(_action)" << "\t" << "failed";
    }
    
    isMoved = false;
}

//-----------------------------------------------------------------------
_SwGuiQActionToWidget::~_SwGuiQActionToWidget()
{

    getIConsumerService().UnregisterConsumedInterface(INTERFACE_WIDGET);
    getIProviderService().UnregisterProvidedInterface(INTERFACE_ACTION);
    
    delete _hostWidget;
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::initializeComponent() throw( SwException )
{
    QVariant tmp;
    
    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    getIProviderService().RegisterProvidedInterface<QAction>(INTERFACE_ACTION, &_action );
    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------
    getIConsumerService().RegisterConsumedInterface<QWidget>(INTERFACE_WIDGET, &_widget);
    
    //--------------------------------------
    //Definition Pins
    //--------------------------------------
    
    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    
    getPropertiesService().CreatePropertiesForQObject( this, QString(), true );
    getPropertiesService().CreatePropertiesForQObject( _hostWidget, "HostWidget", true );
   
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventPropertyChange( ISwProperty * )
{
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventBeforeInterfaceAvailability( QString , SwComponent_Class * )
{
}

//-----------------------------------------------------------------------
void _SwGuiQActionToWidget::eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
    if( interface_name == INTERFACE_WIDGET && _widget)
    {
        _hostWidget->setContainedWidget( _widget );
    }
    else
    {
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
    if( !name.isEmpty() )
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
    if( !name.isEmpty() )
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
    if( _widget != nullptr )
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
