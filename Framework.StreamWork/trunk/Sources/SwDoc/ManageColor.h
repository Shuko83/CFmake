#ifndef ManageColor_H
#define ManageColor_H

/** 
 * Copyright	© 2010, Diginext S.A. All rights reserved
 * @author 		Arnaud AUBRY
 * @version		1.0
 */
#include <QMap>
#include "SwDocConstantes.h"
#include <QObject>

class BUILD_SWDOC ManageColor  : public QObject
{
	Q_OBJECT
public:

	/**
	 * Constructor of the ManageColor which try to connect to the SQL database
	 */
	ManageColor();

	/**
	 * Destructor of the class ManageColor which close the connection with the SQL database
	 */
	virtual ~ManageColor();

	/**
	 * Get the unique instance of the ManageColor	
	 * @return the instance of the singleton ManageColor	
	 */
	static ManageColor *getInstance ();

	/**
	 * Close the database and destroy the instance of ManageColor
	 */
	static void kill ();
	
	/**
	 * @brief    : Get map of color, ask only one time the database
	 * @return   : QMap<QString,QString> - map of interfaceName, Color
	 */
	QMap<QString,QString> getColor();

public slots:
	void colorChangeStatus();

signals : 
	void dbLoaded();
private:

	static ManageColor *m_singleton; /** the singleton of the class */
	QMap<QString,QString> _map; /** Map<InterfaceName,ColorInString> */
};

#endif // ManageColor_H