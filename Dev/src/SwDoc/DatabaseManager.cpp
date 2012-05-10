#include "DatabaseManager.h"
#include <QDebug>
//-------------------------------------------------------------------------
DatabaseManager::DatabaseManager()
	: QThread()
{
	m_db = QSqlDatabase::addDatabase("QMYSQL");
	_isInit = false;
	_nbTry = 0;
}

//-------------------------------------------------------------------------
DatabaseManager::~DatabaseManager()
{

}

//-------------------------------------------------------------------------
void DatabaseManager::run()
{
	while(MaxTryConnection != _nbTry)
	{
		if(!m_db.isOpen())
		{
			if(!_isInit)
				initDB();

			if(openDB())
			{
				emit connectionState(true);
				break;
			}
			else
				emit connectionState(false);

			_nbTry++;
		}
		else
		{
			m_db.close();
			_nbTry = MaxTryConnection;
			emit connectionState(false);
		}

		sleep(1);
	}

	_nbTry = 0;
}

//-------------------------------------------------------------------------
bool DatabaseManager::openDB()
{
	bool openStatus = false;
	if(m_db.isValid())
		openStatus = m_db.open();
	return openStatus;
}

//-------------------------------------------------------------------------
void DatabaseManager::initDB()
{
	
//	m_db.setHostName("localhost");
//	m_db.setDatabaseName("streamworkdoc");
//	m_db.setUserName("root");
//	m_db.setPassword("");
	
 	m_db.setHostName("tt3.aix.diginext.fr");
 	m_db.setDatabaseName("streamworkdoc");
 	m_db.setUserName("streamwork");
 	m_db.setPassword("streamworkdoc");

	_isInit = true;
}
