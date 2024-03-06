/**
 * @file SwGuiQActionToWidget2.h
 * @brief Composant qui consomme une QAction et une QWidget et qui, sur le triggered de l'action
 *        ouvre ou ferme le widget associé.
 *        Le Widget n'est pas encapsulé
 * @version 1.0
 * @date  21/03/2014
 * @author  CGD
 */

#include "Main/SwApplication.h"
#include "_SwGuiQActionConsumedToWidget.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QAction>

const QString ACTION = QStringLiteral("Action");
const QString WIDGET = QStringLiteral("Widget");

//-------------------------------------------------------------------------
_SwGuiQActionConsumedToWidget::_SwGuiQActionConsumedToWidget()
	: Component()
	, _widget (nullptr)
	, _action (nullptr)
	, _isVisible (false)
	, isMoved (false)
{
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
    getIConsumerService().RegisterConsumedInterface<QAction>( ACTION, &_action );
    getIConsumerService().RegisterConsumedInterface<QWidget>( WIDGET, &_widget);
    
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
    if( interface_name == WIDGET  &&  _widget)
    {
        
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
    else if( ( interface_name == ACTION ) && ( _action ) )
    {
        if( !connect( _action, SIGNAL( triggered() ), this, SLOT( ManageAction() ) ) )
        {
            qDebug() << "QObject::connect(_action)" << "\t" << "failed";
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
