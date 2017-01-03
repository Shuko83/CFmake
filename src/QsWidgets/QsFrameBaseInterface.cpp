
#include "QsFrameBase.h"
#include "QsFrameBaseInterface.h"

#include <QtPlugin>

QsFrameBaseInterface::QsFrameBaseInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsFrameBase","QsFrameBase.h",true)
{
}


QWidget * QsFrameBaseInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsFrameBase(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsFrameBaseInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsFrameBase\" name=\"Qsframebase\">\n"
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
QIcon QsFrameBaseInterface::icon() const
{
	return QIcon(":QsFrameBase");
}
