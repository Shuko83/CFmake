/**
@file IAnchorable.h
@brief interface Widget ancrable
@author F.Bighelli
 */

#ifndef _IANCHORABLE_H
#define _IANCHORABLE_H

#include "IAnchorableObserver.h"

//Position d'ancrage possible
typedef enum {TOP_LEFT,TOP_RIGHT,BOTTOM_LEFT,BOTTOM_RIGHT} TAnchorPosition;

/**
@class IAnchorable
@brief interface Widget ancrable
*/
class IAnchorable {

public:
    /** @brief Position d'ancrage */
    virtual TAnchorPosition getAnchorPosition()=0;
    /** @brief Est extensible verticalement */
    virtual bool isVerticalExtensible()=0;
    /** @brief Est extensible horizontalement */
    virtual bool isHorizontalExtensible()=0;
    /** @brief ajout d'observer */
    virtual void addObserver(IAnchorableObserver *)=0;
    /** @brief suppression d'observer */
    virtual void removeObserver(IAnchorableObserver *)=0;
};

#endif
