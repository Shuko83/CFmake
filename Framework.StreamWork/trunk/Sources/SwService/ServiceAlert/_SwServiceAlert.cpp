#include "_SwServiceAlert.h"

//QT includes
#include <QApplication>

//Other External includes
#include "AlertManager.h"

//Local includes
#include "_SwError.h"

// namespace
using namespace alert;


//-------------------------------------------------------------------------
_SwServiceAlert::_SwServiceAlert()
{
	_alertManager = new AlertManager();
}

//-------------------------------------------------------------------------
_SwServiceAlert::~_SwServiceAlert()
{
	delete _alertManager;
}

//-------------------------------------------------------------------------
void _SwServiceAlert::addObserver(IAlertObserver * alertObserver, QString errorCategory)
{
	_alertManager->addObserver(alertObserver, errorCategory);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::removeCategoryObserver(IAlertObserver * alertObserver)
{
	_alertManager->removeCategoryObserver(alertObserver);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseAlert(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level)
{
	SwError * error = new SwError(title, description, level);
	_alertManager->raiseError(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseError(PRefIError error)
{
	_alertManager->raiseError(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseError(PRefIError error, double timeOut)
{
	_alertManager->raiseError(error, timeOut);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseErrorEnd(PRefIError error)
{
	_alertManager->raiseErrorEnd(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseErrorEnd(PRefIErrorEnd errorEnd)
{
	_alertManager->raiseErrorEnd(errorEnd);
}

//-------------------------------------------------------------------------
AlertManager * _SwServiceAlert::getAlertManager()
{
	return _alertManager;
}
