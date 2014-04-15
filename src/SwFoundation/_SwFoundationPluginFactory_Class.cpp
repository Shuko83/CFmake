/*!
 \file _SwFoundationPluginFactory_Class.cpp
 \brief Factory de fondation 
 \date 02/04/2014
 \version 1.0
 \author EPO
*/

#include <QRegExp>
#include <QTime>

#include "SwApplication.h"
#include "ISwServiceExtensions.h"

#include "_SwFoundationPluginFactory_Class.h"
#include "SwServiceUnitSIProvider.h"

using namespace StreamWork::SwFoundation;

/*! \brief Constructeur */
_SwFoundationPluginFactory_Class::_SwFoundationPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwFoundationPluginFactory_Class::~_SwFoundationPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwFoundationPluginFactory_Class::Initialize() {
	RegisterComponent("SwServiceUnitSIProvider","Permet de provider les properties du service unit SI");

}
/*! \brief Liberation */
void _SwFoundationPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwFoundationPluginFactory_Class::CreateInstanceOf(QString name) {
   
	if (name=="SwServiceUnitSIProvider") {
		return new SwServiceUnitSIProvider;
	}
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwFoundationPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
	if (name=="SwServiceUnitSIProvider") {
		return ico;
	}
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwFoundationPluginFactory_Class::GetPluginVersion(){
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

//-------------------------------------------------------------------------
double _SwFoundationPluginFactory_Class::GetPluginCompilationDate(){	
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