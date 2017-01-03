
#include <QToolTip>

#include "QsDoubleSpinBoxSI.h"



//-------------------------------------------------------------------------
QsDoubleSpinBoxSI::QsDoubleSpinBoxSI(QWidget *parent)
: QsDoubleSpinBox(parent),_precisionDoubleSpin(0)
{
	setLocale(QLocale(QLocale::English));
}

//-------------------------------------------------------------------------
QValidator::State QsDoubleSpinBoxSI::validate(QString &input, int &pos) const
{
	QToolTip::hideText();

	//temporary save value
	QString realText = input; 

	//notation with . (decimal)
	realText.replace(',','.');

	//remove if 1 more '.' => Qt return acceptable validation but delete the second '.'
	int posiDot = realText.indexOf('.');
	if (posiDot + 1 <realText.size() && (QString::compare(realText.at(posiDot + 1), ".") == 0))
	{
		realText.remove(posiDot + 1, 1);
		pos --;
	}

	//save this value with .
	input = realText;

	//remove approximation ("~ ") to have 1 000 000.00 ft (example)
	realText.remove("~ ");

	//remove suffix " ft" (example)
	realText.remove(suffix());

	//remove space beetween thousand value  (example) 1 000 000.00 => 1000000.00 
	realText.remove(' ');

	if (_precisionDoubleSpin == 0 && realText.indexOf('.') != -1)
	{
		input.remove('.');
		return QValidator::Invalid;
	}

	//append suffix for Qt validation
	realText.append(suffix());
	
	QValidator::State stateVal = 
		QDoubleSpinBox::validate(realText, pos);

	//If Qt state is invalid we return input corrected by QT
	if (stateVal == QValidator::Invalid)
		input = realText;

	//if valid the value returned is input with state validated by Qt
	return stateVal;
}

//-------------------------------------------------------------------------
double QsDoubleSpinBoxSI::valueFromText(const QString &text) const
{
	//temporary save value
	QString realText = text;

	realText.remove("~ ");
	realText.remove(suffix());
	realText.remove(' ');
	//we have 1000000.00

	double valueFrom = QDoubleSpinBox::valueFromText(realText);


	return valueFrom;
}

//-------------------------------------------------------------------------
QString QsDoubleSpinBoxSI::textFromValue(double val) const
{	
	int precisionMax = 2;
	 if (_precisionDoubleSpin > 2)
		 precisionMax = _precisionDoubleSpin;

	//découpe tous les 3 chiffres
	QString textValueSI = QString::number(val, 'f', precisionMax); //max precision mise à 2

	QStringList listValue = textValueSI.split(".");
	QString prefixInfo;
	QString prefixDot;
	QString suffixDot;

	if (listValue.size() > 0)
	{
		//premier avant la virgule
		if(_precisionDoubleSpin == 0)
			prefixDot = QString::number( qRound( val ), 'f', 0);//listValue[0];
		else
			prefixDot = listValue[0];

		int lengthText = prefixDot.length();
		while (lengthText > 0)
		{

			if ((lengthText - 3) > 0)
				prefixDot.insert(lengthText-3," ");
			lengthText -= 3;
		}

		if (_precisionDoubleSpin > 0)
		{
			suffixDot = ".";
			if (listValue.size() == 2)
			{	//suffix present
				suffixDot += listValue[1];
			}

			//precision + ,
			if (suffixDot.size() < (_precisionDoubleSpin + 1))
			{//manque des valeurs a la fin
				while (suffixDot.size() < (_precisionDoubleSpin + 1))
				{
					suffixDot.append("0");
				}
			}
			else
				suffixDot.resize(_precisionDoubleSpin + 1);
		}

		//transform value without space
		QString resultText = QString(prefixDot+suffixDot);
		resultText.remove(' ');

		double valueTest = resultText.toDouble();
		double valueTextValueSI = textValueSI.toDouble();

		//if visible value doesn't show all decimal precision => prefix ~ 
		if (!qFuzzyCompare(valueTextValueSI, valueTest) )
				prefixInfo = "~ ";
	}

	return prefixInfo+prefixDot+suffixDot;
}

//-------------------------------------------------------------------------
void QsDoubleSpinBoxSI::setPrecision(double precision)
{
	//show precision
	_precisionDoubleSpin = precision;

	//default precision for a doubleSpinbox= 2 , if more decimals will increase else set default precision
	//if precision less than 2 QDoublespinbox round the value and we need 2 decimals or more precision
	if (_precisionDoubleSpin > 2)
		setDecimals(_precisionDoubleSpin);
	else
		setDecimals(2);

	setValue(value());
}

//-------------------------------------------------------------------------
void QsDoubleSpinBoxSI::stepBy(int steps)
{
	QDoubleSpinBox::stepBy(steps);
	QToolTip::hideText();
	emit editingFinished();
}

//-------------------------------------------------------------------------
/*
bool QsDoubleSpinBoxSI::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		if (helpEvent) {
			QToolTip::showText(helpEvent->globalPos(), textFromValue(value()) + suffix());
		} else {
			QToolTip::hideText();
			event->ignore();
		}

		return true;
	}
	return QsDoubleSpinBox::event(event);
}
*/
