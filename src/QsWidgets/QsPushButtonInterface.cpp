
#include "QsPushButton.h"
#include "QsPushButtonInterface.h"

#include <QtPlugin>

QsPushButtonInterface::QsPushButtonInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsPushButton","QsPushButton.h")
{
}


QWidget * QsPushButtonInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsPushButton(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsPushButtonInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsPushButton\" name=\"Qspushbutton\">\n"
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
           "   <string>simple starlinx stylable pushbutton</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsPushButtonInterface::icon() const
{
	return QIcon(":QsPushButton");
}

