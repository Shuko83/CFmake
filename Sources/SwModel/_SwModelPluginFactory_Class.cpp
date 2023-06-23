/*!
 \file _SwModelPluginFactory_Class.cpp
 \brief Factory de modeles
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
 */

#include <QRegularExpression>
#include <SwLoader_Class.h>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

#include <SwApplication.h>
#include <SwException.h>
#include <SwMacros.h>
#include "_SwModelPluginFactory_Class.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"
#include "SwSnapShotPropertiesService.h"
#include "ISwEditionService.h"
#include "SwModelsListAccess.h"
#include "info_SwModel.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwModel;


//-----------------------------------------------------------------------
_SwModelPluginFactory_Class::_SwModelPluginFactory_Class() :SwProtectedPluginFactory_Class()
{

}

//-----------------------------------------------------------------------
_SwModelPluginFactory_Class::~_SwModelPluginFactory_Class()
{

}

//-----------------------------------------------------------------------
void _SwModelPluginFactory_Class::Initialize()
{
	QMap<QString, _SwModelsList::_ModelDesc>::const_iterator it;

	SwModelsListAccess::getInstance()->getInternal()->registerListener(this);

	//Chargement des modeles
	SwModelsListAccess::getInstance()->getInternal()->LoadModels();

	//Enregistrement du modele host
	RegisterComponent("SwModelHost", "Model host");

	//Enregistrement d'un modele vide
	RegisterComponent("SwModel", "Empty Model");

	//Enregistrement des modeles
	for ( it = SwModelsListAccess::getInstance()->getInternal()->GetModelList()->begin(); it != SwModelsListAccess::getInstance()->getInternal()->GetModelList()->end(); it++ )
		RegisterComponent(it.key(), it.value()._model_description);
}

//-----------------------------------------------------------------------
void _SwModelPluginFactory_Class::FinalizeInitialisation()
{
	SW_APP->RegisterService(SwModelsListAccess::getInstance());
}

//-----------------------------------------------------------------------
void _SwModelPluginFactory_Class::modelAdded(QString name, QString description)
{
	RegisterComponent(name, description);
	SW_APP->ComponentsBank().RereadPluginContent(this);
}

//-----------------------------------------------------------------------
void _SwModelPluginFactory_Class::Liberate()
{
    try
    {
        SW_APP->UnregisterService(SwModelsListAccess::getInstance()->GetServiceName());
    }
    catch (SwException& e)
    {
        qCritical() << e.GetReason();
    }
}

//-----------------------------------------------------------------------
SwComponent_Class * _SwModelPluginFactory_Class::CreateInstanceOf(QString name)
{
	QMap<QString, _SwModelsList::_ModelDesc>::const_iterator it;

	if ( name == "SwModelHost" )
		return new _SwModelHost_Class;
	if ( name == "SwModel" )
		return new _SwModel_Class;

	it = SwModelsListAccess::getInstance()->getInternal()->GetModelList()->find(name);
	if ( it != SwModelsListAccess::getInstance()->getInternal()->GetModelList()->end() )
	{
		QFile file;
		QDomDocument doc;
		QString xml_error;
		int error_line, error_column;
		SwComponent_ClassPtr root_component;

		file.setFileName(it.value()._model_source_file);
		if ( !file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			QString msg = QString("Fail to open following model: %1").arg(it.value()._model_source_file);
			LAUNCH_SWEXCEPTION("SwModel", msg)
		}
		if ( !doc.setContent(QString(file.readAll()), &xml_error, &error_line, &error_column) )
		{
			QString msg = QString("XML Parsing:%1 at position %2,%3 of %4").arg(xml_error).arg(error_line).arg(error_column).arg(it.value()._model_source_file);
			LAUNCH_SWEXCEPTION("SwModel", msg)
		}
		SwLoader_Class loader;
		if ( SW_APP->IsVerbose() )
			SW_APP->Logger().Log(LogLvl_Info, QString("Loading model %1\n").arg(it.value()._model_source_file));
		root_component = loader.Load(doc);
		if ( root_component == NULL )
		{
			LAUNCH_SWEXCEPTION("SwCore", "Unable to build stream or stream is empty");
		}
		if ( GET_SW_EDITION_SERVICE != 0 )
		{
			SwSnapShotPropertiesVisitor v;
			root_component->AcceptVisitor(&v);
		}
		return new _SwModel_Class(it.key(), root_component, it.value()._model_host_path, it.value()._model_source_file);
	}

	return NULL;
}

//-----------------------------------------------------------------------
SwData_Class * _SwModelPluginFactory_Class::CreateInstanceOf(unsigned int type_id)
{
	return NULL;
}

//-----------------------------------------------------------------------
QIcon _SwModelPluginFactory_Class::CreateIconOf(QString name) const
{
	QIcon ico;
	QMap<QString, _SwModelsList::_ModelDesc>::const_iterator it;
	if ( name == "SwModelHost" )
	{
		return QIcon(":/SwModel/model_host.png");
	}
	if ( name == "SwModel" )
	{
		return QIcon(":/SwModel/model.png");
	}
	it = SwModelsListAccess::getInstance()->getInternal()->GetModelList()->find(name);
	if ( it != SwModelsListAccess::getInstance()->getInternal()->GetModelList()->end() )
	{
		return it.value()._model_ico;
	}
	return ico;
}

//-----------------------------------------------------------------------
QString _SwModelPluginFactory_Class::GetPluginVersion()
{
	QString build;

	build = QString("%1 %2").arg(__DATE__).arg(__TIME__);
	build = build.toUpper();
	build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
	build += " (Debug)";
#else 
	build+=" (Release)";
#endif
	return build;
}

//-----------------------------------------------------------------------
QDateTime _SwModelPluginFactory_Class::GetPluginCompilationDate()
{
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime, "hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return finalDateTime;
}

//----------------------------------------------------------------------------------------------
QString _SwModelPluginFactory_Class::GetPluginName() const
{
	return SwModel::name();
}

//-----------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwModelPluginFactory_Class;
}
