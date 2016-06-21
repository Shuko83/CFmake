#include "TitleWidgetAction.h"

#include <QLabel>

//---------------------------------------------------------------------------------
TitleWidgetAction::TitleWidgetAction(QObject *parent, QString title /* = "" */) : QWidgetAction(parent),_title(title)
{
}

//---------------------------------------------------------------------------------
QWidget* TitleWidgetAction::createWidget(QWidget *parent)
{
	_widget = new QLabel(_title, parent);
	return _widget;
}