/**
@file _SwBasicPerspective.cpp
@brief Perspective basique
@author F.Bighelli
 */
 
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwBasicPerspective.h"
 
using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwBasicPerspective::_SwBasicPerspective():SwDefaultPerspective() {
	_wHeader=0;
    _wHighPart=0;
    _wLowPart=0;
    _wFooter=0;
    _splitter=0;
    _layout=0;
    _enabledPerspective=false;
    _isCurrent=false;
    _title=0;
	_title_stylesheet="QFrame#Title { border: 1px solid black;  border-radius: 5px; background: \
					  qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 rgb(60,80,150,80%), stop: 1 rgb(60,80,150,20%));}\
					  QLabel#textLabel {color: white; font: bold 12px;}";
}
/*! \brief Destructeur */
_SwBasicPerspective::~_SwBasicPerspective(){
	if (_isCurrent)
        destroyPerspectiveView(0);
    delete _splitter;
    delete _layout;
}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwBasicPerspective::InitializeResources() throw(SwException){
    SwDefaultPerspective::InitializeResources();

    _consumer_service->RegisterConsumedInterface<ISwWidget>("header",&_wHeader);
    _consumer_service->RegisterConsumedInterface<ISwWidget>("highPart",&_wHighPart);
    _consumer_service->RegisterConsumedInterface<ISwWidget>("lowPart",&_wLowPart);
    _consumer_service->RegisterConsumedInterface<ISwWidget>("footer",&_wFooter);

    _splitter=new QSplitter(Qt::Vertical);
    _layout=new QVBoxLayout();
    _layout->setMargin(1);
    _layout->setSpacing(1);
	
	//Gestion du stylesheet
    _title_stylesheet_property=_properties_service->CreateProperty<QString>("TitleStyleSheet");
    if (_title_stylesheet_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register TitleStyleSheet property\n"));
    }
    _title_stylesheet_property->SetDescription("Define title stylesheet");  
    _title_stylesheet_property->SetValue(QVariant(_title_stylesheet));
    _title_stylesheet_property->GetOnChangeSignal().iconnect(*this,&_SwBasicPerspective::OnPropertyChange);
}
/*! \brief Callback sur les changements de propriétés*/
void _SwBasicPerspective::OnPropertyChange(ISwProperty * property) {
    if (_title_stylesheet_property==property) {
        _title_stylesheet=property->GetValue().toString();	
		if (_title != 0) {
			_title->setStyleSheet(_title_stylesheet);
		}
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwBasicPerspective::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (_enabledPerspective) {
        if (_wHeader!=0 && interface_name=="header") {
            if (_isCurrent)
                _wHeader->GetWidget().setParent(0);
        } else
        if (_wHighPart!=0 && interface_name=="highPart") {
            if (_wLowPart==0)
                _enabledPerspective=false;
            if (_isCurrent)
                _wHighPart->GetWidget().setParent(0);
        } else
        if (_wLowPart!=0 && interface_name=="lowPart") {
            if (_wHighPart==0)
                _enabledPerspective=false;
            if (_isCurrent)
                _wLowPart->GetWidget().setParent(0);
        } else
        if (_wFooter!=0 && interface_name=="footer") {
            if (_isCurrent)
                _wFooter->GetWidget().setParent(0);
        }
        if (!_enabledPerspective) {
            OnEnablePerspectiveChange();
        }
    }
}            
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwBasicPerspective::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (!_enabledPerspective & ( _wHighPart!=0 || _wLowPart!=0 )) {
        _enabledPerspective=true;
        OnEnablePerspectiveChange();
    }
}            
//---------------------------------------------------------------------
// Interface ISwPerspective
//---------------------------------------------------------------------
/** @brief Construction de la vue */
void _SwBasicPerspective::buildPerspectiveView(QWidget * widget){
    _mtitle=new QFrame(widget);
    _mtitle->setMinimumHeight(34);
    _mtitle->setMaximumHeight(34);
    _title=new QFrame(_mtitle);
    _title->setObjectName("Title");  
    _title->setStyleSheet(_title_stylesheet);
    
    _title->setMinimumHeight(32);
    _title->setMaximumHeight(32);
    QHBoxLayout * tlayout=new QHBoxLayout(_title);
    tlayout->setAlignment(Qt::AlignLeft);
    tlayout->setMargin(2);
    tlayout->setSpacing(5);
    QLabel *limg=new QLabel(widget);
    limg->setPixmap(QIcon(_iconPath).pixmap(QSize(28,28)));	
    tlayout->addWidget(limg);
    QLabel * textLabel=new QLabel(_label+" ",_title);
    textLabel->setObjectName("textLabel");    
    tlayout->addWidget(textLabel);
    _title->setLayout(tlayout);
    //_title->setMaximumWidth(limg->width()+10+textLabel->width());
    QHBoxLayout * titlelayout=new QHBoxLayout(_mtitle);
    titlelayout->setMargin(0);
    titlelayout->setSpacing(5);
    _mtitle->setLayout(titlelayout);
    //titlelayout->addStretch(10);
    titlelayout->addWidget(_title);
    //titlelayout->addStretch(10);
    _layout->addWidget(_mtitle);
    if (_wHeader!=0) {
        _layout->addWidget(&(_wHeader->GetWidget()));
    }
    if (_wHighPart!=0 && _wLowPart!=0) {
        _splitter->addWidget(&(_wHighPart->GetWidget()));
        _splitter->addWidget(&(_wLowPart->GetWidget()));
        _layout->addWidget(_splitter);
    } else {
        if (_wHighPart!=0) {
            _layout->addWidget(&(_wHighPart->GetWidget()));
        }
        if (_wLowPart!=0) {
            _layout->addWidget(&(_wLowPart->GetWidget()));
        }
    }
    if (_wFooter!=0) {
        _layout->addWidget(&(_wFooter->GetWidget()));
    }
    widget->setLayout(_layout);
    _isCurrent=true;
    if (_wHighPart!=0) {
        _wHighPart->GetWidget().setVisible(true);
    }
    if (_wLowPart!=0) {
        _wLowPart->GetWidget().setVisible(true);
    }

}
/** @brief Destruction de la vue */
void _SwBasicPerspective::destroyPerspectiveView(QWidget *widget){
    _isCurrent=false;
    delete _mtitle;
    if (_wHeader!=0) 
        _wHeader->GetWidget().setParent(0);
    if (_splitter!=0) 
        _splitter->setParent(0);
    if (_wHighPart!=0) 
        _wHighPart->GetWidget().setParent(0);
    if (_wLowPart!=0) 
        _wLowPart->GetWidget().setParent(0);
    if (_wFooter!=0) 
        _wFooter->GetWidget().setParent(0);
    delete _layout;
    _layout=new QVBoxLayout();
    _layout->setMargin(1);
    _layout->setSpacing(1);
}
/** @brief Permet de savoir si la persective est active */
bool _SwBasicPerspective::isPerspectiveEnabled(){
	return _enabledPerspective;
}
