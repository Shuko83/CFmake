#include "SwConfPushButton.h"

SwConfPushButton::SwConfPushButton()
    : _swServiceConfigurationHelper(CG_SW_SERVICE_SAVECONFIGURATION)
	, _confName("")
{
    connect( &_pb, &QPushButton::clicked, this, &SwConfPushButton::onClicked );
}

SwConfPushButton::~SwConfPushButton()
{
    unprovideInterface( "ISwWidget" );
}

void SwConfPushButton::onClicked()
{
    if( _swServiceConfigurationHelper.getService() )
    {
        _swServiceConfigurationHelper.getService()->saveConfigurationFile( _confName );
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
