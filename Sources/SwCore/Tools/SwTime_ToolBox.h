/*!
 \file SwTime_ToolBox.h
 \brief Implementation of the Class SwTime_ToolBox
 \version 1.0
 \date 24-août-2006 12:21:05
 \author F.Bighelli
*/

#ifndef _SwTime_ToolBox_H
#define _SwTime_ToolBox_H

/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwTime_ToolBox 
		 \brief Gestion du temps en double (unite la seconde)
        @ingroup SwCoreGrp

         Cette classe n'est pas instanciable. C'est une boite a outils qui contient des
         methodes statiques permettant de gerer le temps

		*/
		class BUILD_SWCORE SwTime_ToolBox
		{
        private:
            /*! \brief Constructeur */
            SwTime_ToolBox(){}
            /*! \brief Destructeur*/
            ~SwTime_ToolBox(){}
        public:
            /*! \brief Permet de recuperer le nombre de seconde depuis le 1 janvier 1970*/
			static double GetTime();
			/*! \brief Permet d'attendre un certain nombre de millisecondes*/
			static void InternalSleep(unsigned int time_ms);
       };
	}
}
#endif 
