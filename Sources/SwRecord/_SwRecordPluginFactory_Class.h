/**
 * @file _SwRecordPluginFactory_Class.h
 * @brief Plugin Factory
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#ifndef __SwRecordPluginFactory_Class_H
#define __SwRecordPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
#include "SwRecordConstantes.h"
#include "_SwServiceRecording.h"

/*
  * INCLUDES LOCAUX
  */
#include "Main/Plugin/SwPluginFactory_Class.h"

using namespace StreamWork::SwCore;

/**
 *	@class _SwRecordPluginFactory_Class
 *	@brief classe definissant un plugin usine
 */
class _SwRecordPluginFactory_Class : public SwPluginFactory_Class
{
private:
    /** @brief service d'enregistrement */
    _SwServiceRecording *_serviceRecording;
protected:
    /** @brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /** @brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
    /** @brief instanciation d'une data */
    SwData_Class * CreateInstanceOf(const SwUUID & type_id);
public:
    /** @brief Constructeur */
    _SwRecordPluginFactory_Class();
    /** @brief Destructeur */
    virtual ~_SwRecordPluginFactory_Class();
    /** @brief Initialisation */
    void Initialize();
    /*! \brief finalisation de l'initialisation DECLARER LES SERVICES GLOBAUX ICI*/
    void FinalizeInitialisation();
    /** @brief Liberation */
    void Liberate();
    /** @brief Acces a la version du plugin */
    QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin */
    QDateTime GetPluginCompilationDate() override;	
    /*! \brief Acces au nom du plugin*/
    QString GetPluginName() const override;
    //---------------------------------------------------------------------
    // Interface ISwServicesManager_Listener
    //---------------------------------------------------------------------
    /*! \brief sur ajout d'un service */
    void OnRegisterService(ISwService * service) override;
    /*! \brief sur suppression d'une  interface */
    void OnUnregisterService(ISwService * service) override;
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();

#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
