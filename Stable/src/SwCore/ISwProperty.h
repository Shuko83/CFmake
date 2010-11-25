/*!
\file ISwProperty.h
\date 18/04/2006
\brief interface d'une propriete
\author  Big
\version 1.0
 */

#ifndef _ISwProperty_H
#define _ISwProperty_H
/*
  * INCLUDES GLOBAUX
  */
#include <QVariant>
#include <QIcon>
/*
 * INCLUDES LOCAUX
 */
#include "LibIndeSig.h"

namespace StreamWork
{
	namespace SwCore
	{
        class ISwProperties;
        class ISwController;
        /*!
        \interface ISwProperty
        \brief  interface d'une propriete
        @ingroup SwCoreGrp
        @swinterface
        \note
        */

        class ISwProperty  {
        public:
	        /*! \brief methode de recuperer le service de propri�t� parent*/
	        virtual ISwProperties *GetHostingService()=0;
	        /*! \brief methode de recuperer le nom d'une propri�t� */
	        virtual QString GetName()=0;
	        /*! \brief methode de recuperer le nom reel d'une propri�t� */
	        virtual QString GetRealName()=0;
	        /*! \brief methode de recuperer la valeur d'une propri�t� */
	        virtual QVariant GetValue()=0;
	        /*! \brief methode permettant de definir la valeur d'une propri�t�*/
	        virtual void SetValue (const QVariant & val)=0;
	        /*! \brief methode permettant de savoir si la propri�t� est visible*/
	        virtual bool IsVisible()=0;
	        /*! \brief methode permettant de savoir si la propri�t� est editable*/
	        virtual bool IsEditable()=0;
	        /*! \brief methode permettant de savoir si la propri�t� a changer par rapport a sa configuration usine*/
	        virtual bool HasChanged()=0;
	        /*! \brief methode permettant de savoir si la propri�t� est controllable*/
	        virtual bool IsControllable()=0;
	        /*! \brief methode permettant de savoir si la propri�t� est control�e*/
	        virtual bool IsControlled()=0;
	        /*! \brief methode est une sous propri�t�*/
	        virtual bool IsSubProperty()=0;
	        /*! \brief methode permettant d'acceder au controller*/
	        virtual ISwController * GetController()=0;
	        /*! \brief methode permettant de marquer une propri�t� comme si elle n'avait pas changer*/
	        virtual void MarkAsUnchanged()=0;
	        /*! \brief Definition de la description*/
	        virtual void SetDescription(QString desc)=0;
	        /*! \brief Acces a la description*/
	        virtual QString GetDescription()=0;
	        /*! \brief Icon*/
	        virtual void SetIcon(QIcon & icon)=0;
	        /*! \brief Icon*/
	        virtual QIcon & GetIcon()=0;
	        /*! \brief value Icon*/
	        virtual void SetValueIcon(QIcon & icon)=0;
	        /*! \brief value Icon*/
	        virtual QIcon & GetValueIcon()=0;
	        /*! \brief methode d'acces au signal de changement*/
	        virtual LibIndeSig::iSignal1<ISwProperty *> & GetOnChangeSignal()=0;
	        /*! \brief methode d'acces au signal de changement de visibilite*/
	        virtual LibIndeSig::iSignal1<ISwProperty *> & GetOnVisibleChangeSignal()=0;
	        /*! \brief methode d'acces au signal de changement d'edition*/
	        virtual LibIndeSig::iSignal1<ISwProperty *> & GetOnEditableChangeSignal()=0;
	        /*! \brief methode d'acces au signal de changement de controle*/
	        virtual LibIndeSig::iSignal3<ISwProperty *,ISwController *,ISwController *> & GetOnControlChangeSignal()=0;
        };
    }
}

#endif
