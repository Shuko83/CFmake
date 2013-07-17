/*!
 \file WizardIDEPluginFactory_Class.h
 \brief Wizard for plugins generation
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _WizardIDEPluginFactory_Class_H
#define _WizardIDEPluginFactory_Class_H

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
	\class WizardIDEPluginFactory_Class
	\brief classe definissant un plugin usine
*/
class WizardIDEPluginFactory_Class : public SwPluginFactory_Class
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
    WizardIDEPluginFactory_Class();
    /*! \brief Destructeur */
    ~WizardIDEPluginFactory_Class();
    /*! \brief Initialisation */
    void Initialize();
    /*! \brief Liberation */
    void Liberate();
    /*! \brief Acces a la version du plugin */
    QString GetPluginVersion();
	/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
	virtual double GetPluginCompilationDate(){return -1;};
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new WizardIDEPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
    return new WizardIDEPluginFactory_Class();    
}
#endif

#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
