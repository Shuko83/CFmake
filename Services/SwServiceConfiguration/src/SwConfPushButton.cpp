#include "SwConfPushButton.h"

SwConfPushButton::SwConfPushButton()
    : _swServiceConfigurationHelper(CG_SW_SERVICE_SAVECONFIGURATION)
	, _confName("")
	, _pb(new QPushButton())
{
    connect( _pb, &QPushButton::clicked, this, &SwConfPushButton::onClicked );
}

SwConfPushButton::~SwConfPushButton()
{
    unprovideInterface( "Widget" );

	delete _pb;
}

void SwConfPushButton::onClicked()
{
    if( _swServiceConfigurationHelper.getService() )
    {
        _swServiceConfigurationHelper.getService()->saveConfigurationFile( _confName );
    }
}

void SwConfPushButton::initializeComponent() throw(StreamWork::SwCore::SwException)
{
    createPropertiesForThisObject();
    createPropertiesForQObject( _pb, "QPushButton" );
    provideInterface<QWidget>( "Widget", _pb);
}
