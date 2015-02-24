/*!
 \file _SwModelExportedEntity.h
 \brief Class implementant une entite exportable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedEntity_H
#define __SwModelExportedEntity_H

#include <QString>
#include <SwComponent_Class.h>

class _SwModel_Class;
class _SwModelHost_Class;

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedEntity 
	\brief Class implementant une entite exportable
*/
typedef enum {
    Ent_Undefined,
    Ent_Property,
    Ent_InterfaceC,
    Ent_InterfaceP,
    Ent_Pin,
    Ent_Executable,
    Ent_OwnerConfigurable,
    Ent_Activable
    } ExportedEntityType;

class _SwModelExportedEntity {
	public:
		/*! \brief entity type */
        ExportedEntityType _type;
		/*! \brief Name */
		QString _name;
		/*! \brief Nom exporte */
		QString _exported_name;
		/*! \brief Path de l'hote */
		QString _host_path;
		/*! \brief type interne */
		QString _itype;
		/*! \brief Host */
		SwComponent_Class * _host;
		/*! \brief Modele hote */
		_SwModelHost_Class * _model_host;
		/*! \brief Host */
		SwComponent_Class * _model;
	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	    /*! \brief Effectue la construction */
		virtual void SpecificBuild();
	    /*! \brief Effectue la destruction */
		virtual void SpecificDestroy();
		/*! \brief Constructeur */
		_SwModelExportedEntity();
    public:
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedEntity();
	    /*! \brief Effectue la liaison */
		void Bind(SwComponent_Class * model);
	    /*! \brief Defait la liaison */
		void Unbind();
	    /*! \brief Effectue la construction */
		void Build(_SwModelHost_Class * model_host);
	    /*! \brief Effectue la destruction */
		void Destroy();
    public: 
	    /*! \brief Creation d'une entity*/
        static _SwModelExportedEntity * NewEntity(ExportedEntityType type);


};

#endif