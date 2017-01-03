
#include <QtGui>
#include <QApplication.h>

#include "QsCoordinateLine.h"


int LineEditStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
	if (metric == QStyle::PM_TextCursorWidth)
		return 0;

	return QProxyStyle::pixelMetric(metric,option,widget);
}

//-----------------------------------------------------------------------
QsCoordinateLine::QsCoordinateLine(QWidget *parent)	: QsLineEdit(parent),
_DD_Lat_RegExp(QString::fromLatin1("^([0-8][0-9]|)\\.[0-9]{5}[NS]$")),
_DD_Long_RegExp(QString::fromLatin1("^([0-1][0-7][0-9])\\.[0-9]{5}[EW]$")),
_DM_Lat_RegExp(QString::fromLatin1("^([0-8][0-9]|)°([0-8][0-9]|)\\.{1}[0-9]{1,4}'[NS]$")),
_DM_Long_RegExp(QString::fromLatin1("^?(^{0,1}[0-1]{0,1}[0-7]{0,1}[0-9]|[1-9]?[0-9])°([0-8][0-9]|)\\.{1}[0-9]{1,4}'[EW]$")),
_DMS_Lat_RegExp(QString::fromLatin1("^([0-8][0-9]|)°([0-5][0-9]|)'([0-5][0-9]|)\\.{1}[0-9]{1,2}\"[NS]$")),
_DMS_Long_RegExp(QString::fromLatin1("^?(^{0,1}[0-1]{0,1}[0-7]{0,1}[0-9]|[1-9]?[0-9])°([0-5][0-9]|)'([0-5][0-9]|)\\.{1}[0-9]{1,2}\"[EW]$")),
_MGRSRegExp(QString::fromLatin1("^[0-9]{2} [A-H,J-N,P-Z,a-h,j-n,p-z] [A-H,J-N,P-Z,a-h,j-n,p-z]{2} [0-9]{5} [0-9]{5}$")),
_UTMRegExp(QString::fromLatin1("^[0-9]{2} [NS] [0-9]{6} [0-9]{7}$")),
_GEOREFRegExp(QString::fromLatin1("[A-H,J-N,P-Z,a-h,j-n,p-z]{1,4}[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]"))
{

	setMouseTracking(false);
	setStyle(new LineEditStyle(style()));

	//unmodifiable character
	_maskCarac << QString::fromLatin1("°") << QString::fromLatin1(" ") << QString::fromLatin1("'") << QString::fromLatin1("\"") << QString::fromLatin1(".");

	QFont font = this->font();
	font.setPointSize(12);
	setFont(font);

	_currentValidator = 0;
	_textFormat.clear();
	_textInputMask.clear();
	_listCharacAvailable.clear();

	_modificationMode = normalMode;

	setUnitCoordinateLine(decimal_Lat);

	connect(this, SIGNAL(textChanged(const QString &)), this , SIGNAL(coordinateChanged()));
}

//-----------------------------------------------------------------------
QsCoordinateLine::~QsCoordinateLine()
{
	setValidator(0);
	if (_currentValidator != 0)
		delete _currentValidator;
}

//-----------------------------------------------------------------------
QsCoordinateLine::UnitCoordinateLine QsCoordinateLine::getUnitCoordinateLine() const
{
	return _unitCoordinateLine;
}

