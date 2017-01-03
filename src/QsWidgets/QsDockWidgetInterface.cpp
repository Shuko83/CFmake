
#include "QsDockWidget.h"
#include "QsDockWidgetInterface.h"

#include <QtPlugin>

QsDockWidgetInterface::QsDockWidgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsDockWidget","QsDockWidget.h",true)
{
}


QWidget * QsDockWidgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsDockWidget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}



QString QsDockWidgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsDockWidget\" name=\"Qsdockwidget\">\n"
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
           "   <string>simple starlinx stylable dockwidget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsDockWidgetInterface::icon() const
{
	return QIcon(":QsDockWidget");
}
