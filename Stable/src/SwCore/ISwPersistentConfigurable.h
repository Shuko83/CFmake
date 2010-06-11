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
#include <QDomDocument>
#include <QDomElement>
/*
 * INCLUDES LOCAUX
 */

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \class ISwPersistent
        \brief  interface de chargement/sauvegarde configuration
        \note
        */
        class ISwPersistentConfigurable  {
        public:
	        /*! \brief methode permettant de charger des donnees de configuration*/
	        virtual void LoadConfiguration(QDomElement &)=0;
	        /*! \brief methode permettant de sauver des donnees de configuration*/
	        virtual void SaveConfiguration(QDomElement &,QDomDocument &)=0;
        };
    }
}

#endif
