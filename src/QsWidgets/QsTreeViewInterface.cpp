#include "QsTreeViewInterface.h"

#include "QsTreeView.h"

#include <QtPlugin>

//-------------------------------------------------------------------------
QsTreeViewInterface::QsTreeViewInterface(QObject *parent)
    : QsInterfaceAdapter(parent,"QsTreeView","QsTreeView.h")
{
}

//-------------------------------------------------------------------------
QWidget * QsTreeViewInterface::createWidget(QWidget *parent)
{
	QWidget * widget = new QsTreeView(parent);
	if (_styleSheet != "")
	widget->setStyleSheet(_styleSheet);
    return widget;
}

//-------------------------------------------------------------------------
QString QsTreeViewInterface::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QsTreeView\" name=\"treeview\">\n"
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
           "   <string>simple starlinx stylable treeview</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

//-------------------------------------------------------------------------
QIcon QsTreeViewInterface::icon() const
{
	return QIcon(":QsWidget");
}
