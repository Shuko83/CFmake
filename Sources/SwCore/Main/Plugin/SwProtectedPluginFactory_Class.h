#ifndef _SwProtectedPluginFactory_Class_H
#define _SwProtectedPluginFactory_Class_H

#include "Main/Plugin/SwPluginFactory_Class.h"

namespace StreamWork
{
	namespace SwCore
	{
        class BUILD_SWCORE SwProtectedPluginFactory_Class : virtual public SwPluginFactory_Class
		{
		public:
			bool unlock(std::string hash) const;
        };
	}
}

#endif 
