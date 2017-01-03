
#include "QsThumbWheel.h"
#include "QsThumbWheelInterface.h"

#include <QtPlugin>

QsThumbWheelInterface::QsThumbWheelInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsThumbWheel","QsThumbWheel.h")
{
}
       

QWidget * QsThumbWheelInterface::createWidget(QWidget *parent)
{
    QWidget * widget = new QsThumbWheel(parent);
    if (_styleSheet != "")
        widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsThumbWheelInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
        " <widget class=\"QsThumbWheel\" name=\"Qsthumbwheel\">\n"
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
        "   <string>simple starlinx stylable thumbwheel</string>\n"
        "  </property>\n"
        " </widget>\n"
        "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsThumbWheelInterface::icon() const
{
	return QIcon(":QsThumbWheel");
}
