
#include "QsInterfaceAdapter.h"

#include <QtPlugin>


QsInterfaceAdapter::QsInterfaceAdapter(QObject *parent, QString name, QString includeFile, bool isContainer)
    : QObject(parent)
{
    _initialized = false;
    _name = name;
    _includeFile =  includeFile;
	_styleSheet = "";
	_isContainer = isContainer;
}


void QsInterfaceAdapter::setStyleSheet(QString styleSheet)
{
	_styleSheet = styleSheet;
}


void QsInterfaceAdapter::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (_initialized)
        return;

    _initialized = true;
}


bool QsInterfaceAdapter::isInitialized() const
{
    return _initialized;
}


QString QsInterfaceAdapter::name() const
{
    return _name;
}


QString QsInterfaceAdapter::group() const
{
    return "Diginext [Starlinx Widgets]";
}


QIcon QsInterfaceAdapter::icon() const
{
    return QIcon();
}


QString QsInterfaceAdapter::toolTip() const
{
    return "";
}


QString QsInterfaceAdapter::whatsThis() const
{
    return "";
}


bool QsInterfaceAdapter::isContainer() const
{
    return _isContainer;
}


QString QsInterfaceAdapter::includeFile() const
{
    return _includeFile;
}


