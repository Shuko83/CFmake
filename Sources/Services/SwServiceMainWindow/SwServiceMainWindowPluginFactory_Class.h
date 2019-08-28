#pragma once

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>

/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class
	\brief classe definissant un plugin usine
	*/
class SwServiceMainWindowPluginFactory_Class : public SwPluginFactory_Class
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
	SwServiceMainWindowPluginFactory_Class();
	/*! \brief Destructeur */
	~SwServiceMainWindowPluginFactory_Class();
	/*! \brief Initialisation */
	void Initialize();
	/*! \brief Liberation */
	void Liberate();
	/*! \brief Acces a la version du plugin */
	QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate();
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();
