#include "manageSQL.h"

/**
/*Include QT 
*/
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QtConcurrentRun>
#include "DatabaseManager.h"

// Initialisation du singleton à NULL
ManageSQL *ManageSQL::m_singleton = NULL;

//-------------------------------------------------------------------
ManageSQL::ManageSQL( void )
{
	_isOpen = false;
	
	_threadSQL = new DatabaseManager();
	connect(_threadSQL,SIGNAL(connectionState(bool)),this,SLOT(setDatabaseState(bool)));
	_threadSQL->start(QThread::LowestPriority);
}

//-------------------------------------------------------------------
ManageSQL::~ManageSQL(void)
{
	if(_threadSQL)
	{
		_threadSQL->quit();
		delete _threadSQL;
	}

	if(_isOpen)
		QSqlDatabase::database().close();
}

//-------------------------------------------------------------------
ManageSQL* ManageSQL::getInstance ()
{
	if (m_singleton  == NULL)
		m_singleton = new ManageSQL;

	return m_singleton;
}

//-------------------------------------------------------------------
void ManageSQL::kill()
{
	if (NULL != m_singleton)
	{
		delete m_singleton;
		m_singleton = NULL;
	}
}

//-------------------------------------------------------------------
bool ManageSQL::isOpen()
{
	if (_isOpen)
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------------
void ManageSQL::setDatabaseState(bool val)
{
	if(val)
	{
		_isOpen = true;
		QDjango::setDatabase(QSqlDatabase::database());

		//Register model
		QDjango::registerModel<TUser>();
		QDjango::registerModel<TKeyComp>();
		QDjango::registerModel<TKeyword>();
		QDjango::registerModel<TComponent>();
		QDjango::registerModel<TColor>();

		QDjango::createTables();
	}
	else
	{
		_isOpen = false;
		//QDjango::setDatabase(QSqlDatabase());
	}

	emit connectionStateChange();
}

//-------------------------------------------------------------------------
void ManageSQL::tryOpen()
{
	if(!_threadSQL->isRunning())
	{
		_threadSQL->start(QThread::LowestPriority);
	}
}

