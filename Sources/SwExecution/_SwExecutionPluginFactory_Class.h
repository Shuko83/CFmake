/*!
 \file _SwExecutionPluginFactory_Class.h
 \brief Factory d'execution
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef __SwPluginFactory_Class_H
#define __SwPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
//#include "SwGuiConstantes.h"
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwExecutionPluginFactory_Class : public SwPluginFactory_Class
{ 
protected:
    /*! \brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /*! \brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
    /*! \brief instanciation d'une data */
    SwData_Class * CreateInstanceOf(unsigned int type_id);
public:
    /*! \brief Constructeur */
    _SwExecutionPluginFactory_Class();
    /*! \brief Destructeur */
    ~_SwExecutionPluginFactory_Class();
    /*! \brief Initialisation */
    void Initialize();
    /*! \brief Liberation */
    void Liberate();
    /*! \brief Acces a la version du plugin */
    QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate();
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();

#endif 
