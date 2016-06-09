#pragma once

/**
* INCLUDES GLOBAUX
*/
#include <QObject>
#include <QWidget>

/**
* INCLUDES LOCAUX
*/
#include "SwServiceContextMenuConstantes.h"
#include "ISwServiceContextMenu.h"

/**
* Interfaces
*/
#include "ContextMenu.h"

namespace StreamWork
{
	namespace Service
	{
		class BUILD_SWSERVICECONTEXTMENU SwServiceContextMenu : public QObject,
																public ContextMenu,
																public ISwServiceContextMenu
		{
			Q_OBJECT;

		public:

			SwServiceContextMenu() = default;			
			virtual ~SwServiceContextMenu() = default;

			void registerContextAction(Action::IAction * action) override { ContextMenu::registerContextAction(action); };
			void registerContextAction(QAction * action) override { ContextMenu::registerContextAction(action); };
			QMenu * getMenu() override { return ContextMenu::getMenu();};			
		};
	}
}

