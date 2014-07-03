/*!
 \file SwSimpleComponent_Class.h
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _SwSimpleComponent_Class_H
#define _SwSimpleComponent_Class_H

/*
  * INCLUDES LOCAUX
  */
#include <QDomElement>
#include <QDomDocument>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;

/*!
	\class SwSimpleComponent_Class
	\brief Component simple
*/
class SwSimpleComponent_Class : public SwComponent_Class {
	Q_OBJECT
protected:
	//--------------------------------------------------------------
	//Services
	//--------------------------------------------------------------
	/* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
protected:
	/* internal save */
    QDomElement InternalSave(QDomDocument & doc);
	/* internal save attributs*/
    void InternalSaveAttributs(QDomElement & parent,QDomDocument & doc);
	/* internal save childrens*/
    void InternalSaveChildrens(QDomElement & parent,QDomDocument & doc);
public:
    /*! \brief Constructeur */
    SwSimpleComponent_Class();
    /*! \brief Destructeur */
    virtual ~SwSimpleComponent_Class();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    void InitializeResources() throw(SwException);
    /*! \brief Sauvegarde*/
    void Save(QDomDocument & doc);
public: //A surcharger
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! label noeud*/
    virtual QString GetNodeLabelVisu();
public slots:
    /*! slot destruction enfants*/
    void Action_Delete();

};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
