#pragma once


#include "BaseCustomListWidget.h"

/**
* @class SearchCustomListWidget
* @brief Classe composant la SmartCombo
*		 Héritée d'une ListWidget elle customise la liste d'items à l'intérieur
*		 Elle gère une fonction de recherche dans les items
*		 Elle utilise un customDelegate pour gérer les polices d'affichage et couleurs de texte
* @sa QWidget
*/
class SearchCustomListWidget : public BaseCustomListWidget
{
	Q_OBJECT

public:
	SearchCustomListWidget(QWidget *parent);
	~SearchCustomListWidget();



	/** @brief : Renseigne la plage de valeurs int */
	void setIntValues(int minValue, int maxValue, int stepValue);

	/** @brief : Renseigne la plage de valeurs doubles */
	void setDoublesValues(double minValue, double maxValue, double stepValue);


	/** @brief : Rempli la QListWidget avec les items de la liste _itemList et de la _itemSearchList */
	void buildItemList(QString textSearched = "");

	
	/** @brief : Recherche la valeur en paramètre dans la liste des items
	 ** @param : Texte à rechercher dans la liste d'items
	 ** @return : QStringList résultat de la recherche, liste des items contenant le "texte" passé en paramètre 
	*/
	QStringList search(QString text);
	
	

private:
	/** Plage de valeurs int */
	int			_minIntValue;
	int			_maxIntValue;
	int			_stepIntValue;


	/** Plage de valeurs double */
	double		_minDoubleValue;
	double		_maxDoubleValue;
	double		_stepDoubleValue;

	int			_digitSize;

	/** @brief : Rempli la _itemSearchList avec les items numériques INT computed */
	void buildIntSearchList(QString text);

	/** @brief : Rempli la _itemSearchList avec les items numériques DOUBLES computed */
	void buildDoubleSearchList(QString text);

	bool appendToList(int value);
	bool appendToList(double value);
};

