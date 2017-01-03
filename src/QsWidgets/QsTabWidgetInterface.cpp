
#include "QsTabWidget.h"
#include "QsTabWidgetInterface.h"

#include <QtPlugin>

QsTabWidgetInterface::QsTabWidgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsTabWidget","QsTabWidget.h",true)
{
}


QWidget * QsTabWidgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsTabWidget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsTabWidgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsTabWidget\" name=\"Qstabwidget\">\n"
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
           "   <string>simple starlinx stylable tabwidget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsTabWidgetInterface::icon() const
{
	return QIcon(":QsTabWidget");
}
