/*!
\file ISwFinalizer.h
\date 18/04/2006
\brief interface de finalisation (appel apres le chargement complet)
\author  Big
\version 1.0
 */

#ifndef _ISwFinalizer_H
#define _ISwFinalizer_H
/*
  * INCLUDES GLOBAUX
  */
#include <QtGlobal>

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \interface ISwFinalizer
        \brief  Interface de finalisation (appel apres le chargement complet)
        @ingroup SwCoreGrp
        @swinterface
        Permet de realiser des operations dans le même ordre que lors de la création du stream.

        Ce mecanisme est utilisé par le services de consommations d'interfaces (SwInterfaces_Consumer_Class) pour relier les interfaces aux composants
        dans le même ordre que lors de la construction... Une des principales conséquences est q'une IHM est
        reconstruite a l'identique de la construction initiale.
        
        Utilisation

        Dans le cas ou une sequence d'appel doit être refait à l'identique au prochain lancement de l'application

        Lors de la definition de la sequence d'appel, pour chaque appel demander un index d'historique
        au singletion application et l'enregistrer
        \code
        appel_index=SW_APP->GetHistoricCpt();
        \endcode

        Lors de l'enregistrement, enregistrer l'index correspondant à chaque appel.

        Lors de la phase de rechargemement, enregistrer les phases construction de sequence
        avec en parametre l'index d'historique et le handle du finalizer auprès du manager 
        de finalisation (le chargeur soit SwLoader)

        \code
        finalizer_manager.RegisterFinalization(index,this); //this herite de ISwFinalize
        \endcode

        Lors de la phase de finalization la methode Finalize est appelé avec l'index historique
        La methode Finalize effectue aors l'appel correspondant a l'index. et on garantit
        que tout les appels sont effetués dans le même ordre que la premier fois.

        \warning Lors de la phase de finalization, redemander un nouvel index d'historique

        Voir aussi l'interface ISwFinalizerManager.

        \todo Fournir un exemple concret. En attendant voir le code de SwInterfaces_Consumer_Class.
        */

        class ISwFinalizer  {
        public:
            /*! \brief finalize l'operation correspondant a l'index d'historique
            \param[in] historic_index index d'historique
            \return false si la finalisation n'a pas eu lieu et true si ok*/
	        virtual bool Finalize(quint64 historic_index)=0;
        };
    }
}

#endif
