/*!
 \file _SwConfigurationExportedEntity.h
 \brief Class implementant une entite exportable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfigurationExportedEntity_H
#define __SwConfigurationExportedEntity_H

#include <QString>
#include "Component/Base/SwComponent_Class.h"
#include "SwConfigurationConstantes.h"
#include <QDebug>
#include <QMetaType>

using namespace StreamWork::SwCore;
/*!
	\class _SwConfigurationExportedEntity 
	\brief Class implementant une entite exportable
*/


class SWCONFIGURATION_EXPORT _SwConfigurationExportedEntity {
	public:
	    typedef enum {
            Ent_Undefined,
            Ent_Property,
            Ent_Perspective,
            Ent_Execution,
            Ent_OwnerConfigurable
        } ExportedEntityType;
		/*! \brief entity type */
        ExportedEntityType _type;
		/*! \brief Name */
		QString _name;
		/*! \brief Nom exporte */
		QString _exported_name;
		/*! \brief Path de l'hote */
		QString _host_path;
		/*! \brief Host */
		SwComponent_Class * _host;
		/*! \brief Modele hote */
		SwComponent_Class * _model_host;
		/*! \brief Model */
		SwComponent_Class * _model;

	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
    public:


		/*! \brief Constructeur */
		_SwConfigurationExportedEntity();

	    /*! \brief Destructeur */
		virtual ~_SwConfigurationExportedEntity();
	    /*! \brief Effectue la liaison */
		void Bind(SwComponent_Class * model_host,SwComponent_Class * model);
	    /*! \brief Defait la liaison */
		void Unbind();
    public: 
	    /*! \brief Creation d'une entity*/
        static _SwConfigurationExportedEntity * NewEntity(ExportedEntityType type);


};



Q_DECLARE_METATYPE(_SwConfigurationExportedEntity);

QDebug SWCONFIGURATION_EXPORT operator<<(QDebug dbg, const _SwConfigurationExportedEntity &entity);


#endif
