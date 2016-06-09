#pragma once

/*
* INCLUDES QT
*/
#include <QString>
#include <QMap>

/*
* INCLUDES LOCAUX
*/
#include "SwServiceContextMenu.h"

/*
* INCLUDES GLOBAUX
*/
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/*!
\class SwPluginFactory_Class
\brief classe definissant un plugin usine
*/
class SwServiceContextMenuPluginFactory_Class : public SwPluginFactory_Class
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
	SwServiceContextMenuPluginFactory_Class();
	/*! \brief Destructeur */
	~SwServiceContextMenuPluginFactory_Class();
	/*! \brief Initialisation */
	void Initialize();
	/*! \brief Liberation */
	void Liberate();
	/*! \brief Acces a la version du plugin */
	QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate();

private:
	/**
	* @var		_contextMenuService
	* @brief	Pointeur vers le service de context menu
	**/
	StreamWork::Service::SwServiceContextMenu *_contextMenuService;
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD(){
	return new SwServiceContextMenuPluginFactory_Class();
}

#else 
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
	return new SwServiceContextMenuPluginFactory_Class();
}
#endif

