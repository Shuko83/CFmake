/**
 * @file SwGuiQActionToWidget2.h
 * @brief Composant qui consomme un ISwAction et une ISwWidget et qui, sur le triggered de l'action
 *        ouvre ou ferme le widget associé.
 *        Le Widget n'est pas encapsulé
 * @version 1.0
 * @date  21/03/2014
 * @author  CGD
 */


#include <SwApplication.h>
#include "_SwGuiQActionConsumedToWidget.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>



//-------------------------------------------------------------------------
_SwGuiQActionConsumedToWidget::_SwGuiQActionConsumedToWidget(): Component()
{
    _i_Widget = NULL;
    _widget = NULL;
    _i_Action = NULL;
    _action = NULL;
    
    _isVisible = false;
    
    
    isMoved = false;
}

//-------------------------------------------------------------------------
_SwGuiQActionConsumedToWidget::~_SwGuiQActionConsumedToWidget()
{
    //DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    //delete _hostWidget;//fait planter
}

//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::initializeComponent() throw( SwException )
{
    getIConsumerService().RegisterConsumedInterface<ISwAction>( "Action", &_i_Action );
    getIConsumerService().RegisterConsumedInterface<ISwWidget>( "Widget", &_i_Widget );
    
    getPropertiesService().CreatePropertiesForQObject( this, "", true );
}

//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::eventPropertyChange( ISwProperty * property )
{
}

//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::eventBeforeInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
}

//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
    if( interface_name == "Widget"  &&  _i_Widget )
    {
        _widget = _i_Widget->GetWidget();
        
        // Récupération de la fenêtre parente du widget concerné
        topParent = qobject_cast<QWidget *>( parent() );
        while( qobject_cast<QWidget *>( _widget->parent() ) )
        {
            QWidget * par = qobject_cast<QWidget *>( _widget->parent() );
            if( par && par != topParent )
                topParent = par;
            else
                break;
        }
    }
    else
    {
        if( _i_Widget == 0 )
        {
            _widget = 0;
        }
    }
    
    if( ( interface_name == "Action" ) && ( _i_Action != 0 ) )
    {
        _action = &( _i_Action->GetAction() );
        if( !connect( _action, SIGNAL( triggered() ), this, SLOT( ManageAction() ) ) )
        {
            qDebug() << "QObject::connect(_i_Action)" << "\t" << "failed";
        }
    }
}



//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::ManageAction()
{
    if( _widget != 0 )
    {
        // récupération de l'état de la fenêtre associée
        bool state = _widget->isVisible();
        
        
        // L'action déclenche l'ouverture ou la fermeture du widget associé
        if( topParent )
            topParent->setVisible( !state );
        _widget->setVisible( !state );
        
        if( !isMoved )
        {
            //centre la widget si c'est le 1er affichage
            MoveCenter();
            isMoved = true;
        }
    }
}


//-------------------------------------------------------------------------
void _SwGuiQActionConsumedToWidget::MoveCenter()
{
    QWidget * toMove;
    
    if( topParent )
        toMove = topParent;
    else
        toMove = _widget;
        
    //centre la widget
    QDesktopWidget * desktop = QApplication::desktop();
    QRect r = toMove->frameGeometry();
    QRect rd = desktop->availableGeometry();
    QPoint pCenter = rd.center(); //centre de l'écran
    r.moveCenter( pCenter );
    toMove->move( r.topLeft() );
}