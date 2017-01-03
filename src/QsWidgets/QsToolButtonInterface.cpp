
#include "QsToolButton.h"
#include "QsToolButtonInterface.h"

#include <QtPlugin>

QsToolButtonInterface::QsToolButtonInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsToolButton","QsToolButton.h")
{
}


QWidget * QsToolButtonInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsToolButton(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsToolButtonInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsToolButton\" name=\"Qstoolbutton\">\n"
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
           "   <string>simple starlinx stylable toolbutton</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsToolButtonInterface::icon() const
{
	return QIcon(":QsToolButton");
}
