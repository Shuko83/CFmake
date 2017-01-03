#ifndef QS_WIDGETS_QSSIMPLEUNIT_H
#define QS_WIDGETS_QSSIMPLEUNIT_H

#include <IServiceUnitSI.h>

#include "QsDoubleSpinBoxSI.h"
#include "SiEnums.h"
#include <QsWidget.h>

#include <QsWidgets_Global.h>
#include <QGridLayout>
#include <QHash>
#include <QVariant>
#include <QAction>
#include <QMenu>
#include <QTimer>

namespace Ui{class QsSimpleUnitWidgetClass;}

/** @class	QsSimpleUnit
	*	@brief	QsSimpleUnit widget.
	*			Contains data value with accuracy
	*			widget is modulates through special properties
	*	@see enum CoordinatesFamily
	*/
class QSWIDGETS_EXPORT QsSimpleUnit : public QsWidget, UnitSIListener
{
	Q_OBJECT

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------
	Q_PROPERTY (FamilyUnit				familyUnit				READ	getFamilyUnit				WRITE	setFamilyUnit			NOTIFY		familyChanged			)
	Q_PROPERTY (bool					visualization			READ	isVisualizationWidget		WRITE	setVisualizationWidget)
	Q_PROPERTY (bool					signalWhenValueChanged	READ	getSignalWhenValueChanged	WRITE	setSignalWhenValueChanged)
	Q_PROPERTY (bool					roundByStepValue		READ	getRoundByStepValue			WRITE	setRoundByStepValue)

	Q_ENUMS(FamilyUnit)
public:

	/**
	* @enum QsSimpleUnit::FamilyUnit
	* This represents the family unit used.
	*/
	enum FamilyUnit 
	{ 
		AltitudeFamily = 0,				/** Altitude family */
		AngleFamily ,					/** Angle family */
		RangeFamily ,					/** Range family */
		SpeedFamily	,					/** Speed family */
		TimeFamily	,					/** Time family */
		FrequencyFamily					/** Frequency family */
	};

	/**
	* @brief	: Constructor
	* @param	: parent's widget
	* @param	: signalWhenValueChanged - set value when valueChanged or when value is finished
	*/
	QsSimpleUnit(QWidget *parent = 0, bool signalWhenValueChanged = false);

	/**
	* @brief	: Destructor
	*/
	virtual ~QsSimpleUnit();

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------
	/**
	* @brief	: operator's family unit modification
	* @param	: familyUnit - property QT's widget
	*/
	void setFamilyUnit(FamilyUnit familyUnit);
	FamilyUnit getFamilyUnit() const;

	/**
	* @brief	: visualization or modification widget
	* @param	: visualization - property QT's widget mode
	*/
	void setVisualizationWidget(bool visualization);
	bool isVisualizationWidget() const;


	bool getSignalWhenValueChanged();
	void setSignalWhenValueChanged(bool setValueOnChange);

	bool getRoundByStepValue();
	void setRoundByStepValue(bool value);

	//------------------------------------------------------------------
	// Properties (Internal)
	//------------------------------------------------------------------

	/**
	* @brief	: show or hide change button unit (use for converter's widget)
	* @param	: showButton - boolean
	*/
	void showButtonChangeUnit(bool hide);
	bool isButtonChangeUnitVisible() const;

	/**
	* @brief	: allow or forgive unit modification (use for unit service SI)
	* @param	: showButton - boolean
	*/
	void setChangeUnitEnabled(bool enabledChangeUnit);
	bool isChangeUnitEnabled() const {return _isChangeUnitEnabled;}

	//------------------------------------------------------------------
	// Service & unit
	//------------------------------------------------------------------

	/**
	* @brief	: set the service to register the widget
	* @param	: serviceUnitSI - ISwServiceUnitSI * 
	*/
	void setServiceUnitSI(IServiceUnitSI * serviceUnitSI);

	/**
	* @brief	: set the family unit and unit from Sw
	* @param	: unitFamilySI - SwSiUnitFamily, unitCoordinate - SiEnums::SiUnit
	*/
	void setSIFamilyUnit(SiEnums::SiUnitFamily unitFamilySI, SiEnums::SiUnit unitSI);
	void setSIUnit(SiEnums::SiUnit unitSI);

	//------------------------------------------------------------------
	// Interface ISwUnitSI
	//------------------------------------------------------------------

	/**
	* @brief	: Notification unit changed
	* @param	: unit - the new unit 
	* @return	: void
		*/
	virtual void processUnitChanged(SiEnums::SiUnit unit);
						
	/**
	* @brief	: Notification precision changed
	* @param	: unit - the new precision
	* @return	: void
	*/
	virtual void processPrecisionChanged(int precision);


	/*****************************************************************/
	/*						ACCESSORS
	/*****************************************************************/

	/**
	* @brief	: return the starlinx default unit value in the current family
	* @return	: value to starlinx default unit
	*/
	double getValue();

	/**
	* @brief	: set value starlinx default unit value in the current family
	* @param	: value to starlinx default unit 
	*/
	void setValue(double value);

	/**
	* @brief	: return the minimum starlinx default unit value in the current family
	* @return	: minimum value to starlinx default unit
	*/
	double getMinimumValueOperator();

	/**
	* @brief	: set minimum starlinx default (or unit operator) unit value in the current family
	* @param	: minimum value to starlinx default (or unit operator) unit 
	*/
	void setMinimumValueOperator(double value);
			
	/**
	* @brief	: set minimum starlinx in unit value (can be different on default unit starlinx) in the current family
	* @param	: minimum value to starlinx in unit 
	* @param	: unit's value application
	*/
	void setMinimumValueOperatorInUnit(double value, SiEnums::SiUnit unit);

