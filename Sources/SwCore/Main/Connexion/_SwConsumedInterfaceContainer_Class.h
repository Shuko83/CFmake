/*!
\file _SwConsumedInterfaceContainer_Class.h
\date 12/04/2006
\brief conteneur de stockage des informations relatives a une interface consommée
\author  Big
\version 1.0
 */

#ifndef __SwConsumedInterfaceContainer_Class_H
#define __SwConsumedInterfaceContainer_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QSet>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "SwException.h"
#include "SwMacros.h"
#include "ISwInterfaces_Provider.h"
#include "SwInterfaces_Consumer_Class.h"

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \class _SwConsumedInterfaceContainer_Class
        \brief conteneur de stockage des informations relatives a une interface consommée
        */
        class _SwConsumedInterfaceContainer_Class {
            /*! \brief Nom de l'interface */
            QString _name;
            /*! \brief Nom de l'interface chez le fournisseur*/
            QString _provider_name;
            /*! \brief Type de l'interface */
            QString _type;
            /*! \brief flag indicateur que l'interface est disponible */
            bool _isAvailable;
            /*! \brief Handle de l'interface */
            void ** _hhandle;
            /*! \brief consommateur parent */
            SwInterfaces_Consumer_Class * _parent;
            /*! \brief fournisseur parent */
            ISwInterfaces_Provider * _provider_parent;
            /*! \brief index d'historique de l'operation register du fournisseur */
            quint64 _historical_index;
            /*! \brief Nom de l'interface potentielle chez le fournisseur (utiliser pour le load)*/
            QString _potential_provider_name;
            /*! \brief Path pour acceder au fournisseur (utiliser pour le load)*/
            QString _potential_provider_path;
        public:
	        /*! \brief Construsteur */
	        _SwConsumedInterfaceContainer_Class(SwInterfaces_Consumer_Class * parent);
	        /*! \brief Destrusteur */
	        ~_SwConsumedInterfaceContainer_Class();
	        /*! \brief Definit les caracteristiques de l'interfaces */
	        void Define(QString name,QString type);
	        /*! \brief Acces au type de l'interface */
	        QString GetType();
	        /*! \brief Acces au nom de l'interface */
	        QString GetName();
	        /*! \brief Definition du handle de l'interface */
	        void SetInterfaceHandle(void ** handle);
	        /*! \brief Definition de l'interface */
	        void SetHandle(void * handle);
	        /*! \brief Revoie la disponibilite */
	        bool IsAvailable();
	        /*! \brief Renvoie l'index d'historique*/
            quint64 GetHistoricalIndex();
	        /*! \brief Renvoie le producteur ou NULL si non definit*/
            ISwInterfaces_Provider * GetProvider();
	        /*! \brief Renvoie le nom de l'interface produite ou chaine vide si non definit*/
            QString GetProvidedInterfaceName();
	        /*! \brief Enregistre une interface consommatrice */
            void RegisterProviderInterface(ISwInterfaces_Provider * provider,QString pinterface) throw(SwException);
	        /*! \brief Desenregistre une interface consommatrice */
            void UnregisterProviderInterface();
	        /*! \brief Definition de l'interface potentielle et du fournisseur */
            void DefinePotentialProviderInterfaceDescription(QString ppotential_interface,QString ppotential_provider_path, quint64 index);
	        /*! \brief Acces au nom de l'interface potentielle */
            QString GetPotentialProviderInterfaceName();
	        /*! \brief Acces au path du fournisseur potentielle */
            QString GetPotentialProviderPath();

        };
    }
}

#endif
