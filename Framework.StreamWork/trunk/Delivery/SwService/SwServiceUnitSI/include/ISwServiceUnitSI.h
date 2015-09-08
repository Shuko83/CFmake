#pragma once

#include "ISwService.h"
#include "IServiceUnitSI.h"


//Nom du service
#define CG_SW_SERVICE_UNIT_SI "ServiceUnitSI"

namespace StreamWork 
{    
	namespace SwCore 
	{
		class ISwServiceUnitSI : public ISwService, public virtual IServiceUnitSI
		{
		public:
			//------------------------------------------------------------------
			// Interface ISwService
			//------------------------------------------------------------------
			virtual void Liberate(){};
			virtual QString GetServiceName(){return QString(CG_SW_SERVICE_UNIT_SI);}
			virtual QString GetServiceRealName(){return QString(typeid(this).name());}

		};
	}
}
