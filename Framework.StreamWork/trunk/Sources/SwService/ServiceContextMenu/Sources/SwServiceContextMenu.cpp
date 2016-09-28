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
void StreamWork::Service::SwServiceContextMenu::setMenuRequestCartoPosition(QVector3D geoPos)
{
	_requestCartoPosition = geoPos;
}

//---------------------------------------------------------------------------------
void StreamWork::Service::SwServiceContextMenu::resetMenuRequestCartoPosition()
{
	_requestCartoPosition = QVector3D(NAN, NAN, NAN);
}

//---------------------------------------------------------------------------------
bool StreamWork::Service::SwServiceContextMenu::isMenuRequestCartoPositionValid()
{
	if (isnan(_requestCartoPosition.x()) && isnan(_requestCartoPosition.y()) && isnan(_requestCartoPosition.z()))
		return false;
	
	return true;
}

//---------------------------------------------------------------------------------
QVector3D StreamWork::Service::SwServiceContextMenu::getMenuRequestCartoPosition()
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