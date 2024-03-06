/*!
 \file DisabledSwRef.cpp
 \brief Implementation du ISwRef dont la gestion du cycle de vie est laissé au developpeur (pointeur classique)
 \author QBN
*/


#ifndef _DisabledSwRef_H
#define _DisabledSwRef_H

#include "SwCoreConstantes.h"
#include "Tools/Signal/LibIndeSig.h"
#include "SwRef/ISwRef.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		@brief Implementation du ISwRef dont la gestion du cycle de vie est laissé au developpeur (pointeur classique)
		*/
		class BUILD_SWCORE DisabledSwRef : public virtual ISwRef
		{
		public: 

			DisabledSwRef();
			virtual ~DisabledSwRef();

			virtual void _addRef();
			virtual void _release();
			virtual unsigned long _getReferencesNb() const;
			virtual LibIndeSig::iSignal0 & getOnDestroySignal();

		protected:
			LibIndeSig::iSignal0 OnDestroy; 
	
		};
	}

}
#endif
