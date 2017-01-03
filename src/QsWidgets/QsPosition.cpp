#include <QtGui>

#include "ui_QsPosition.h"
#include "ISwServiceUnitSI.h"

#include "QsPosition.h"
#include "QsCoordinateLine.h"
#include <QVBoxLayout>
#include <SwApplication.h>


using namespace StreamWork::SwCore;


//-----------------------------------------------------------------------
QsPosition::QsPosition(QWidget *parent) 
: QsWidget(parent),_serviceUnitSI(NULL), _latitudeTemp(), _longitudeTemp()
{
	_latitudeDefaultUnit = "0";
	_longitudeDefaultUnit = "0";

	_ui = new Ui::QsPositionWidgetClass();
	_ui->setupUi(this);
	_isButtonClicked = false;

	_familyCoordinate = SiEnums::SiUnitFamilyCoordinates;
	_currentCoordinateUnit = SiEnums::SiUnitCoordDegreeDecimal;

	setVisualizationWidget(false);
	setAlignmentVertical(false);
	setChangeUnitEnabled(true); 

	//Récupération du service unité système international

#ifdef _DEBUG
	QLibrary swCore("SwCored");
#else
	QLibrary swCore("SwCore");
#endif

	//récupération dynamique afin de ne pas linker QtDesigner avec la librairie Streamwork
	typedef StreamWork::SwCore::ISwService *(*SWCORE_SERVICE)(QString) ;
	SWCORE_SERVICE swQueryService = (SWCORE_SERVICE) swCore.resolve("queryService");

	if(swQueryService)
	{
		_serviceUnitSI = dynamic_cast <IServiceUnitSI *> (swQueryService(CG_SW_SERVICE_UNIT_SI));
		if(_serviceUnitSI)
			setServiceUnitSI(_serviceUnitSI);
	}
	
	setSIUnitCoordinate(_currentCoordinateUnit);

	connect(_ui->pbChangeUnit, SIGNAL(clicked(bool)), this , SLOT(changeUnit()));

	connect(_ui->latitude, SIGNAL(resetValue()), this , SLOT(resetCoordinate()));
	connect(_ui->longitude, SIGNAL(resetValue()), this , SLOT(resetCoordinate()));
	connect(_ui->latitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));
	connect(_ui->longitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));

	connect(_ui->pbWarning, SIGNAL(clicked(bool)), this , SLOT(showWarning()));

	//le bouton warning ne sera visible que si l'opérateur met une valeur incohérente dans les coordonnées
	_ui->pbWarning->setVisible(false);
}


//-----------------------------------------------------------------------
QsPosition::~QsPosition()
{
	if (_serviceUnitSI)
	{
		//Desenregistrement auprès du service unité système international pour le type Coordonnées
		_serviceUnitSI->unregisterTypeSI(SiEnums::SiUnitFamilyCoordinates, this);
		_serviceUnitSI = NULL;
	}
}

//-----------------------------------------------------------------------
QsPosition::UnitCoordinate QsPosition::getUnitCoordinate() const
{
	UnitCoordinate unitCoordinate = decimal_LatLon;

	switch(_currentCoordinateUnit)
	{
	case SiEnums::SiUnitCoordDegreeDecimal:
		unitCoordinate = decimal_LatLon ;
		break;
	case SiEnums::SiUnitCoordDegreeMinute:
		unitCoordinate = decimal_Minuts_Lat_Lon;
		break;
	case SiEnums::SiUnitCoordDegreeMinuteSecond:
		unitCoordinate = sexagesimal_LatLon ;
		break;
	case SiEnums::SiUnitCoordMGRS:
		unitCoordinate = mgrs ;
		break;
	case SiEnums::SiUnitCoordUTM:
		unitCoordinate = utm;
		break;
	case SiEnums::SiUnitCoordGEOREF:
		unitCoordinate = georef;
		break;
	}
	return unitCoordinate;
}

