/*!
 \file NodeDescription.h
 \brief Component noeud Description
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeDescription_H
#define _NodeDescription_H

#include <QStringList>
/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud Description
*/
class NodeDescription : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QStringList Text READ Text WRITE setText)
public:
    /*! \brief Property Name */
    QStringList _text;
    void setText(QStringList text) {_text=text;}
    QStringList Text() const {return _text;}
public:
    /*! \brief Constructeur */
    NodeDescription();
    /*! \brief Destructeur */
    virtual ~NodeDescription();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
