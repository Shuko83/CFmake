#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include "ui_interface.h"

class Interface : public QWidget
{
	Q_OBJECT

public:
	Interface(QWidget *parent = 0);
	~Interface();

public:
	Ui::interfaceui ui;
	QRegExpValidator *v ;
};

#endif // INTERFACE_H
