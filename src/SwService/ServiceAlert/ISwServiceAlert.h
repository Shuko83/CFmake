#ifndef _ISWSERVICEALERT_H
#define _ISWSERVICEALERT_H
//QT includes
#include <QString>

//Other External includes
#include "IFieldError.h"
#include "IError.h"
#include "IErrorEnd.h"
#include "IErrorDb.h"

//Local includes
#include "ISwService.h"


using namespace StreamWork::SwCore;

//Nom du service
#define CG_SW_SERVICE_ALERT "ServiceAlert"


namespace alert {
	class IAlertObserver;
	class AlertManager;
}

namespace StreamWork
{
    namespace SwCore
    {
		
        /*!
        \interface ISwServiceAlert
        \brief service de gestion des Alert
        @ingroup Swcore
        @swinterface
        */
        class ISwServiceAlert: virtual public ISwService{
        public:
			
			/**
			  * @brief ajout d'un observateur à un catégorie d'erreur donnée
			  */
			virtual void addObserver(alert::IAlertObserver * alertObserver, QString errorCategory)=0;
			/**
			  * @brief suprime l'alertObserver des observers a notifié
			  */
			virtual void removeCategoryObserver(alert::IAlertObserver * alertObserver)=0;			
			/**
			  * @brief permet de notifié les observer d'une d'erreur simple de category SwAlert
			  */
			virtual void raiseAlert(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level)=0;
			/**															
			  * @brief permet de notifié d'une erreur!
			  * Les observers qui sont notifié sont :
			  * Ceux qui observe la meme catégory que celle qu'on trouve dans l'erreur ou les catégory parent 
			  * ex de catégory avec un parent : "catParent1::catParent2::cat3" dans ce cas les observer de "CatParent1::CatParent2" et "CatParent1" seront notifié
			  * Si l'erreur à une fin (willEnd = true), la méthode raiseErrorEnd() doit etre appellé pour mettre fin à l'erreur
			  */
			virtual void raiseError(alert::IError * error)=0;
			/**
			  * @brief permet de notifié d'une erreur et du delay pour envoyé sa fin
			  * timeOut est en Secondes
			  * Les observers sont notififié d'un erreur comme dans le raiseError(Error * error)
			  * puis à la fin du delai on notifie automatiquement ces meme observers via un raiseErrorEnd(Error * error)
			  * ATTENTION il faut que l'erreur est une fin (willEnd doit etre à true) sinon on ne notifie pas de la fin
			  */
			virtual void raiseError(alert::IError * error, double timeOut) = 0;
			/**
			  * @brief permet de notifié d'une fin d'erreur à partir de l'erreur qui a signalé le début!
			  * Un ErrorEnd est créé,
			  * puis on notifie les observers avec la méthode  raiseErrorEnd(ErrorEnd * errorEnd)
			  */
			virtual void raiseErrorEnd(alert::IError * error) = 0;
			/**
			  * @brief permet de notifié les observer d'une fin d'erreur
			  * ATTENTION : pour que les observer de catégory soit notifié d'une fin d'erreur il faut que l'erreur initial ait été envoyé et 
			  * que le paramètre willEnd de cette erreur soit à true
			  */
			virtual void raiseErrorEnd(alert::IErrorEnd *errorEnd)=0;	
			/**
			* @brief Accès direct à l'alertManager
			* utile dans le cas ou l'on veux que les module utilise directement le composant AlertManager plutot que le Service Stramwork 
			* Par ex avec la GUSA
			*/
			virtual alert::AlertManager * getAlertManager() = 0;

            virtual alert::IErrorDb * getErrorDb() =0;
			

			//---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
			/**
			 * @brief    : Est appele uniquement par le service manager aupres duquel le service est enregistré
						   lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
			 */
			virtual void Liberate(){}; 
            /**
             * @brief    : Renvoie le nom du service
             * @return   : le nom du service 
             */
            virtual QString GetServiceName(){return QString(CG_SW_SERVICE_ALERT); }

             /**
             * @brief    : Renvoie le nom du service reel (le nom de l'interface)
			 * @return   : le nom du service reel (le nom de l'interface)
			 * @note     : au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()
             */
            virtual QString GetServiceRealName() {return QString(typeid(this).name());}
        };
    }
}

#endif
