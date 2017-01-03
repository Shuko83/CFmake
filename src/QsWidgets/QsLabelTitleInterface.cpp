
#include "QsLabelTitle.h"
#include "QsLabelTitleInterface.h"

#include <QtPlugin>

QsLabelTitleInterface::QsLabelTitleInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsLabelTitle","QsLabelTitle.h")
{
}


QWidget * QsLabelTitleInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsLabelTitle(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsLabelTitleInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsLabelTitle\" name=\"Qslabeltitle\">\n"
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
           "   <string>simple starlinx stylable label</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsLabelTitleInterface::icon() const
{
	return QIcon(":QsLabelTitle");
}

