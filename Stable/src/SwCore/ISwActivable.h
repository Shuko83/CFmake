/*!
\file ISwActivable.h
\date 15/10/11
\brief interface pour la gestion de l'activation
\author  GBS
\version 1.0
 */

#ifndef _ISwActivable_H
#define _ISwActivable_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwActivable;

 
        /*!
        \class ISwAlerter
        \brief interface pour les alertes
        */
        class ISwActivable  
        {
        
        public:

	        /*! \brief Permet de changer la valeur d'activation
            \param[in] value d'activation
            */
	        virtual void setActive(bool value)=0;
	        /*! \brief Permet de lire la valeur d'activation
            \param[out] valeur d'activation
            */
	        virtual bool isActive()=0;
            
        };
    } // namespace SwCore
} // namespace StreamWork

#endif
