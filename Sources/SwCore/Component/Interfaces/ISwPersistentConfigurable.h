/*!
\file ISwPersistentConfigurable.h
\date 18/04/2006
\brief interface de chragement/sauvegarde configuration
\author  Big
\version 1.0
 */

#ifndef _ISwPersistentConfigurable_H
#define _ISwPersistentConfigurable_H
/*
  * INCLUDES GLOBAUX
  */
#include <QXmlStreamWriter>
#include <QDomElement>
/*
 * INCLUDES LOCAUX
 */

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \interface ISwPersistent
        \brief  interface de chargement/sauvegarde configuration
        @ingroup SwCoreGrp
        @swinterface
        \note
        */
        class ISwPersistentConfigurable  {
        public:
	        /*! \brief methode permettant de charger des donnees de configuration*/
	        virtual void LoadConfiguration(QDomElement &)=0;
	        /*! \brief methode permettant de sauver des donnees de configuration*/
			virtual void SaveConfiguration(QXmlStreamWriter &)=0;
        };
    }
}

#endif
