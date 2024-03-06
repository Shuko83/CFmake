/*!
 \file _SwGuiCompPluginsTree.h
 \brief Implementation of the Class _SwGuiCompPluginsTree generant un QTreeView contenant la liste des plugins
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompPluginsTree_H
#define __SwGuiCompPluginsTree_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QTreeView>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include <QAction>


using namespace StreamWork::SwCore;

/*!
	\class _SwGuiCompPluginsTree 
	\brief Implementation of the Class _SwGuiCompPluginsTree generant un QTreeView contenant la liste des plugins
*/
class _SwGuiCompPluginsTree : public SwComponent_Class
{
    Q_OBJECT
protected:
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
	/* @brief propriété window title */
    ISwProperty * _window_title_property;
	/* @brief window title utilise */
	QString	_window_title;
    /* Action d'affichage */
    QAction * _action;
public:
    /*! \brief Constructeur */
    _SwGuiCompPluginsTree();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompPluginsTree();

    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);

    /*! \brief Renvoie le Action
    \return le Action */
	QAction & GetAction();
	/** @brief changement de proprietes */
	void OnPropertyChange(ISwProperty * property);
protected slots:
    /*! \brief Affiche les plugins */
	void showplugins();

};
#endif 
