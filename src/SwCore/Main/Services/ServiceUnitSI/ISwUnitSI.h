///////////////////////////////////////////////////////////
//  ISwUnitSI.h
//  Implementation of the Class ISwUnitSI
//  Created on:      31-déc.-2013 11:25:37
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#ifndef STREAMWORK_SWCORE_ISWUNITSI_H
#define STREAMWORK_SWCORE_ISWUNITSI_H

#include "SwSiEnums.h"
/**
 * ! \interface ISwUnitSI \brief unit service listener
 * @ingroup SwGuiGrp
 * @swinterface
 */
namespace StreamWork 
{    
	namespace SwCore 
	{
		class ISwUnitSI
		{

		public:
			
			/**
			* @brief	: Notification unit changed
			* @param	: unit - the new unit 
			* @return	: void
			 */
			virtual void processUnitChanged(SwSiUnit unit) =0;

			/**
			* @brief	: Notification precision changed
			* @param	: unit - the new precision
			* @return	: void
			*/
			virtual void processPrecisionChanged(int precision) =0;

		};
	}
}
#endif // STREAMWORK_SWCORE_ISWUNITSI_H
