#ifndef QS_WIDGETS_QSDOUBLESPINBOXSI_H
#define QS_WIDGETS_QSDOUBLESPINBOXSI_H

#include "QsDoubleSpinBox.h"


//Double spinBox SI unit
class QsDoubleSpinBoxSI : public QsDoubleSpinBox
{
	Q_OBJECT
	//requirement : Set space between each 3 number
	//example : 1 000 000,00
	// show precision or ~ if isn't present

public:
	QsDoubleSpinBoxSI(QWidget *parent = 0);

	QValidator::State validate(QString &input, int &pos) const;

	double	valueFromText(const QString &text) const;
	QString textFromValue(double value) const;

	void	stepBy(int steps);
	/*bool	event(QEvent * event);*/

	void	setPrecision(double precision);
	double	getPrecision()const{return _precisionDoubleSpin;}

private:
	double _precisionDoubleSpin;
};

#endif //QS_WIDGETS_QSDOUBLESPINBOXSI_H