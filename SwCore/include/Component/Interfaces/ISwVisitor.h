/*!
\file ISwVisitor.h
\date 18/04/2006
\brief interface visiteur (DP visitor)
\author  Big
\version 1.0
 */

#ifndef _ISwVisitor_H
#define _ISwVisitor_H
/*
 * INCLUDES LOCAUX
 */

namespace StreamWork 
{
	namespace SwCore
	{
        class SwComponent_Class;
        /*!
        \interface ISwVisitor
        \brief  interface visiteur (DP visitor)
        @ingroup SwCoreGrp
        @swinterface
        \note
        */
        class ISwVisitor  {
        public:
	        /*! \brief methode de visite */
	        virtual void Visit(SwComponent_Class *)=0;
        };
    }
}

#endif
