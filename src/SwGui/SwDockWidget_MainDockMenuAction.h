#ifndef MENUACTION_H
#define MENUACTION_H


#include <QAction>

class SwDockWidget_MainDockMenuAction : public QAction
{
	Q_OBJECT

public:
	SwDockWidget_MainDockMenuAction(QObject * parent, QString enabledIcon, QString disabledIcon, QString checkedIcon);
	~SwDockWidget_MainDockMenuAction();

protected slots:
	void updateState();

private:
	QIcon _enabled;
	QIcon _disabled;
	QIcon _checked;
};






#endif