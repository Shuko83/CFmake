#pragma once

/**
* INCLUDES GLOBAUX
*/
#include <tuple>
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

			void setMenuRequestCartoPosition(double latitude, double longitude, double altitude) override;
			std::tuple<double, double, double> getMenuRequestCartoPosition() override;

		private:
			std::tuple<double, double, double> _requestCartoPosition; //latitude, longitude et altitude (deg decimal, deg decimal et mètre) 
		};
	}
}