//-----------------------------------------------------------------------
void QsPosition::setUnitCoordinate(UnitCoordinate unitCoordinate)
{
	switch(unitCoordinate)
	{
	case decimal_LatLon:
		_currentCoordinateUnit = SiEnums::SiUnitCoordDegreeDecimal;
		break;
	case decimal_Minuts_Lat_Lon:
		_currentCoordinateUnit = SiEnums::SiUnitCoordDegreeMinute;
		break;
	case sexagesimal_LatLon:
		_currentCoordinateUnit = SiEnums::SiUnitCoordDegreeMinuteSecond;
		break;
	case mgrs:
		_currentCoordinateUnit = SiEnums::SiUnitCoordMGRS;
		break;
	case utm:
		_currentCoordinateUnit = SiEnums::SiUnitCoordUTM;
		break;
	case georef:
		_currentCoordinateUnit = SiEnums::SiUnitCoordGEOREF;
		break;
	}
	setSIUnitCoordinate(_currentCoordinateUnit);
}

//-----------------------------------------------------------------------
void QsPosition::setSIUnitCoordinate(SiEnums::SiUnit unitCoordinate)
{
	_currentCoordinateUnit = unitCoordinate;

	//update action contextuel
	QHashIterator<QAction *, SiEnums::SiUnit> i(_listAction);
	while (i.hasNext()) 
	{
		i.next();
		if (i.value() == _currentCoordinateUnit)
			i.key()->setChecked(true);
	}

	if (!isVisualizationWidget())
	{
		switch(_currentCoordinateUnit)
		{
		case SiEnums::SiUnitCoordDegreeDecimal:
			{
				_ui->frameLongitude->show();
				_ui->frameLatitude->setMinimumWidth(90);
				_ui->frameLatitude->setMaximumWidth(90);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::decimal_Lat);
				_ui->longitude->setUnitCoordinateLine(QsCoordinateLine::decimal_Lon);
				_ui->frameWarning->hide();
			}
			break;

		case SiEnums::SiUnitCoordDegreeMinute:
			{
				_ui->frameLongitude->show();
				_ui->frameLatitude->setMinimumWidth(90);
				_ui->frameLatitude->setMaximumWidth(90);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::decimal_Minuts_Lat);
				_ui->longitude->setUnitCoordinateLine(QsCoordinateLine::decimal_Minuts_Lon);
				_ui->frameWarning->hide();
			}
			break;

		case SiEnums::SiUnitCoordDegreeMinuteSecond:
			{
				_ui->frameLongitude->show();
				_ui->frameLatitude->setMinimumWidth(90);
				_ui->frameLatitude->setMaximumWidth(90);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::sexagesimal_Lat);
				_ui->longitude->setUnitCoordinateLine(QsCoordinateLine::sexagesimal_Lon);
				_ui->frameWarning->hide();
			}
			break;

		case SiEnums::SiUnitCoordMGRS:
			{
				_ui->frameLongitude->hide();
				_ui->frameLatitude->setMinimumWidth(118);
				_ui->frameLatitude->setMaximumWidth(118);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::mgrs);
				_ui->frameWarning->show();
			}
			break;

		case SiEnums::SiUnitCoordUTM:
			{
				_ui->frameLongitude->hide();
				_ui->frameLatitude->setMinimumWidth(118);
				_ui->frameLatitude->setMaximumWidth(118);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::utm);
				_ui->frameWarning->show();
			}
			break;
		case SiEnums::SiUnitCoordGEOREF:
			{
				_ui->frameLongitude->hide();
				_ui->frameLatitude->setMinimumWidth(100);
				_ui->frameLatitude->setMaximumWidth(100);
				_ui->latitude->setUnitCoordinateLine(QsCoordinateLine::georef);
				_ui->frameWarning->show();
			}
			break;

		default :;
		}
	}
	else
	{
		//mode visualization
		switch(_currentCoordinateUnit)
		{
		case SiEnums::SiUnitCoordMGRS:
			{
				_ui->labelLatitudeCoord->setMinimumWidth(32);
				_ui->labelLatitudeCoord->setMaximumWidth(32);
				_ui->latitudeCoord->setMinimumWidth(116);
				_ui->latitudeCoord->setMaximumWidth(116);
			}
			break;

		case SiEnums::SiUnitCoordUTM:
			{
				_ui->labelLatitudeCoord->setMinimumWidth(26);
				_ui->labelLatitudeCoord->setMaximumWidth(26);
				_ui->latitudeCoord->setMinimumWidth(114);
				_ui->latitudeCoord->setMaximumWidth(114);
			}
			break;
		case SiEnums::SiUnitCoordGEOREF:
			{
				_ui->labelLatitudeCoord->setMinimumWidth(40);
				_ui->labelLatitudeCoord->setMaximumWidth(40);
				_ui->latitudeCoord->setMinimumWidth(94);
				_ui->latitudeCoord->setMaximumWidth(94);
			}
			break;
		default:
			{
				_ui->labelLatitudeCoord->setMinimumWidth(20);
				_ui->labelLatitudeCoord->setMaximumWidth(20);
				_ui->latitudeCoord->setMinimumWidth(80);
				_ui->latitudeCoord->setMaximumWidth(80);
				_ui->labelLongitudeCoord->setMinimumWidth(20);
				_ui->labelLongitudeCoord->setMaximumWidth(20);
				_ui->longitudeCoord->setMinimumWidth(80);
				_ui->longitudeCoord->setMaximumWidth(80);
			}
		}
	}
	emit unitChanged();
	setTitleCoordinate();
}

