/**
 * @file _SwRecordPluginFactory_Class.cpp
 * @brief Plugin Factory
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */


#include <QRegExp>

#include "_SwRecordPluginFactory_Class.h"
#include "SwApplication.h"
#include "_RecordManager.h"
#include "_RecordPoint.h"
#include "_ReplayManager.h"
#include "_SwRecordDataCodecDefaultFactory.h"

/** @brief Constructeur */
_SwRecordPluginFactory_Class::_SwRecordPluginFactory_Class():SwPluginFactory_Class() {

}
/** @brief Destructeur */
_SwRecordPluginFactory_Class::~_SwRecordPluginFactory_Class() {

}
/** @brief Initialisation */
void _SwRecordPluginFactory_Class::Initialize() {
	RegisterComponent("RecordManager","Record Manager");
	RegisterComponent("ReplayManager","Replay Manager");
	RegisterComponent("RecordPoint","Record Point");
}
/*! \brief finalisation de l'initialisation DECLARER LES SERVICES GLOBAUX ICI*/
void _SwRecordPluginFactory_Class::FinalizeInitialisation() {
    _serviceRecording =new _SwServiceRecording();
    SW_APP->RegisterService(_serviceRecording);
}
/** @brief Liberation */
void _SwRecordPluginFactory_Class::Liberate() {

}
/** @brief instanciation d'un composant */
SwComponent_Class * _SwRecordPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="RecordManager") {
        return new _RecordManager(_serviceRecording);
    }
    if (name=="ReplayManager") {
        return new _ReplayManager(_serviceRecording);
    }
    if (name=="RecordPoint") {
        return new _RecordPoint;
    }
    return NULL;
}
/** @brief instanciation d'une data */
SwData_Class * _SwRecordPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) {
    return NULL;
}
/** 
 * @brief acces a l'icone d'un composant
 * @param[in] name nom du composant
 * @return l'incone ou icone vide si non trouvé
 */
QIcon _SwRecordPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="RecordManager") {
        return QIcon(":/SwRecord/record_icon.png");
    }
    if (name=="ReplayManager") {
        return QIcon(":/SwRecord/replay_icon.png");
    }
    if (name=="RecordPoint") {
        return QIcon(":/SwRecord/record_point_icon.png");
    }
    return ico;
}
/** 
 * @brief Acces a la version du plugin
 * @return version
 */
QString _SwRecordPluginFactory_Class::GetPluginVersion(){
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

//---------------------------------------------------------------------
// Interface ISwServicesManager_Listener
//---------------------------------------------------------------------
/*! \brief sur ajout d'un service */
void _SwRecordPluginFactory_Class::OnRegisterService(ISwService * service) {
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(service);
    if (rservice!=0)
        rservice->registerCodecFactory(new _SwRecordDataCodecDefaultFactory());
}
/*! \brief sur suppression d'une  interface */
void _SwRecordPluginFactory_Class::OnUnregisterService(ISwService * service) {

}

double _SwRecordPluginFactory_Class::GetPluginCompilationDate(){	
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

//---------------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwRecordPluginFactory_Class;    
}
