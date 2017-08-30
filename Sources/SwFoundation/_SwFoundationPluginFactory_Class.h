/*!
 \file _SwFoundationPluginFactory_Class.cpp
 \brief Factory de fondation 
 \date 02/04/2014
 \version 1.0
 \author EPO
*/

#ifndef __SwFoundationPluginFactory_Class_H
#define __SwFoundationPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;
/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwFoundationPluginFactory_Class : public SwPluginFactory_Class
{ 
protected:
    /*! \brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /*! \brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
public:
    /*! \brief Constructeur */
    _SwFoundationPluginFactory_Class();
    /*! \brief Destructeur */
    ~_SwFoundationPluginFactory_Class();
    /*! \brief Initialisation */
    void Initialize();
    /*! \brief Liberation */
    void Liberate();
    /*! \brief Acces a la version du plugin */
    QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate();
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new _SwFoundationPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
    return new _SwFoundationPluginFactory_Class();    
}
#endif
#endif 
