
#include "QsComboBox.h"
#include "QsComboBoxInterface.h"

#include <QtPlugin>

QsComboBoxInterface::QsComboBoxInterface(QObject *parent)   : QsInterfaceAdapter(parent,"QsComboBox","QsComboBox.h")
{
}


QWidget * QsComboBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsComboBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsComboBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsComboBox\" name=\"Qscombobox\">\n"
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
QIcon QsComboBoxInterface::icon() const
{
	return QIcon(":QsComboBox");
}

