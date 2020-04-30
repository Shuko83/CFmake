/*!
\file _SwPropertyImpl_Class.h
\date 18/04/2006
\brief implementation generale d'une propriete
\author  Big
\version 1.0
 */

#ifndef __SwPropertyImpl_Class_H
#define __SwPropertyImpl_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QVariant>
/*
 * INCLUDES LOCAUX
 */
#include "ISwProperty.h"
#include "ISwController.h"
#include "ISwProperties.h"
#include "ISwComplexeTypeAdapters.h"
#include "LibIndeSig.h"

#pragma warning(disable:4250)


namespace StreamWork
{
	namespace SwCore
	{
        /*!
        \class _SwPropertyImpl_Class
        \brief  implementation generale d'une propriete
        \note
        */

        class BUILD_SWCORE _SwPropertyImpl_Class : public virtual ISwProperty {
        protected:
            /*! \brief service hote */
            ISwProperties * _hosting_service;
            /*! \brief nom de la propri�t� */
            QString _name;
            /*! \brief description de la propri�t� */
            QString _desc;
            /*! \brief icon label */
            QIcon _icon;
            /*! \brief icon value */
            QIcon _valueIcon;
            /*! \brief Valeur de la propri�t� */
            QVariant _value;
            /*! \brief Valeur de la propri�t� initiale */
            QVariant _initialValue;
            bool _hasBeenInitialed;
            /*! \brief est visible */
            bool _is_visible;
            /*! \brief est editable */
            bool _is_editable;
            /*! \brief a changer */
            bool _has_changed;
            /*! \brief ets controlable */
            bool _is_controllable;
            /*! \brief controller */
            ISwController * _controller;
            /*! \brief complexe adapter */
            ISwComplexeTypeAdapters * _adapters;
            /*! \brief signal de changement de la propri�t� */
            LibIndeSig::iSignal1<ISwProperty *> _OnChangeValue;
            /*! \brief signal de changement de l'attribut visible de la propri�t� */
            LibIndeSig::iSignal1<ISwProperty *> _OnVisibleChangeValue;
            /*! \briefsignal de changement de l'attribut editable de la propri�t� */
            LibIndeSig::iSignal1<ISwProperty *> _OnEditableChangeValue;
            /*! \briefsignal de changement du controle de la propri�t� */
            LibIndeSig::iSignal3<ISwProperty *,ISwController *,ISwController *> _OnControlChange;
        public:
            /*! \brief Constructor */
            _SwPropertyImpl_Class(QString name,ISwProperties * hosting_service);
            /*! \brief Destructor */
            virtual ~_SwPropertyImpl_Class();
	        /*! \brief methode de recuperer le service de propri�t� parent*/
	        ISwProperties *GetHostingService();
	        /*! \brief methode de recuperer le nom d'une propri�t� */
	        QString GetName();
	        /*! \brief methode de recuperer le nom reel d'une propri�t� */
	        virtual QString GetRealName();
	        /*! \brief methode de recuperer la valeur d'une propri�t� */
	        QVariant GetValue();
	        /*! \brief methode permettant de definir la valeur d'une propri�t�*/
	        void SetValue (const QVariant & val, bool force = false);
	        /*! \brief methode permettant de definir la valeur d'une propri�t� par son controller*/
	        void SetValueByController(const QVariant & val);
	        /*! \brief methode de recuperer la valeur d'une propri�t� en interne*/
	        virtual QVariant GetInternalValue();
	        /*! \brief methode permettant de definir la valeur d'une propri�t� en interne*/
	        virtual void SetInternalValue (const QVariant & val);
	        /*! \brief methode permettant de savoir si la propri�t� est visible*/
	        bool IsVisible();
	        /*! \brief methode permettant de savoir si la propri�t� est editable*/
	        bool IsEditable();
	        /*! \brief methode permettant de savoir si la propri�t� est controllable*/
	        bool IsControllable();
	        /*! \brief methode permettant de savoir si la propri�t� est control�e*/
	        bool IsControlled();
	        /*! \brief methode est une sous propri�t�*/
	        virtual bool IsSubProperty();
	        /*! \brief methode permettant d'acceder au controller*/
	        ISwController * GetController();
	        /*! \brief methode d'acces au signal de changement*/
	        LibIndeSig::iSignal1<ISwProperty *> & GetOnChangeSignal();
	        /*! \brief methode d'acces au signal de changement de visibilite*/
	        LibIndeSig::iSignal1<ISwProperty *> & GetOnVisibleChangeSignal();
	        /*! \brief methode d'acces au signal de changement d'edition*/
	        LibIndeSig::iSignal1<ISwProperty *> & GetOnEditableChangeSignal();
	        /*! \brief methode d'acces au signal de changement de controle*/
	        LibIndeSig::iSignal3<ISwProperty *,ISwController *,ISwController *> & GetOnControlChangeSignal();
 	        /*! \brief methode permettant de changer la visibilite de la propri�t�*/
	        void SetIsVisible(bool is_visible);
	        /*! \brief methode permettant de changer l'edition de la propri�t�*/
	        virtual void SetIsEditable(bool is_editable);
	        /*! \brief methode permettant de changer l'aspect controlable de la propri�t�*/
	        void SetControllable(bool is_controllable);
	        /*! \brief methode permettant de definir le controller*/
	        void SetController(ISwController * controller);
	        /*! \brief methode permettant de savoir si la propri�t� a changer par rapport a sa configuration usine*/
	        bool HasChanged();
	        /*! \brief methode permettant de marquer une propri�t� comme si elle n'avait pas changer*/
	        void MarkAsUnchanged();
	        /*! \brief methode permettant de marquer une propri�t� comme si elle avait  changer et genere le signal associé*/
	        void MarkAsChanged();
	        /*! \brief Definition de la description*/
	        void SetDescription(QString desc);
	        /*! \brief Acces a la description*/
	        QString GetDescription();
	        /*! \brief Icon*/
	        void SetIcon(QIcon & icon);
	        /*! \brief Icon*/
	        QIcon & GetIcon();
	        /*! \brief value Icon*/
	        void SetValueIcon(QIcon & icon);
	        /*! \brief value Icon*/
	        QIcon & GetValueIcon();
	        /*! \brief Assignation d'adapteurs*/
	        void SetComplexeTypeAdapters(ISwComplexeTypeAdapters * adapters);
	        /*! \brief Recuperation d'adapteurs*/
	        ISwComplexeTypeAdapters * GetComplexeTypeAdapters();
            /*! \brief prpriete resettable*/
            virtual bool isResettable();
            /*! \brief reset de la propriété*/
            virtual void reset();

			bool checkForUserType (const QVariant & val);
       };
    }
}

#endif
