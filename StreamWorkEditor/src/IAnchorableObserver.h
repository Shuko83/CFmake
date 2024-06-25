/**
@file IAnchorableObserver.h
@brief interface observer d'un Widget ancrable
@author F.Bighelli
 */

#ifndef _IANCHORABLEOBSERVER_H
#define _IANCHORABLEOBSERVER_H

#include <QtWidgets>

/**
@class IAnchorableObserver
@brief interface observer d'un Widget ancrable
*/
class IAnchorableObserver {

public:
    /*@brief indique que l'entité a changé */
    virtual void anchorableAsChanged(QWidget *)=0;
};

#endif
