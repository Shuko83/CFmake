#include "SwActivationToQAction.h"

#include "SwTime_ToolBox.h"
#include "SwAddress_ToolBox.h"

using namespace StreamWork::SwCore;

const QString PROVIDED_QACTION = QStringLiteral("ProvidedQAction");

namespace StreamWork {
	namespace SwGui {

		//---------------------------------------------------------------------------------
		SwActivationToQAction::SwActivationToQAction(): _isConstructed(false), _action(this)
		{
			setOwnerServiceAvaibility(true);
			_activableComponent = 0;
			connect(&_action, &QAction::triggered, this, &SwActivationToQAction::toggleExecute);
		}

		//---------------------------------------------------------------------------------
		SwActivationToQAction::~SwActivationToQAction()
		{
			unprovideInterface(PROVIDED_QACTION);
		}

		//---------------------------------------------------------------------------------
		void SwActivationToQAction::setComponentName( QString name )
		{
			_componentToControl = name;
			if(_isConstructed && !name.isEmpty() )
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
			provideInterface<QAction>(PROVIDED_QACTION, &_action);
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