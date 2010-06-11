/*!
\file SwController_Class.h
\date 30/05/2007
\brief implementation du service controller
\author  Big
\version 1.0
 */

#ifndef _SwController_Class_H
#define _SwController_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QList>
#include <QVariant>
#include <QString>
#include <QMetaType>
#include <QObject>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "SwException.h"
#include "SwMacros.h"
#include "ISwController.h"
#include "LibIndeSig.h"
#include "ISwProperty.h"
#include "ISwFinalizer.h"

namespace StreamWork
{
    namespace SwCore
    {
        class _SwPropertyImpl_Class;
		/*!
        \class ISwController
        \brief  service controller
        */
        class BUILD_SWCORE SwController_Class: public ISwController, public ISwFinalizer{
		protected:
			/*! \brief hote */
			SwComponent_Class * _host;
			/*! \brief related property */
			ISwProperty * _related_property;
			/*! \brief related properties_service*/
			ISwProperties * _related_properties_service;
			/*! \brief index de construction*/
			quint64 _historic_index;
			/*! \brief index de construction du load*/
			quint64 _historic_index_on_load;
			/*! \brief property name du load*/
			QString _property_name_on_load;
			/*! \brief host path du load*/
			QString _host_path_on_load;
			/*! \brief callback de changement de la propri�t� par le parent*/
			void _OnRelatedPropertyChange(ISwProperty *p);
        public:
            /*! \brief Sur changement de la propri�t� du parent */
            LibIndeSig::iSignal0 OnParentPropertyChange;
            /*! \brief Sur attachement de la propri�t� parent */
            LibIndeSig::iSignal0 OnParentPropertyAttach;
            /*! \brief Sur attachement de la propri�t� parent */
            LibIndeSig::iSignal0 OnParentPropertyDetach;
        public:
	        /*! \brief Constructeur */
			SwController_Class(SwComponent_Class * host);
	        /*! \brief Destructeur */
			virtual ~SwController_Class();
	        /*! \brief Acces a la valeur de la propri�t� du parent */
	        QVariant GetParentPropertyValue();
	        /*! \brief Definition de la valeur de la propri�t� du parent */
	        void SetParentPropertyValue(QVariant val);
            //---------------------------------------------------------------------
            // Interface ISwController
            //---------------------------------------------------------------------
	        /*! \brief Permet de cr�e une propri�t� */
	        void InitializeControl(ISwProperties * properties,QString property_name);
	        /*! \brief le controle est supprimer au niveau de la propri�t� */
	        void DetachControl();
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistr�
			lorsque ce premier se detruit ou une operation de desenregistrement du service est r�alis�e*/
            void Liberate();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief acces a son composant hote */
	        SwComponent_Class * GetHostComponent();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief methode permettant de charger des donnees */
	        void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
	        /*! \brief methode permettant de sauver des donnees */
	        void Save(QDomElement & elt,QDomDocument & doc);
             //---------------------------------------------------------------------
            // Interface ISwFinalizer
            //---------------------------------------------------------------------
             /*! \brief finalize l'operation correspondant a l'index d'historique
            \return false si la finalisation n'a pas eu lieu et true si ok*/
	        bool Finalize(quint64 historic_index);
        };
    }
}

#endif