//-----------------------------------------------------------------------
void QsCoordinateLine::setUnitCoordinateLine(UnitCoordinateLine unitCoordinateLine)
{
	_unitCoordinateLine = unitCoordinateLine;

	// _textFormat value corresponding to 0° lat and 0°
	_textFormat.clear();
	_textInputMask.clear();
	_listCharacAvailable.clear();

	if (_currentValidator != 0)
		delete _currentValidator;

	switch(unitCoordinateLine) 
	{
	case decimal_Lat:
		{
			_currentValidator = new QRegExpValidator(_DD_Lat_RegExp, this);
			_textFormat = "00.00000N";
			_textInputMask = "99.99999A";
			_listCharacAvailable.append(QStringList() << "N" << "S");
		}
		break;

	case decimal_Lon:
		{
			_currentValidator = new QRegExpValidator(_DD_Long_RegExp, this);
			_textFormat = "000.00000E";
			_textInputMask = "999.99999A";
			_listCharacAvailable.append(QStringList() << "E" << "W");
		}
		break;

	case decimal_Minuts_Lat:
		{
			_currentValidator = new QRegExpValidator(_DM_Lat_RegExp, this);
			_textFormat = QString::fromLatin1("00°00.0000'N");
			_textInputMask = QString::fromLatin1("99°99.9999'A");
			_listCharacAvailable.append(QStringList() << "N" << "S");
		}
		break;

	case decimal_Minuts_Lon:
		{
			_currentValidator = new QRegExpValidator(_DM_Long_RegExp, this);
			_textFormat = QString::fromLatin1("000°00.0000'E");
			_textInputMask = QString::fromLatin1("999°99.9999'A");
			_listCharacAvailable.append(QStringList() << "E" << "W");
		}
		break;

	case sexagesimal_Lat:
		{
			_currentValidator = new QRegExpValidator(_DMS_Lat_RegExp, this);
			_textFormat = QString::fromLatin1("00°00'00.00\"N");
			_textInputMask = QString::fromLatin1("99°99'99.99\"A");
			_listCharacAvailable.append(QStringList() << "N" << "S");
		}
		break;

	case sexagesimal_Lon:
		{
			_currentValidator = new QRegExpValidator(_DMS_Long_RegExp, this);
			_textFormat = QString::fromLatin1("000°00'00.00\"E");
			_textInputMask = QString::fromLatin1("999°99'99.99\"A");
			_listCharacAvailable.append(QStringList() << "E" << "W");
		}
		break;

	case mgrs:
		{
			_currentValidator = new QRegExpValidator(_MGRSRegExp, this);
			_textFormat = "31 N AA 66021 00000";
			_textInputMask = "99 A AA 99999 99999";
			_listCharacAvailable.append(QStringList() << "A"<< "B"<< "C"<< "D"<< "E"<< "F"<< "G"<< "H"<< "J"<< 
				"K"<< "L"<< "M"<< "N"<< "P"<< "Q"<< "R"<< "S"<< "T"<< "U"<< "V"<< "W"<< "X"<< "Y" << "Z");
		}
		break;

	case utm:
		{
			_currentValidator = new QRegExpValidator(_UTMRegExp, this);
			_textFormat = "31 N 166021 0000000";
			_textInputMask = "99 A 999999 9999999";
			_listCharacAvailable.append(QStringList() << "N" << "S");
		}
		break;

	case georef:
		{
			_currentValidator = new QRegExpValidator(_GEOREFRegExp, this);
			_textFormat = "NGAA0000000000";
			_textInputMask = "AAAA9999999999";
			_listCharacAvailable.append(QStringList() << "A"<< "B"<< "C"<< "D"<< "E"<< "F"<< "G"<< "H"<< "J"<< 
				"K"<< "L"<< "M"<< "N"<< "P"<< "Q"<< "R"<< "S"<< "T"<< "U"<< "V"<< "W"<< "X"<< "Y" << "Z");
		}
		break;

	default :;
	}

	//Avoid sending the textchanged 
	blockSignals(true);

	setValidator(_currentValidator);
	setInputMask(_textInputMask);
	setCoordinate(_textFormat);

	blockSignals(false);

}

//-----------------------------------------------------------------------
void QsCoordinateLine::setModificationMode(ModificationMode modificationMode)
{
	_modificationMode = modificationMode;
}

//-----------------------------------------------------------------------
QsCoordinateLine::ModificationMode QsCoordinateLine::getModificationMode() const
{
	return _modificationMode;
}