	/**
	* @brief	: return the maximum starlinx default (or unit operator) unit value in the current family
	* @return	: maximum value to starlinx default (or unit operator) unit
	*/
	double getMaximumValueOperator();

	/**
	* @brief	: set maximum starlinx default unit (or unit operator) value in the current family
	* @param	: maximum value to starlinx default (or unit operator) unit 
	*/
	void setMaximumValueOperator(double value);

	/**
	* @brief	: set maximum starlinx default in unit value (can be different on default unit starlinx) in the current family
	* @param	: maximum value to starlinx in unit 
	* @param	: unit's value application
	*/
	void setMaximumValueOperatorInUnit(double value, SiEnums::SiUnit unit);

	/**
	* @brief	: set the maximum value to defaut unit
	* @param	: valueOpe - double
	*/
	void setMaximumValueOperatorToDefaultUnit(double valueOpe);

	/**
	* @brief	: set the minimum value to defaut unit
	* @param	: valueOpe - double
	*/
	void setMinimumValueOperatorToDefaultUnit(double valueOpe);

	/**
	* @brief	: return the single step value starlinx default unit in the current family
	* @return	: single step value to starlinx default unit
	*/
	double getSingleStepOperator();

	/**
	* @brief	: set  the single step value starlinx default unit in the current family
	* @param	: single step value to starlinx default unit
	* @param	: precisionStep determines precision number of the value
	*/
	void setSingleStepOperator(double value);

	/**
	* @brief	: set  the single step value starlinx in unit value (can be different on default unit starlinx) in the current family
	* @param	: single step value to starlinx in unit 
	* @param	: unit's value application
	*/
	void setSingleStepOperatorInUnit(double value, SiEnums::SiUnit unit);

	/**
	* @brief	: set unit used to updated minimum, maximum, single step and set setValue/getValue 
	* @param	: unit's operator
	*/
	void setUnitOperator(SiEnums::SiUnit unit){_unitOperator = unit;}

	/**
	* @brief	: return unit used to updated minimum, maximum, single step and set setValue/getValue 
	* @return	: unit's operator
	*/
	SiEnums::SiUnit getUnitOperator()const{return _unitOperator;}

	/**
	* @brief	: set precision to the default unit (this will be changed if processPrecision or processUnit are called)
	* @param	: precision
	*/
	void setPrecision(int precision);
			

	/**
	* @brief	: calculate minimum value for unit beetween system or operator value (if present)
	* @param	: unit - the unit 
	*/
	double getMinimumValueSystemOperator(SiEnums::SiUnit unit);

	/**
	* @brief	: calculate maximum value for unit beetween system or operator value (if present)
	* @param	: unit - the unit 
	*/
	double getMaximumValueSystemOperator(SiEnums::SiUnit unit);
			
	/**
	* @brief	: calculate step value for unit
	* @param	: unit - the unit 
	*/
	double getSingleStepSystemOperator(SiEnums::SiUnit unit);

	/**
	* @brief	: set the single step value to defaut unit
	* @param	: valueOpe - double
	*/
	void setSingleStepOperatorToDefaultUnit(double valueOpe);

	/**
	* @brief	: calculate step value for default unit
	*/
	double getSingleStepToDefaultUnit();

	SiEnums::SiUnit getCurrentUnit(){return _currentUnit;}
	int getCurrentPrecision(){return _precision;}

	void setNoStaValue(int value);
	int getNoStaValue();
			
signals:
			
	/**
	* @brief	: signal emitted when enum family unit changed
	* @param	: QT's property
	*/
	void familyChanged();

	/**
	* @brief	: signal emitted when enum unit changed
	*/
	void unitChanged();

	/**
	* @brief	: Signal emitted when value changed
	*/
	void valueSimpleChanged();

	void mouseEnter();
	void mouseLeave();

public slots :
			
	/**
	* @brief	: slot called when value changed in the double spinbox
	*/
	void valueSetChanged();

protected slots:

	/**
	* @brief	: Slot called when push button change unit is clicked
	*/
	void changeUnit();
	/**
	* @brief	: Slot called when change unit action is triggered
	*/
	void changeUnitTriggered(QAction * action);

	void onRoundTimer();

protected:
			
	/**
	* @brief	: event right click on push button change unit
	*/
	bool eventFilter(QObject * obj, QEvent *event);

	/**
	* @brief	: update menu contextual (depend of current family unit)
	*/
	void setMenuContext();	

	void setValueFromDefaultToCurrentUnit();	

	IServiceUnitSI *								_serviceUnitSI;
	SiEnums::SiUnitFamily							_familyUnitSimple;
	SiEnums::SiUnit									_currentUnit;
	SiEnums::SiUnit									_unitOperator;
	int												_precision;

	Ui::QsSimpleUnitWidgetClass *					_ui;

	QActionGroup	*								_alignmentGroup; 
	QMenu *											_menu;
	QHash<QAction *, SiEnums::SiUnit>	_listAction;
	QsDoubleSpinBoxSI *								_qsDoubleSpinBoxSI;

	bool											_isChangeUnitEnabled;
	bool											_isVisualizationWidget;
	bool											_isMinimumValueSet;
	bool											_isMaximumValueSet;
	bool											_isSingleStepValueSet;
	bool											_isSignalEmittedOnValueChanged;

	double											_minValueOperator;
	double											_maxValueOperator;
	double											_singleStepOperator;

	double											_valueDefaultUnit;
	int												_noStaValue;
	bool											_roundByStepValue;
	QTimer											_roundTimer;
};

#endif //QS_WIDGETS_QSSIMPLEUNIT_H