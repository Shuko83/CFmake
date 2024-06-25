/*!
\file SwGuiEnumIntComboBox.h
\author QBN

*/

#ifndef _STREAMWORK_SWGUI_SWGUIENUMINTCOMBOBOX_H
#define _STREAMWORK_SWGUI_SWGUIENUMINTCOMBOBOX_H

/*
* INCLUDES LOCAUX
*/
#include "Types/SwIntegerEnum.h"
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

			void SetEnum(SwCore::SwIntegerEnum & source);
			SwCore::SwIntegerEnum GetEnum();

		public slots:
			void onEditFinished();
			void slotActivated(int index);
		};
	}
}


#endif 
