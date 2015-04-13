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
#include <QString>
#include <QObject>
#include <QDjango.h>
#include <QDjangoQuerySet.h>
#include <QFutureWatcher>

/*
 * Include SQL Class
 */
#include "TUser.h"
#include "TKeyComp.h"
#include "TKeyword.h"
#include "TComponent.h"
#include "TColor.h"
#include "SwDocConstantes.h"


class DatabaseManager;

/**
 * this class :
 *	- is a singleton.
 *	- connect to the SQL database
 *	- manage all SQL Query
 * @author Arnaud AUBRY
 * @version 1.0
 */

class BUILD_SWDOC ManageSQL : public QObject
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

	void tryOpen();
	
	/**
	 *	Return un pointeur sur un element de bdd
	 *
	 */
	template<typename T> QList<T*> getOrmObject(QString param="",QVariant val=QVariant(),QDjangoWhere::Operation op=QDjangoWhere::None)
	 {
			QList<T*> returnL;

			if(!_isOpen)
				return returnL;

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
public slots:
	void setDatabaseState(bool);

signals : 
	void connectionStateChange();

private:

	static ManageSQL *m_singleton; /** the singleton of the class*/

	bool _isOpen;

	DatabaseManager *_threadSQL;

	
};

#endif // MANAGESQL_H