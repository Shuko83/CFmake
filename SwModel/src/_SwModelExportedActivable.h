/*!
 \file _SwModelExportedActivable.h
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedActivable_H
#define __SwModelExportedActivable_H

#include <QString>
#include "Component/Base/SwComponent_Class.h"
#include "_SwModelExportedEntity.h"
#include "Properties/ISwProperty.h"
#include "Component/Services/ISwProperties.h"
#include "Tools/Signal/LibIndeSig.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Component/Interfaces/ISwActivable.h"

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedActivable 
	\brief Class implementant une entite exportable activable
*/
class _SwModelExportedActivable : 
    public _SwModelExportedEntity , 
    virtual public ISwActivable{

	public:
		/*! \brief Constructeur */
		_SwModelExportedActivable();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedActivable();
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
    
		

		/*! \brief Service d'executable interne*/
		ISwActivable * _internal_activable;

	protected:

	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();

        bool _isActive;
};

#endif
