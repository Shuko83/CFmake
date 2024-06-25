/**
@file IStreamViewObserver.h
@brief Observer de la streamview
@author F.Bighelli
 */

#ifndef _ISTREAMVIEWOBSERVER_H
#define _ISTREAMVIEWOBSERVER_H

/**
@class IStreamViewObserver
@brief Observer de la streamview
*/
class IStreamViewObserver {

public:
    virtual void viewHasChanged()=0;
    virtual ~IStreamViewObserver(){};
};

#endif
