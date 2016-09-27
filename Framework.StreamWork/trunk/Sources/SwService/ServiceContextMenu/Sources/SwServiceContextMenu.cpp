#include "SwServiceContextMenu.h"

//---------------------------------------------------------------------------------
StreamWork::Service::SwServiceContextMenu::SwServiceContextMenu() : _hasBeenInitialized(false), _actionServiceHelper(nullptr)
{

}

//---------------------------------------------------------------------------------
StreamWork::Service::SwServiceContextMenu::~SwServiceContextMenu()
{
	delete _actionServiceHelper;
}

//---------------------------------------------------------------------------------
void StreamWork::Service::SwServiceContextMenu::setMenuRequestCartoPosition(double latitude, double longitude, double altitude)
{
	_requestCartoPosition = std::make_tuple(latitude, longitude, altitude);
}

//---------------------------------------------------------------------------------
std::tuple<double, double, double> StreamWork::Service::SwServiceContextMenu::getMenuRequestCartoPosition()
{
	return _requestCartoPosition;
}

//---------------------------------------------------------------------------------
QMenu * StreamWork::Service::SwServiceContextMenu::getMenu()
{
	if (!_hasBeenInitialized)
	{
		_actionServiceHelper = new SwServiceManager_Helper<SwActionService>();
		_actionServiceHelper->setService(CG_SW_SERVICE_ACTION);		
		SwActionService* actionService = _actionServiceHelper->getService();
		
		if(!actionService)
			return nullptr;

		QList<Action::IAction*> functionlityActionList = actionService->getAllActionsFromFunctionality(Action::FunctionnalityFlag::CONTEXTMENU);

		for (auto action : functionlityActionList)
		{
			registerContextAction(action);
		}

		_hasBeenInitialized = true;
	}

	return ContextMenu::getMenu();
}