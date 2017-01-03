/**
@file ManageLinkColor.h
@brief gestion des couleurs des liens
@author A.Aubry
 */

#ifndef _MANAGELINKCOLOR_H
#define _MANAGELINKCOLOR_H

#include <QColor>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

class ManageLinkColor : public QObject
{
	Q_OBJECT
public:

	static ManageLinkColor * getInstance();

	/**
	 * @brief    : Return QColor of the interfaceName
	 * @return   : QColor - 
	 * @param	 : QString interfaceName - The interfaceName
	 */
	QColor getColorForInterface(QString interfaceName);

	/**
	 * @brief    : Get map for drawing legend
	 * @return   : QMap<QString,QString> - <Interface,ColorHex>
	 */
	QMap<QString,QString> getMapColor() const { return _mapColor; }

private:
	/**
	 * @brief	: load static if no color loaded for database
	 */
	void loadStaticColor();


signals:
	void colorHasChange();
		
private :
	ManageLinkColor();
	~ManageLinkColor();

	static ManageLinkColor * _instance;
	QMap<QString,QString> _mapColor; // <Interface,ColorHex>
};


#endif
