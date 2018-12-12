/*!
 \file _SwConfigurationExportedExecution.h
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfigurationExportedExecution_H
#define __SwConfigurationExportedExecution_H

#include <QString>
#include <SwComponent_Class.h>
#include "_SwConfigurationExportedEntity.h"
#include <ISwProperty.h>
#include <ISwProperties.h>
#include <LibIndeSig.h>
#include <SwProperties_Class.h>
#include <ISwExecution_Service.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
/*!
	\class _SwConfigurationExportedExecution 
	\brief Class implementant une entite exportable executable
*/
class _SwConfigurationExportedExecution : 
    public _SwConfigurationExportedEntity , 
    public ISwExecution_Service{
	public:
		/*! \brief Service d'executable interne*/
		ISwExecution_Service * _internal_execution;
	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	public:
		/*! \brief Constructeur */
		_SwConfigurationExportedExecution();
	    /*! \brief Destructeur */
		virtual ~_SwConfigurationExportedExecution();
		

        //---------------------------------------------------------------------
        // Interface ISwService
        //---------------------------------------------------------------------
        /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
        lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
        void Liberate();  

        //---------------------------------------------------------------------
        // Interface IExecution_SwService
        //---------------------------------------------------------------------
        
	    /*! \brief Demarrage de l'execution */
        virtual void StartExecution();
	    /*! \brief demande d'arret de l'execution*/
        virtual void AskForStopExecution();
	    /*! \brief demande d'arret de l'execution*/
        virtual bool IsExecutionStopped();
	    /*! \brief Arret de l'execution */
        virtual void StopExecution();
	    /*! \brief definition du fournisseur de temps */
        virtual void SetClockProvider(ISwClockProvider * provider);   
        //---------------------------------------------------------------------
        // Interface ISwPersistant
        //---------------------------------------------------------------------
        
        /*! \brief methode permettant de charger des donnees
        \param[in] elt neoud parent
        \param[in] finalizer_manager manager de finalisation
        */
        virtual void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
        /*! \brief methode permettant de sauver des donnees
        \param[in] elt neoud parent
        \param[in] doc document parent
        */
        virtual void Save(QXmlStreamWriter &);	
        //---------------------------------------------------------------------
        // Interface ISwHost
        //---------------------------------------------------------------------
        
        /*! \brief acces a son composant hote */
        virtual SwComponent_Class * GetHostComponent();   
};

#endif