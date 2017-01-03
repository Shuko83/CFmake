#include "QsSmartComboBox.h"
#include "SmartCustomListWidget.h"
#include "ui_QsBaseComboBox.h"
#include "SearchView.h"

#include <QMessageBox>

QsSmartComboBox::QsSmartComboBox(QWidget *parent) : QsBaseComboBox(parent)
{
 	_customListWidget = new SmartCustomListWidget(this);
	_customListWidget->installEventFilter(this);
 	_searchView->setCustomListWidget(_customListWidget);
}

QsSmartComboBox::~QsSmartComboBox()
{
}



//-------------------------------------------------------------------------
void QsSmartComboBox::checkEnteredValue()
{
	QString textEntered = _ui->lineEdit->text();
	bool stringFound = false;


	for (auto i = 0; i < _itemList.size() && !stringFound; i++)
	{
		if (_itemList.at(i).contains(textEntered, Qt::CaseInsensitive))
		{
			stringFound = true;
		}
	}

	// Suppression du texte entré, lineEdit vide à nouveau et on déroule toutes les valeurs possibles
	if (textEntered.isEmpty())
	{
		_ui->pushButton->toggle();
	}
	// Lettre interdite à la saisie 
	else if (!stringFound)
	{
		textEntered.truncate(textEntered.size() - 1);
		_ui->lineEdit->setText(textEntered);
	}
	else
	{
		// Si la recherche dans la liste des items est successful
		if (_searchView->getCustomListWidget()->search(textEntered).size() > 0)
		{
			// Set de la dernière valeur sélectionnée
			_searchView->onItemChanged();

			// Placement / rezise / Affichage
			placeAndShowWidget(120);

			checkPushButton(false);
			onFocusLineEdit();
		}
	}
}
