/*!
 \brief SpinBox pour l'edition d'un entier
 \author QBN

*/

#ifndef _STREAMWORK_GUI_SWGUIINTEGERSPINBOX_H
#define _STREAMWORK_GUI_SWGUIINTEGERSPINBOX_H


/*
  * INCLUDES LOCAUX
  */
#include "Types/SwInteger.h"
#include "SwGuiConstantes.h"
/*
  * INCLUDES GLOBAUX
  */
#include <QSpinBox>

namespace StreamWork
{
	namespace SwGui
	{

		/*!
		\brief  SpinBox pour l'edition d'un SwInteger
		 */
		class SWGUI_EXPORT SwGuiIntegerSpinBox: public QSpinBox
		{
		    Q_OBJECT
        private:
            /*! \brief contenu */
            SwCore::SwInteger _content;
		public:
            /*! \brief Constructeur */
		    SwGuiIntegerSpinBox(SwCore::SwInteger & source,QWidget *parent = 0);
            /*! \brief Destructeur */
		    virtual ~SwGuiIntegerSpinBox();

            void SetInteger(SwCore::SwInteger & source);
            SwCore::SwInteger GetInteger();

        public slots: 
            /*! \brief changement de valeur */
		    void onValueChanged(int value);

		};
	}
}


#endif 
