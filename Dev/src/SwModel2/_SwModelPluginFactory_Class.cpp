/*!
 \file _SwModelPluginFactory_Class.cpp
 \brief Factory de modeles
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>
#include <SwLoader_Class.h>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

#include <SwApplication.h>
#include <SwException.h>
#include <SwMacros.h>
#include "_SwModelPluginFactory_Class.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelPluginFactory_Class::_SwModelPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwModelPluginFactory_Class::~_SwModelPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwModelPluginFactory_Class::Initialize() {
    QMap<QString,_SwModelsList::_ModelDesc>::const_iterator it;

    //Chargement des modeles
    _mlist.LoadModels();
    //Enregistrement du modele host
    RegisterComponent("SwModel2Host","Model host");
    //Enregistrement d'un modele vide
    RegisterComponent("SwModel2","Empty Model");
    //Enregistrement des modeles
    for (it=_mlist.GetModelList()->begin();it!=_mlist.GetModelList()->end();it++) {
        RegisterComponent(it.key(),it.value()._model_description);   
    }
}
/*! \brief Liberation */
void _SwModelPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwModelPluginFactory_Class::CreateInstanceOf(QString name) {
    QMap<QString,_SwModelsList::_ModelDesc>::const_iterator it;

    if (name=="SwModel2Host") {
        return new _SwModelHost_Class;
    }
    if (name=="SwModel2") {
        return new _SwModel_Class;
    }
    it=_mlist.GetModelList()->find(name);
    if (it!=_mlist.GetModelList()->end()) {
        QFile file; 
        QDomDocument doc;
        QString xml_error;
        int error_line,error_column;
        SwComponent_ClassPtr root_component;

        file.setFileName(it.value()._model_source_file);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString msg=QString("Fail to open following model: %1").arg(it.value()._model_source_file);
            LAUNCH_SWEXCEPTION("SwModel2",msg)
        }
        if (!doc.setContent(QString(file.readAll()),&xml_error,&error_line,&error_column)) {
            QString msg=QString("XML Parsing:%1 at position %2,%3 of %4").arg(xml_error).arg(error_line).arg(error_column).arg(it.value()._model_source_file);
            LAUNCH_SWEXCEPTION("SwModel2",msg)
        }
        SwLoader_Class loader;
        if (SW_APP->IsVerbose()) 
            SW_APP->Logger().Log(LogLvl_Info,QString("Loading model %1\n").arg(it.value()._model_source_file));
        root_component=loader.Load(doc);
        if (root_component==NULL) {
            LAUNCH_SWEXCEPTION("SwCore","Unable to build stream or stream is empty");
        }
        return new _SwModel_Class(it.key(),root_component,it.value()._model_host_path);
    }
    
    return NULL;
}
/*! \brief instanciation d'une data */
SwData_Class * _SwModelPluginFactory_Class::CreateInstanceOf(unsigned int type_id) {
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwModelPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    QMap<QString,_SwModelsList::_ModelDesc>::const_iterator it;
    if (name=="SwModel2Host") {
        return QIcon(":/SwModel/model_host.png"); 
    }
    if (name=="SwModel2") {
        return QIcon(":/SwModel/model.png"); 
    }
    it=_mlist.GetModelList()->find(name);
    if (it!=_mlist.GetModelList()->end()) {
        return it.value()._model_ico;
    }
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwModelPluginFactory_Class::GetPluginVersion(){
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

