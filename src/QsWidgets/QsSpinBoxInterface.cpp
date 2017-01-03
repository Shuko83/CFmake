
#include "QsSpinBox.h"
#include "QsSpinBoxInterface.h"

#include <QtPlugin>

QsSpinBoxInterface::QsSpinBoxInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsSpinBox","QsSpinBox.h")
{
}


QWidget * QsSpinBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsSpinBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsSpinBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsSpinBox\" name=\"Qsspinbox\">\n"
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
           "   <string>simple starlinx stylable spinbox</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsSpinBoxInterface::icon() const
{
	return QIcon(":QsSpinBox");
}
