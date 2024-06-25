/*!
\file ISwPersistent.h
\date 18/04/2006
\brief interface de chragement/sauvegarde admin
\author  Big
\version 1.0
 */

#ifndef _ISwPersistent_H
#define _ISwPersistent_H
/*
  * INCLUDES GLOBAUX
  */
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamWriter>
/*
 * INCLUDES LOCAUX
 */
#include "Component/Interfaces/ISwFinalizerManager.h"

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \interface ISwPersistent
        \brief  interface de chargement/sauvegarde admin
        @ingroup SwCoreGrp
        @swinterface
        \note
        Toute entité implémentant cette interface peut être enregistrer et charger.
        */

        class ISwPersistent  {
        public:
	        /*! \brief methode permettant de charger des donnees
            \param[in] 
            \param[in] finalizer_manager manager de finalisation
            */
	        virtual void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager)=0;
	        /*! \brief methode permettant de sauver des donnees
            \param[in] writer stream XML.
            */
	        virtual void Save(QXmlStreamWriter& writer) = 0;
        };
    }
}

#endif
