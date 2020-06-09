#ifndef _SwServiceToolsPluginFactory_Class_H
#define _SwServiceToolsPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>
#include "_SwServiceRefProfiler.h"
#include "_SwServiceParametersImpl.h"
#include "_SwServiceExtensionsImpl.h"
#include "_SwServiceCodeTimer.h"
#include "_SwFileEditorManager.h"


using namespace StreamWork::SwCore;

/*!
	\class SwPluginFactory_Class 
	\brief classe definissant un plugin usine
*/
class _SwServiceToolsPluginFactory_Class : public SwPluginFactory_Class
{ 
	protected:
		/*! \brief instanciation d'un composant */
		SwComponent_Class * CreateInstanceOf(QString name) ;
		/*! \brief acces a l'icone d'un composant */
		QIcon CreateIconOf(QString name) const;
		/*! \brief instanciation d'une data */
		SwData_Class * CreateInstanceOf(const SwUUID & type_id);
	public:
		/*! \brief Constructeur */
		_SwServiceToolsPluginFactory_Class();
		/*! \brief Destructeur */
		~_SwServiceToolsPluginFactory_Class();
		/*! \brief Initialisation */
		void Initialize();
		/*! \brief Liberation */
		void Liberate();
		/*! \brief Acces a la version du plugin */
		QString GetPluginVersion();
		/*! \brief Acces a la date de compilation du plugin */
		QDateTime GetPluginCompilationDate() override;

		QString GetPluginName() const override;

	private:
		_SwServiceRefProfiler * _serviceRefProfiler;
		_SwServiceParametersImpl * _serviceParameters;
		_SwServiceExtensionsImpl * _serviceExtensions;
		_SwServiceCodeTimer * _serviceCodeTimer;
		_SwFileEditorManager * _serviceFileEditorManager;
};

extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();

#endif 