//-----------------------------------------------------------------------
void QsPosition::setTitleCoordinate()
{
	if (_serviceUnitSI)
	{
		_ui->labelLatitude->clear();
		_ui->labelLongitude->clear();
		_ui->labelLatitudeCoord->clear();
		_ui->labelLongitudeCoord->clear();

		QString suffixUnit = _serviceUnitSI->getSuffix(_currentCoordinateUnit);
		if (_currentCoordinateUnit != SiEnums::SiUnitCoordDegreeDecimal
			&&	_currentCoordinateUnit != SiEnums::SiUnitCoordDegreeMinute
			&&	_currentCoordinateUnit != SiEnums::SiUnitCoordDegreeMinuteSecond)
		{
			//only label latitude is visible in for this coordinate 
			//modification mode 
			_ui->labelLatitude->setText(suffixUnit);

			//visualisation mode 
			_ui->labelLatitudeCoord->setText(suffixUnit + " :");
		}
		else
		{
			//For latitude and longitude coordinates

			//modification mode 
			_ui->labelLatitude->setText("Lat (" + suffixUnit +")");
			_ui->labelLongitude->setText("Lon (" + suffixUnit +")");

			//visualisation mode 
			_ui->labelLatitudeCoord->setText("Lat  :");
			_ui->labelLongitudeCoord->setText("Lon :");
		}
	}
}

//-----------------------------------------------------------------------
void QsPosition::setVisualizationWidget(bool visualization)
{
	_isVisualizationWidget = visualization;

	//change unit button only visible in modification mode
	_ui->pbChangeUnit->setVisible(!_isVisualizationWidget);


	//if visualisation mode activated, change widget by stackedWidget
	if (visualization)
		_ui->stackedWidget->setCurrentIndex(1);
	else
		_ui->stackedWidget->setCurrentIndex(0);

	//for updating visu mode
	setSIUnitCoordinate(_currentCoordinateUnit);

	setSize();
}

//-----------------------------------------------------------------------
bool QsPosition::isVisualizationWidget() const
{
	return _isVisualizationWidget;
}

