/*!
 \file _SwExecutionPluginFactory_Class.cpp
 \brief Factory d'execution
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>

#include "_SwExecutionPluginFactory_Class.h"
#include "_SwExecutor.h"
#include "_SwSwitchExecutor.h"
#include "_SwSimpleExecutable.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwExecutionPluginFactory_Class::_SwExecutionPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwExecutionPluginFactory_Class::~_SwExecutionPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwExecutionPluginFactory_Class::Initialize() {
    RegisterComponent("SwExecutor","Executor component");
    RegisterComponent("SwSwitchExecutor","SwitchExecutor component");
    RegisterComponent("SwSimpleExecutable","Simple executable component");
}
/*! \brief Liberation */
void _SwExecutionPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwExecutionPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwExecutor") {
        return new _SwExecutor;
    }
    if (name=="SwSwitchExecutor") {
        return new _SwSwitchExecutor;
    }
    if (name=="SwSimpleExecutable") {
        return new _SwSimpleExecutable;
    }
    return NULL;
}
/*! \brief instanciation d'une data */
SwData_Class * _SwExecutionPluginFactory_Class::CreateInstanceOf(unsigned int type_id) {
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwExecutionPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="SwExecutor") {
        return QIcon(":/SwExecution/executor.png"); 
    }
    if (name=="SwSwitchExecutor") {
        return QIcon(":/SwExecution/switchExecutor.png"); 
    }
    if (name=="SwSimpleExecutable") {
        return QIcon(":/SwExecution/executor_simple.png"); 
    }
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwExecutionPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}