//-----------------------------------------------------------------------
void QsCoordinateLine::enterEvent(QEvent * /*event*/)
{
	QFontMetrics fm(font());
	int pixelsWide = fm.width('0');
	int pixelsHigh = fm.height();

	QPixmap pixMap (pixelsWide, pixelsHigh);
	pixMap.fill(QColor(50,102,255,102));
	setCursor(QCursor(pixMap, 0, -1));
}

//-----------------------------------------------------------------------
void QsCoordinateLine::focusInEvent (QFocusEvent * /*event*/)
{
	enterEvent(0);

	if (!hasSelectedText())
	{
		//select first caract
		setSelection(0,1);	
	}
}

//-----------------------------------------------------------------------
void QsCoordinateLine::mousePressEvent (QMouseEvent * event)
{
	if (event->button() == Qt::RightButton)
	{
		QWidget::mousePressEvent(event);
		return;
	}

	int positionCursor = cursorPositionAt (event->pos());

	//selection de 1 caractère
	QString valueToPress = text().mid(positionCursor, 1);
	int indexCarac = _maskCarac.indexOf(valueToPress);
	//_maskCarac = selection d'un caractère non selectionnable 
	if (indexCarac == -1)
	{
		setCursorPosition(cursorPositionAt (event->pos()));
		_initDrag = cursorPosition();

		if (_initDrag < maxLength())
			setCursorPosition(_initDrag);
		else
			setCursorPosition(maxLength()-1);

		setSelection(cursorPosition(), 1);
	}
	else
	{
		//on est sur un caractère spécial,
		//on relève la demi largeur du curseur de souris
		QFontMetrics fm(font());
		int cursorHalfWidth = (fm.width('0'))/2.;

		//On cherche quel est le caractère se situant sous le milieu du curseur de souris
		QPoint hotSpot =  cursor().pos();
		int middleCursor = mapFromGlobal(QPoint(hotSpot.x() + cursorHalfWidth, hotSpot.y())).x();
		int position = cursorPositionAt (QPoint(middleCursor, cursor().pos().y()));

		//on sélectionne le caractère précédent ou suivant.
		int index = 0;
		if (position == positionCursor)
			index = positionCursor-1;
		else
			index = positionCursor+1;

		setCursorPosition(index);
		_initDrag = cursorPosition();

		if (_initDrag < maxLength())
			setCursorPosition(_initDrag);
		else
			setCursorPosition(maxLength()-1);

		setSelection(cursorPosition(), 1);
	}
} 

//-----------------------------------------------------------------------
void QsCoordinateLine::mouseDoubleClickEvent(QMouseEvent * /*event*/)
{
	selectAll();
}

//-----------------------------------------------------------------------
void QsCoordinateLine::mouseMoveEvent (QMouseEvent * event)
{
	int startSelection = _initDrag;

	//Si sélection de droite à gauche (afin de prendre dans la sélection le dernier caractère)
	if (0 > (cursorPositionAt(event->pos()) - startSelection) && startSelection < maxLength())
		startSelection ++;

	int endDrag = cursorPositionAt(event->pos()) - startSelection;

	// cursorPositionAt(event->pos()) != _initDrag => pas le même caactère qui est sélectionné 
	if (cursorPositionAt(event->pos()) != _initDrag)
	{
		setSelection(startSelection, endDrag);
	}
}

