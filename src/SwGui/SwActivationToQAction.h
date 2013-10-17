#ifndef _STREAMWORK_SWGUI_SWACTIVATIONTOQACTION_H
#define _STREAMWORK_SWGUI_SWACTIVATIONTOQACTION_H

#include "SwAssistedComponent.h"
#include "ISwAction.h"
#include <QAction>

namespace StreamWork {

    namespace SwGui {
   
		class SwActivationToQAction : public SwFoundation::SwAssistedComponent, public ISwAction, public ISwFinalizer
		{
			Q_OBJECT
			Q_PROPERTY(QString componentNameToControl READ getComponentName WRITE setComponentName)
		public :
			SwActivationToQAction();
			~SwActivationToQAction();

			void setComponentName(QString);
			QString getComponentName();
			
			// ------------------------------------------------------------------------
			//				SwAssistedComponent
			// ------------------------------------------------------------------------
			virtual void initializeComponent() throw(SwException);

			// ------------------------------------------------------------------------
			//				ISwAction
			// ------------------------------------------------------------------------
			virtual QAction & GetAction();

			// ------------------------------------------------------------------------
			//				ISwFinalizer
			// ------------------------------------------------------------------------
			virtual bool Finalize( quint64 historic_index );

		public slots:
			void toggleExecute(bool);

		private:
			QString _componentToControl;
			SwComponent_Class * _activableComponent;
			QAction _action;
			bool _isConstructed;
        };
    }
}

#endif
