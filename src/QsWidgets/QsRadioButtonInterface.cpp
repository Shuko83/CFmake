
#include "QsRadioButton.h"
#include "QsRadioButtonInterface.h"

#include <QtPlugin>

QsRadioButtonInterface::QsRadioButtonInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsRadioButton","QsRadioButton.h")
{
}


QWidget * QsRadioButtonInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsRadioButton(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsRadioButtonInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsRadioButton\" name=\"Qsradiobutton\">\n"
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
           "   <string>simple starlinx stylable radiobutton</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsRadioButtonInterface::icon() const
{
	return QIcon(":QsRadioButton");
}
