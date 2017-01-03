#ifndef QS_WIDGETS_QSCOORDINATELINE_H
#define QS_WIDGETS_QSCOORDINATELINE_H

#include <QLineEdit>
#include <QsWidgets_Global.h>

#include <QString>
#include <QsLineEdit.h>

#include <QProxyStyle>

class LineEditStyle : public QProxyStyle
{
	public:
		LineEditStyle(QStyle *style = 0) : QProxyStyle(style) { }

		int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;
};

class QSWIDGETS_EXPORT QsCoordinateLine : public QsLineEdit
{
	Q_OBJECT

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------
	Q_PROPERTY (UnitCoordinateLine unitCoordinateLine READ getUnitCoordinateLine WRITE setUnitCoordinateLine)
	Q_ENUMS(UnitCoordinateLine)

public:

	/**
	* @enum QsCoordinateLine::UnitCoordinateLine
	* This represents the line's type defining the validator
	* and the input mask
	*/
	enum UnitCoordinateLine 
	{ 
		decimal_Lat = 0,				/** Latitude Decimal degrees */
		decimal_Lon,					/** Longitude Decimal degrees */
		decimal_Minuts_Lat,				/** Latitude Decimal minute degrees */
		decimal_Minuts_Lon,				/** Longitude Decimal minute degrees */
		sexagesimal_Lat,				/** Latitude Sexagesimal degrees */
		sexagesimal_Lon,				/** Longitude Sexagesimal degrees */
		mgrs,							/** Metric mgrs */
		utm,							/** Metric utm */
		georef							/** GEOREF (1/100 minute precision)*/
	};		

	/**
	* @enum QsCoordinateLine::ModificationMode
	* This represents the modification's action
	*/
	enum ModificationMode 
	{ 
		normalMode = 0,			/** normal mode  */
		writeMode,				/** write character(s) */
		resetMode,				/** reset all character(s) */
		deleteMode,				/** delete character(s) (suppr key) */
		removeMode,				/** remove character(s) (backspace key) */
		pasteMode,				/** paste mode (Ctr+V or paste) */
		changeUnitMode,			/** change unit mode */
		changeCharacMode		/** character modification mode (key up or down) */
	};

	/**
	* @brief	: Constructor
	*/
	QsCoordinateLine(QWidget *parent = 0);
						
	/**
	* @brief	: Destructor
	*/
	virtual ~QsCoordinateLine();

	//------------------------------------------------------------------
	// Properties QT
	//------------------------------------------------------------------

	/**
	* @brief operator's unit modification
	* @param unitCoordinateLine - property QT's widget
	*/
	void setUnitCoordinateLine(UnitCoordinateLine unitCoordinateLine);
	UnitCoordinateLine getUnitCoordinateLine() const;

	//------------------------------------------------------------------
	// Properties (Internal)
	//------------------------------------------------------------------
	/**
	* @brief operator's action
	* @param modificationMode - action
	*/
	void setModificationMode(ModificationMode modificationMode);
	ModificationMode getModificationMode() const;

	/**
	* @brief set and get current value
	* @param coordinate
	*/
	QString getCoordinate(){return text();}
	void setCoordinate(QString coordinate);

	//------------------------------------------------------------------
	// Event QT
	//------------------------------------------------------------------

	/**
	* @brief key press event handler
	* @param : QKeyEvent * - event to handle
	*/
	void keyPressEvent (QKeyEvent* event);

	/**
	* @brief simple click event
	* @param event : QMouseEvent *  - event
	*/
	void mousePressEvent (QMouseEvent * event);

	/**
	* @brief double click event
	* @param event : QMouseEvent *  - event  
	*/
	void mouseDoubleClickEvent(QMouseEvent * event);

	/**
	* @brief Drag management
	*		 from click's pointed digit to mouse release pointed digit
	* @param event : QMouseEvent * - event 
	*/
	void mouseMoveEvent (QMouseEvent * event);

	/**
	* @brief	Enter event		
	* @param : QEvent * - event 
	*/
	void enterEvent (QEvent * event);

	/**
	* @brief	focus in event, gets text's font attributs to set the cursor's shape
	* @param event : QFocusEvent * - event 
	*/
	void focusInEvent (QFocusEvent * event);

signals :
	/**
	* @brief reset value in the unit 
	*/
	void resetValue();

	/**
	* @brief signal emitted when coordinate changed (cf. QsPosition)
	*/
	void coordinateChanged();

public slots:			
	/**
	* @brief slot called when key ctr+C are clicked 
	*/
	void  paste (); 

private:
	/**
	* @brief return if all text is selected by the mouse
	*/
	bool isAllTextSelected();

protected:

	int											_initDrag;
	QString										_validationString;
	UnitCoordinateLine							_unitCoordinateLine;
	ModificationMode							_modificationMode;
	QStringList									_listCharacAvailable;

	QString										_textFormat;
	QString										_textInputMask;

	QValidator *								_currentValidator;
	QStringList									_maskCarac;

	QRegExp										_DD_Lat_RegExp; 
	QRegExp										_DD_Long_RegExp; 
	QRegExp										_DM_Lat_RegExp;  
	QRegExp										_DM_Long_RegExp; 
	QRegExp										_DMS_Lat_RegExp;
	QRegExp										_DMS_Long_RegExp;

	QRegExp										_MGRSRegExp;
	QRegExp										_UTMRegExp;
	QRegExp										_GEOREFRegExp;
};

#endif
