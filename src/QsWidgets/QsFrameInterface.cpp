
#include "QsFrame.h"
#include "QsFrameInterface.h"

#include <QtPlugin>

QsFrameInterface::QsFrameInterface(QObject *parent)    : QsInterfaceAdapter(parent,"QsFrame","QsFrame.h",true)
{
}


QWidget * QsFrameInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsFrame(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsFrameInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsFrame\" name=\"Qsframe\">\n"
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
           "   <string>simple starlinx stylable frame</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsFrameInterface::icon() const
{
	return QIcon(":QsFrame");
}
