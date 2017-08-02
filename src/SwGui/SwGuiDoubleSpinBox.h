/*!
\brief SpinBox pour l'edition d'un double
\author QBN

*/

#ifndef _STREAMWORK_GUI_SWGUIDOUBLESPINBOX_H
#define _STREAMWORK_GUI_SWGUIDOUBLESPINBOX_H


/*
* INCLUDES LOCAUX
*/
#include "SwDouble.h"
#include "SwGuiConstantes.h"
/*
* INCLUDES GLOBAUX
*/
#include <QDoubleSpinBox>
#include <QtDesigner/QDesignerExportWidget>

namespace StreamWork
{
	namespace SwGui
	{

		/*!
		\brief  SpinBox pour l'edition d'un SwDouble
		*/
		class SWGUI_EXPORT SwGuiDoubleSpinBox: public QDoubleSpinBox 
		{
			Q_OBJECT
		private:
			/*! \brief contenu */
			SwCore::SwDouble _content;
		public:
			/*! \brief Constructeur */
			SwGuiDoubleSpinBox(SwCore::SwDouble & source ,QWidget *parent = 0);
			/*! \brief Destructeur */
			virtual ~SwGuiDoubleSpinBox();

			void SetDouble(SwCore::SwDouble & source);
			SwCore::SwDouble GetDouble();

			public slots: 
			/*! \brief changement de valeur */
			void onValueChanged(double value);
		};
	}
}


#endif 
