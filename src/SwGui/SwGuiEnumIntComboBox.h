/*!
 \file SwGuiEnumIntComboBox.h
 \author QBN

*/

#ifndef _STREAMWORK_SWGUI_SWGUIENUMINTCOMBOBOX_H
#define _STREAMWORK_SWGUI_SWGUIENUMINTCOMBOBOX_H


/*
  * INCLUDES LOCAUX
  */
#include <SwIntegerEnum.h>
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

		class SWGUI_EXPORT SwGuiEnumIntComboBox: public QComboBox
		{
		    Q_OBJECT
        private:
            /*! \brief contenu */
            SwCore::SwIntegerEnum _content;
			bool isPopulating;
		public:
            /*! \brief Constructeur */
		    SwGuiEnumIntComboBox(SwCore::SwIntegerEnum & source,QWidget *parent = 0);
            /*! \brief Destructeur */
		    virtual ~SwGuiEnumIntComboBox();

            /*! \brief Definition Enum */
            void SetEnum(SwCore::SwIntegerEnum & source);
            /*! \brief Definition Enum */
            SwCore::SwIntegerEnum GetEnum();

		public slots:
			void onEditFinished();
			void slotActivated(int index);
		};
		
	}
}


#endif 
