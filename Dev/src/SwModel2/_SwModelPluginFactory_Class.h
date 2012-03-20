/*!
 \file _SwModelPluginFactory_Class.h
 \brief Factory de modeles
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
#include "_SwModelsList.h"

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwModelPluginFactory_Class : public SwPluginFactory_Class
{ 
protected:

protected:
    /*! \brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /*! \brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
    /*! \brief instanciation d'une data */
    SwData_Class * CreateInstanceOf(unsigned int type_id);
public:
    /*! \brief Constructeur */
    _SwModelPluginFactory_Class();
    /*! \brief Destructeur */
    ~_SwModelPluginFactory_Class();
    /*! \brief Initialisation */
    void Initialize();
    /*! \brief Liberation */
    void Liberate();
    /*! \brief Acces a la version du plugin */
    QString GetPluginVersion();
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new _SwModelPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
    return new _SwModelPluginFactory_Class();    
}
#endif

#endif 
