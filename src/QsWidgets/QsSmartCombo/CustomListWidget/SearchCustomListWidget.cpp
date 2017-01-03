#include "CustomListWidget/SearchCustomListWidget.h"

#include <QStyle>

//-------------------------------------------------------------------------
SearchCustomListWidget::SearchCustomListWidget(QWidget *parent) : BaseCustomListWidget(parent)
{
	_minIntValue = -1;
	_maxIntValue = -1;
	_stepIntValue = -1;

	_minDoubleValue = -1;
	_maxDoubleValue = -1;
	_stepDoubleValue = -1;

	_digitSize = -1;
}


//-------------------------------------------------------------------------
SearchCustomListWidget::~SearchCustomListWidget()
{
}


//-------------------------------------------------------------------------
void SearchCustomListWidget::setIntValues(int minValue, int maxValue, int stepValue)
{
	_minIntValue = minValue;
	_maxIntValue = maxValue;
	_stepIntValue = stepValue;

	if (_stepIntValue <= 0)
		_stepIntValue = 1;
}

//-------------------------------------------------------------------------
void SearchCustomListWidget::setDoublesValues(double minValue, double maxValue, double stepValue)
{
	_minDoubleValue = minValue;
	_maxDoubleValue = maxValue;
	_stepDoubleValue = stepValue;

	if (_stepDoubleValue <= 0.0)
		_stepDoubleValue = 0.1;


	_digitSize = QString::number(_stepDoubleValue, 'f').remove(QRegExp("0*$")).remove(QRegExp("^.*\\.")).size();

}

//-------------------------------------------------------------------------
void SearchCustomListWidget::buildItemList(QString /*textSearched*/)
{
	QStringList listToBuild = _itemConvertedSearchList + _itemConvertedList;

	QFont font = this->font();
	QString lengthMax = "";
	for each (auto item in listToBuild)
	{
		// Remplissage de la QListWidget
		QListWidgetItem *itm = new QListWidgetItem();
		itm->setFont(font);
		itm->setText(item);
		itm->setData(Qt::UserRole, item);
		addItem(itm);

		// Calcul de l'item de plus long
		if (item.size() > lengthMax.size())
			lengthMax = item;
	}

	// Taille en pixel de l'item de plus grand
	font.setBold(true);
	QFontMetrics fm(font);
	_sizeMax = fm.width(lengthMax) + 20;
}

//-------------------------------------------------------------------------
void SearchCustomListWidget::buildIntSearchList(QString text)
{
	bool ok;
	int enteredValue = text.toInt(&ok);		// Conversion en int du nombre entré
	if (ok)
	{
		text = QString::number(enteredValue);

		// On ne gère pas les cas ou la valeur entrée est supérieure à la valeur max
		if (enteredValue > _maxIntValue)
			return;
		// Si la valeur entrée est inférieure à la valeur min, il faut proposer des valeurs à l'utilisateur
		if (enteredValue < _minIntValue)
		{
			// Dans tous les cas on propose la valeur min
			QString minValueString = QString::number(_minIntValue);
			appendToList(_minIntValue);


			// On compute la valeur supérieure en ajoutant autant de 0 après la valeur entrée que la valeur min possède de dizaines
			int nearestPossibleValue = enteredValue;
			if (enteredValue != 0)
			{
				while (QString::number(nearestPossibleValue).size() < minValueString.size())
					nearestPossibleValue *= 10;
			}

			nearestPossibleValue = nearestPossibleValue - nearestPossibleValue % _stepIntValue;

			// Si la valeur computée commence par le même chiffre que celui entré, on l'ajoute à la liste proposée
			QString nextPossibleValueString = QString::number(nearestPossibleValue);
			if (nextPossibleValueString.at(0) == text.at(0) )
			{
				appendToList(nearestPossibleValue);
			}
			// Sinon on essaie avec la valeur incrémentée du step
			else if (QString::number(nearestPossibleValue + _stepIntValue).at(0) == text.at(0) )
			{
				appendToList(nearestPossibleValue + _stepIntValue);
			}
			

			if (text.size() == minValueString.size() && enteredValue != 0)
			{
				int nextPossibleValue = enteredValue * 10;
				nextPossibleValue = nextPossibleValue - nextPossibleValue % _stepIntValue;
				
				QString nextPossibleValueString = QString::number(nextPossibleValue);
				if (nextPossibleValueString.at(0) == text.at(0))
				{
					appendToList(nextPossibleValue);
				}
				else
				{
					appendToList(nextPossibleValue + _stepIntValue);
				}
			}
		}
		// Si la valeur entrée est supérieur à la valeur min, il faut proposer les valeurs au dessus et au dessous à l'utilisateur
		else 
		{
			// La valeur entrée est une valeur acceptable, on la propose dans la liste
			if (enteredValue % _stepIntValue == 0)
			{
				appendToList(enteredValue);
			}
			// Valeur < et > à la valeur entrée proposée
			else
			{
				appendToList(enteredValue - enteredValue % _stepIntValue);
				appendToList(enteredValue - enteredValue % _stepIntValue + _stepIntValue);
			}

			// Valeur suivante possible à la saisie proposée
			int nextPossibleValue = enteredValue * 10;
			nextPossibleValue = nextPossibleValue - nextPossibleValue % _stepIntValue;
			
			QString nextPossibleValueString = QString::number(nextPossibleValue);
			if (nextPossibleValueString.at(0) == text.at(0))
			{
				appendToList(nextPossibleValue);
			}
			else
			{
				appendToList(nextPossibleValue + _stepIntValue);
			}
		}
		_itemConvertedSearchList.removeDuplicates();
	}	
}

