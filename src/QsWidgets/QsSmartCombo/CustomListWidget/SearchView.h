#pragma once

#include <QWidget>

#include "CustomListWidget/BaseCustomListWidget.h"

namespace Ui{ class SearchView; }

/**
* @class SearchView
* @brief Classe composant la SmartCombo
*		 Elle gère  une ListWidget customisée ainsi qu'une zone de pré-selection des items (LineEdit)
* @sa QWidget
*/
class SearchView : public QWidget
{
	Q_OBJECT

public:
	SearchView(QWidget *parent);
	~SearchView();

	/** @brief : Retourne le pointeur sur la ListWidget */
	BaseCustomListWidget * getCustomListWidget();

	void setCustomListWidget(BaseCustomListWidget *customListWidget);

	/** @brief : RAffiche la valeur en paramètre dans la zone de pré-seléction */
	void displayRefValue(QString refValue);
	
	/** @brief : Interception des events touche clavier */
	void keyPressEvent(QKeyEvent *event);

	

public slots:
	/** @brief : Gestion de la pré-sélection */
	void onItemChanged();

	void onItemHovered(QListWidgetItem *item);


signals:
	/** @brief : Signal émit lors de la sélection d'un item */
	void selectionItemChanged(QString value);

	/** @brief : Signal émit lors de l'annulation d'une sélection */
	void cancelSelection();

	/** @brief : Signal émit lors de la perte de focus de la lineEdit (on veut que le widget soit hidé) */
	void closeWindow();
	
	/** @brief : Signal émit lors de la reprise de focus de la lineEdit (on ne veut plus que le widget soit hidé) */
	void cancelCloseWindow();

	/** @brief : Signal émit lorsque l'utilisateur clique sur un item de la listView et permettant de garder le focus sur la LineEdit de saisie */
	void focusLineEdit();

	void itemHovered(QString value);


private:
	Ui::SearchView			*_ui;

	BaseCustomListWidget	*_customListWidget;
	
	/** @brief : Retourne le texte de la sélection courante 
	 ** note : remove les éventuelles balise de "soulignement" (<u>) présentes lors d'une recherche */
	QString getCurrentSelection();

	/** @brief : Valide la sélection en paramètre (double clic / touche Enter...) */
	void validateSelection(QString selection);

	/** @brief : Gestion du move et resize */
	bool eventFilter(QObject * object, QEvent * event);



private slots:
	/** @brief : Gestion de la sélection et la fermeture de la combo */
	void onItemDoubleClicked();

};

