
#include "QsCheckBox.h"
#include "QsCheckBoxInterface.h"

#include <QtPlugin>

QsCheckBoxInterface::QsCheckBoxInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsCheckBox","QsCheckBox.h")
{
}


QWidget * QsCheckBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsCheckBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsCheckBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsCheckBox\" name=\"Qscheckbox\">\n"
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
           "   <string>simple starlinx stylable checkbox</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsCheckBoxInterface::icon() const
{
	return QIcon(":QsCheckBox");
}


