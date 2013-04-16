/*!
 \file _SwModelsList.cpp
 \brief Liste des modeles
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QDomDocument>
#include <QDomElement>
#include <SwApplication.h>
#include <SwException.h>
#include <SwMacros.h>

#include "_SwModelsList.h"

#define CL_MODELS_DIRECTORY "models"
#define CL_MODELS_LIST "models.list"

#define CL_MAIN_NODE "swmodels"
#define CL_MODEL_NODE "swmodel"
#define CL_MODEL_NAME "name"
#define CL_MODEL_FILE "file"
#define CL_MODEL_ICO "ico"
#define CL_MODEL_IPATH "ipath"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelsList::_SwModelsList() {

}
/*! \brief Destructeur */
_SwModelsList::~_SwModelsList() {

}
/*! \brief Chargement de la liste des modeles */
void _SwModelsList::LoadModels() {
    QString xml_error;
    int error_line,error_column;
    QFile file; 
    QString filename; 
    QDomDocument doc;
    QDomElement root_elt;
    QDomElement model_elt;
    QDir dir;

    //Test arguments
    QStringList liste_arg=QCoreApplication::instance()->arguments();
    for(int i=0;i<(liste_arg.count()-1);i++) {
        if (liste_arg[i]=="-models") {
            filename=liste_arg[i+1];
            QFileInfo fi(filename);
            dir=QDir(fi.absoluteDir().absolutePath()+QDir::separator()+CL_MODELS_DIRECTORY);
        }
    }
    //Test ressources
    QFile resourceList(QString(":/SwModel/")+CL_MODELS_LIST);
    if (filename.isEmpty() && resourceList.exists()) {
        filename=QString(":/SwModel/")+CL_MODELS_LIST;
        dir=QDir(QString(":/SwModel/")+CL_MODELS_DIRECTORY);
    }
    //Sinon
    if(filename.isEmpty()) {
        dir=QDir(QCoreApplication::applicationDirPath());

        //Si aucun repertoire de modeles n'existe alors on ne fait rien
        if (!dir.cd(CL_MODELS_DIRECTORY)) 
            return;
        //Chargement de la liste des models
        file.setFileName(dir.absoluteFilePath(CL_MODELS_LIST));
    } else {
        file.setFileName(filename);
    }
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        return;

    if (!doc.setContent(QString(file.readAll()),&xml_error,&error_line,&error_column)) {
        QString msg=QString("XML Parsing:%1 at position %2,%3 of %4").arg(xml_error).arg(error_line).arg(error_column).arg(CL_MODELS_LIST);
        LAUNCH_SWEXCEPTION("SwModel",msg)
    }

    //Lecture de la liste des models
    root_elt=doc.documentElement();
    for(QDomElement model_elt = root_elt.firstChildElement(CL_MODEL_NODE); !model_elt.isNull(); model_elt = model_elt.nextSiblingElement(CL_MODEL_NODE))
    {
        QString name,ico_file;
        if (model_elt.hasAttribute(CL_MODEL_NAME) && 
            model_elt.hasAttribute(CL_MODEL_FILE) && 
            model_elt.hasAttribute(CL_MODEL_IPATH)) {
            _ModelDesc mdesc;
            name=model_elt.attribute(CL_MODEL_NAME);
            mdesc._model_source_file=dir.absoluteFilePath(model_elt.attribute(CL_MODEL_FILE));
            mdesc._model_host_path=model_elt.attribute(CL_MODEL_IPATH);
            mdesc._model_description=model_elt.text();
            if (model_elt.hasAttribute(CL_MODEL_ICO)) {
                ico_file=model_elt.attribute(CL_MODEL_ICO);
                if (!ico_file.isEmpty() && dir.exists(ico_file)) {
                    mdesc._model_ico=QIcon(dir.absoluteFilePath(ico_file));   
                }
            }
            _liste.insert(name,mdesc);
        }
    }
}
/*! \brief Chargement de la liste des modeles */
const QMap<QString,_SwModelsList::_ModelDesc> * _SwModelsList::GetModelList() const {
    return &_liste;
}
