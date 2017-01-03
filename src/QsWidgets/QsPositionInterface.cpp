
#include "QsPosition.h"
#include "QsPositionInterface.h"

#include <QtPlugin>

QsPositionInterface::QsPositionInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsPosition","QsPosition.h")
{
}


QWidget * QsPositionInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsPosition(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsPositionInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsPosition\" name=\"Qsposition\">\n"		
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>400</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string></string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>simple starlinx stylable position setting widget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsPositionInterface::icon() const
{
	return QIcon(":QsPosition");
}
