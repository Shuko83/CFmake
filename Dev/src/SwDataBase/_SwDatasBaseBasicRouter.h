/*!
 \file _SwDatasBaseBasicRouter.h
 \brief Implementation of the Class _SwDatasBaseBasicRouter generant un router de données basique
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwDatasBaseBasicRouter_H
#define __SwDatasBaseBasicRouter_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include <ISwPin_Listener.h>
using namespace StreamWork::SwCore;

/*!
	\class _SwDatasBaseBasicRouter 
	\brief Implementation of the Class _SwDatasBaseBasicRouter generant un router de données basique
*/
class _SwDatasBaseBasicRouter : public SwComponent_Class, public ISwPin_Listener
{
protected:
    /* service de gestion des pins */
    SwPins_Manager_Class * _pins_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* nombre pins */
    uint _pins_nb;
    /* propriété nombre de pins */
    ISwProperty * _pins_nb_property;
    /* pin type */
    SwEnum _pin_type;
    /* propriété type de pins */
    ISwProperty * _pin_type_property;
    /* propriété Pin liste */
    QList<SwPin *> _pins_list;
public:
    /*! \brief Constructeur */
    _SwDatasBaseBasicRouter();
    /*! \brief Destructeur */
    virtual ~_SwDatasBaseBasicRouter();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
    /*! \brief Callback sur les changements de propriétés*/
	/*! \brief Sur reception d'une donnée*/
	void OnReceiveData(SwPin * src,SwData_Class * data);            


};
#endif 
