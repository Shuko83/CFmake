/**
@file EditionService.h
@brief Si present, alors en mode edition
@author Big
 */

#ifndef _EDITIONSERVICE_H
#define _EDITIONSERVICE_H

#include "Main/Services/ISwEditionService.h"



/**
@class EditionService
@brief Si present, alors en mode edition
*/
class EditionService: public StreamWork::SwCore::ISwEditionService {
public:
    /** @brief Constructor */
    EditionService();
    /** @brief Destructor */
    virtual ~EditionService();
    /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
    lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
    void Liberate();
    /*! \brief verifie la licence*/
    virtual bool editorSavingInProgress();
    /*! \brief verifie la licence*/
    virtual void setEditorSavingInProgress(bool val);
    
private:
    bool _saveInProgress;

};

#endif
