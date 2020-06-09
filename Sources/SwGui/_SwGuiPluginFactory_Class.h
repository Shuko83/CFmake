/*!
 \file _SwGuiPluginFactory_Class.h
 \brief Factory de gui de base
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
 */

#ifndef __SwPluginFactory_Class_H
#define __SwPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include "SwGuiConstantes.h"
#include "SwProtectedPluginFactory_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class
	\brief classe definissant un plugin usine
	*/
class _SwGuiPluginFactory_Class : public SwProtectedPluginFactory_Class
{
protected:
	/*! \brief instanciation d'un composant */
	SwComponent_Class * CreateInstanceOf(QString name);
	/*! \brief acces a l'icone d'un composant */
	QIcon CreateIconOf(QString name) const;
public:
	/*! \brief Constructeur */
	_SwGuiPluginFactory_Class();
	/*! \brief Destructeur */
	~_SwGuiPluginFactory_Class();
	/*! \brief Initialisation */
	void Initialize();
	/*! \brief Liberation */
	void Liberate();
	/*! \brief Acces a la version du plugin */
	QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin */
	QDateTime GetPluginCompilationDate() override;

	/*! \brief finalisation de l'initialisation DECLARER LES SERVICES GLOBAUX ICI*/
	void FinalizeInitialisation();

	//---------------------------------------------------------------------
	// Interface ISwServicesManager_Listener
	//---------------------------------------------------------------------
	/*! \brief sur ajout d'un service */
	void OnRegisterService(ISwService * service);
	/*! \brief sur suppression d'une  interface */
	void OnUnregisterService(ISwService * service);

	QString GetPluginName() const override;

private:
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();
#endif 
