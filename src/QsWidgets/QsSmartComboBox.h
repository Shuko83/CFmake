#ifndef SMARTCOMBO_H
#define SMARTCOMBO_H

#include "QsSmartCombo/QsBaseComboBox.h"
#include "QsWidgets_Global.h"

/**
* @class QsSmartComboBox 
* @brief Classe principale de la QsSmartComboBox
*		 La combo intégre une fonction de recherche dans les différents items.
*		 Aussi une fonction de pré-sélection d'un item (et annulation)
* @sa QWidget
*/
class QSWIDGETS_EXPORT QsSmartComboBox : public QsBaseComboBox
{
	Q_OBJECT
	Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsSmartComboBox(QWidget *parent = 0);
	~QsSmartComboBox();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }



private slots:
	/** @brief : permet de chercker si la chaine de caractères entrée est matchée dans la liste
	**		     Si elle ne l'est pas, le dernier caractère n'est pas écrit dans la QLineEdit
	*/
	void checkEnteredValue();


private:
	QString _classCSS;

};

#endif // SMARTCOMBO_H
