/*!
 \file _SwModelExportedExecutable.h
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedExecutable_H
#define __SwModelExportedExecutable_H

#include <QString>
#include "Component/Base/SwComponent_Class.h"
#include "_SwModelExportedEntity.h"
#include "Properties/ISwProperty.h"
#include "Component/Services/ISwProperties.h"
#include "Tools/Signal/LibIndeSig.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "ISwExecutable_Service.h"
#include "ISwSupportReplay.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
/*!
	\class _SwModelExportedExecutable 
	\brief Class implementant une entite exportable executable
*/
class _SwModelExportedExecutable : 
    public _SwModelExportedEntity , 
    virtual public ISwExecutable_Service, 
    public ISwSupportReplay{

	public:
		/*! \brief Constructeur */
		_SwModelExportedExecutable();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedExecutable();
		//---------------------------------------------------------------------
		// Interface ISwService
		//---------------------------------------------------------------------            
		/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
		lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
		void Liberate();  
		//---------------------------------------------------------------------
		// Interface ISwHost
		//---------------------------------------------------------------------
		/*! \brief acces a son composant hote */
		SwComponent_Class * GetHostComponent();     

        //---------------------------------------------------------------------
		// Interface ISwActivable
		//---------------------------------------------------------------------
        /*! \brief Permet de changer la valeur d'activation
        \param[in] value d'activation
        */
        virtual void setActive(bool value);
        /*! \brief Permet de lire la valeur d'activation
        \param[out] valeur d'activation
        */
        virtual bool isActive();
		//---------------------------------------------------------------------
		// Interface ISwExecutable_Service
		//---------------------------------------------------------------------
		/*! \brief Initialisation */
		void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);            
		/*! \brief Demarrage */
		void Start(double current_time) throw (SwException);            
		/*! \brief Execution */
		void Execute(double current_time,bool is_first_call) throw (SwException);            
		/*! \brief Execution */
		void Stop(double current_time);            
		//---------------------------------------------------------------------
		// Interface ISwSupportReplay
		//---------------------------------------------------------------------
        /*@brief setter replay mode */
        void setReplayMode(bool replayMode);
        /*@brief getter replay mode */
        bool getReplayMode();
		

		/*! \brief Service d'executable interne*/
		ISwExecutable_Service * _internal_executable;

	protected:

	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();

        bool _isActive;
};

#endif