//-----------------------------------------------------------------------
void QsPosition::setAlignmentVertical(bool vertical)
{
	_isVertical = vertical;

	_ui->layoutCoordinate->setHorizontalSpacing(2);

	if (_isVertical)
	{
		//VERTICAL
		//visualization's mode
		_ui->layoutVisu->addWidget(_ui->frameLatitudeCoord, 0, 0, 1, 1);
		_ui->layoutVisu->addWidget(_ui->frameLongitudeCoord, 1, 0, 1, 1);

		//modification's mode
		_ui->layoutCoordinate->addWidget(_ui->frameChangeUnit, 0, 0, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameLatitude, 0, 1, 1, 1);
		QFrame *fr_helpAlign = new QFrame(this);
		fr_helpAlign->setMinimumSize(20,40);
		fr_helpAlign->setMaximumSize(20,40);
		fr_helpAlign->setObjectName("tutu");
		_ui->layoutCoordinate->addWidget(fr_helpAlign, 1, 0, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameLongitude, 1, 1, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameWarning, 0, 2, 1, 1);
		_ui->layoutCoordinate->setRowStretch(0,1);

		_ui->layoutCoordinate->setVerticalSpacing(3);

		// Enlever le décalage
		_ui->frameLongitude->layout()->setContentsMargins(0, 0, 0, 0);
		_ui->frameLongitude->setMinimumWidth(90);
		_ui->frameLongitude->setMaximumWidth(90);

	}
	else
	{
		//HORIZONTAL
		//visualization's mode
		_ui->layoutVisu->addWidget(_ui->frameLatitudeCoord, 0, 0, 1, 1);
		_ui->layoutVisu->addWidget(_ui->frameLongitudeCoord, 0, 1, 1, 1);

		//modification's mode
		_ui->layoutCoordinate->addWidget(_ui->frameChangeUnit, 0, 0, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameLatitude, 0, 1, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameLongitude, 0, 2, 1, 1);
		_ui->layoutCoordinate->addWidget(_ui->frameWarning, 0, 3, 1, 1);		
		_ui->layoutCoordinate->setVerticalSpacing(0);

		// Ajouter un décalage
		_ui->frameLongitude->layout()->setContentsMargins(4, 0, 0, 0);
		_ui->frameLongitude->setMinimumWidth(94);
		_ui->frameLongitude->setMaximumWidth(94);
	}

	setSize();
}

//-----------------------------------------------------------------------
bool QsPosition::isAlignmentVertical() const
{
	return _isVertical;
}

//-----------------------------------------------------------------------
void QsPosition::setSize()
{
	if (isVisualizationWidget())
	{
		//visualization's mode
		if (isAlignmentVertical())
		{
			//vertical
			setMinimumHeight(40);
			setMaximumHeight(40);
			setMinimumWidth(150);
			setMaximumWidth(150);

		}
		else
		{
			//horizontal
			setMinimumHeight(20);
			setMaximumHeight(20);
			setMinimumWidth(208);
			setMaximumWidth(208);
		}
	}
	else
	{
		//modification's mode
		if (isAlignmentVertical())
		{
			//vertical
			setMinimumHeight(72);
			setMaximumHeight(72);
			setMinimumWidth(162);
			setMaximumWidth(162);

		}
		else
		{
			//horizontal
			setMinimumHeight(34);
			setMaximumHeight(34);
			setMinimumWidth(214);
			setMaximumWidth(214);
		}
	}
}

//-------------------------------------------------------------------------
QString QsPosition::getLatitude()
{
	return _latitudeDefaultUnit;
}

//-------------------------------------------------------------------------
QString QsPosition::getLongitude()
{
	return _longitudeDefaultUnit;
}

//-------------------------------------------------------------------------
double QsPosition::getLatitudeDD_Sign()
{
	double valueLat = 0;

	QString valueLatitude = _latitudeDefaultUnit;

	if ( valueLatitude.contains('N') || valueLatitude.contains('S') )
	{
		if ( valueLatitude.contains('S'))
			valueLat = -1;
		else
			valueLat = 1;

		//remove character and space to have value
		valueLatitude.remove('N');
		valueLatitude.remove('S');
		valueLatitude.remove(' ');

		valueLat *= valueLatitude.toDouble();
	}
	return valueLat;
}

