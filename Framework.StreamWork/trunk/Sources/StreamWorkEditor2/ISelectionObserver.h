/**
@file ISelectionObserver.h
@brief Observer de la selection
@author F.Bighelli
 */

#ifndef _ISELECTIONOBSERVER_H
#define _ISELECTIONOBSERVER_H

#include <QList>
#include <SwComponent_Class.h>

/**
@class ISelectionObserver
@brief Observer de la selection
*/
class ISelectionObserver {

public:
    virtual void setSelection(QList<StreamWork::SwCore::SwComponent_Class *> & sel)=0;
    virtual ~ISelectionObserver() {}
};

#endif
