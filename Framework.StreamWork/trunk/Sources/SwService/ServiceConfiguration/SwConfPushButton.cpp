#include "SwConfPushButton.h"



SwConfPushButton::SwConfPushButton()
{
    _confName = "";
    _iSwServiceConfiguration = 0;
    _swServiceConfigurationHelper = new SwServiceManager_Helper<ISwServiceConfiguration, SwConfPushButton>;
    _swServiceConfigurationHelper->setService(CG_SW_SERVICE_SAVECONFIGURATION, this, &SwConfPushButton::onServiceConfigurationChange);

    connect(&_pb, &QPushButton::clicked, this, &SwConfPushButton::onClicked);
}

SwConfPushButton::~SwConfPushButton()
{
    unprovideInterface("ISwWidget");
}



void SwConfPushButton::onServiceConfigurationChange()
{
    if (_swServiceConfigurationHelper)
    {
        _iSwServiceConfiguration = _swServiceConfigurationHelper->getService();
    }
}

void SwConfPushButton::onClicked()
{
    if (_iSwServiceConfiguration)
    {
        _iSwServiceConfiguration->saveConfigurationFile(_confName);
    }
}

void SwConfPushButton::initializeComponent()
{
   createPropertiesForThisObject();
   createPropertiesForQObject(&_pb,"QPushButton");
   provideInterface<ISwWidget>("ISwWidget",this);
}

QWidget & SwConfPushButton::GetWidget()
{
    return _pb;
}


