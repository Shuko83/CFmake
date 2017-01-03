#include "SmartCustomListWidget.h"


//-------------------------------------------------------------------------
SmartCustomListWidget::SmartCustomListWidget(QWidget *parent) : BaseCustomListWidget(parent)
{
}


//-------------------------------------------------------------------------
SmartCustomListWidget::~SmartCustomListWidget()
{
}


//-------------------------------------------------------------------------
void SmartCustomListWidget::buildItemList(QString textSearched)
{
	QStringList listToBuild;
	if (textSearched != "")
		listToBuild = _itemConvertedSearchList;
	else
		listToBuild = _itemConvertedList;

	QFont font = this->font();
	QString lengthMax = "";
	for each (auto item in listToBuild)
	{
		QString valueText = item;

		// Si recheche, on souligne les lettres concernées
		if (textSearched != "")
		{
			QRegularExpression regExp("(" + textSearched + ")", QRegularExpression::CaseInsensitiveOption);
			valueText.replace(regExp, "<u>\\1</u>");
		}

		// Remplissage de la QListWidget
		QListWidgetItem *itm = new QListWidgetItem();
		itm->setFont(font);
		itm->setText(valueText);


		// La ligne de séparateurs "----" ne peux pas être sélectionnée
		if (valueText.contains("----"))
		{
			itm->setFlags(Qt::NoItemFlags);
			itm->setData(Qt::UserRole, "-----");
		}
		else
		{
			// Pour conserver le texte pas formaté en HTML, on le met dans la Data de l'item
			int indexOfItem = _itemConvertedList.indexOf(item);
			if (indexOfItem >= 0)
				itm->setData(Qt::UserRole, _itemRawList.at(indexOfItem));
		}

		addItem(itm);

		// Calcul de l'item de plus long
		if (valueText.size() > lengthMax.size())
			lengthMax = valueText;
	}

	// Taille en pixel de l'item de plus grand
	font.setBold(true);
	QFontMetrics fm(font);
	_sizeMax = fm.width(lengthMax) + 20;
}

//-------------------------------------------------------------------------
QStringList SmartCustomListWidget::search(QString text)
{
	clearAll();
	
	// Blocage des signaux car le clear déclenche le signal "onItemChanged" sinon...
	selectionModel()->blockSignals(true);
	
	QStringList tmpConvertedList;
	QStringList tmpRawList;
	QStringList tmpListStartingWith;
	QString lengthMax = "";
	QString formatedTextToSearch = text.toHtmlEscaped();


	// Sous liste contenant uniquement les items contenant le texte de la recherche
	for each (auto it in _itemConvertedList)
	{
		if (it.contains(formatedTextToSearch, Qt::CaseInsensitive))
		{
			tmpConvertedList.append(it);

			// On garde aussi une liste de valeur "raw" pour la renvoyer
			int indexOfItem = _itemConvertedList.indexOf(it);
			if (indexOfItem >= 0)
				tmpRawList.append(_itemRawList.at(indexOfItem));

			// Calcul de l'item de plus long
			if (it.size() > lengthMax.size())
				lengthMax = it;
		}
	}
	
	// Si le text entré est une valeur numérique, on affiche en premier les valeur dont la valeur de l'enum contient ces chiffres 
	bool ok = false;
	text.toInt(&ok);
	if (ok)
	{
		for each (auto it in tmpConvertedList)
		{
			int index1 = it.lastIndexOf("(");
			int index2 = it.lastIndexOf(")");
			int stanagEnumSize = (index2 - index1 - 1);
			QString stanagEnum = it.mid(index1 + 1, stanagEnumSize);

			if (stanagEnum.startsWith(text))
				tmpListStartingWith.append(it);
			else
				_itemConvertedSearchList.append(it);
		}
	}
	// Tri par ordre alphabétique + item commençant par le texte de la recherche au début
	else
	{
		tmpConvertedList.sort(Qt::CaseInsensitive);
		for (auto it : tmpConvertedList)
		{
			if (it.startsWith(formatedTextToSearch, Qt::CaseInsensitive))
				tmpListStartingWith.append(it);
			else
				_itemConvertedSearchList.append(it);
		}
	}

	// Ajout d'un séparateur ------------------------
	if (tmpListStartingWith.size() > 0 && _itemConvertedSearchList.size() > 0)
	{
		QFont font = this->font();
		QFontMetrics fm(font);
		_sizeMax = fm.width(lengthMax);
		int nbSep = _sizeMax / fm.width("-");

		tmpListStartingWith.append(QString(nbSep, *(QString("-").data()) ));
	}
	_itemConvertedSearchList = tmpListStartingWith + _itemConvertedSearchList;

	// Remplissage de la ListWidget avec la sélection
	buildItemList(formatedTextToSearch);

	selectionModel()->blockSignals(false);

	return tmpRawList;
}
