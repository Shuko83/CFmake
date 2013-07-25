/*!
 \file WizardIDEPluginFactory_Class.cpp
 \brief Wizard for plugins generation
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <QRegExp>
#include <QTime>

#include "WizardIDEPluginFactory_Class.h"
#include "SwWizard.h"
#include "NodeSwFactoryEntry.h"
#include "NodeComponents.h"
#include "NodeConnectors.h"
#include "NodeProperties.h"
#include "NodeInterfaces.h"
#include "NodeDescription.h"

/*! \brief Constructeur */
WizardIDEPluginFactory_Class::WizardIDEPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
WizardIDEPluginFactory_Class::~WizardIDEPluginFactory_Class() {

}
/*! \brief Initialisation */
void WizardIDEPluginFactory_Class::Initialize() {
	RegisterComponent("SwWizard","Component Wizard");
	RegisterComponent("NodeSwFactoryEntry","NodeSwFactoryEntry");
	RegisterComponent("NodeComponents","NodeComponents");
	RegisterComponent("NodeConnectors","NodeConnectors");
	RegisterComponent("NodeProperties","NodeProperties");
	RegisterComponent("NodeInterfaces","NodeInterfaces");
	RegisterComponent("NodeDescription","NodeDescription");
}
/*! \brief Liberation */
void WizardIDEPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * WizardIDEPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwWizard") {
        return new SwWizard;
    }
    if (name=="NodeSwFactoryEntry") {
        return new NodeSwFactoryEntry;
    }
    if (name=="NodeComponents") {
        return new NodeComponents;
    }
    if (name=="NodeConnectors") {
        return new NodeConnectors;
    }
    if (name=="NodeProperties") {
        return new NodeProperties;
    }
    if (name=="NodeInterfaces") {
        return new NodeInterfaces;
    }
    if (name=="NodeDescription") {
        return new NodeDescription;
    }
    return NULL;
}
/*! \brief instanciation d'une data */
SwData_Class * WizardIDEPluginFactory_Class::CreateInstanceOf(unsigned int type_id) {
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon WizardIDEPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="SwWizard") {
        return QIcon(":/WizardIDE/comp_edit.png");
    }
    if (name=="NodeSwFactoryEntry") {
        return QIcon(":/WizardIDE/comp_toolbox.png");
    }
    if (name=="NodeComponents") {
        return QIcon(":/WizardIDE/router.png");
    }
    if (name=="NodeConnectors") {
        return QIcon(":/WizardIDE/connector.png");
    }
    if (name=="NodeProperties") {
        return QIcon(":/WizardIDE/property.png");
    }
    if (name=="NodeInterfaces") {
        return QIcon(":/WizardIDE/interface_out.png");
    }
    if (name=="NodeDescription") {
        return QIcon(":/WizardIDE/comp_edit.png");
    }
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString WizardIDEPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}
double WizardIDEPluginFactory_Class::GetPluginCompilationDate(){	
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime,"hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return (finalDateTime.toMSecsSinceEpoch() / 1000.0);//retourne la date en secondes depuis le 1er janvier 1970

}

