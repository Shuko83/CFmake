/*!
\file ISwAdminSetup.h
\date 18/04/2006
\brief interface pour la configuration administrateur
\author  Big
\version 1.0
 */

#ifndef _ISwAdminSetup_H
#define _ISwAdminSetup_H
/*
 * INCLUDES LOCAUX
 */

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \class ISwAdminSetup
        \brief  interface pour la configuration administrateur
        \note
        */
        class ISwAdminSetup  {
        public:
	        /*! \brief Permet d'acceder aux preferences d'administration*/
	        virtual void AdminSetup()=0;
        };
    }
}

#endif
