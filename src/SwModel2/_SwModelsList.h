/*!
 \file _SwModelsList.h
 \brief Liste des modeles
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef __SwModelsList_H
#define __SwModelsList_H

#include <QMap>
#include <QIcon>
#include <QFile>
#include "_ISwModelsListListener.h"

/*!
	\class classe conteneur de la liste des models
	\brief classe definissant un plugin usine
*/
class _SwModelsList {
public:
    /*! \brief classe decrivant un modele */
    class _ModelDesc {
        public:
            QIcon   _model_ico;
            QString _model_description;
            QString _model_source_file;
            QString _model_host_path;
            /*! \brief Constructeur */
            _ModelDesc() {
                _model_ico=QIcon(":/SwModel/model.png");
            }
            _ModelDesc(const _ModelDesc &other) {
                _model_ico=other._model_ico;
                _model_description=other._model_description;
                _model_source_file=other._model_source_file;
                _model_host_path=other._model_host_path;
            }
            _ModelDesc &operator=(const _ModelDesc &other) {
                _model_ico=other._model_ico;
                _model_description=other._model_description;
                _model_source_file=other._model_source_file;
                _model_host_path=other._model_host_path;
                return *this;
            }
    };
protected:
    /*! \brief Map contenant la liste des modeles */
    QMap<QString,_ModelDesc> _liste;
    /*! \brief Path des models */
    QString modelsPath;
    /*! \brief model list file name */
    QFile modelsListFile;
    /*! \brief listener*/
    _ISwModelsListListener * listener;
public:
    /*! \brief Constructeur */
    _SwModelsList();
    /*! \brief Destructeur */
    ~_SwModelsList();
    /*! \brief Chargement de la liste des modeles */
    void LoadModels();
    /*! \brief Chargement de la liste des modeles */
    const QMap<QString,_SwModelsList::_ModelDesc> * GetModelList() const;
    /*! \brief getModelsPath */
    QString getModelPaths();
    /** @brief add model */
    void addModel(QString hostComponent,QString modelName);

    void registerListener(_ISwModelsListListener * listener);
};


#endif
