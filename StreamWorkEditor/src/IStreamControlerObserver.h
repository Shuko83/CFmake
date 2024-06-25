/**
@file IStreamControlerObserver.h
@brief Interface observer du controleur de stream
@author F.Bighelli
 */

#ifndef _ISTREAMCONTROLEROBSERVER_H
#define _ISTREAMCONTROLEROBSERVER_H

/**
@class IStreamControlerObserver
@brief Interface observer du controleur de stream
*/
class IStreamControlerObserver {

public:
    virtual void controlerHasChanged()=0;
    virtual ~IStreamControlerObserver() {}
    
};

#endif
