#include "QsSearchComboBox.h"
#include "CustomDoubleValidator.h"
#include "SearchCustomListWidget.h"
#include "ui_QsBaseComboBox.h"
#include "QsBaseComboBox.h"
#include "SearchView.h"

/*#include <QDebug>*/

QsSearchComboBox::QsSearchComboBox(QWidget *parent) : QsBaseComboBox(parent)
{
	_customListWidget = new SearchCustomListWidget(this);
	_customListWidget->installEventFilter(this);
	_searchView->setCustomListWidget(_customListWidget);
}

QsSearchComboBox::~QsSearchComboBox()
{

}


//-------------------------------------------------------------------------
void QsSearchComboBox::setIntValues(int minValue, int maxValue, int stepValue, QString regexp)
{
	_placeHolderText = QString::number(minValue) + "..." + QString::number(maxValue) + " (+/- "+ QString::number(stepValue) + ")";

	if (_lineEditValidator != 0)
		delete _lineEditValidator;

	if (regexp.isNull())
	{
		_lineEditValidator = new QIntValidator(minValue, maxValue);
	}
	else
	{
		_lineEditValidator = new QRegExpValidator(QRegExp(regexp));
	}
	_ui->lineEdit->setValidator(_lineEditValidator);

	dynamic_cast<SearchCustomListWidget*>(_customListWidget)->setIntValues(minValue, maxValue, stepValue);
}

//-------------------------------------------------------------------------
void QsSearchComboBox::setDoublesValues(double minValue, double maxValue, double stepValue, QString regexp)
{
	_placeHolderText = QString::number(minValue) + "..." + QString::number(maxValue) + " (+/- " + QString::number(stepValue) + ")";
	
	int digitSize = QString::number(stepValue, 'f').remove(QRegExp("0*$")).remove(QRegExp("^.*\\.")).size();

	// Utilisation ici de notre Custom Validator de Double
	if (_lineEditValidator != 0)
		delete _lineEditValidator;

	if (regexp.isNull())
	{
		_lineEditValidator = new CustomDoubleValidator(minValue, maxValue, digitSize);
		static_cast<CustomDoubleValidator*>(_lineEditValidator)->setNotation(QDoubleValidator::StandardNotation);
	}
	else
	{
		_lineEditValidator = new QRegExpValidator(QRegExp(regexp));
	}
	_lineEditValidator->setLocale(QLocale::English);
	_ui->lineEdit->setValidator(_lineEditValidator);

	dynamic_cast<SearchCustomListWidget*>(_customListWidget)->setDoublesValues(minValue, maxValue, stepValue);
}

//-------------------------------------------------------------------------
//								SLOTS
//-------------------------------------------------------------------------

void QsSearchComboBox::checkEnteredValue()
{
	QString textEntered = _ui->lineEdit->text();

	int pos = 0;
	// Saisie invalide ou Suppression du texte entré => clear de la lineEdit et on déroule toutes les valeurs possibles
	if (_ui->lineEdit->validator()->validate(textEntered, pos) == QValidator::Invalid
		|| textEntered.isEmpty())
	{
		_ui->pushButton->toggle();
	}
	else
	{
		// Si la recherche dans la liste des items est successful
		if (_searchView->getCustomListWidget()->search(textEntered).size() > 0)
		{
			// Set de la dernière valeur sélectionnée
			_searchView->onItemChanged();

			// Placement / resize / Affichage
			placeAndShowWidget(120);

			checkPushButton(false);
			onFocusLineEdit();
		}
	}
}
