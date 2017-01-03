
#include "QsSearchComboBox.h"
#include "QsSearchComboBoxInterface.h"

#include <QtPlugin>

QsSearchComboBoxInterface::QsSearchComboBoxInterface(QObject *parent)   : QsInterfaceAdapter(parent,"QsSearchComboBox","QsSearchComboBox.h")
{
}


QWidget * QsSearchComboBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsSearchComboBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsSearchComboBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsSearchComboBox\" name=\"searchComboBox\">\n"
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
           "   <string>simple starlinx stylable combobox</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsSearchComboBoxInterface::icon() const
{
	return QIcon(":QsComboBox");
}

