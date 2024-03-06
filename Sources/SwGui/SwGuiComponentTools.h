/*
\file SwGuiComponentTools.h
\brief 
\version 1.0
\author EPO
\date 09/01/2015
*/

#ifndef SWGUI_SWGUICOMPONENTTOOLS_H
#define SWGUI_SWGUICOMPONENTTOOLS_H

/*
* INCLUDES GLOBAUX
*/
#include <QDialog>
#include "Component/Base/SwComponent_Class.h"


/*
* INCLUDES LOCAUX
*/

namespace Ui{ class ComponentToolsGui; }

#include "SwGuiConstantes.h"
#include "ComponentTools/ComponentTableWidget.h"

namespace StreamWork 
{
	namespace SwGui
	{
		/**
		*	@class SwGuiComponentTools
		*	@brief
		*/
		class SWGUI_EXPORT SwGuiComponentTools : public QDialog
		{
			/*Q_OBJECT*/
		public:

			SwGuiComponentTools();

			/*! \brief Constructeur */
			SwGuiComponentTools(QWidget *parent, StreamWork::SwCore::SwComponent_Class * root_component, QList<QString> current_list);

			/*! \brief Destructeur */
			~SwGuiComponentTools();

			QList<QString> getListComponents();

		private:
			Ui::ComponentToolsGui * _ui;

			/** @brief widget IHM pour la QTable où glisser les composants */
			ComponentTableWidget *_compoTableWidget;

			/* List des path des composants */
			QList<QString> _comp_paths;

			/*! \brief composant racine */
			StreamWork::SwCore::SwComponent_Class * _root_component; 
		};
	}
}



#endif 
