/*!
 \file _SwModelExportedOwnerConfigurable.h
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedOwnerConfigurable_H
#define __SwModelExportedOwnerConfigurable_H

#include <QString>
#include <SwComponent_Class.h>
#include "_SwModelExportedEntity.h"
#include <ISwProperty.h>
#include <ISwProperties.h>
#include <LibIndeSig.h>
#include <SwProperties_Class.h>
#include <ISwServiceOwnerConfigurable.h>
#include <ISwObservable.h>
#include <ISwObserver.h>

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedOwnerConfigurable 
	\brief Class implementant une entite exportable executable
*/
class _SwModelExportedOwnerConfigurable : 
        public _SwModelExportedEntity , 
        public ISwServiceOwnerConfigurable,
        public StreamWork::SwCore::ISwObservable,
        public StreamWork::SwCore::ISwObserver{

	public:
		/*! \brief Service d'executable interne*/
		ISwServiceOwnerConfigurable * _internal_ownerConfigurable;

        QList<ISwObserver *>          _configurationObservers;


	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	public:
		/*! \brief Constructeur */
		_SwModelExportedOwnerConfigurable();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedOwnerConfigurable();
		//---------------------------------------------------------------------
		// Interface ISwService
		//---------------------------------------------------------------------            
		/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
		lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
		void Liberate();  
      
		//---------------------------------------------------------------------
		// Interface ISwServiceOwnerConfigurable
		//---------------------------------------------------------------------
        /*! \brief methode permettant de charger des donnees de configuration*/
        virtual void LoadConfiguration(QDomElement &);
        /*! \brief methode permettant de sauver des donnees de configuration*/
        virtual void SaveConfiguration(QDomElement &,QDomDocument &);
        
		
        //----------------------------------------------------
        // Interface ISwServiceObservers pour ISwServiceOwnerConfigurable
        //----------------------------------------------------
        /*! \brief Permet d'attacher un observer*/
        virtual void Attach(ISwObserver * confChangeObserver);
        /*! \brief Permet de detacher un observer*/
        virtual void Detach(ISwObserver * confChangeObserver);
        /*! \brief Permet notifier aux observers un changement */
        virtual void Notify();    
        //----------------------------------------------------
        // Interface ISwServiceObservable pour ISwServiceOwnerConfigurable
        //----------------------------------------------------
		virtual void Update(StreamWork::SwCore::ISwObservable* sender = nullptr);
};

#endif