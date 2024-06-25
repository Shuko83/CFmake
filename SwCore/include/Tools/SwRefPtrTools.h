#ifndef _SwRefPtrTools_H
#define _SwRefPtrTools_H

#include "SwCoreConstantes.h"
//#include <iostream>
//#include <QSet>
#include <QString>
//#include "ISwRef.h"

namespace StreamWork
{
	namespace SwCore
	{


		/*class SwRefGenericContainer {
		public :
			SwRefGenericContainer(ISwRef * elt);

			void setNull();

			inline void connectDestroyToNull()
			{
					if(_elt != 0)
						_elt->getOnDestroySignal().iconnect(*this, &SwRefGenericContainer::setNull);
			}

			inline void disconnectDestroyToNull()
			{
				if(_elt != 0)
					_elt->getOnDestroySignal().idisconnect(*this, &SwRefGenericContainer::setNull);
			}

			QString getResult() const;
		
		private :			
			ISwRef * _elt;
			QString _eltName;
		};*/

		class BUILD_SWCORE SwRefPtrTools {
			/*friend class SwRefGenericContainer;*/
		public :

			static SwRefPtrTools * getInstance();

			void debug(std::string msg);

			void startWatch();

			void stopWatch();

			void cleanWatch();

			void printResults();

			bool isWatching();

			/*void registerSwRef(ISwRef * elt);*/

		private:
			SwRefPtrTools();
			virtual ~SwRefPtrTools();

			static SwRefPtrTools * _instance;

			bool							_isWatching;
		/*	QSet<SwRefGenericContainer *>	_swRefList;
			QMutex							_mutex;*/
		};
	}
}

#endif
