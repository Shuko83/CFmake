#include "SwServiceContextMenu.h"

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
