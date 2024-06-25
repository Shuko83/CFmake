/**
@file _ISwModelsListListener.h
@brief listener de la liste des mode
@author F.Bighelli
 */

#ifndef __ISWMODELSLISTLISTENER_H
#define __ISWMODELSLISTLISTENER_H

#include <QtCore>

/**
@class _ISwModelsListListener
@brief listener de la liste des mode
*/
class _ISwModelsListListener {
public:
    //Here public method
    virtual void modelAdded(QString name,QString description)=0;
};

#endif
