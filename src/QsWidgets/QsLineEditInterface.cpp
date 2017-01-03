
#include "QsLineEdit.h"
#include "QsLineEditInterface.h"

#include <QtPlugin>

QsLineEditInterface::QsLineEditInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsLineEdit","QsLineEdit.h")
{
}


QWidget * QsLineEditInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsLineEdit(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsLineEditInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsLineEdit\" name=\"Qslineedit\">\n"
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
           "   <string>simple starlinx stylable lineedit</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsLineEditInterface::icon() const
{
	return QIcon(":QsLineEdit");
}


