
#include "QsWidget.h"
#include "QsWidgetInterface.h"

#include <QtPlugin>

QsWidgetInterface::QsWidgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsWidget","QsWidget.h",true)
{
}


QWidget * QsWidgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsWidget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsWidgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsWidget\" name=\"Qswidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string></string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>simple starlinx stylable widget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsWidgetInterface::icon() const
{
	return QIcon(":QsWidget");
}