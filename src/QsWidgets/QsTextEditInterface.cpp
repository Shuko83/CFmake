
#include "QsTextEdit.h"
#include "QsTextEditInterface.h"

#include <QtPlugin>

QsTextEditInterface::QsTextEditInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsTextEdit","QsTextEdit.h")
{
}


QWidget * QsTextEditInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsTextEdit(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsTextEditInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsTextEdit\" name=\"Qstextedit\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>The current time</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>simple starlinx stylable textedit</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsTextEditInterface::icon() const
{
	return QIcon(":QsTextEdit");
}
