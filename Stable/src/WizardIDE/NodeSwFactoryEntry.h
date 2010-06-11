/*!
 \file NodeSwFactoryEntry.h
 \brief Component noeud SwFactoryEntry
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeSwFactoryEntry_H
#define _NodeSwFactoryEntry_H

/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud SwFactoryEntry
*/
class NodeSwFactoryEntry : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QString Name READ Name WRITE setName)
    Q_PROPERTY(QString Author READ Author WRITE setAuthor)
    Q_PROPERTY(QString Path READ Path WRITE setPath)
    Q_PROPERTY(InDistrib IsOwnPluginInDistribution READ IsOwnPluginInDistribution WRITE setIsOwnPluginInDistribution)
    Q_ENUMS(InDistrib)
public:
    /*! \brief Property Name */
    QString _name;
    void setName(QString name) {_name=name;}
    QString Name() const {return _name;}
    /*! \brief Property Author */
    QString _author;
    void setAuthor(QString author) {_author=author;}
    QString Author() const {return _author;}
    /*! \brief Property Author */
    QString _path;
    void setPath(QString path) {_path=path;}
    QString Path() const {return _path;}
    /*! \brief Property Author */
    enum InDistrib {yes,no};
    enum InDistrib _isOwnPluginInDistribution;
    void setIsOwnPluginInDistribution(enum InDistrib isOwnPluginInDistribution) {_isOwnPluginInDistribution=isOwnPluginInDistribution;}
    enum InDistrib IsOwnPluginInDistribution() const {return _isOwnPluginInDistribution;}
public:
    /*! \brief Constructeur */
    NodeSwFactoryEntry();
    /*! \brief Destructeur */
    virtual ~NodeSwFactoryEntry();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
public slots:
    /*! slot ajout composants*/
    void Action_Add_Components();
    /*! slot ajout description*/
    void Action_Add_Description();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
