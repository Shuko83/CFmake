/*!
 \file _SwEditorPluginFactory_Class.h
\brief factory SwEditor
 \date 2006-11-08
 \version 1.0
 \author F.Bighelli
 \note first generation by plugin generator
*/

#ifndef __SwEditorPluginFactory_Class_H
#define __SwEditorPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QIcon>
#include <map>
/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwEditorPluginFactory_Class : public SwPluginFactory_Class
{ 
protected:
    /*! \brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name);
    /*! \brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
public:
    /*! \brief Constructeur */
    _SwEditorPluginFactory_Class();
    /*! \brief Destructeur */
    ~_SwEditorPluginFactory_Class();
    /*! \brief Initialisation */
    void Initialize();
    /*! \brief Liberation */
    void Liberate();
    /*! \brief Acces a la version du plugin */
    QString GetPluginVersion();
};

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new _SwEditorPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
    return new _SwEditorPluginFactory_Class();    
}
#endif


#endif 
