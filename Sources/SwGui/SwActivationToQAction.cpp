#include "SwActivationToQAction.h"

#include "SwTime_ToolBox.h"
#include "SwAddress_ToolBox.h"

using namespace StreamWork::SwCore;

namespace StreamWork {
	namespace SwGui {

		//---------------------------------------------------------------------------------
		SwActivationToQAction::SwActivationToQAction(): _isConstructed(false), _action(this)
		{
			setOwnerServiceAvaibility(true);
			_activableComponent = 0;
			connect(&_action, SIGNAL(triggered(bool)), this, SLOT(toggleExecute(bool)));
		}

		//---------------------------------------------------------------------------------
		SwActivationToQAction::~SwActivationToQAction()
		{
			unprovideInterface("ProvidedISwAction");
		}

		//---------------------------------------------------------------------------------
		void SwActivationToQAction::setComponentName( QString name )
		{
			_componentToControl = name;
			if(_isConstructed && name != "")
			{
				SwComponent_Class * component = SwAddress_ToolBox::FindTarget(_componentToControl, this->GetHostComponent());
				if(component)
				{
					_activableComponent = component;
					return;
				}
			}
			_activableComponent = 0;
		}

		//---------------------------------------------------------------------------------
		QString SwActivationToQAction::getComponentName()
		{
			return _componentToControl;
		}

		//---------------------------------------------------------------------------------
		QAction & SwActivationToQAction::GetAction()
		{
			return _action;
		}

		//---------------------------------------------------------------------------------
		void SwActivationToQAction::toggleExecute(bool)
		{
			if(_activableComponent)
			{
				if(!_activableComponent->isActive())
					_activableComponent->setActive(true);
				else
					_activableComponent->setActive(false);
			}
		}

		//---------------------------------------------------------------------------------
		void SwActivationToQAction::initializeComponent() throw(SwException)
		{
			provideInterface<ISwAction>("ProvidedISwAction", this);
			createPropertiesForThisObject();
			createPropertiesForQObject(&_action);
		}

		//---------------------------------------------------------------------------------
		bool SwActivationToQAction::Finalize( quint64 historic_index )
		{
			_isConstructed = true;
			setComponentName(_componentToControl);
			return true;
		}

	}
}