
#include "QsScrollArea.h"
#include "QsScrollAreaInterface.h"

#include <QtPlugin>

QsScrollAreaInterface::QsScrollAreaInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsScrollArea","QsScrollArea.h",true)
{
}


QWidget * QsScrollAreaInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsScrollArea(parent);
	if (_styleSheet != "")
		widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsScrollAreaInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsScrollArea\" name=\"Qsscrollarea\">\n"
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
           "   <string>simple starlinx stylable widget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsScrollAreaInterface::icon() const
{
	return QIcon(":QsScrollArea");
}