//-----------------------------------------------------------------------
void QsCoordinateLine::keyPressEvent(QKeyEvent* event)
{
	_modificationMode = normalMode;

	//Ctr+A, Ctr+C, Ctr+V
	if (event->modifiers() == Qt::ControlModifier)
	{
		if (event->key() == Qt::Key_C)
			QLineEdit::copy();		
		if (event->key() == Qt::Key_A)
			selectAll();
		if (event->key() == Qt::Key_V)
			paste();
		return;
	}

	//Key_End
	if (event->key() == Qt::Key_End)
	{
		setCursorPosition(maxLength()-1);
		setSelection(cursorPosition(), 1);
		return;
	}

	//Key_Home
	if (event->key() == Qt::Key_Home)
	{
		setCursorPosition(0);
		setSelection(cursorPosition(), 1);
		return;
	}
	if (isReadOnly())
		return;

	
	int keyPressed = event->key();
	if (keyPressed == Qt::Key_Left)
	{
		//cursor movement
		setCursorPosition(cursorPosition()-2);
		setSelection(cursorPosition(), 1);
	}
	else if (keyPressed == Qt::Key_Right)
	{
		//cursor movement
		if (cursorPosition() < maxLength())
		{
			setCursorPosition(cursorPosition()-1);
			cursorForward(false);
			setSelection(cursorPosition(), 1);
		}
	}
	else if (hasSelectedText())
	{	
		if ((keyPressed == Qt::Key_Up) || (keyPressed == Qt::Key_Down))
		{
			bool isUpKey = true;
			if (keyPressed == Qt::Key_Down)
				isUpKey = false;

			QString textSelected = selectedText();
			QString newValue;

			//1 character selected
			if (textSelected.length() == 1)
			{
				//number modification
				if (textSelected.at(0).isDigit())
				{
					int valueCoord = textSelected.toInt();
					if (isUpKey)
					{
						if ((valueCoord+1) != 10)
							valueCoord++;
					}
					else
					{
						if ((valueCoord-1) != -1)
							valueCoord--;
					}
					newValue = QString::number(valueCoord);
				}
				else
				{
					//character modification
					int index = _listCharacAvailable.indexOf(textSelected);
					if (index != -1)
					{
						if (isUpKey)
						{
							if ((index+1) != _listCharacAvailable.size())
								index++;
						}
						else
						{
							if ((index-1) != -1)
								index--;
						}
						newValue = _listCharacAvailable.at(index);
					}
				}

				_modificationMode = changeCharacMode;

				//text entier
				QString newInput = text();

				int selectionBegin = selectionStart();
				newInput.replace(selectionBegin, 1, newValue);
				setCoordinate(newInput);
			}	
		}
		else if ((keyPressed == Qt::Key_Backspace) || (keyPressed == Qt::Key_Delete))
		{
			if (isAllTextSelected())
				_modificationMode = resetMode;
			else if (keyPressed == Qt::Key_Delete) 
				_modificationMode = deleteMode;
			else if (keyPressed == Qt::Key_Backspace)
				_modificationMode = removeMode;

			//QsPosition will update data
			emit resetValue();
		}
		else
		{
			_modificationMode = writeMode;

			//text entier
			QString newInput = text();

			//normal mode => Write mode
			QChar insertingChar = event->text()[0];

			//caractère modifié
			insertingChar = insertingChar.toUpper();

			int selectionBegin = selectionStart();
			newInput.replace(selectionBegin, 1, insertingChar);
			setCoordinate(newInput);
		}
	}
}

