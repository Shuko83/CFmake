#ifndef COMPONENT_H
#define COMPONENT_H

#include <Qtgui>
#include "ui_Component.h"

class Component : public QWidget
{
	Q_OBJECT

public:
	Component(QWidget *parent = 0);
	~Component();
	Ui::component ui;

	QString getIconPath() const { return _iconPath; }
	void setIconPath(QString val);



protected slots:
	void gbToggled(bool);
	void nbPropertiesChanged(int val);
	void nbShortcutChanged( int val );
	void nbCInterfacesChanged( int val );
	void nbPInterfacesChanged( int val );
	void lookForIcon();
	void nameChanged(const QString&val);

private:

	int _nbProperties;
	int _nbPInterfaces;
	int _nbCInterfaces;
	int _nbShortcuts;
	QString _iconPath;
	QRegExpValidator *v ;

	
};

#endif // COMPONENT_H
