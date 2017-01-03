
#include "QsGroupRadio.h"
#include "QsGroupRadioInterface.h"

#include <QtPlugin>

QsGroupRadioInterface::QsGroupRadioInterface(QObject *parent) : QsInterfaceAdapter(parent, "QsGroupRadio", "QsGroupRadio.h", true)
{
}


QWidget * QsGroupRadioInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsGroupRadio(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsGroupRadioInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsGroupRadio\" name=\"Qsgroupradio\">\n"
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
           "   <string>...</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsGroupRadioInterface::icon() const
{
	return QIcon(":QsGroupBox");
}
