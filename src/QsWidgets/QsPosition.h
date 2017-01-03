#ifndef QS_WIDGETS_QSPOSITION_H
#define QS_WIDGETS_QSPOSITION_H

#include <QGridLayout>
#include <QsWidgets_Global.h>
#include <IServiceUnitSI.h>
#include <SiEnums.h>

#include <QsWidget.h>
#include <QMessageBox>
#include <QActionGroup>
#include <QMenu>

namespace Ui{class QsPositionWidgetClass;}

//////////////////////////////////////////////////////////////////////
//////////////////  COORDINATES FORMAT (RegExp)  ////////////////////

///////////////////////////  DD  ////////////////////////////////////
//LATITUDE	: Entre 0 et 89.999999 (6 chiffres décimaux optionnels)
//			  "^([0-8][0-9]|)\\.{1}[0-9]{1,6} [NS]$"
//
//LONGITUDE 	: Entre 0 et 179.999999 (6 chiffres décimaux optionnels)
//			  "^?(^{0,1}[0-1]{0,1}[0-7]{0,1}[0-9]|[1-9]?[0-9])\\.{1}[0-9]{1,6} [EW]$"
/////////////////////////////////////////////////////////////////////

//////////////////////////  DM  /////////////////////////////////////
//LATITUDE	: Degré entre 0 et 89, Minute entre 0 et 59.9999 (4 chiffres décimaux optionnels)
//			  ^([0-8][0-9]|)°([0-8][0-9]|)\\.{1}[0-9]{1,4}' [NS]$
//
//LONGITUDE 	: Degré entre 0 et 179, Minute entre 0 et 59.9999 (4 chiffres décimaux optionnels)
//			  "^?(^{0,1}[0-1]{0,1}[0-7]{0,1}[0-9]|[1-9]?[0-9])°([0-8][0-9]|)\\.{1}[0-9]{1,4}' [EW]$"
/////////////////////////////////////////////////////////////////////

///////////////////////////  DMS  ////////////////////////////////////
//LATITUDE	: Degré entre 0 et 89, Minute entre 0 et 59, Seconde entre 0 et 59.99 (2 chiffres décimaux optionnels)
//			  ^([0-8][0-9]|)°([0-5][0-9]|)'([0-5][0-9]|)\\.{1}[0-9]{1,2}'' [NS]$
//
//LONGITUDE 	: Degré entre 0 et 179, Minute entre 0 et 59, Seconde entre 0 et 59.99 (2 chiffres décimaux optionnels)
//			  ^?(^{0,1}[0-1]{0,1}[0-7]{0,1}[0-9]|[1-9]?[0-9])°([0-5][0-9]|)'([0-5][0-9]|)\\.{1}[0-9]{1,2}'' [EW]$
/////////////////////////////////////////////////////////////////////

///////////////////////////  MGRS  ////////////////////////////////////
//			  Format 00X XX 00000 00000
//			  "[0-9][0-9][A-H,J-N,P-Z,a-h,j-n,p-z] [A-Z,a-z][A-Z,a-z] [0-9][0-9][0-9][0-9][0-9] [0-9][0-9][0-9][0-9][0-9]"
/////////////////////////////////////////////////////////////////////

///////////////////////////  GEOREF  ////////////////////////////////////
//			  Format XXXX 00000 00000
//			  "[A-H,J-N,P-Z,a-h,j-n,p-z]{1,4} [0-9][0-9][0-9][0-9][0-9] [0-9][0-9][0-9][0-9][0-9]"
/////////////////////////////////////////////////////////////////////

///////////////////////////  UTM  ////////////////////////////////////
//			  Format 00X 000000 000000 (a voir Band letter)
//			  "^[0-9]{2}[NS] [0-9]{6} [0-9]{7}$"
/////////////////////////////////////////////////////////////////////



/** @class	QsPosition
	*	@brief	QsPosition widget.
	*			Contains latitude and longitude coordinates
	*			widget is modulates through special properties
	*	@see enum CoordinatesFamily
	*/
class QSWIDGETS_EXPORT QsPosition : public QsWidget, UnitSIListener
{
	Q_OBJECT

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------
	Q_PROPERTY (UnitCoordinate unitCoordinate READ getUnitCoordinate WRITE setUnitCoordinate)
	Q_PROPERTY (bool visualization READ isVisualizationWidget WRITE setVisualizationWidget)
	Q_PROPERTY (bool vertical READ isAlignmentVertical WRITE setAlignmentVertical)

	Q_ENUMS(UnitCoordinate)
public:

	/**
	* @enum QsPosition::UnitCoordinate
	* This represents the unit used.
	*/
	enum UnitCoordinate 
	{ 
		decimal_LatLon = 0,				/** Decimal degrees */
		decimal_Minuts_Lat_Lon = 1,		/** Decimal minute degrees */
		sexagesimal_LatLon = 2 ,		/** Sexagesimal degrees */
		mgrs = 3,						/** Metric mgrs */
		utm = 4,						/** Metric utm */
		georef = 5						/** GEOREF (1/100 minute precision)*/
	};

