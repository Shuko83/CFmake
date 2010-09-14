/*!
 \file NodeComponents.h
 \brief Component noeud SwFactoryEntry
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeComponents_H
#define _NodeComponents_H

/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud SwFactoryEntry
*/
class NodeComponents : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QString Name READ Name WRITE setName)
    Q_PROPERTY(QString Description READ Description WRITE setDescription)
    Q_PROPERTY(bool Executable READ Executable WRITE setExecutable)
public:
    /*! \brief Property Name */
    QString _name;
    void setName(QString name) {_name=name;}
    QString Name() const {return _name;}
    /*! \brief Property Name */
    QString _description;
    void setDescription(QString description) {_description=description;}
    QString Description() const {return _description;}
    /*! \brief Property Executable */
    bool _executable;
    void setExecutable(bool executable) {_executable=executable;}
    bool Executable() const {return _executable;}
public:
    /*! \brief Constructeur */
    NodeComponents();
    /*! \brief Destructeur */
    virtual ~NodeComponents();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
public slots:
    /*! slot ajout Interfaces*/
    void Action_Add_Interfaces();
    /*! slot ajout Properties*/
    void Action_Add_Properties();
    /*! slot ajout Connectors*/
    void Action_Add_Connectors();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
