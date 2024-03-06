/*!
\file _SwGuiCssEditor.cpp
\brief
\version 1.0
\date
\author
*/

#include <QApplication>
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "SwRef/SwRefPtr.h"
#include <QList.h>
#include "_SwGuiCssEditor.h"
#include "_SwGuiCssDialog.h"
#include <QDialog>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//---------------------------------------------------------------------
_SwGuiCssEditor::_SwGuiCssEditor() :Component()
{
	_styleFile = "";
}

//---------------------------------------------------------------------
_SwGuiCssEditor::~_SwGuiCssEditor()
{


}

//---------------------------------------------------------------------
void _SwGuiCssEditor::initializeComponent() throw(SwException)
{
	getPropertiesService().CreatePropertiesForQObject(this, QString(), true);
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::terminateComponent()
{
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::eventPropertyChange(ISwProperty * property)
{
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::eventReceiveData(SwPin * src, SwData_Class * data)
{

}

//---------------------------------------------------------------------
void _SwGuiCssEditor::eventBeforeInterfaceAvailability(QString interface_name, SwComponent_Class * provider_host)
{
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::eventAfterInterfaceAvailability(QString interface_name, SwComponent_Class * provider_host)
{
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::eventActivationChanged()
{
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::AdminSetup()
{
	_SwGuiCssDialog *tmpCssDialog = new _SwGuiCssDialog;
	tmpCssDialog->setMyStyleSheet(SW_APP->GetApplicationDirPath() + QDir::separator() + getStyle());
	tmpCssDialog->show();
	connect(tmpCssDialog, SIGNAL(saveCSS()), this, SLOT(applyStyle()));
}

//---------------------------------------------------------------------
QString _SwGuiCssEditor::getStyle()
{
	//remove the app dir path
	return _styleFile.remove(QString(SW_APP->GetApplicationDirPath() + QDir::separator()));
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::applyStyle()
{
	setStyle(getStyle());
}

//---------------------------------------------------------------------
void _SwGuiCssEditor::setStyle(QString val)
{
	if ( _cssWatcher.files().isEmpty() && SW_APP->developerMode() )
	{
		_cssWatcher.addPath(SW_APP->GetApplicationDirPath() + QDir::separator() + val);
		QObject::connect(&_cssWatcher, SIGNAL(fileChanged(QString)), this, SLOT(setStyle(QString)));
	}

	if ( val.contains(SW_APP->GetApplicationDirPath() + QDir::separator(), Qt::CaseInsensitive) )
		val.remove(SW_APP->GetApplicationDirPath() + QDir::separator());

	//Add app dir path 
	_styleFile = SW_APP->GetApplicationDirPath() + QDir::separator() + val;

	QFile file(SW_APP->GetApplicationDirPath() + QDir::separator() + val);
	if ( file.open(QFile::ReadOnly) )
	{
		QString styleSheet = QLatin1String(file.readAll());
		qApp->setStyleSheet(styleSheet);
	}

}
