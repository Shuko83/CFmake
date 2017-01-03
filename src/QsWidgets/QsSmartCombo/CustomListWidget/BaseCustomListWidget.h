#pragma once

#include <QListWidget>
#include <QKeyEvent>

/**
* @class BaseCustomListWidget
* @brief Classe composant la SmartCombo
*		 Héritée d'une ListWidget elle customise la liste d'items à l'intérieur
*		 Elle gère une fonction de recherche dans les items
*		 Elle utilise un customDelegate pour gérer les polices d'affichage et couleurs de texte
* @sa QWidget
*/
class BaseCustomListWidget : public QListWidget
{
	Q_OBJECT

public:
	BaseCustomListWidget(QWidget *parent);
	~BaseCustomListWidget();

	/** @brief : Renseigne la liste d'items totale de la QListWidget */
	void setItemList(QStringList val);

	
	/** @brief : clear de la séléction et des valeurs dans la QListWidget */
	void clearAll();

	/** @brief : récupération de la valeur courante (ligne sélectionnée de la QListWidget) */
	QString getCurrentSelection();

	/** @brief : Set de la séléction courante + sélection de la ligne de l'item concerné
	 **			 + Scroll de la ListWidget sur l'item sélectionné 
	*/
	void setCurrentSelection(QString text);
	
	/** @brief : Retourne la taille max en pixels de la liste d'items */
	int getSizeMax();

	//----------------------------------------------------------------------------//
	//							Fonctions virtuelles pures
	//----------------------------------------------------------------------------//
	/** @brief : Rempli la QListWidget avec les items de la liste _itemList et de la _itemSearchList 
	 ** @param : QString : Texte à rechercher dans la liste d'items, DOIT ETRE FORMATE EN HTML via toHtmlEscaped()
	 */
	virtual void buildItemList(QString textSearched = "") = 0;


	/** @brief : Recherche la valeur en paramètre dans la liste des items
	** @param : Texte à rechercher dans la liste d'items
	** @return : QStringList résultat de la recherche, liste des items contenant le "texte" passé en paramètre
	*/
	virtual QStringList search(QString text) = 0;



protected:
	QStringList _itemConvertedList;
	QStringList _itemConvertedSearchList;
	QStringList _itemRawList;

	/** Taille max en pixels d'un item de la liste globale */
	int			_sizeMax;
};

