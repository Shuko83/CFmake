
#include "QsListWidget.h"
#include "QsListWidgetInterface.h"

#include <QtPlugin>

QsListWidgetInterface::QsListWidgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsListWidget","QsListWidget.h",true)
{
}


QWidget * QsListWidgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsListWidget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsListWidgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsListWidget\" name=\"Qslistwidget\">\n"
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
QIcon QsListWidgetInterface::icon() const
{
	return QIcon(":QsListWidget");
}

