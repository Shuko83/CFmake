
#include "QsGroupBox.h"
#include "QsGroupBoxInterface.h"

#include <QtPlugin>

QsGroupBoxInterface::QsGroupBoxInterface(QObject *parent)    : QsInterfaceAdapter(parent,"QsGroupBox","QsGroupBox.h",true)
{
}


QWidget * QsGroupBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsGroupBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsGroupBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsGroupBox\" name=\"Qsgroupbox\">\n"
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
           "   <string>simple starlinx stylable groupbox</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsGroupBoxInterface::icon() const
{
	return QIcon(":QsGroupBox");
}
