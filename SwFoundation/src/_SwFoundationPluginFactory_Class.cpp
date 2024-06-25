/*!
 \file _SwFoundationPluginFactory_Class.cpp
 \brief Factory de fondation 
 \date 02/04/2014
 \version 1.0
 \author EPO
*/

#include <QRegularExpression>
#include <QTime>
#include <QLocale>
#include <QDate>

#include "Main/SwApplication.h"

#include "_SwFoundationPluginFactory_Class.h"

#include "info_SwFoundation.h"

/*! \brief Constructeur */
_SwFoundationPluginFactory_Class::_SwFoundationPluginFactory_Class():SwProtectedPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwFoundationPluginFactory_Class::~_SwFoundationPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwFoundationPluginFactory_Class::Initialize() {

}
/*! \brief Liberation */
void _SwFoundationPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwFoundationPluginFactory_Class::CreateInstanceOf(QString name) {
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwFoundationPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwFoundationPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else
    build+=" (Release)";
#endif
    return build;
}

//-------------------------------------------------------------------------
QDateTime _SwFoundationPluginFactory_Class::GetPluginCompilationDate(){	
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

	return finalDateTime;
}

//-------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwFoundationPluginFactory_Class;
}

//-------------------------------------------------------------------------
QString _SwFoundationPluginFactory_Class::GetPluginName() const
{
	return SwFoundation::name();
}
