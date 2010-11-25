/*!
 \file SwLoader_Class.h
 \brief Implementation of the Class SwLoader_Class
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

 \note Principe charge un neoud racine depuis une structure
 \note Implementation
 Verifie neoud header nommé streamlib
 Crée le composant racine avec les attribut name et factory_name
 Si le composant n'existe pas
    Si le factory_name n'existe pas ou est vide le composant crée une instance de SwComponent_Class
 Charger les services (qui implemente ISwPersistent)
 Charge les composants enfants du neoud (algo identique au neoud racine)
 Effectue les finalisations enregistrer
*/

#ifndef _SwLoader_Class_H
#define _SwLoader_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QDomDocument>
/*
  * INCLUDES LOCAUX
  */
#include "SwApplication.h"
#include "ISwFinalizerManager.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwLoader_Class
		 \brief class de chargement du stream
        @ingroup SwCoreGrp
		*/
        class BUILD_SWCORE SwLoader_Class : public ISwFinalizerManager
		{
        private:
            /*! registre des finalisations */
            QMap<quint64,ISwFinalizer *> _finalizations;
            /*! construction du stream */
            SwComponent_ClassPtr BuildStream(QDomElement & node,SwComponent_Class * parent_component,bool create_always=false) throw(SwException);
        public:
            /*! Constructeur */
            SwLoader_Class();
            /*! Destructeur */
            virtual ~SwLoader_Class();
            /*! Chargement d'un stream */
            SwComponent_ClassPtr Load(QDomDocument & doc) throw(SwException);
            /*! Chargement d'un groupe */
            void LoadGroup(SwComponent_Class * parent_component,QDomDocument & doc);
            /*! resolution des finalizations manquantes */
            void FinalizeUnfinalized();
            //-----------------------------------------------------------
            // Interface de ISwFinalizerManager
            //-----------------------------------------------------------
            /*! \brief enregistre une finalisation pour un index d'historique donnée*/
	        void RegisterFinalization(quint64 historic_index,ISwFinalizer * finalizer) throw(SwException);
		};
	}
}
#endif
