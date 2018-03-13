#include "SwConfPushButton.h"

SwConfPushButton::SwConfPushButton()
{
    _confName = "";
    _swServiceConfigurationHelper = new SwServiceManager_Helper<ISwServiceConfiguration>;
    _swServiceConfigurationHelper->setService( CG_SW_SERVICE_SAVECONFIGURATION );
    
    connect( &_pb, &QPushButton::clicked, this, &SwConfPushButton::onClicked );
}

SwConfPushButton::~SwConfPushButton()
{
    unprovideInterface( "ISwWidget" );
    
    delete _swServiceConfigurationHelper;
}

void SwConfPushButton::onClicked()
{
    if( _swServiceConfigurationHelper->getService() )
    {
        _swServiceConfigurationHelper->getService()->saveConfigurationFile( _confName );
    }
}

void SwConfPushButton::initializeComponent()
{
    createPropertiesForThisObject();
    createPropertiesForQObject( &_pb, "QPushButton" );
    provideInterface<ISwWidget>( "ISwWidget", this );
}

QWidget * SwConfPushButton::GetWidget()
{
    return &_pb;
}
