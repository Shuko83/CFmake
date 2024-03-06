/*!
 \file SwGuiEnumComboBox.h
 \brief ComboBox pour l'edition d'un SwEnum
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#ifndef _SwGuiEnumComboBox_H
#define _SwGuiEnumComboBox_H


/*
  * INCLUDES LOCAUX
  */
#include "Types/SwEnum.h"
#include "Types/SwEnumModel.h"
#include "SwGuiConstantes.h"
/*
  * INCLUDES GLOBAUX
  */
#include <QWidget>
#include <QComboBox>

namespace StreamWork
{
	namespace SwGui
	{

		/*!
		\class SwGuiEnumComboBox
		\date 12/04/2006
		\brief  ComboBox pour l'edition d'un SwEnum
		\author  BIG
		\version 1.0
		 */
		class SWGUI_EXPORT SwGuiEnumComboBox: public QComboBox
		{
		    Q_OBJECT
        private:
            /*! \brief contenu */
            SwCore::SwEnum _content;
		public:
            /*! \brief Constructeur */
		    SwGuiEnumComboBox(SwCore::SwEnum & source, QWidget *parent = 0);
            /*! \brief Destructeur */
		    virtual ~SwGuiEnumComboBox();

            /*! \brief Definition Enum */
            void SetEnum(SwCore::SwEnum & source);
            /*! \brief Definition Enum */
            SwCore::SwEnum GetEnum();
        public slots: 
            /*! \brief Activation d'un item */
		    void slotActivated(int index);

		};
		
	}
}


#endif 
