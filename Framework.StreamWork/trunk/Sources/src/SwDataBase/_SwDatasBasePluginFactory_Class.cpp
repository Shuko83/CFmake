/*!
 \file _SwDatasBasePluginFactory_Class.cpp
 \brief Factory de gui de base 
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>
#include <QDateTime>
#include <QLocale>

#include "_SwDatasBasePluginFactory_Class.h"
#include "_SwDatasBaseBasicRouter.h"
#include "SwDataBasicBuffer_Class.h"
#include "SwMacros.h"
#include "_SwSimpleDataExecutable.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwDatasBasePluginFactory_Class::_SwDatasBasePluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwDatasBasePluginFactory_Class::~_SwDatasBasePluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwDatasBasePluginFactory_Class::Initialize() {
    RegisterComponent("SwDatasBaseBasicRouter","Basic router for any data");
    RegisterComponent("SwSimpleDataExecutable","Sample data generator");
    RegisterData(CG_SW_DATA_BASIC_BUFFER_ID,CG_SW_DATA_BASIC_BUFFER_NAME);
}
/*! \brief Liberation */
void _SwDatasBasePluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwDatasBasePluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwDatasBaseBasicRouter") {
        return new _SwDatasBaseBasicRouter;
    }
    if (name=="SwSimpleDataExecutable") {
        return new _SwSimpleDataExecutable;
    }
    return NULL;
}
/*! \brief instanciation d'une data */
SwData_Class * _SwDatasBasePluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) {

    if (type_id==CG_SW_DATA_BASIC_BUFFER_ID) {
        return new SwDataBasicBuffer_Class();
    }
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwDatasBasePluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="SwDatasBaseBasicRouter") {
        return QIcon(":/SwDatasBase/router.png"); 
    }
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwDatasBasePluginFactory_Class::GetPluginVersion(){
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
double _SwDatasBasePluginFactory_Class::GetPluginCompilationDate(){	
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

