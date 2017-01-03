
#include "QsButget.h"
#include "QsButgetInterface.h"

#include <QtPlugin>

QsButgetInterface::QsButgetInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsButget","Qs/widgets/QsButget.h")
{
}


QWidget * QsButgetInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsButget(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}

QString QsButgetInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsButget\" name=\"Qsbutget\">\n"
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
           "   <string>simple starlinx stylable pushbutton</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsButgetInterface::icon() const
{
	return QIcon(":QsButget");
}


