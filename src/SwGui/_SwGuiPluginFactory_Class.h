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
#include <SwPluginFactory_Class.h>
#include "_SwServiceShortcuts.h"


using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwGuiPluginFactory_Class : public SwPluginFactory_Class
{ 
protected:
    /*! \brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
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
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate();

	/*! \brief finalisation de l'initialisation DECLARER LES SERVICES GLOBAUX ICI*/
	void FinalizeInitialisation();

	//---------------------------------------------------------------------
	// Interface ISwServicesManager_Listener
	//---------------------------------------------------------------------
	/*! \brief sur ajout d'un service */
	void OnRegisterService(ISwService * service);
	/*! \brief sur suppression d'une  interface */
	void OnUnregisterService(ISwService * service);

private:
	/** @brief service d'enregistrement */
	_SwServiceShortcuts *_serviceShortcuts;
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new _SwGuiPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
    return new _SwGuiPluginFactory_Class();    
}
#endif
#endif 
