#ifndef _SwRefPtrTools_H
#define _SwRefPtrTools_H

#include "SwCoreConstantes.h"
#include <iostream>

namespace StreamWork
{
	namespace SwCore
	{
		class BUILD_SWCORE SwRefPtrTools {
		public :

			static SwRefPtrTools * getInstance();

			void debug(std::string msg);

		private:
			SwRefPtrTools();
			virtual ~SwRefPtrTools();

			static SwRefPtrTools * _instance;
		};
	}
}

#endif
