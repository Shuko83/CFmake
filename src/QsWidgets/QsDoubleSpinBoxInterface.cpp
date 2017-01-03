
#include "QsDoubleSpinBox.h"
#include "QsDoubleSpinBoxInterface.h"

#include <QtPlugin>

QsDoubleSpinBoxInterface::QsDoubleSpinBoxInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsDoubleSpinBox","QsDoubleSpinBox.h")
{
}


QWidget * QsDoubleSpinBoxInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsDoubleSpinBox(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsDoubleSpinBoxInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsDoubleSpinBox\" name=\"Qsdoublespinbox\">\n"
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
           "   <string>simple starlinx stylable double spinbox</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsDoubleSpinBoxInterface::icon() const
{
	return QIcon(":QsDoubleSpinBox");
}


