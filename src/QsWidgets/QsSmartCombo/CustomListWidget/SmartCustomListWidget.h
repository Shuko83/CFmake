#pragma once


#include "BaseCustomListWidget.h"

/**
* @class SmartCustomListWidget
* @brief Classe composant la SmartCombo
*		 Héritée d'une ListWidget elle customise la liste d'items à l'intérieur
*		 Elle gère une fonction de recherche dans les items
*		 Elle utilise un customDelegate pour gérer les polices d'affichage et couleurs de texte
* @sa QWidget
*/
class SmartCustomListWidget : public BaseCustomListWidget
{
	Q_OBJECT

public:
	SmartCustomListWidget(QWidget *parent);
	~SmartCustomListWidget();


	/** @brief : Rempli la QListWidget avec les items de la liste _itemList 
	 ** @param : QString : Texte à rechercher dans la liste d'items, DOIT ETRE FORMATE EN HTML via toHtmlEscaped()
	 */
	void buildItemList(QString textSearched = "");

	/** @brief : Recherche la valeur en paramètre dans la liste des items
	 ** @param : Texte à rechercher dans la liste d'items
	 ** @return : QStringList résultat de la recherche, liste des items contenant le "texte" passé en paramètre 
	*/
	QStringList search(QString text);

};

