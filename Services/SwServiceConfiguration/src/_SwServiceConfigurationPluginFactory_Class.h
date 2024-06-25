#ifndef _SwServiceConfigurationPluginFactory_Class_H
#define _SwServiceConfigurationPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>

/*
  * INCLUDES LOCAUX
  */
#include "Main/Plugin/SwProtectedPluginFactory_Class.h"
#include "SwServiceSaveConfiguration.h"

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class
	\brief classe definissant un plugin usine
	*/
class _SwServiceConfigurationPluginFactory_Class : public SwProtectedPluginFactory_Class
{
protected:
	/*! \brief instanciation d'un composant */
	SwComponent_Class * CreateInstanceOf(QString name);
	/*! \brief acces a l'icone d'un composant */
	QIcon CreateIconOf(QString name) const;
	/*! \brief instanciation d'une data */
	SwData_Class * CreateInstanceOf(const SwUUID & type_id);
public:
	/*! \brief Constructeur */
	_SwServiceConfigurationPluginFactory_Class();
	/*! \brief Destructeur */
	~_SwServiceConfigurationPluginFactory_Class();
	/*! \brief Initialisation */
	void Initialize();
	/*! \brief Liberation */
	void Liberate();
	/*! \brief Acces a la version du plugin */
	QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin */
	QDateTime GetPluginCompilationDate() override;

	QString GetPluginName() const override;

private:
	SwServiceSaveConfiguration * _serviceconfiguration;
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();

#endif 
