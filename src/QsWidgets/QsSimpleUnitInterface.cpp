
#include "QsSimpleUnit.h"
#include "QsSimpleUnitInterface.h"

#include <QtPlugin>

QsSimpleUnitInterface::QsSimpleUnitInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsSimpleUnit","QsSimpleUnit.h")
{
}


QWidget * QsSimpleUnitInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsSimpleUnit(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsSimpleUnitInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsSimpleUnit\" name=\"QssimpleUnit\">\n"
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
QIcon QsSimpleUnitInterface::icon() const
{
	return QIcon(":QsSimpleUnit");
}


