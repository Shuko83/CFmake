/*!
 \file _SwEditorPluginFactory_Class.cpp
 \brief factory SwEditor
 \date 2006-11-08
 \version 1.0
 \author F.Bighelli
 \note first generation by plugin generator
*/

#include <QRegularExpression>
#include <QTime>

#include "_SwEditorPluginFactory_Class.h"
#include "_SwEditorCompToolbox.h"
#include "_SwEditorApplicativeCore.h"
#include "_SwEditorCompStreamTabBar.h"
#include "_SwEditorStreamsManagerActions.h"
#include "_SwEditorStreamNavigatorActions.h"
#include "_SwEditorStreamOperationsActions.h"
#include "_SwEditorStreamView.h"
#include "info_SwEditor.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorPluginFactory_Class::_SwEditorPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwEditorPluginFactory_Class::~_SwEditorPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwEditorPluginFactory_Class::Initialize() {

    RegisterComponent("SwEditorCompToolbox","Toolbox with list of available components");
    RegisterComponent("SwEditorApplicativeCore","Core of SwEditor");
    RegisterComponent("SwEditorCompStreamTabBar","TabBar of streams");
    RegisterComponent("SwEditorStreamsManagerActions","Actions for stream management");
    RegisterComponent("SwEditorStreamNavigatorActions","Actions for in-stream navigation");
    RegisterComponent("SwEditorStreamOperationsActions","Actions for in-stream operations");
    RegisterComponent("SwEditorStreamView","Graphical view of stream");
}
/*! \brief Liberation */
void _SwEditorPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwEditorPluginFactory_Class::CreateInstanceOf(QString name) {

    if (name=="SwEditorCompToolbox") {
        return new _SwEditorCompToolbox;
    }
    if (name=="SwEditorApplicativeCore") {
        return new _SwEditorApplicativeCore;
    }
    if (name=="SwEditorCompStreamTabBar") {
        return new _SwEditorCompStreamTabBar;
    }
    if (name=="SwEditorStreamsManagerActions") {
        return new _SwEditorStreamsManagerActions;
    }
    if (name=="SwEditorStreamNavigatorActions") {
        return new _SwEditorStreamNavigatorActions;
    }
    if (name=="SwEditorStreamOperationsActions") {
        return new _SwEditorStreamOperationsActions;
    }
    if (name=="SwEditorStreamView") {
        return new _SwEditorStreamView;
    }

    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwEditorPluginFactory_Class::CreateIconOf(QString name) const{
    QIcon ico;

    if (name=="SwEditorCompToolbox") {
        return QIcon(":/SwEditor/comp_toolbox.png"); 
    }
    if (name=="SwEditorApplicativeCore") {
        return QIcon(":/SwEditor/brain.png"); 
    }
    if (name=="SwEditorCompStreamTabBar") {
        return QIcon(":/SwEditor/tabbar.png"); 
    }
    if (name=="SwEditorStreamsManagerActions") {
        return QIcon(":/SwEditor/actions.png"); 
    }
    if (name=="SwEditorStreamNavigatorActions") {
        return QIcon(":/SwEditor/actions.png"); 
    }
    if (name=="SwEditorStreamOperationsActions") {
        return QIcon(":/SwEditor/actions.png"); 
    }
    if (name=="SwEditorStreamView") {
        return QIcon(":/SwEditor/graphicsview.png"); 
    }
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwEditorPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}

QDateTime _SwEditorPluginFactory_Class::GetPluginCompilationDate(){	
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime,"hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return finalDateTime;
}

QString _SwEditorPluginFactory_Class::GetPluginName() const
{
	return SwEditor::name();
}

SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwEditorPluginFactory_Class;
}
