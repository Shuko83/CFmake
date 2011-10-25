#ifndef MANAGESQL_H
#define MANAGESQL_H

/** 
 * Copyright	© 2010, Diginext S.A. All rights reserved
 * @author 		Arnaud AUBRY
 * @version		1.0
 */

/**
 * Include QT
 */
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QObject>
#include <QDjango>
#include <QDjangoQuerySet>

/*
 * Include SQL Class
 */
#include "TUser.h"
#include "TKeyComp.h"
#include "TKeyword.h"
#include "TComponent.h"

/**
 * this class :
 *	- is a singleton.
 *	- connect to the SQL database
 *	- manage all SQL Query
 * @author Arnaud AUBRY
 * @version 1.0
 */

class ManageSQL : public QObject
{
	Q_OBJECT

public:

	/**
	 * Constructor of the ManageSQL which try to connect to the SQL database
	 */
	ManageSQL();

	/**
	 * Destructor of the class ManageSQL which close the connection with the SQL database
	 */
	virtual ~ManageSQL();

	/**
	 * Get the unique instance of the ManageSQL	
	 * @return the instance of the singleton ManageSQL	
	 */
	static ManageSQL *getInstance ();

	/**
	 * Test if the bdd is open otherwise, try to reconnect
	 * @return true if opened
	 */
	bool isOpen();
	
	/**
	 * Close the database and destroy the instance of ManageSQL
	 */
	static void kill ();
	
	/**
	 * return the latest log
	 * After making a query, you can use this method to get the latest log.
	 * @return a QString of the latest log
	 */
	QString getLastTxtLog();

	/**
	 *	Return un pointeur sur un element de bdd
	 *
	 */
	template<typename T> QList<T*> getOrmObject(QString param="",QVariant val=QVariant(),QDjangoWhere::Operation op=QDjangoWhere::None)
	 {
			QList<T*> returnL;
			QDjangoQuerySet<T> query;
			if(param != "")
			{
				query = query.filter(QDjangoWhere(param, op, val));
			}
			for (int i = 0; i < query.size(); ++i) 
			{
				T* tmpval = query.at(i);
				if (tmpval) 
				{
					returnL << tmpval;
				}
			}
			return returnL;
	 }

private:

	static ManageSQL *m_singleton; /** the singleton of the class*/

	QString m_log;  /** the latest log for query*/

	QSqlDatabase m_db;  /** the database connection*/

	bool m_isQueryViewable; /** if true display all query in debug windows*/
	
};

#endif // MANAGESQL_H