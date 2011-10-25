
/**
@file ManageLinkColor.h
@brief gestion des couleurs des liens
@author A.Aubry
 */

#ifndef _MANAGELINKCOLOR_H
#define _MANAGELINKCOLOR_H

#include <QColor>
#include <QtGui>


class ManageLinkColor
{
public:

	static ManageLinkColor * getInstance();
	QColor getColorForInterface(QString interfaceName);

private :
	ManageLinkColor();
	~ManageLinkColor();

	void loadColor();

	static ManageLinkColor * _instance;
	QMap<QString,QString> _mapColor;

};


#endif