//-------------------------------------------------------------------------
void SearchCustomListWidget::buildDoubleSearchList(QString text)
{
	bool ok;
	text.replace(",", ".");
	
	bool endsWithPoint = text.endsWith(".");
	bool hasDecimals = text.contains(".");
	if (hasDecimals)
		text.append("0");
	
	double enteredValue = text.toDouble(&ok);
	if (ok)
	{
		text = QString::number(enteredValue, 'f', _digitSize);

		if (enteredValue < _minDoubleValue)
		{
			QString minValueString = QString::number(_minDoubleValue, 'f', _digitSize);
			// Affichage de la valeur min si la valeur entrée est au dessous
			appendToList(_minDoubleValue);

			double nearestPossibleValue = enteredValue;

			if (enteredValue != 0)
			{
				while (QString::number(nearestPossibleValue, 'f', _digitSize).size() < minValueString.size())
					nearestPossibleValue *= 10;
			}

			nearestPossibleValue = nearestPossibleValue - fmod(nearestPossibleValue,_stepDoubleValue);
	
			QString nextPossibleValueString = QString::number(nearestPossibleValue, 'f', _digitSize);
			if (nextPossibleValueString.at(0) == text.at(0))
			{
				appendToList(nearestPossibleValue);
			}
			else if (QString::number(nearestPossibleValue + _stepDoubleValue, 'f', _digitSize).at(0) == text.at(0))
			{
				appendToList(nearestPossibleValue + _stepDoubleValue);
			}

			if (text.size() == minValueString.size() && !hasDecimals)
			{
				double nextPossibleValue = enteredValue * 10;
				nextPossibleValue = nextPossibleValue - fmod(nextPossibleValue,_stepDoubleValue);
				
				QString nextPossibleValueString = QString::number(nextPossibleValue, 'f', _digitSize);
				if (nextPossibleValueString.at(0) == text.at(0))
				{
					appendToList(nextPossibleValue);
				}
				else
				{
					nextPossibleValue += _stepDoubleValue;
					nextPossibleValueString = QString::number(nextPossibleValue, 'f', _digitSize);
					if (nextPossibleValueString.at(0) == text.at(0))
					{
						appendToList(nextPossibleValue + _stepDoubleValue);
					}
				}
			}
		}
		else
		{
			// Valeur < et > à la valeur entrée proposée
			if (fmod(enteredValue, _stepDoubleValue) == 0 && !endsWithPoint)
			{
				appendToList(enteredValue);
			}
			else
			{
				appendToList(enteredValue - fmod(enteredValue, _stepDoubleValue));
				appendToList(enteredValue - fmod(enteredValue, _stepDoubleValue) + _stepDoubleValue);
			}

			if (!hasDecimals)
			{
				// Valeur suivante possible proposée
				double nextPossibleValue = enteredValue * 10;
				nextPossibleValue = nextPossibleValue - fmod(nextPossibleValue, _stepDoubleValue);
				
				QString nextPossibleValueString = QString::number(nextPossibleValue, 'f', _digitSize);
				if (nextPossibleValueString.at(0) == text.at(0))
				{
					appendToList(nextPossibleValue);
				}
				else
				{
					appendToList(nextPossibleValue + _stepDoubleValue);
				}
			}
		}
		_itemConvertedSearchList.removeDuplicates();
	}
}


//-------------------------------------------------------------------------
bool SearchCustomListWidget::appendToList(int value)
{
	bool ret = false;
	if (value >= _minIntValue && value <= _maxIntValue)
	{
		_itemConvertedSearchList.append(QString::number(value));
		ret = true;
	}
	return ret;
}

//-------------------------------------------------------------------------
bool SearchCustomListWidget::appendToList(double value)
{
	bool ret = false;
	if (value >= _minDoubleValue && value <= _maxDoubleValue)
	{
		_itemConvertedSearchList.append(QString::number(value, 'f', _digitSize));
		ret = true;
	}
	return ret;
}

//-------------------------------------------------------------------------
QStringList SearchCustomListWidget::search(QString text)
{
	clearAll();
	
	// Blocage des signaux car le clear déclenche le signal "onItemChanged" sinon...
	selectionModel()->blockSignals(true);
	
	// Est-ce que la valeur cherchée est un nombre?
	bool okDouble = false;
	text.toDouble(&okDouble);
	if (!okDouble)
	{
		// Si non, on vérifie si elle fait partie de la liste des Enums
		QString formatedTextToSearch = text.toHtmlEscaped();
		for each (auto it in _itemConvertedList)
		{
			if (it.contains(formatedTextToSearch, Qt::CaseInsensitive))
			{
				return _itemConvertedList;
			}
		}
	}
	else
	{
		// Sinon, build la "_itemConvertedSearchList" selon le type de valeur numérique
		if (_minIntValue != -1 && _maxIntValue != -1 && _stepIntValue != -1)
			buildIntSearchList(text);
		else if (_minDoubleValue != -1 && _maxDoubleValue != -1 && _stepDoubleValue != -1)
			buildDoubleSearchList(text);
	}

	// Remplissage de la ListWidget avec la sélection (_itemConvertedSearchList + _itemConvertedList)
	buildItemList();

	selectionModel()->blockSignals(false);
	return _itemConvertedSearchList;
}
