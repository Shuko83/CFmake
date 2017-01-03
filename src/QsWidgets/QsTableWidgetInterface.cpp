
#include "QsTableWidget.h"
#include "QsTableWidgetInterface.h"

#include <QtPlugin>

QsTableWidgetInterface::QsTableWidgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsTableWidget","QsTableWidget.h",true)
{
}


QWidget * QsTableWidgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsTableWidget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsTableWidgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsTableWidget\" name=\"Qstablewidget\">\n"
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
           "   <string>simple starlinx stylable tablewidget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsTableWidgetInterface::icon() const
{
	return QIcon(":QsTableWidget");
}
