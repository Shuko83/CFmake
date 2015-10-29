/**
@file ComponentTableWidget.h
@date 02/2014
@brief TableWidget view pour la vue des composants
@author  EPO
@version 1.0
*/
#ifndef _SW_GUI_COMPONENTTABLEWIDGET_H
#define _SW_GUI_COMPONENTTABLEWIDGET_H


// Includes globaux
#include <QTableWidget>
#include <QList>

// Includes locaux


namespace StreamWork
{
	namespace SwGui
	{
		class ComponentTableWidget : public QTableWidget
		{
			Q_OBJECT;

		public:

			/** @brief Constructeur */
			ComponentTableWidget(QWidget * parent = 0);

			~ComponentTableWidget();

			/** @brief Action de drop */
			virtual bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);

			/** @brief set component list of list */
			void setComponentsSaved(QList<QString> listCompo);

			/** @brief get component selected list */
			QList <QString>  getComponentsSaved() const { return _components; };


		protected:
			/** @brief Drag enter event */
			void dragEnterEvent(QDragEnterEvent * event);

			/** @brief Drag leave event */
			void dragLeaveEvent(QDragLeaveEvent * event);

			/** @brief Drag move event */
			void dragMoveEvent(QDragMoveEvent * event);

			/** @brief Sur key press */
			void keyPressEvent(QKeyEvent * event);

			QList<QString>	_components;

		private:
			/** @brief Ajoute un composant dans la liste _components
			* @param : QString 1 : hostName
			* @return bool : succès ou échec de l'ajout (composant déjà présent)
			*/
			bool addComponentToList(QString compo);

			/** @brief Supprime un composant de la liste _components
			* @param : QString: Component name
			* @return bool : succès ou échec de la suppresion (composant plus présent)
			*/
			bool removeComponentToList(QString compo);

			/** @brief Ajoute un composant dans la QTable
			* @param : QString : Component name
			*/
			void addComponentInQTable(QString compo);
		};
	}
}

#endif
