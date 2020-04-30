#pragma once

#include <QPushButton>
#include "ISwServiceConfiguration.h"
#include "SwServiceManager_Helper.h"
#include "SwAssistedComponent.h"

// Push Button

//destine a permettre un save sur un conf précise a partir d' nimporte ou

//On peut l'intégrer dans un widget dans l'application dont on veut sauver la conf

//Du moment qu'on a ajouté un confCollector

class SwConfPushButton : public StreamWork::SwFoundation::SwAssistedComponent
{
    Q_OBJECT
    Q_PROPERTY( QString confName READ getConfName WRITE setConfName )
public:
    SwConfPushButton();
    ~SwConfPushButton();
    
    void initializeComponent();
    
    QString getConfName() const { return _confName; }
    void setConfName( QString val ) { _confName = val; }
    
public slots:
    void onClicked();
    
private:
    QPushButton* _pb;
    
    //Service de conf
    SwServiceManager_Helper<ISwServiceConfiguration> _swServiceConfigurationHelper;
    
    QString _confName;
};
