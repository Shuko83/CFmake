#ifndef BASECOMBO_H
#define BASECOMBO_H

#include <QMainWindow>


#include "QsWidgets_Global.h"

class BaseCustomListWidget;
class SearchView;
class QValidator;
namespace Ui{ class BaseComboClass; }

/**
* @class QsBaseComboBox 
* @brief Classe principale de la QsBaseComboBox
*		 La combo intégre une fonction de recherche dans les différents items.
*		 Aussi une fonction de pré-sélection d'un item (et annulation)
* @sa QWidget
*/
class QSWIDGETS_EXPORT QsBaseComboBox : public QWidget
{
	Q_OBJECT


public:
	QsBaseComboBox(QWidget *parent = 0);
	~QsBaseComboBox();


	/** @brief : Renseigne la liste de valeurs de la Combo */
	void setComboValues(QStringList val);

	/** @brief : Retourne la liste des items de la combo */
	QStringList comboValues();

	/** @brief : set l'item courant sélectionné dans tous les cas :
	à utiliser avec les binders car soit ils bouclent, soit il ne se passe rien en utilisant setCurrentValue */
	void forceCurrentValue(QString value);

	/** @brief : set l'item courant sélectionné (s'il existe) */
	bool setCurrentValue(QString value, int index = -1);

	/** @brief : Retourne l'item courant sélectionné */
	QString currentValue();



	/** @brief : permet de fermer la ListeView lorsqu'on close le widget "QsBaseComboBox" */
	void closeEvent(QCloseEvent *event);


	/** @brief : Gestion des touches Enter et Return */
	void keyPressEvent(QKeyEvent *event);


	/** @brief : Gestion du move et resize */
	bool eventFilter(QObject * object, QEvent * event);

	void removeItems(QStringList items);


signals:
	/** @brief : Signal emit lors de la sélection d'une nouvelle valeur dans la combo */
	void selectionItemChanged();

	/** @brief : Signal emit lors du passage en hover de la souris sur un élément de la liste déroulante de la combo */
	void highlighted(QString item);


	void mouseEnter();
	void mouseLeave();

	void comboValuesChanged();

protected slots:
	/** @brief : Set le focus curseur sur la lineEdit */
	void onFocusLineEdit();



private slots:
	
	/** @brief : Affiche ou masque la liste "déroulante" de valeurs */
	void displayFullComboList(bool buttonValue);

	/** @brief : permet de chercker si la chaine de caractères entrée est matchée dans la liste
	**		     Si elle ne l'est pas, le dernier caractère n'est pas écrit dans la QLineEdit
	*/
	virtual void checkEnteredValue() = 0;


	/** @brief : Gestion lors de la sélection d'un item dans la combo déroulante */
	void onItemSelected(QString item);

	/** @brief : Gestion lors de la préview d'un item lors d'un hover de la souris sur la liste déroulante */
	void onItemHovered(QString item);

	/** @brief : Gestion lors de l'annulation d'une séléction */
	void onCancelSelection();

	
	/** @brief : Demande du masquage de la fenêtre */
	void onCloseWindow();
	
	/** @brief : Annulation de demande du masquage de la fenêtre */
	void onCancelCloseWindow();

	/** @brief : Gestion du masquage de la fenêtre lors de la perte de focus */
	void processHideWindow();



protected:
	Ui::BaseComboClass	*_ui;

	BaseCustomListWidget *_customListWidget;
	/** @brief : Liste des éléments TOTAUX de la combo	*/
	QStringList			_itemList;

	/** @brief : Dernière valeur sélectionnée	*/
	QString				_lastSelectedValue;

	/** @brief : IHM des ListViews dédiées	*/
	SearchView			*_searchView;

	/** @brief : Text à afficher dans le placeHolder (min/max/step)	*/
	QString				_placeHolderText;

	/** @brief : Validateur pour les valeurs numériques entrées dans la LineEdit	*/
	QValidator *		_lineEditValidator;

	/** @brief : valeur de confirmation de fermeture de la fenêtre lors de perte de focus	*/
	bool				 _hideWindowConfirmed;


	


	/** @brief : Mise à jour des valeurs de la ListView avec tous les items */
	void displayAllItemList();

	/** @brief : Déplace/Resize et affiche les widgets en fonction du widget parent */
	void placeAndShowWidget(int heightSize);

	
	/** @brief : Set le focus sur le push button */
	void focusPushButton();

	/** @brief : change la valeur de check du boutton (sans notif de signal) */
	void checkPushButton(bool checkValue);

	/** @brief : Affiche dans la ListWidget uniquement la dernière valeur sélectionnée 
	 **			=> Dissused pour le moment		*/
	void showLastSelection();

	/** @brief : Gestion du masquage de la listeView si on quitte le focus du widget */
	bool manageFocusListview(QEvent * event);
};

#endif // SMARTCOMBO_H
