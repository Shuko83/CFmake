
#include "QsWidgetsPlugin.h"
#include "QsWidgetInterface.h"
#include "QsListWidgetInterface.h"
#include "QsPushButtonInterface.h"  
#include "QsToolButtonInterface.h"   
#include "QsButgetInterface.h"
#include "QsLabelInterface.h"
#include "QsLabelTitleInterface.h"
#include "QsLineEditInterface.h"
#include "QsCheckBoxInterface.h"
#include "QsComboBoxInterface.h"
#include "QsSpinBoxInterface.h"
#include "QsDoubleSpinBoxInterface.h"
#include "QsGroupBoxInterface.h"
#include "QsFrameInterface.h"
#include "QsFrameBaseInterface.h"
#include "QsTextEditInterface.h"
#include "QsSliderInterface.h"
#include "QsRadioButtonInterface.h"
#include "QsTabWidgetInterface.h"    
#include "QsTableWidgetInterface.h"    
#include "QsDockWidgetInterface.h"
#include "QsScrollAreaInterface.h"
#include "QsThumbWheelInterface.h"
#include "QsCoordinateLineInterface.h"
#include "QsPositionInterface.h"
#include "QsSimpleUnitInterface.h"
#include "QsSmartComboBoxInterface.h"
#include "QsSearchComboBoxInterface.h"
#include "QsTreeViewInterface.h"
#include "QsGroupRadioInterface.h"

void QsWidgets::addWidget(QsInterfaceAdapter * widget, QString styleSheet)
 {
	  widget->setStyleSheet(styleSheet);
	  widgets.append(widget);
 }

 QsWidgets::QsWidgets(QObject *parent)
		 : QObject(parent)
 {

	 // ATTENTION : Passe en dur vers fichier CSS de Starlinx
	/*QFile styleFile("C:/Projects/Products/Starlinx/Basic/Dev/config/Stylesheet/style.css");

	QString mStyleString ="";
	bool retBool = styleFile.open(QFile::ReadOnly);
	if (retBool)
	{
		mStyleString = QLatin1String(styleFile.readAll());
		styleFile.close();
	 }*/
	 QString mStyleString ="";
	   // Widget
	 addWidget(new QsWidgetInterface(this), mStyleString);
	 addWidget(new QsListWidgetInterface(this), mStyleString);
	 addWidget(new QsPushButtonInterface(this), mStyleString);
	 addWidget(new QsToolButtonInterface(this), mStyleString);
	 addWidget(new QsButgetInterface(this), mStyleString);
	 addWidget(new QsLabelInterface(this), mStyleString);
	 addWidget(new QsLabelTitleInterface(this), mStyleString);
	 addWidget(new QsLineEditInterface(this), mStyleString);
	 addWidget(new QsCheckBoxInterface(this), mStyleString);
	 addWidget(new QsComboBoxInterface(this), mStyleString);
	 addWidget(new QsSpinBoxInterface(this), mStyleString);
	 addWidget(new QsDoubleSpinBoxInterface(this), mStyleString);
	 addWidget(new QsGroupBoxInterface(this), mStyleString);
	 addWidget(new QsFrameInterface(this), mStyleString);
	 addWidget(new QsFrameBaseInterface(this), mStyleString);
	 addWidget(new QsTextEditInterface(this), mStyleString);
	 addWidget(new QsSliderInterface(this), mStyleString);
	 addWidget(new QsRadioButtonInterface(this), mStyleString);
	 addWidget(new QsTabWidgetInterface(this), mStyleString);
	 addWidget(new QsTableWidgetInterface(this), mStyleString);
	 addWidget(new QsDockWidgetInterface(this), mStyleString);
	 addWidget(new QsScrollAreaInterface(this), mStyleString);
	 addWidget(new QsThumbWheelInterface(this), mStyleString);
	 addWidget(new QsCoordinateLineInterface(this), mStyleString);
	 addWidget(new QsPositionInterface(this), mStyleString);
	 addWidget(new QsSimpleUnitInterface(this), mStyleString);
	 addWidget(new QsSmartComboBoxInterface(this), mStyleString);
	 addWidget(new QsSearchComboBoxInterface(this), mStyleString);
	 addWidget(new QsTreeViewInterface(this), mStyleString);
	 addWidget(new QsGroupRadioInterface(this), mStyleString);
 }

 QList<QDesignerCustomWidgetInterface*> QsWidgets::customWidgets() const
 {
	 return widgets;
 }



#if QT_VERSION < 0x050000
		Q_EXPORT_PLUGIN2(qswidgetsplugin, QsWidgets)
#endif // QT_VERSION < 0x050000