/*!
 \file _SwSimpleExecutable.h
 \brief Implementation of the Class _SwSimpleExecutable permettant de tester l'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwSimpleExecutable_H
#define __SwSimpleExecutable_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include "ISwExecutable_Service.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*!
	\class _SwSimpleExecutable 
	\brief Implementation of the Class _SwSimpleExecutablepermettant de tester l'executeur
*/
class _SwSimpleExecutable : public SwComponent_Class , public ISwExecutable_Service
{
protected:
    /*! \brief Start time */
    double _start_time;
     
public:
    /*! \brief Constructeur */
    _SwSimpleExecutable();
    /*! \brief Destructeur */
    virtual ~_SwSimpleExecutable();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
     //----------------------------------------------------
    // Interface ISwExecutable_Service
    //----------------------------------------------------
	/*! \brief Initialisation */
	void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);            
	/*! \brief Demarrage */
	void Start(double current_time) throw (SwException);            
	/*! \brief Execution */
	void Execute(double current_time,bool is_first_call) throw (SwException);            
	/*! \brief Execution */
	void Stop(double current_time);            
	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------            
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();  
	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief acces a son composant hote */
	SwComponent_Class * GetHostComponent();            


};
#endif 
