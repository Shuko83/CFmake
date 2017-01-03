#ifndef SEARCHCOMBO_H
#define SEARCHCOMBO_H

#include "QsSmartCombo/QsBaseComboBox.h"
#include "QsWidgets_Global.h"

/**
* @class QsSearchComboBox 
* @brief Classe principale de la QsSearchComboBox
*		 La combo intégre une fonction de recherche dans les différents items.
*		 Aussi une fonction de pré-sélection d'un item (et annulation)
* @sa QWidget
*/
class QSWIDGETS_EXPORT QsSearchComboBox : public QsBaseComboBox
{
	Q_OBJECT
	Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsSearchComboBox(QWidget *parent = 0);
	~QsSearchComboBox();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	/** @brief : Renseigne la plage de valeurs int */
	void setIntValues(int minValue, int maxValue, int stepValue, QString regexp = QString());

	/** @brief : Renseigne la plage de valeurs doubles */
	void setDoublesValues(double minValue, double maxValue, double stepValue, QString regexp = QString());


private slots:
	/** @brief : permet de chercker si la chaine de caractères entrée est matchée dans la liste
	**		     Si elle ne l'est pas, le dernier caractère n'est pas écrit dans la QLineEdit
	*/
	void checkEnteredValue();


private:
	QString _classCSS;
	
};

#endif // SEARCHCOMBO_H
