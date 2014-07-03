/**
@file IAnchorableObserver.h
@brief interface observer d'un Widget ancrable
@author F.Bighelli
 */

#ifndef _IANCHORABLEOBSERVER_H
#define _IANCHORABLEOBSERVER_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

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
