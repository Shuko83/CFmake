#include <QtGui>

#include "ui_QsSimpleUnit.h"
#include "ISwServiceUnitSI.h"

#include "QsSimpleUnit.h"
#include <QLocale>
#include <QVBoxLayout>
#include <SwApplication.h>



using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
QsSimpleUnit::QsSimpleUnit(QWidget *parent, bool signalWhenValueChanged) 
	: QsWidget(parent), _serviceUnitSI(NULL), _isMinimumValueSet(false), _isMaximumValueSet(false),
	_isSingleStepValueSet(false), _isVisualizationWidget(false), _isSignalEmittedOnValueChanged(false)
{
	_ui = new Ui::QsSimpleUnitWidgetClass();
	_ui->setupUi(this);

	//custom QsDoubleSpinbox
	_qsDoubleSpinBoxSI = new QsDoubleSpinBoxSI();
	_qsDoubleSpinBoxSI->setMaximum(9999999);
	_qsDoubleSpinBoxSI->setMinimum(-9999999);
	_qsDoubleSpinBoxSI->setAlignment(Qt::AlignRight);
	_qsDoubleSpinBoxSI->setClassCSS("doubleSpinBoxCfg");

	//init step, min, max
	_singleStepOperator = 1;
	_minValueOperator = -9999999;
	_maxValueOperator = 9999999;
	_valueDefaultUnit = 0;

	_ui->verticalLayoutSpinBox->addWidget(_qsDoubleSpinBoxSI);

	_ui->pbChangeUnit->setFocusPolicy(Qt::NoFocus);

	connect(_qsDoubleSpinBoxSI, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
	connect(_qsDoubleSpinBoxSI, SIGNAL(mouseLeave()), this, SIGNAL(mouseLeave()));

	//default
	_precision = 2;
	_qsDoubleSpinBoxSI->setPrecision(_precision);
	_familyUnitSimple = SiEnums::SiUnitFamilyAltitude;
	_currentUnit = SiEnums::SiUnitAltMeter;
	_unitOperator = SiEnums::SiUnitNull;

	_roundByStepValue = false;
	_roundTimer.setSingleShot(true);

	connect(&_roundTimer, &QTimer::timeout, this, &QsSimpleUnit::onRoundTimer);

	//_ui->labelValueSI->setText(QString::number(0., 'f', _precision));

	//setVisualizationWidget(false);

	//Récupération du service unité système international

#ifdef _DEBUG
	QLibrary swCore("SwCored");
#else
	QLibrary swCore("SwCore");
#endif

	typedef StreamWork::SwCore::ISwService *(*SWCORE_SERVICE)(QString) ;
	SWCORE_SERVICE swQueryService = (SWCORE_SERVICE) swCore.resolve("queryService");

	if(swQueryService)
	{
		_serviceUnitSI = dynamic_cast <IServiceUnitSI *> (swQueryService(CG_SW_SERVICE_UNIT_SI));
		if(_serviceUnitSI)
		{
			//register family unit
			_serviceUnitSI->registerTypeSI(_familyUnitSimple, this);

			setServiceUnitSI(_serviceUnitSI);
			setMenuContext();
			_ui->pbChangeUnit->installEventFilter(this);
		}
	}

	connect(_ui->pbChangeUnit, SIGNAL(clicked(bool)), this , SLOT(changeUnit()));

	setSignalWhenValueChanged(signalWhenValueChanged);

	setMinimumWidth(80);
	setMaximumWidth(200);
}


//-----------------------------------------------------------------------
QsSimpleUnit::~QsSimpleUnit()
{
	if (_serviceUnitSI)
	{
		//Desenregistrement auprès du service unité système international pour les différents types 
		_serviceUnitSI->unregisterTypeSI(_familyUnitSimple, this);
		_serviceUnitSI = NULL;
	}
}

//-----------------------------------------------------------------------
QsSimpleUnit::FamilyUnit QsSimpleUnit::getFamilyUnit() const
{
	FamilyUnit familyUnit = AltitudeFamily;

	switch(_familyUnitSimple)
	{
	case SiEnums::SiUnitFamilyAltitude:
		familyUnit = AltitudeFamily;
		break;
	case SiEnums::SiUnitFamilyAngle:
		familyUnit = AngleFamily;
		break;
	case SiEnums::SiUnitFamilyDistance:
		familyUnit = RangeFamily;
		break;
	case SiEnums::SiUnitFamilySpeed:
		familyUnit = SpeedFamily;
		break;
	case SiEnums::SiUnitFamilyDuration:
		familyUnit = TimeFamily;
		break;
	case SiEnums::SiUnitFamilyFrequency:
		familyUnit = FrequencyFamily;
		break;
	}
	return familyUnit;
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setFamilyUnit(FamilyUnit familyUnit)
{
	SiEnums::SiUnitFamily	newFamilyUnit = SiEnums::SiUnitFamilyNull;
	SiEnums::SiUnit		newUnit = SiEnums::SiUnitNull;

	//modification's family
	switch(familyUnit)
	{
	case AltitudeFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilyAltitude;
		newUnit			= SiEnums::SiUnitAltMeter;
		break;
	case AngleFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilyAngle;
		newUnit			= SiEnums::SiUnitDegree;
		break;
	case RangeFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilyDistance;
		newUnit			= SiEnums::SiUnitMeter;
		break;
	case SpeedFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilySpeed;
		newUnit			= SiEnums::SiUnitMeterPerSecond;
		break;
	case TimeFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilyDuration;
		newUnit			= SiEnums::SiUnitTimeSecond;
		break;
	case FrequencyFamily:
		newFamilyUnit	= SiEnums::SiUnitFamilyFrequency;
		newUnit			= SiEnums::SiUnitHertz;
		break;
	}

	if (_serviceUnitSI)
		newUnit = _serviceUnitSI->getCurrentUnit(newFamilyUnit);

	setSIFamilyUnit(newFamilyUnit, newUnit);
}

//-------------------------------------------------------------------------
void QsSimpleUnit::showButtonChangeUnit(bool hide)
{
	_ui->pbChangeUnit->setVisible(hide);
}

//-----------------------------------------------------------------------
bool QsSimpleUnit::isButtonChangeUnitVisible() const
{
	return _ui->pbChangeUnit->isVisible();
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setChangeUnitEnabled(bool enabledChangeUnit) 
{
	if (!enabledChangeUnit)
	{
		//for converter, set precision = 2 (max)
		processPrecisionChanged(2);
	}
	_isChangeUnitEnabled = enabledChangeUnit;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getMinimumValueOperator()
{	
	double minVal = 0;
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//if unit operator defined
		if (_unitOperator != SiEnums::SiUnitNull)
			return getMinimumValueSystemOperator(_unitOperator);
		else
			return getMinimumValueSystemOperator(unitSI);
	}
	return minVal;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getMinimumValueSystemOperator(SiEnums::SiUnit unit)
{
	double minVal = 0;

	if (_serviceUnitSI)
	{
		if (_isMinimumValueSet)
		{//minimum value set by operator

			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			//return value converted from default starlinx unit to unit (new unit) conversion
			bool errorConversion;

			double convertedValue = _serviceUnitSI->convertUnit(_minValueOperator, unitSI, unit, &errorConversion);

			if (!errorConversion)
				minVal = convertedValue;
		}
		else
			minVal = _serviceUnitSI->getMinimumValue(unit);
	}
	return minVal;
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMinimumValueOperator(double valueOpe)
{
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//if unit operator defined
		if ((_unitOperator != SiEnums::SiUnitNull) && (_unitOperator != unitSI))
			setMinimumValueOperatorInUnit(valueOpe, _unitOperator);
		else
			setMinimumValueOperatorToDefaultUnit(valueOpe);
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMinimumValueOperatorInUnit(double valueOpe, SiEnums::SiUnit unit)
{
	//unit defined by operator
	if (_serviceUnitSI)
	{
		//search the unit in the current family
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
		if (listUnit.indexOf(unit) != -1)
		{
			//default unit
			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			bool errorConversion ;

			//convert unit to default unit
			double convertedValue = _serviceUnitSI->convertUnit(valueOpe, unit, unitSI, &errorConversion);

			//convertedValue = minimum value converted from unit(unit operator) to uniSI (default unit)
			if (!errorConversion)
			{	
				setMinimumValueOperatorToDefaultUnit(convertedValue);
			}
		}
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMinimumValueOperatorToDefaultUnit(double valueOpe)
{
	//unit defined by default unit starlinx
	if (_serviceUnitSI)
	{
		//set minimum value for default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		double minValueSystem = _serviceUnitSI->getMinimumValue(unitSI);
		double maxValueSystem = _serviceUnitSI->getMaximumValue(unitSI);
		double maxValueOperator = getMaximumValueSystemOperator(unitSI);

		//Minimum operator is smaller than minimum system
		if ((valueOpe < minValueSystem) || (valueOpe > maxValueSystem)
			|| (valueOpe >maxValueOperator))
			valueOpe = minValueSystem;

	}	

	_minValueOperator = valueOpe;

	//convert minValue from default unit to currentUnit
	//_qsDoubleSpinBoxSI->setMinimum(_minValueOperator);
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		bool errorConversion ;

		//convertedValue = minimum value converted from default unit to Current unit
		double convertedValue = _serviceUnitSI->convertUnit(_minValueOperator, unitSI, _currentUnit, &errorConversion);

		if (!errorConversion)
		{	
			_qsDoubleSpinBoxSI->setMinimum(convertedValue);
		}
	}

	_isMinimumValueSet = true;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getMaximumValueOperator()
{
	double maxVal = 100000;
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//if unit operator defined
		if (_unitOperator != SiEnums::SiUnitNull)
			return getMaximumValueSystemOperator(_unitOperator);
		else
			return getMaximumValueSystemOperator(unitSI);
	}
	return maxVal;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getMaximumValueSystemOperator(SiEnums::SiUnit unit)
{
	double maxVal = 999999;
	if (_serviceUnitSI)
	{
		if (_isMaximumValueSet)
		{//maximum value set by operator

			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			//return value converted from default starlinx unit to unit (new unit) conversion
			bool errorConversion;

			double convertedValue = _serviceUnitSI->convertUnit(_maxValueOperator, unitSI, unit, &errorConversion);

			if (!errorConversion)
				maxVal = convertedValue;
		}
		else
			maxVal = _serviceUnitSI->getMaximumValue(unit);
	}

	return maxVal;
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMaximumValueOperator(double valueOpe)
{
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//if unit operator defined
		if ((_unitOperator != SiEnums::SiUnitNull) && (_unitOperator != unitSI))
			setMaximumValueOperatorInUnit(valueOpe, _unitOperator);
		else
			setMaximumValueOperatorToDefaultUnit(valueOpe);
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMaximumValueOperatorInUnit(double valueOpe, SiEnums::SiUnit unit)
{
	if (_serviceUnitSI)
	{
		//search the unit in the current family
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
		if (listUnit.indexOf(unit) != -1)
		{
			//default unit
			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			bool errorConversion;

			//convert unit to default unit
			double convertedValue = _serviceUnitSI->convertUnit(valueOpe, unit, unitSI, &errorConversion);

			//convertedValue = maximum value converted from unit(unit operator) to uniSI (default unit)
			if (!errorConversion)
			{
				setMaximumValueOperatorToDefaultUnit(convertedValue);
			}
		}
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setMaximumValueOperatorToDefaultUnit(double valueOpe)
{
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		double minValue = _serviceUnitSI->getMinimumValue(unitSI);
		double maxValue = _serviceUnitSI->getMaximumValue(unitSI);
		double minValueOperator = getMinimumValueSystemOperator(unitSI);

		//Minimum operator is higher than maximum system
		if ((valueOpe > maxValue) || (valueOpe < minValue)
			|| (valueOpe < minValueOperator))
			valueOpe = maxValue;

	}

	_maxValueOperator = valueOpe;

	//convert maxValue from default unit to currentUnit
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		bool errorConversion ;

		//convertedValue = maximum value converted from default unit to Current unit
		double convertedValue = _serviceUnitSI->convertUnit(_maxValueOperator, unitSI, _currentUnit, &errorConversion);

		if (!errorConversion)
		{	
			_qsDoubleSpinBoxSI->setMaximum(convertedValue);
		}
	}
	_isMaximumValueSet = true;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getSingleStepToDefaultUnit()
{
	double stepVal = 1;

	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);
		
		return getSingleStepSystemOperator(unitSI);
	}
	return stepVal;
}


//-----------------------------------------------------------------------
double QsSimpleUnit::getSingleStepOperator()
{
	double stepVal = 1;
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//if unit operator defined
		if (_unitOperator != SiEnums::SiUnitNull)
			return getSingleStepSystemOperator(_unitOperator);
		else
			return getSingleStepSystemOperator(unitSI);
	}
	return stepVal;
}

//-----------------------------------------------------------------------
double QsSimpleUnit::getSingleStepSystemOperator(SiEnums::SiUnit unit)
{
	double singleStepOp = 1;

	if (_serviceUnitSI)
	{
		if (_isSingleStepValueSet)
		{
			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			//return value single step converted from default starlinx unit to unit (new unit) conversion
			bool errorConversion;

			double convertedValue = _serviceUnitSI->convertUnit(_singleStepOperator, unitSI, unit, &errorConversion);

			if (!errorConversion)
				singleStepOp = convertedValue;
		}
		else
			singleStepOp = 1;
	}

	return singleStepOp;
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setSingleStepOperator(double valueOpe)
{
	if (_serviceUnitSI)
	{
		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		double valueApprox = QString::number(valueOpe, 'f', 3).toDouble();
		//double valueApprox = double(qRound(valueOpe * 1000000000.0)) / 1000000000.0; 

		//if unit operator defined
		if ((_unitOperator != SiEnums::SiUnitNull) && (_unitOperator != unitSI))
			setSingleStepOperatorInUnit(valueApprox, _unitOperator);
		else
			setSingleStepOperatorToDefaultUnit(valueApprox);
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setSingleStepOperatorInUnit(double valueOpe, SiEnums::SiUnit unit)
{
	if (_serviceUnitSI)
	{
		//search the unit in the current family
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
		if (listUnit.indexOf(unit) != -1)
		{
			//default unit
			SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

			bool errorConversion;

			//convert unit to default unit
			double convertedValue = _serviceUnitSI->convertUnit(valueOpe, unit, unitSI, &errorConversion);

			if (!errorConversion)
			{
				setSingleStepOperatorToDefaultUnit(convertedValue);
			}
		}
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setSingleStepOperatorToDefaultUnit(double valueOpe)
{
	_isSingleStepValueSet = true;

	valueOpe = QString::number(valueOpe, 'f', 3).toDouble();

	//Step = 0 is not possible or value bigger than visible value 
	if (valueOpe == 0 || valueOpe > 10000 ||  valueOpe < -0.001 )
		_isSingleStepValueSet = false;
	else
		_singleStepOperator = valueOpe; 

	_qsDoubleSpinBoxSI->setSingleStep(getSingleStepSystemOperator(_currentUnit));
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setSIFamilyUnit(SiEnums::SiUnitFamily unitFamilySI, SiEnums::SiUnit unitSI)
{
	if (_serviceUnitSI)
	{
		if (unitFamilySI != SiEnums::SiUnitFamilyNull)
		{
			if (unitFamilySI != _familyUnitSimple)
			{
				//unregister old family unit
				_serviceUnitSI->unregisterTypeSI(_familyUnitSimple, this);
				//register new family unit
				_serviceUnitSI->registerTypeSI(unitFamilySI, this);
			}

			int currentPrecision = _serviceUnitSI->getCurrentPrecision(unitFamilySI);
			processPrecisionChanged(currentPrecision);
		}
	}

	//update new family unit
	_familyUnitSimple	= unitFamilySI;
	setMenuContext();
	setSIUnit(unitSI);

	//init designer property
	emit familyChanged();
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setSIUnit(SiEnums::SiUnit unitSI)
{
	_currentUnit = unitSI;
	
	if(_serviceUnitSI)
	{
		//suffix
		if (isVisualizationWidget())
			_ui->labelUnitSI->setText(" "+ _serviceUnitSI->getSuffix(_currentUnit));
		else
		{
			_qsDoubleSpinBoxSI->setSuffix(" " + _serviceUnitSI->getSuffix(_currentUnit));
			double minVal = getMinimumValueSystemOperator(_currentUnit);
			double maxVal = getMaximumValueSystemOperator(_currentUnit);
			double sStep =  getSingleStepSystemOperator(_currentUnit);

			_qsDoubleSpinBoxSI->setMinimum(minVal);
			_qsDoubleSpinBoxSI->setMaximum(maxVal);
			_qsDoubleSpinBoxSI->setSingleStep(sStep);
		}
	}
}

//-----------------------------------------------------------------------
void QsSimpleUnit::setVisualizationWidget(bool visualization)
{
	_isVisualizationWidget = visualization;

	_ui->pbChangeUnit->setVisible(!_isVisualizationWidget);

	//if visualisation mode activated, change widget by stackedWidget
	if (visualization)
	{
		_ui->stackedWidget->setCurrentIndex(1);
		//update suffix
		if (_serviceUnitSI)
			_ui->labelUnitSI->setText(" "+ _serviceUnitSI->getSuffix(_currentUnit));
	}
	else
	{
		_ui->stackedWidget->setCurrentIndex(0);
		//update suffix
		if (_serviceUnitSI)
			_qsDoubleSpinBoxSI->setSuffix(" " + _serviceUnitSI->getSuffix(_currentUnit));
	}
}

//-----------------------------------------------------------------------
bool QsSimpleUnit::isVisualizationWidget() const
{
	return _isVisualizationWidget;
}


//---------------------------------------------------------------------------------
bool QsSimpleUnit::getSignalWhenValueChanged()
{
	return _isSignalEmittedOnValueChanged;
}

//---------------------------------------------------------------------------------
void QsSimpleUnit::setSignalWhenValueChanged(bool setValueOnChange)
{
	if (_isSignalEmittedOnValueChanged)
		disconnect(_qsDoubleSpinBoxSI, SIGNAL(valueChanged(double)), this, SLOT(valueSetChanged()));
	else
		disconnect(_qsDoubleSpinBoxSI, SIGNAL(editingFinished()), this, SLOT(valueSetChanged()));

	_isSignalEmittedOnValueChanged = setValueOnChange;

	// valueChanged = set the value when the operator is editing. This signal is better for convertor unit SI because
	//					when want converter the current value to different unit during the operator is editing
	// editingFinished = set the value when the operator finished to edit => this is better for configuration starlinx
	//						because when want to check the value at the end
	if (_isSignalEmittedOnValueChanged)
		connect(_qsDoubleSpinBoxSI, SIGNAL(valueChanged(double)), this, SLOT(valueSetChanged()));
	else
		connect(_qsDoubleSpinBoxSI, SIGNAL(editingFinished()), this, SLOT(valueSetChanged()));
}


//-------------------------------------------------------------------------
double QsSimpleUnit::getValue()
{
	double valueSI = -1;

	if (_serviceUnitSI)
	{
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//value in default unit starlinx
		valueSI = _valueDefaultUnit;

		//unit operator
		if ((_unitOperator != SiEnums::SiUnitNull) && (_unitOperator != unitSI))
		{
			//search the unit in the current family
			QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
			if (listUnit.indexOf(_unitOperator) != -1)
			{
				bool errorConversionUnitOpe = false;

				//convert unit to default unit
				double convertedValueOpe = _serviceUnitSI->convertUnit(valueSI, unitSI, _unitOperator, &errorConversionUnitOpe);

				if (!errorConversionUnitOpe)
				{
					valueSI = convertedValueOpe;
				}
			}
		}
	}
	return valueSI;
}
//-------------------------------------------------------------------------
void QsSimpleUnit::setValue(double valueOpe)
{
	if (_serviceUnitSI)
	{
		//default Starlinx unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		SiEnums::SiUnit unitOpeDefault = SiEnums::SiUnitNull;

		//_unitOperator exist and present in the current family
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
		if ((_unitOperator != SiEnums::SiUnitNull) && (listUnit.indexOf(_unitOperator) != -1))
			unitOpeDefault = _unitOperator;
		else
			unitOpeDefault = unitSI;

		//set value in default unit
		bool errorConversionDefault = false;

		//convert _unitOperator to default unit
		double convertedValueDefault = _serviceUnitSI->convertUnit(valueOpe, unitOpeDefault, unitSI, &errorConversionDefault);

		if (!errorConversionDefault && (convertedValueDefault !=_valueDefaultUnit) )
		{
			_valueDefaultUnit = convertedValueDefault;
			setValueFromDefaultToCurrentUnit();
			emit valueSimpleChanged();
		}
	}
}
//-------------------------------------------------------------------------
void QsSimpleUnit::valueSetChanged()
{
	//value changed by operator value setted is in current unit
	if (_serviceUnitSI)
	{
		//default Starlinx unit		
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		//set value in default unit
		bool errorConversionDefault = false;

		double valueOpe = _qsDoubleSpinBoxSI->value();

		//convert _unitOperator to default unit
		double convertedValueDefault = _serviceUnitSI->convertUnit(valueOpe, _currentUnit, unitSI, &errorConversionDefault);

		if (!errorConversionDefault)
		{
			if (_roundByStepValue)
			{
				double roundValue = round(convertedValueDefault / getSingleStepToDefaultUnit()) * getSingleStepToDefaultUnit();
				if (!qFuzzyCompare(roundValue, _valueDefaultUnit))
				{
					_valueDefaultUnit = roundValue;
					_roundTimer.start();
				}
			}
			else if (_valueDefaultUnit!=convertedValueDefault)
			{
				_valueDefaultUnit = convertedValueDefault;				
			}	

			setValueFromDefaultToCurrentUnit();
			emit valueSimpleChanged();
		}
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setValueFromDefaultToCurrentUnit()
{
	//convert value from default starlinx unit to current unit conversion
	if (_serviceUnitSI)
	{
		//default Starlinx unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		bool errorConversion = false;
		double convertedValue = _serviceUnitSI->convertUnit(_valueDefaultUnit, unitSI, _currentUnit, &errorConversion);

		if (!errorConversion)
		{
			if (isVisualizationWidget())
				_ui->labelValueSI->setText( QString::number( convertedValue , 'f', _precision ) );
			else
			{
				_qsDoubleSpinBoxSI->setValue(convertedValue);

				//if more than 2 decimals precision we show it in the tooltip else we show always 2 decimals
				int precisionShow = 2;
				if (_qsDoubleSpinBoxSI->getPrecision() > 2)
					precisionShow = _qsDoubleSpinBoxSI->getPrecision();

				_qsDoubleSpinBoxSI->setToolTip(QString::number(convertedValue, 'f', precisionShow) + " " + _qsDoubleSpinBoxSI->suffix());
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setServiceUnitSI(IServiceUnitSI * serviceUnitSI)
{
	_serviceUnitSI = serviceUnitSI; 
	setFamilyUnit(getFamilyUnit());
	if (_serviceUnitSI)
	{
		if (isVisualizationWidget())
			_ui->labelUnitSI->setText(" "+ _serviceUnitSI->getSuffix(_currentUnit));
		else
		{
			_qsDoubleSpinBoxSI->setSuffix(" " + _serviceUnitSI->getSuffix(_currentUnit));

			double minVal = getMinimumValueSystemOperator(_currentUnit);
			double maxVal = getMaximumValueSystemOperator(_currentUnit);
			double sStep =  getSingleStepSystemOperator(_currentUnit);

			_qsDoubleSpinBoxSI->setMinimum(minVal);
			_qsDoubleSpinBoxSI->setMaximum(maxVal);
			_qsDoubleSpinBoxSI->setSingleStep(sStep);
		}


		SiEnums::SiUnit unitService = _serviceUnitSI->getCurrentUnit(_familyUnitSimple);
		processUnitChanged(unitService);
		int currentPrecision = _serviceUnitSI->getCurrentPrecision(_familyUnitSimple);
		processPrecisionChanged(currentPrecision);
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::processUnitChanged(SiEnums::SiUnit unit)
{
	//Si unité de la widget différente de l'unité générale => conversion
	if ((unit != _currentUnit) && _serviceUnitSI && isChangeUnitEnabled())
	{
		double valueUnit = 0;

		valueUnit = _valueDefaultUnit;

		//default unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyUnitSimple);

		bool errorConversion;
		double convertedValue = _serviceUnitSI->convertUnit(_valueDefaultUnit, unitSI, unit, &errorConversion);

		if (!errorConversion)
		{
			//set new unit
			_currentUnit = unit;

			//update action contextuel
			QHashIterator<QAction *, SiEnums::SiUnit> i(_listAction);
			while (i.hasNext()) 
			{
				i.next();
				if (i.value() == _currentUnit)
					i.key()->setChecked(true);
			}

			//update other unit 
			emit unitChanged();

			if (isVisualizationWidget())
			{
				_ui->labelUnitSI->setText(" "+ _serviceUnitSI->getSuffix(unit));
				_ui->labelValueSI->setText(QString::number(convertedValue, 'f', _precision));
			}
			else
			{
				_qsDoubleSpinBoxSI->setSuffix(" " + _serviceUnitSI->getSuffix(unit));

				double minVal = getMinimumValueSystemOperator(unit);
				double maxVal = getMaximumValueSystemOperator(unit);
				double sStep =  getSingleStepSystemOperator(unit);

				if (minVal > convertedValue)
					convertedValue = minVal;

				if (maxVal < convertedValue)
					convertedValue = maxVal;


				disconnect (_qsDoubleSpinBoxSI, SIGNAL(editingFinished()), this , SLOT(valueSetChanged()));

				_qsDoubleSpinBoxSI->setMinimum(minVal);
				_qsDoubleSpinBoxSI->setMaximum(maxVal);
				_qsDoubleSpinBoxSI->setValue(convertedValue);
				_qsDoubleSpinBoxSI->setSingleStep(sStep);

				//if more than 2 decimals precision we show it in the tooltip else we show always 2 decimals
				int precisionShow = 2;
				if (_qsDoubleSpinBoxSI->getPrecision() > 2)
					precisionShow = _qsDoubleSpinBoxSI->getPrecision();
				_qsDoubleSpinBoxSI->setToolTip( QString::number( convertedValue, 'f', precisionShow ) + " " + _qsDoubleSpinBoxSI->suffix() );

				connect (_qsDoubleSpinBoxSI, SIGNAL(editingFinished()), this , SLOT(valueSetChanged()));
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::processPrecisionChanged(int precision)
{
	if (isChangeUnitEnabled())
		setPrecision(precision);
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setPrecision(int precision)
{
	_precision = precision;

	if (isVisualizationWidget())
	{
		double valueCurrent = _ui->labelValueSI->text().toDouble();
		QString tst = QString::number(valueCurrent, 'f', _precision);
		_ui->labelValueSI->setText(QString::number(valueCurrent, 'f', _precision));
	}
	else
		_qsDoubleSpinBoxSI->setPrecision(_precision);
}

//-------------------------------------------------------------------------
void QsSimpleUnit::changeUnit()
{
	if (_serviceUnitSI)
	{
		SiEnums::SiUnit newUnit;
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);

		int i = listUnit.indexOf(_currentUnit);
		if (i != -1)
		{
			// if the current unit is the last
			if (i == listUnit.size() - 1)  
			{
				//the first is used
				newUnit = listUnit.at(0);
			}
			else
				newUnit = listUnit.at(i+1);

			processUnitChanged(newUnit); 
		}
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::changeUnitTriggered(QAction * action)
{
	if (action)
	{
		//update other unit 
		QHashIterator<QAction *, SiEnums::SiUnit> i(_listAction);
		while (i.hasNext()) 
		{
			i.next();

			if (i.key() == action)
				processUnitChanged(i.value());
		}
	}
}

//-------------------------------------------------------------------------
bool QsSimpleUnit::eventFilter(QObject * obj, QEvent *event)
{	
	if (obj == _ui->pbChangeUnit && event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if(mouseEvent)
		{
			if(mouseEvent->button() == Qt::RightButton)
			{
				_menu->exec(mouseEvent->globalPos());
			}
		}
	}
	return QObject::eventFilter(obj,event);
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setMenuContext()
{
	if (_serviceUnitSI)
	{
		_menu = _ui->pbChangeUnit->menu();

		if (_menu)
			_menu->clear();
		else
		{
			_menu = new QMenu(this);
			_menu->setObjectName("MenuUnitSI");

			_alignmentGroup = new QActionGroup(this);
			_alignmentGroup->setExclusive(true);
		}

		SiEnums::SiUnit unitService = _serviceUnitSI->getCurrentUnit(_familyUnitSimple);

		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyUnitSimple);
		if (listUnit.size() != 0)
		{
			//Push button change unit's action
			for (int nbUnit = 0; nbUnit < listUnit.size(); nbUnit++)
			{
				QString nameUnit = _serviceUnitSI->getNameUnit(listUnit.at(nbUnit));
				QAction * actionUnit = new QAction(nameUnit, this);   
				actionUnit->setCheckable(true);
				_alignmentGroup->addAction(actionUnit);
				_menu->addAction(actionUnit);
				_listAction.insert(actionUnit,listUnit.at(nbUnit));

				if (listUnit.at(nbUnit) == unitService)
					actionUnit->setChecked(true);
			}
			connect(_alignmentGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeUnitTriggered(QAction *)));
		}
	}
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setNoStaValue(int value)
{
	_noStaValue = value;
}

//-------------------------------------------------------------------------
int QsSimpleUnit::getNoStaValue()
{
	return _noStaValue;
}

//-------------------------------------------------------------------------
bool QsSimpleUnit::getRoundByStepValue()
{
	return _roundByStepValue;
}

//-------------------------------------------------------------------------
void QsSimpleUnit::setRoundByStepValue(bool value)
{
	_roundByStepValue = value;
}

//-------------------------------------------------------------------------
void QsSimpleUnit::onRoundTimer()
{
	valueSetChanged();
}
