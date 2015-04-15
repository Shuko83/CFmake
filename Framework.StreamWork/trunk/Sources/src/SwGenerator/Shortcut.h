#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QWidget>
#include "ui_shortcut.h"

class Shortcut : public QWidget
{
	Q_OBJECT

public:
	Shortcut(QWidget *parent = 0);
	~Shortcut();

public:
	Ui::shortcut ui;
	QRegExpValidator *v ;

};

#endif // SHORTCUT_H
