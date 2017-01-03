

#include "QsSmartComboBox.h"
#include "QsSmartComboBoxInterface.h"

#include <QtPlugin>


QsSmartComboBoxInterface::QsSmartComboBoxInterface(QObject *parent)   : QsInterfaceAdapter(parent,"QsSmartComboBox","QsSmartComboBox.h")
{
}


QWidget * QsSmartComboBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsSmartComboBox(parent);
	if (_styleSheet != "")
    widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsSmartComboBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsSmartComboBox\" name=\"smartComboBox\">\n"
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
QIcon QsSmartComboBoxInterface::icon() const
{
	return QIcon(":QsComboBox");
}