//-----------------------------------------------------------------------
void QsCoordinateLine::setCoordinate(QString coordinate)
{	
	//coordinate reset
	QString coordinateValue = coordinate;

	// texte selectionne + position depart + taille selection
	QString selection = selectedText();
	int selectionBegin = selectionStart();
	int selectionLength = selection.length();

	//text entier
	QString newInput = text();

	switch(_modificationMode)
	{
	case resetMode:				/** reset all character(s) */	
	{
		//selection à récuperer
		QString valueToReplace = coordinate.mid(selectionBegin, selectionLength);

		//on remplace la partie qui est sélectionné
		newInput.replace(selectionBegin, selectionLength, valueToReplace);

		coordinateValue = newInput;
	}
	break;

	case deleteMode:			/** delete character(s) (suppr key)*/
	{
		//selection à récuperer
		QString valueToReplace = coordinate.mid(selectionBegin, selectionLength);

		//on remplace la partie qui est sélectionné
		newInput.replace(selectionBegin, selectionLength, valueToReplace);

		coordinateValue = newInput;
	}
	break;

	case removeMode:			/** remove character(s) (backspace key) => left movement + suppr character*/
	{
		//1 character
		if (selectionLength == 1)
		{
			//On se trouve sur le dernier caractère
			if (selectionBegin == (text().length() - 1))
			{
				//selection à récuperer
				QString lastCharacToReplace = coordinate.mid(selectionBegin, selectionLength);

				//on remplace la partie qui est sélectionnée
				newInput.replace(selectionBegin, selectionLength, lastCharacToReplace);
			}

			//left movement 
			if (selectionBegin != 0)
				selectionBegin--;

			//suppr previous character 
			QString valueToDel = coordinateValue.mid(selectionBegin, selectionLength);

			//cherche si le caractère d'avant est un _maskCarac
			int indexCarac = _maskCarac.indexOf(valueToDel);
			if (indexCarac != -1 && selectionBegin != 0)
				selectionBegin--;
		}

		//selection à récuperer
		QString valueToReplace = coordinate.mid(selectionBegin, selectionLength);

		//on remplace la partie qui est sélectionnée
		newInput.replace(selectionBegin, selectionLength, valueToReplace);

		coordinateValue = newInput;
	}
	break;

	case writeMode:
	{
		selectionBegin ++;	
		if (selectionBegin > text().length())
			selectionBegin = text().length();

		QString nextString = coordinateValue.mid(selectionBegin, selectionLength);
		int indexCarac = _maskCarac.indexOf(nextString);
		if (indexCarac != -1)
			selectionBegin++;
	}
	break;

	case changeCharacMode:
	case changeUnitMode:
	{
		//nothing to do
	}
	break;

	case pasteMode:
	{
		if (selectionLength == 1)
		{
			//si juste un caractère sélectionné
			// -> on colle a partir de ce caractère
			selectionLength = newInput.size();
		}

		//selection à récupérer = juste le nombre de caractere selectionné
		if (selectionLength > coordinate.size())
			selectionLength = coordinate.size();

		coordinate.resize(selectionLength);

		//on remplace la partie qui est sélectionnée
		newInput.replace(selectionBegin, selectionLength, coordinate);

		coordinateValue = newInput;

		//position du cursor à la fin de la partie copiée
		selectionBegin += selectionLength;

		if (selectionBegin > text().length())
			selectionBegin = text().length();

		//regarde si le prochain caractère est un mask character
		QString nextString = coordinateValue.mid(selectionBegin, 1);
		int indexCarac = _maskCarac.indexOf(nextString);
		if (indexCarac != -1)
			selectionBegin++;
	}
	break;

	}

	int nextSelection = 0;
	//validate new value
	QValidator::State state = (validator()->validate(coordinateValue, nextSelection));

	if ( state == QValidator::Acceptable)
	{
			//new value acceptable
		setText(coordinateValue);

		if ((_modificationMode != normalMode) && (_modificationMode != changeUnitMode))
		{
			int cursorPos = selectionBegin;
			if (cursorPos == text().length())
				cursorPos--;

			setCursorPosition(selectionBegin);
			setSelection(cursorPos, 1);
		}
		else if (_modificationMode == changeUnitMode)
		{
			setFocus(Qt::MouseFocusReason);
			setCursorPosition(0);
			setSelection(cursorPosition(), 1);
		}
	}
}

//-----------------------------------------------------------------------
void  QsCoordinateLine::paste ()
{
	//get clipboard text to get correct size and put cursor in the correct position or at end
	QString clip = QApplication::clipboard()->text(QClipboard::Clipboard);
	setModificationMode(pasteMode);

	setCoordinate(clip);
}

//-----------------------------------------------------------------------
bool QsCoordinateLine::isAllTextSelected()
{
	if (QString::compare(selectedText(), text()) == 0)
		return true;
	return false;
}