	/**
	* @brief	: Constructor
	*/
	QsPosition(QWidget *parent = 0);
						
	/**
	* @brief	: Destructor
	*/
	virtual ~QsPosition();

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------
	/**
	* @brief	: operator's unit modification
	* @param	: unitCoordinate - property QT's widget
	*/
	UnitCoordinate getUnitCoordinate() const;
	void setUnitCoordinate(UnitCoordinate unitCoordinate);

	/**
	* @brief	: visualization or modification widget
	* @param	: visualization - property QT's widget mode
	*/
	bool isVisualizationWidget() const;
	void setVisualizationWidget(bool visualization);

	/**
	* @brief	: vertical or horizontal alignment widget
	* @param	: vertical - property QT's widget alignment
	*/
	bool isAlignmentVertical() const;
	void setAlignmentVertical(bool vertical);

	//------------------------------------------------------------------
	// Properties (Internal)
	//------------------------------------------------------------------
			
	/**
	* @brief	: show or hide change button unit (use for converter's widget)
	* @param	: showButton - boolean
	*/
	void showButtonChangeUnit(bool showButton);
	bool isButtonChangeUnitVisible() const;

	/**
	* @brief	: allow or forgive unit modification (use for unit service SI)
	* @param	: showButton - boolean
	*/
	void setChangeUnitEnabled(bool enabledChangeUnit) {_isChangeUnitEnabled = enabledChangeUnit;}
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
	* @brief	: set the unit from Sw
	* @param	: unitCoordinate - SiUnit
	*/
	void setSIUnitCoordinate(SiEnums::SiUnit unitCoordinate);

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
	* @brief	: return the current latitude coordinate
	* @return	: Latitude in starlinx default unit
	*/
	QString getLatitude();
				
	/**
	* @brief	: return the current longitude coordinate
	* @return	: longitude in starlinx default unit
	*/
	QString getLongitude();

	/**
	* @brief	: return the current latitude coordinate in Degree decimal (with sign)
	* @return	: Latitude in Degree decimal (with sign)
	*/
	double getLatitudeDD_Sign();

	/**
	* @brief	: return the current longitude coordinate in Degree decimal (with sign)
	* @return	: Longitude in Degree decimal (with sign)
	*/
	double getLongitudeDD_Sign();

	/**
	* @brief	: set coordinates (latitude and longitude)
	* @param	: latitude in starlinx default unit
	* @param	: longitude in starlinx default unit
	* @param	: signalModification - emit signal changedCoordinateSI or not
	*/
	void setCoordinates(QString latitude, QString longitude, bool signalModification = false);

	/**
	* @brief	: set coordinates (latitude)
	* @param	: latitude in starlinx default unit
	*/
	void setLatitude(double latitude);

	/**
	* @brief	: set coordinates (longitude)
	* @param	: longitude in starlinx default unit
	*/
	void setLongitude(double longitude);

	SiEnums::SiUnit getCurrentUnit(){ return _currentCoordinateUnit; }

	//Event for configuration
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

protected slots:

	/**
	* @brief	: Slot called when push button change unit is clicked
	*/
	void changeUnit();

	/**
	* @brief	: Slot called when change unit action is triggered
	*/
	void changeUnitTriggered(QAction * action);

	/**
	* @brief	: Slot called when reset value is required in QsCoordinateLine
	*/
	void resetCoordinate();

	/**
	* @brief	: Slot called when push button warning is clicked
	*/
	void showWarning();

	/**
	* @brief	: Slot called when coordinate value changed
	*/
	void changedCoordinate();

signals:
	/**
	* @brief	: Signal emitted when coordinates changed
	*/
	void changedCoordinateSI();

	/**
	* @brief	: Signal emitted when mouse enter in the widget position
	*/
	void mouseEnter();

	/**
	* @brief	: Signal emitted when mouse leaves the widget position
	*/
	void mouseLeave();

	void unitChanged();


protected:

	/**
	* @brief	: set dimension's widget (depends of widget's alignment)
	*/
	void setSize();

	/**
	* @brief	: reset latitude and longitude
	*/
	void resetAllCoordinate();

	/**
	* @brief	: set correct title widget (depends of current unit and modification, visualization's mode)
	*/
	void setTitleCoordinate();
			
	/**
	* @brief	: event right click on push button change unit
	*/
	bool eventFilter(QObject * obj, QEvent *event);

	IServiceUnitSI *							_serviceUnitSI;
	SiEnums::SiUnitFamily						_familyCoordinate;
	SiEnums::SiUnit								_currentCoordinateUnit;

	Ui::QsPositionWidgetClass *					_ui;
	QActionGroup	*							_alignmentGroup; 

	bool										_isButtonClicked;
	bool										_isVisualizationWidget;
	bool										_isChangeUnitEnabled;
	bool										_isVertical;

	QMenu *										_menu;
	QHash<QAction *, SiEnums::SiUnit>			_listAction;
	QString										_messageError;

	QString										_latitudeTemp;
	QString										_longitudeTemp;

	QString										_latitudeDefaultUnit;
	QString										_longitudeDefaultUnit;
};

#endif