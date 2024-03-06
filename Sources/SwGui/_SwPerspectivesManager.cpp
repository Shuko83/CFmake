/**
@file _SwPerspectivesManager.cpp
@brief Perspectives Manager
@author F.Bighelli
 */
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwPerspectivesManager.h"

using namespace StreamWork::SwGui;

#define CL_PERSPECTIVE_INTERFACE_NAME "Perspective_%1"
#define CL_MARGIN 10

/** @brief Constructor */
_SwPerspectivesManager::_SwPerspectivesManager()
{
    _mainWidget = 0;
    _provider_service = 0;
    _consumer_service = 0;
    _properties_service = 0;
    _perspectives_nb = 0;
    _currentPerspective = 0;
    _buttons_stylesheet = "QFrame { border:1px solid black; border-radius: 5px; background:\
						qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(60,80,150,80%), stop: 1 rgb(60,80,150,20%)); }";
    _perspective_title = "DefaultTitle";
}
/*! \brief Destructeur */
_SwPerspectivesManager::~_SwPerspectivesManager()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    if( _mainWidget != NULL )
        delete _mainWidget;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwPerspectivesManager::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    //Construction du widget
    
    _mainWidget = new QWidget();
    QHBoxLayout * hlayout = new QHBoxLayout( _mainWidget );
    hlayout->setMargin( 0 );
    hlayout->setSpacing( 0 );
    _mainWidget->setLayout( hlayout );
    _tabWidget = new QWidget( _mainWidget );
    
    _tabWidget->setMaximumWidth( CG_PERSPECTIVE_BUTTON_SIZE + 2 * CL_MARGIN );
    _tabWidget->setMinimumWidth( CG_PERSPECTIVE_BUTTON_SIZE + 2 * CL_MARGIN );
    hlayout->addWidget( _tabWidget );
    _perspectiveWidget = new QWidget( _mainWidget );
    hlayout->addWidget( _perspectiveWidget );
    _vlayout = new QVBoxLayout( _tabWidget );
    _vlayout->setMargin( CL_MARGIN );
    _vlayout->setSpacing( CL_MARGIN );
    _tabWidget->setLayout( _vlayout );
    _vlayout->addSpacerItem( new QSpacerItem( CG_PERSPECTIVE_BUTTON_SIZE, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
    
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _mainWidget );
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //Gestion des perspectives
    _perspectives_nb_property = _properties_service->CreateProperty<uint>( "PerspectiveCount" );
    _perspectives_nb_property->SetDescription( "Define how many ISwPersopective interfaces this component accept" );
    _perspectives_nb_property->SetValue( QVariant( _perspectives_nb ) );
    _perspectives_nb_property->GetOnChangeSignal().iconnect( *this, &_SwPerspectivesManager::OnPropertyChange );
    
    //Gestion du stylesheet des boutons
    _buttons_stylesheet_property = _properties_service->CreateProperty<QString>( "ButtonsStyleSheet" );
    if( _buttons_stylesheet_property == NULL )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register ButtonsStyleSheet property\n" ) );
    }
    _buttons_stylesheet_property->SetDescription( "Define buttons stylesheet" );
    _buttons_stylesheet_property->SetValue( QVariant( _buttons_stylesheet ) );
    _buttons_stylesheet_property->GetOnChangeSignal().iconnect( *this, &_SwPerspectivesManager::OnPropertyChange );
    
    //Gestion du titre de la perspective
    _perspective_title_property = _properties_service->CreateProperty<QString>( "Title" );
    if( _perspective_title_property == NULL )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register Tilte property\n" ) );
    }
    _perspective_title_property->SetDescription( "Define the title of the perspective" );
    _perspective_title_property->SetValue( QVariant( _perspective_title ) );
    _perspective_title_property->GetOnChangeSignal().iconnect( *this, &_SwPerspectivesManager::OnPropertyChange );
    
}
/*! \brief Callback sur les changements de propriétés*/
void _SwPerspectivesManager::OnPropertyChange( ISwProperty * property )
{
    if( _buttons_stylesheet_property == property )
    {
        QString styleSheet = property->GetValue().toString();
        if( styleSheet.isEmpty() )
        {
            property->SetValue( QVariant( _buttons_stylesheet ) );
        }
        else
        {
            _buttons_stylesheet = styleSheet;
        }
        QList<_SwPerspectiveButton *>::iterator it = _buttons.begin();
        while( it != _buttons.end() )
        {
            ( *it )->setButtonStyleSheet( _buttons_stylesheet );
            ++it;
        }
    }
    else if( _perspectives_nb_property == property )
    {
        uint val;
        QString interface_name;
        val = property->GetValue().toUInt();
        if( val == _perspectives_nb ) return;
        if( val < _perspectives_nb )
        {
            for( uint i = val; i < _perspectives_nb; i++ )
            {
                interface_name = QString( CL_PERSPECTIVE_INTERFACE_NAME ).arg( i );
                _consumer_service->UnregisterConsumedInterface( interface_name );
            }
            removeButtons( _perspectives_nb - val );
            
        }
        else
        {
            for( uint i = _perspectives_nb; i < val; i++ )
            {
                interface_name = QString( CL_PERSPECTIVE_INTERFACE_NAME ).arg( i );
                _consumer_service->RegisterConsumedInterface<ISwPerspective>( interface_name, &_tmp_handle_perspective );
            }
            addButtons( val - _perspectives_nb );
        }
        _perspectives_nb = val;
    }
    else if( _perspective_title_property == property )
    {
        QString title = property->GetValue().toString();
        if( title.isEmpty() )
        {
            property->SetValue( QVariant( _perspective_title ) );
        }
        else
        {
            _perspective_title = title;
            _mainWidget->setWindowTitle( _perspective_title );
        }
    }
    
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwPerspectivesManager::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, ISwPerspective *>::iterator perspective_it;
    
    //Si c'est un menu
    perspective_it = _perspectives.find( interface_name );
    if( perspective_it != _perspectives.end() )
    {
        if( perspective_it.value() != NULL )
        {
            //Et qu'il etait defini, on le detache de la widgetbar
            QStringList list = interface_name.split( "_" );
            if( list.count() == 2 )
            {
                detachPerspective( perspective_it.value(), list[1].toInt() );
            }
            else
            {
                detachPerspective( perspective_it.value(), 0 );
            }
            perspective_it.value() = NULL;
            _perspectives.erase( perspective_it );
        }
    }
    
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwPerspectivesManager::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, ISwPerspective *>::iterator perspective_it;
    //Si c'est un widget
    perspective_it = _perspectives.find( interface_name );
    if( perspective_it == _perspectives.end()  && _tmp_handle_perspective != NULL )
    {
        _perspectives.insert( interface_name, _tmp_handle_perspective );
        QStringList list = interface_name.split( "_" );
        if( list.count() == 2 )
        {
            attachPerspective( _tmp_handle_perspective, list[1].toInt() );
        }
        else
        {
            attachPerspective( _tmp_handle_perspective, 0 );
        }
        return;
    }
}
/** @brief sur changement de l'activation d'une perspective*/
void _SwPerspectivesManager::attachPerspective( ISwPerspective * perspective, int index )
{
    _buttons[index]->setPerspective( perspective );
    if( _currentPerspective == 0 && perspective->isPerspectiveEnabled() && perspective->isPerspectiveVisible() )
    {
        _buttons[index]->setChecked( true );
    }
}
/** @brief sur changement de l'activation d'une perspective*/
void _SwPerspectivesManager::detachPerspective( ISwPerspective * perspective, int index )
{
    if( _currentPerspective == perspective )
    {
        onPerspectiveToggle( perspective, false );
    }
    _buttons[index]->setPerspective( 0 );
}
/** @brief ajout bouton*/
void _SwPerspectivesManager::addButtons( int number )
{
    //    int count=_buttons.count();
    for( int i = 0; i < number; i++ )
    {
        _SwPerspectiveButton * button = new _SwPerspectiveButton( _tabWidget, this );
        _vlayout->insertWidget( _vlayout->count() - 1, button );
        //button->move(CL_MARGIN,CL_MARGIN+(CG_PERSPECTIVE_BUTTON_SIZE+CL_MARGIN)*(i+count));
        connect( button, SIGNAL( onPerspectiveToggle( ISwPerspective *, bool ) ), this, SLOT( onPerspectiveToggle( ISwPerspective *, bool ) ) );
        button->setButtonStyleSheet( _buttons_stylesheet );
        _buttons.push_back( button );
    }
    updatePositionButton();
}
/** @brief suppression bouton*/
void _SwPerspectivesManager::removeButtons( int number )
{
    QList<_SwPerspectiveButton *>::iterator it = _buttons.begin();
    it += _buttons.count() - number;
    while( it != _buttons.end() )
    {
        delete *it;
        it = _buttons.erase( it );
    }
    updatePositionButton();
}
/** @brief updateButtonsPosition*/
void _SwPerspectivesManager::updatePositionButton()
{
    //    int i=0;
    //for(_SwPerspectiveButton * button : _buttons) {
    //    if (button->isVisible()){
    //        button->move(CL_MARGIN,CL_MARGIN+(CG_PERSPECTIVE_BUTTON_SIZE+CL_MARGIN)*i);
    //        i++;
    //    }
    //}
    QList<_SwPerspectiveButton *>::iterator it = _buttons.begin();
    if( it != _buttons.end() )
    {
        ( *it )->setChecked( true );
    }
}
/** @brief sur changement de checked*/
void _SwPerspectivesManager::onPerspectiveToggle( ISwPerspective * perspective, bool checked )
{
    if( checked )
    {
        _perspectiveWidget->hide();
        if( _currentPerspective != 0 )
        {
            _currentPerspective->destroyPerspectiveView( _perspectiveWidget );
        }
        _currentPerspective = perspective;
        _currentPerspective->buildPerspectiveView( _perspectiveWidget );
        _perspectiveWidget->show();
        for( int i = 0; i < _buttons.count(); i++ )
        {
            if( _buttons[i]->getPerspective() != _currentPerspective )
                _buttons[i]->setChecked( false );
        }
    }
    else
    {
        if( _currentPerspective == perspective )
        {
            _currentPerspective->destroyPerspectiveView( _perspectiveWidget );
            _currentPerspective = 0;
        }
    }
}
