
#include "QsLabel.h"
#include "QsLabelInterface.h"

#include <QtPlugin>

QsLabelInterface::QsLabelInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsLabel","QsLabel.h")
{
}


QWidget * QsLabelInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsLabel(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsLabelInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsLabel\" name=\"Qslabel\">\n"
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
           "   <string>simple starlinx stylable label</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsLabelInterface::icon() const
{
	return QIcon(":QsLabel");
}
