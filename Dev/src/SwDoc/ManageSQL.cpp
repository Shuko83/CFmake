#include "manageSQL.h"

/**
/*Include QT 
*/
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>



// Initialisation du singleton à NULL
ManageSQL *ManageSQL::m_singleton = NULL;

//-------------------------------------------------------------------
ManageSQL::ManageSQL( void )
{
	m_db = QSqlDatabase::addDatabase("QMYSQL");
// 	m_db.setHostName("localhost");
// 	m_db.setDatabaseName("streamworkdoc");
// 	m_db.setUserName("root");
// 	m_db.setPassword("");

	m_db.setHostName("tt3.aix.diginext.fr");
	m_db.setDatabaseName("streamworkdoc");
	m_db.setUserName("streamwork");
	m_db.setPassword("streamworkdoc");


	QDjango::setDatabase(m_db);

	m_isQueryViewable = false;


	//Register model
	QDjango::registerModel<TUser>();
	QDjango::registerModel<TKeyComp>();
	QDjango::registerModel<TKeyword>();
	QDjango::registerModel<TComponent>();

}

//-------------------------------------------------------------------
ManageSQL::~ManageSQL(void)
{
	m_db.close();
}

//-------------------------------------------------------------------
ManageSQL* ManageSQL::getInstance ()
{
	if (m_singleton  == NULL)
		m_singleton = new ManageSQL;

	m_singleton->isOpen();

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
	if (m_db.isOpen())
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------
QString ManageSQL::getLastTxtLog()
{
	return m_log;
}

