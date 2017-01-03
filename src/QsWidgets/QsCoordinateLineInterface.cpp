
#include "QsCoordinateLine.h"
#include "QsCoordinateLineInterface.h"

#include <QtPlugin>


QsCoordinateLineInterface::QsCoordinateLineInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsCoordinateLine","QsCoordinateLine.h")
{
}


QWidget * QsCoordinateLineInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsCoordinateLine(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsCoordinateLineInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsCoordinateLine\" name=\"Qscoordinateline\">\n"
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
           "   <string>simple starlinx stylable coordinate line edit</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsCoordinateLineInterface::icon() const
{
	return QIcon(":QsCoordinateLine");
}
