/*!
 \file _SwConfigurationPluginFactory_Class.cpp
 \brief Factory d'execution
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>
#include <QTime>

#include "_SwConfigurationPluginFactory_Class.h"
#include "_SwConfiguration.h"
#include "_SwConfigurationManager.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwConfigurationPluginFactory_Class::_SwConfigurationPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwConfigurationPluginFactory_Class::~_SwConfigurationPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwConfigurationPluginFactory_Class::Initialize() {
    RegisterComponent("SwConfiguration","Configuration Component");
    RegisterComponent("SwConfigManager","Configuration Manager");

}
/*! \brief Liberation */
void _SwConfigurationPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwConfigurationPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwConfiguration") {
        return new _SwConfiguration;
    }
    if (name=="SwConfigManager") {
        return new _SwConfigurationManager;
    }
    return NULL;
}

/*! \brief instanciation d'une data */
SwData_Class * _SwConfigurationPluginFactory_Class::CreateInstanceOf(unsigned int type_id) {
    return NULL;
}

/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwConfigurationPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="SwConfiguration") {
        return QIcon(":/SwConfiguration/configuration.png"); 
    }
    if (name=="SwConfigManager") {
        return QIcon(":/SwConfiguration/configurationManager.png"); 
    }
    return ico;
}

/*! \brief Acces a la version du plugin
\return version*/
QString _SwConfigurationPluginFactory_Class::GetPluginVersion(){
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

double _SwConfigurationPluginFactory_Class::GetPluginCompilationDate(){	
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