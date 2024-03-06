/*!
\file ISwComponentProvider.h
\date 18/04/2006
\brief Fournisseur d'instance de composant
\author  Big
\version 1.0
 */

#ifndef _ISwComponentProvider_H
#define _ISwComponentProvider_H
/*
 * INCLUDES LOCAUX
 */
#include "Tools/Signal/LibIndeSig.h"


namespace StreamWork
{
	namespace SwCore
	{
        class SwComponent_Class;
        /*!
        \interface ISwComponentProvider
        \brief  Fournisseur d'instance de composant
        @ingroup SwCoreGrp
        @swinterface
        \note
        */
        class ISwComponentProvider  {
        public:
	        /*! \brief Fournit un composant */
	        virtual SwComponent_Class * ProvideComponent()=0;
			/*! \brief sur changement du composant fournit */
			LibIndeSig::iSignal1<ISwComponentProvider *> OnProvidedComponentChange;
		};
    }
}

#endif
