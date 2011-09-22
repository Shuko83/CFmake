#ifndef PROPERTY_H
#define PROPERTY_H

#include <QWidget>
#include "ui_property.h"

class Property : public QWidget
{
	Q_OBJECT

public:
	Property(QWidget *parent = 0);
	~Property();

protected slots:
	void changeValidator(const QString & val);

public:
	Ui::property ui;
	QRegExpValidator *_vString ;
	QRegExpValidator *_vQString ;
	QRegExpValidator *_vInt ;
	QRegExpValidator *_vUnsignedInt ;
	QRegExpValidator *_vDouble ;
	QRegExpValidator *_vBool ;

};

#endif // PROPERTY_H
