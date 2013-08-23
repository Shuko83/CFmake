	/*!
\file SwPin.h
\date 12/04/2006
\brief Definition d'un pin simple
\author  Big
\version 1.0
 */

#ifndef _SwPin_H
#define _SwPin_H

/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "SwMacros.h"
#include "SwData_Class.h"
#include "ISwPin_Listener.h"
 
namespace StreamWork 
{
    namespace SwCore
    {
        class ISwPins_Manager;
		/*!
        \class SwPin
        \brief Definition d'un pin simple
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwPin  {
		protected:
			/*! \brief Manager hote */
			ISwPins_Manager * _manager;
			/*! \brief Pin distant */
			SwPin * _remote_pin;
			/*! \brief Liste des listener */
			QList<ISwPin_Listener *> _listeners;
			/*! \brief data type*/
			QString _data_type;
			/*! \brief pin name*/
			QString _pin_name;
			/*! \brief "date" de connection*/
            quint64 _connection_historic_date;
			/*! \brief Liste des listener */
			virtual void ProcessReceivedData(SwData_Class * data);
        public:
	        /*! \brief Constructeur */
			SwPin(ISwPins_Manager * manager,QString name,QString data_type);
	        /*! \brief Destructeur */
			virtual ~SwPin();
	        /*! \brief Acces au type de données pris en charge par le pin */
	        virtual QString GetType();
	        /*! \brief Acces au nom du pin */
	        virtual QString GetName();
	        /*! \brief Permet au pin de recevoir une donnée */
	        virtual void PushData(SwData_Class * data);
	        /*! \brief Permet au pin d'envoyer une donnée */
	        void SendData(SwData_Class * data);
	        /*! \brief Permet d'assigner un pin remote a ce pin */
	        void AssignRemote(SwPin * remote_pin,quint64 historic_date);
	        /*! \brief Permet de supprimer l'assignation du pin distant */
	        void Desassign();
	        /*! \brief Permet de connaitre le pin connecte
			\return NULL si aucun pin n'est connected*/
	        SwPin * GetConnected();
	        /*! \brief Permet d'acceder a la date de connection */
            quint64 GetHistoricConnectionDate();
	        /*! \brief Permet de connaitre le manager du pin */
	        ISwPins_Manager * GetManager();
	        /*! \brief Enregistre un listener*/
	        void RegisterListener(ISwPin_Listener * listener);
	        /*! \brief Desenregistre  un listener*/
	        void UnregisterListener(ISwPin_Listener * listener);	
        };
    }
}

#endif
