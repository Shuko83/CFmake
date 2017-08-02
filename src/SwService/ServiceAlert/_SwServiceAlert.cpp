#include "_SwServiceAlert.h"

//QT includes
#include <QApplication>

//Other External includes
#include "AlertManager.h"
#include "ErrorDb.h"

//Local includes
#include "_SwError.h"
#include "_SwResponse.h"

// namespace
using namespace alert;


//-------------------------------------------------------------------------
_SwServiceAlert::_SwServiceAlert()
{
	_alertManager = new AlertManager();
    _errorDb = new ErrorDb();
    addObserver(_errorDb, "");
}

//-------------------------------------------------------------------------
_SwServiceAlert::~_SwServiceAlert()
{
    removeCategoryObserver(_errorDb);
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
   IError * error = new SwError(title, description, level, _alertManager); //FIXME A CORRIGER EN IError * TRUC MUCHE

	_alertManager->raiseError(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseError(IError * error)
{
	_alertManager->raiseError(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseError(IError * error, double timeOut)
{
	_alertManager->raiseError(error, timeOut);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseErrorEnd(IError * error)
{
	_alertManager->raiseErrorEnd(error);
}

//-------------------------------------------------------------------------
void _SwServiceAlert::raiseErrorEnd(IErrorEnd *errorEnd)
{
	_alertManager->raiseErrorEnd(errorEnd);
}

//-------------------------------------------------------------------------
AlertManager * _SwServiceAlert::getAlertManager()
{
	return _alertManager;
}

//-------------------------------------------------------------------------
alert::IErrorDb * StreamWork::SwCore::_SwServiceAlert::getErrorDb()
{
    return _errorDb;
}
