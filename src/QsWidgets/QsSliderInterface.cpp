
#include "QsSlider.h"
#include "QsSliderInterface.h"

#include <QtPlugin>

QsSliderInterface::QsSliderInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsSlider","QsSlider.h")
{
}


QWidget * QsSliderInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsSlider(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}


QString QsSliderInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsSlider\" name=\"Qsslider\">\n"
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
           "   <string>simple starlinx stylable slider</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//-------------------------------------------------------------------------
QIcon QsSliderInterface::icon() const
{
	return QIcon(":QsSlider");
}