//-------------------------------------------------------------------------
double QsPosition::getLongitudeDD_Sign()
{
	double valueLon = 0;

	QString valueLongitude = _longitudeDefaultUnit;
	if ( valueLongitude.contains('E') || valueLongitude.contains('W') )
	{
		if ( valueLongitude.contains('W'))
			valueLon = -1;
		else
			valueLon = 1;

		//remove character and space to have value
		valueLongitude.remove('E');
		valueLongitude.remove('W');
		valueLongitude.remove(' ');

		valueLon *= valueLongitude.toDouble();
	}
	return valueLon;
}

//-------------------------------------------------------------------------
void QsPosition::setCoordinates(QString latitude, QString longitude, bool signalModification)
{
	if (_serviceUnitSI)
	{
		//default Starlinx unit
		SiEnums::SiUnit unitSI = _serviceUnitSI->getDefaultUnit(_familyCoordinate);

		//check if value is convertible in default unit
		QMap<SiEnums::SiUnit, QPair<QString, QString>> mapConversionDefault;
		try
		{
			//conversion current value in degree decimal to check correct conversion
			_serviceUnitSI->convertComplexUnit(latitude, longitude, unitSI, unitSI, mapConversionDefault);
		}
		catch(...) 
		{
			//if error => incorrect conversion => incorrect value setted
			return;
		}

		QMap<SiEnums::SiUnit, QPair<QString, QString>>::const_iterator iter = mapConversionDefault.constBegin();
		if ((iter != mapConversionDefault.constEnd()) && !(iter.value().second.isEmpty()))
		{
			//value already setted
			if ((QString::compare(_latitudeDefaultUnit, iter.value().first) == 0)
				&&(QString::compare(_longitudeDefaultUnit, iter.value().second) == 0))
			{
				_latitudeTemp.clear();
				_longitudeTemp.clear();
				return;
			}

			_latitudeDefaultUnit	 = iter.value().first;
			_longitudeDefaultUnit	 = iter.value().second;
		}
		else
			return;

		//CONVERT DEFAULT UNIT VALUE TO CURRENT UNIT
		//return latitude value from default starlinx unit to current unit conversion
		QMap<SiEnums::SiUnit, QPair<QString, QString>> mapConversion;

		try
		{
			_serviceUnitSI->convertComplexUnit(latitude, longitude, unitSI,_currentCoordinateUnit, mapConversion);
		}
		catch ( std::runtime_error & )
		{
			return;
		}
		catch(SwException &)
		{
			return;
		}	

		QMap<SiEnums::SiUnit, QPair<QString, QString>>::const_iterator i = mapConversion.constBegin();
		if (i != mapConversion.constEnd())
		{

			if (isVisualizationWidget())
			{
				_ui->latitudeCoord->setText(i.value().first);
				_ui->longitudeCoord->setText(i.value().second);
			}
			else
			{
				_latitudeTemp.clear();
				_longitudeTemp.clear();

				_ui->latitude->setModificationMode(QsCoordinateLine::normalMode);
				_ui->longitude->setModificationMode(QsCoordinateLine::normalMode);

				if (!signalModification)
				{
					//blockSignals true
					_ui->latitude->blockSignals(true);
					_ui->longitude->blockSignals(true);
				}

				//update widget result
				_ui->latitude->setCoordinate(i.value().first);
				_ui->longitude->setCoordinate(i.value().second);

				if (!signalModification)
				{
					//blockSignals false
					_ui->latitude->blockSignals(false);
					_ui->longitude->blockSignals(false);
				}
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsPosition::setLatitude(double latitude)
{
	//set latitude to temp value waiting longitude value 
	//=> for Configuration Starlinx whose latitude and longitude are separated
	_latitudeTemp = QString::number(latitude, 'f', 5);

	if (!_longitudeTemp.isEmpty())
		setCoordinates(QString(_latitudeTemp), QString(_longitudeTemp));
}

//-------------------------------------------------------------------------
void QsPosition::setLongitude(double longitude)
{
	//set latitude to temp value waiting longitude value 
	//=> for Configuration Starlinx whose latitude and longitude are separated
	_longitudeTemp = QString::number(longitude, 'f', 5);

	if (!_latitudeTemp.isEmpty())
		setCoordinates(QString(_latitudeTemp), QString(_longitudeTemp));
}

//-------------------------------------------------------------------------
void QsPosition::showButtonChangeUnit(bool showButton)
{
	_ui->pbChangeUnit->setVisible(showButton);
	//_ui->frameWarning->setVisible(showButton);
	if (showButton)
		setMaximumWidth(270);
	else
		setMaximumWidth(200);
}

//-----------------------------------------------------------------------
bool QsPosition::isButtonChangeUnitVisible() const
{
	return _ui->pbChangeUnit->isVisible();
}

//-------------------------------------------------------------------------
void QsPosition::setServiceUnitSI(IServiceUnitSI * serviceUnitSI)
{
	_serviceUnitSI = serviceUnitSI; 

	if (_serviceUnitSI)
	{
		//Enregistrement auprès du service unité système international pour le type Coordonnées
		_serviceUnitSI->registerTypeSI(SiEnums::SiUnitFamilyCoordinates, this);
		setCoordinates(QString::number(0), QString::number(0));

		//reset label value coordinates
		_ui->latitudeCoord->setText("0");
		_ui->longitudeCoord->setText("0");

		SiEnums::SiUnit unitService = _serviceUnitSI->getCurrentUnit(SiEnums::SiUnitFamilyCoordinates);

		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(SiEnums::SiUnitFamilyCoordinates);

		if (listUnit.size() != 0)
		{
			//Push button change unit's action
			_menu = new QMenu(this);
			_menu->setObjectName("MenuUnitSI");
			_alignmentGroup = new QActionGroup(this);
			_alignmentGroup->setExclusive(true);

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

			//set le menu contextuel
			_ui->pbChangeUnit->installEventFilter(this);
		}

		processUnitChanged(unitService);
		setTitleCoordinate();
	}
}

//-------------------------------------------------------------------------
void QsPosition::processUnitChanged(SiEnums::SiUnit unit)
{
	//if new unit different of last unit and if unit's modification is available
	//=> conversion

	if (_serviceUnitSI && (unit != _currentCoordinateUnit)
		&& isChangeUnitEnabled())
	{
		SiEnums::SiUnit unitService = _serviceUnitSI->getDefaultUnit(SiEnums::SiUnitFamilyCoordinates);

		//reset warning => if error before, the last correct is converted (so no error)
		_ui->pbWarning->setVisible(false);
		_ui->pbChangeUnit->setEnabled(true);

		//convert default value to new unit
		QMap<SiEnums::SiUnit, QPair<QString, QString>> mapConversion;
		try
		{
			_serviceUnitSI->convertComplexUnit(_latitudeDefaultUnit, _longitudeDefaultUnit, unitService, unit, mapConversion);
		}
		catch ( std::runtime_error &e )
		{
			_ui->pbWarning->setVisible( true );
			_messageError = e.what();
			showWarning();
		}
		catch(SwException & e)
		{
			//if (isButtonChangeUnitVisible())
			_ui->pbWarning->setVisible(true);
			_messageError = e.GetReason();
			showWarning();
		}

		//conversion result
		QMap<SiEnums::SiUnit, QPair<QString, QString>>::const_iterator i = mapConversion.constBegin();
		if (i != mapConversion.constEnd()) 
		{
			if (isVisualizationWidget())
			{
				//visualization's mode
				setSIUnitCoordinate(unit);
				//set latitude result
				_ui->latitudeCoord->setText(i.value().first);

				//if longitude doesn't have value, hide this => coordinate where only latitude is visible
				if ((!isAlignmentVertical()) && (i.value().second.isEmpty()))
					_ui->frameLongitudeCoord->hide();
				else
				{
					//set longitude result
					_ui->frameLongitudeCoord->show();
					_ui->longitudeCoord->setText(i.value().second);
				}
			}
			else
			{
				//change unit button clicked by operator
				if (_isButtonClicked)
				{
					//only on the first coordinate
					_ui->latitude->setModificationMode(QsCoordinateLine::changeUnitMode);
					_ui->longitude->setModificationMode(QsCoordinateLine::normalMode);
					_isButtonClicked = false;
				}
				else
				{
					_ui->latitude->setModificationMode(QsCoordinateLine::normalMode);
					_ui->longitude->setModificationMode(QsCoordinateLine::normalMode);

				}

				//blockSignals true
				_ui->latitude->blockSignals(true);
				_ui->longitude->blockSignals(true);
				disconnect(_ui->latitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));
				disconnect(_ui->longitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));

				//Change unit and value
				setSIUnitCoordinate(unit);
				_ui->latitude->setCoordinate(i.value().first);
				_ui->longitude->setCoordinate(i.value().second);

				//blockSignals false
				_ui->latitude->blockSignals(false);
				_ui->longitude->blockSignals(false);


				connect(_ui->latitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));
				connect(_ui->longitude, SIGNAL(coordinateChanged()), this , SLOT(changedCoordinate()));
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsPosition::processPrecisionChanged(int precision)
{
	//NOTHING TO DO FOR COORDINATE
}

//-------------------------------------------------------------------------
void QsPosition::changeUnit()
{
	//push button change unit is clicked
	if (_serviceUnitSI)
	{
		SiEnums::SiUnit newUnit;
		QList<SiEnums::SiUnit> listUnit = _serviceUnitSI->getUnitListFamily(_familyCoordinate);

		int i = listUnit.indexOf(_currentCoordinateUnit);
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

			//operator's action
			_isButtonClicked = true;
			processUnitChanged(newUnit); 
		}
	}
}

//-------------------------------------------------------------------------
void QsPosition::changeUnitTriggered(QAction * action)
{
	//change unit's action is triggered
	if (action)
	{
		//find action triggered
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
bool QsPosition::eventFilter(QObject * obj, QEvent *event)
{	
	if (obj == _ui->pbChangeUnit && event->type() == QEvent::MouseButtonRelease)
	{
		//show menu contextual behind the push button change unit
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if(mouseEvent)
		{
			if(mouseEvent->button() == Qt::RightButton)
				_menu->exec(mouseEvent->globalPos());
		}
	}
	return QObject::eventFilter(obj,event);
}

//-------------------------------------------------------------------------
void QsPosition::changedCoordinate()
{
	//slot called when value coordinate changed
	if (_serviceUnitSI)
	{
		QString lat, lon;
		if (isVisualizationWidget())
		{
			//visualization's mode
			lat = _ui->latitudeCoord->text();
			lon = _ui->longitudeCoord->text();
		}
		else
		{
			//modification's mode
			lat = _ui->latitude->getCoordinate();
			lon = _ui->longitude->getCoordinate();
		}

		_ui->pbWarning->setVisible(false);
		_ui->pbChangeUnit->setEnabled(true);

		QMap<SiEnums::SiUnit, QPair<QString, QString>> mapConversion;
		try
		{
			//conversion current value in degree decimal to check correct conversion
			_serviceUnitSI->convertComplexUnit(lat, lon, _currentCoordinateUnit, SiEnums::SiUnitCoordDegreeDecimal, mapConversion);
		}
		catch ( std::runtime_error &e )
		{
			if ( isButtonChangeUnitVisible() )
			{
				//show button error
				_ui->pbChangeUnit->setEnabled( false );
			}
			_ui->pbWarning->setVisible( true );
			//if incorrect value, the current value is not saved
			_messageError = e.what();
			return;
		}
		catch(SwException & e) 
		{
			//if error => incorrect conversion => incorrect value setted
			if (isButtonChangeUnitVisible())
			{
				//show button error
				_ui->pbChangeUnit->setEnabled(false);
			}
			_ui->pbWarning->setVisible(true);
			//if incorrect value, the current value is not saved
			_messageError = e.GetReason();
			return;
		}

		QMap<SiEnums::SiUnit,QPair<QString, QString>>::const_iterator i = mapConversion.constBegin();
		if ((i != mapConversion.constEnd()) && !(i.value().second.isEmpty()))
		{
			_latitudeDefaultUnit = i.value().first;
			_longitudeDefaultUnit = i.value().second;
			emit changedCoordinateSI();
		}

		_latitudeTemp.clear();
		_longitudeTemp.clear();
	}
}

//-------------------------------------------------------------------------
void QsPosition::resetCoordinate()
{
	//operator order to reset latitude or longitude value 
	//RESET is not set letter at A or number at 0, but is to set the coordinates value to latitude and longitude at 0° (in Degree decimal)
	if (_serviceUnitSI)
	{
		QsCoordinateLine * cLine = dynamic_cast<QsCoordinateLine*> (sender());
		if (cLine)
		{
			QString firstValue, secondValue;	
			try
			{
				if (_serviceUnitSI->resetComplexUnit(_currentCoordinateUnit, firstValue, secondValue))
				{
					if (cLine == _ui->latitude)
						cLine->setCoordinate(firstValue);

					else if (cLine == _ui->longitude)
						cLine->setCoordinate(secondValue);
				}
			}
			catch ( std::runtime_error &e )
			{
				QMessageBox::warning( this, "Convertor error", QString( e.what() ) );
			}
			catch(SwException & e) 
			{
				QMessageBox::warning(this,"Convertor error",QString(e.GetReason()));
				return;	
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsPosition::resetAllCoordinate()
{
	//if error (push button warning visible) and operator pushed reset
	if (_serviceUnitSI)
	{
		//reset all value (latitude and longitude)
		QString firstValue, secondValue;	
		if (_serviceUnitSI->resetComplexUnit(_currentCoordinateUnit, firstValue, secondValue))
		{
			QString lat, lon;
			if (isVisualizationWidget())
			{
				//visualization's mode
				_ui->latitudeCoord->setText(firstValue);
				_ui->longitudeCoord->setText(secondValue);
			}
			else
			{
				//modification's mode
				_ui->latitude->setModificationMode(QsCoordinateLine::normalMode);
				_ui->longitude->setModificationMode(QsCoordinateLine::normalMode);

				_ui->latitude->setCoordinate(firstValue);
				_ui->longitude->setCoordinate(secondValue);
			}
		}
	}
}

//-------------------------------------------------------------------------
void QsPosition::showWarning()
{
	QMessageBox msgBoxError;
	msgBoxError.setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint );
	msgBoxError.setWindowTitle("Conversion error"); 
	msgBoxError.setText(_messageError + "\nDo you want to reset coordinates ?");
	msgBoxError.setStandardButtons(QMessageBox::Discard | QMessageBox::Ok); 
	msgBoxError.setDefaultButton(QMessageBox::Ok);
	if (msgBoxError.exec() == QMessageBox::Ok)
		resetAllCoordinate();
}

//-------------------------------------------------------------------------
void QsPosition::enterEvent(QEvent *eventEnter)
{
	emit mouseEnter();
	QWidget::enterEvent(eventEnter);
}

//-------------------------------------------------------------------------
void QsPosition::leaveEvent(QEvent *eventLeave)
{
	emit mouseLeave();
	QWidget::leaveEvent(eventLeave);
}
