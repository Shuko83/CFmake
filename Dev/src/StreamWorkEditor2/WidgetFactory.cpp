/**
@file WidgetFactory.cpp
@brief Fabrique de widget
@author F.Bighelli
 */

#include "WidgetFactory.h"
#include "PluginsListModel.h"
#include "ComponentListModel.h"
#include "AnchorableContainer.h"
#include <SwPropertiesModelImpl.h>
#include <SwGuiDefaultItemDelegate.h>


static WidgetFactory * _instance=0;

static const char * ScrollBarVerticalStyle = " \
    QScrollBar:vertical { \
          border: 0px; \
          border-color :#111111; \
          background: #111111; \
          width: 10px; \
          padding: 2px 2px 2px 2px; \
    } \
\
    QScrollBar::handle:vertical {\
        color: #EEEEEE;\
        background: #EEEEEE;\
              min-height: 20px;\
          }\
      QScrollBar::add-line:vertical {\
          border: 2px solid grey;\
          background: solid grey;\
          height: 20px;\
          subcontrol-position: bottom;\
          subcontrol-origin: margin;\
      }\
\
      QScrollBar::sub-line:vertical {\
          border: 2px solid grey;\
          background: solid grey;\
          height: 20px;\
          subcontrol-position: top;\
          subcontrol-origin: margin;\
      }\
";

static const char * ScrollBarHorizontalStyle = " \
    QScrollBar:horizontal { \
          border: 0px; \
          border-color :#111111; \
          background: #111111; \
          width: 10px; \
          padding: 2px 2px 2px 2px; \
    } \
\
    QScrollBar::handle:horizontal {\
        color: #EEEEEE;\
        background: #EEEEEE;\
              min-height: 20px;\
          }\
      QScrollBar::add-line:horizontal {\
          border: 2px solid grey;\
          background: solid grey;\
          height: 20px;\
          subcontrol-position: right;\
          subcontrol-origin: margin;\
      }\
\
      QScrollBar::sub-line:horizontal {\
          border: 2px solid grey;\
          background: solid grey;\
          height: 20px;\
          subcontrol-position: left;\
          subcontrol-origin: margin;\
      }\
";

/** @brief Constructor */
WidgetFactory::WidgetFactory() {
	_graphPalette.setColor(QPalette::Window,QColor(50,50,50,255));
	_graphPalette.setColor(QPalette::Base,QColor(50,50,50,255));
	_graphPalette.setColor(QPalette::AlternateBase,QColor(10,10,10,255));
	_graphPalette.setColor(QPalette::Button,QColor(128,128,128,255));
	_graphPalette.setColor(QPalette::WindowText,QColor(255,255,255,255));
	_graphPalette.setColor(QPalette::Text,QColor(255,255,255,255));
	_graphPalette.setColor(QPalette::BrightText,QColor(128,128,128,255));
	_graphPalette.setColor(QPalette::ButtonText,QColor(255,255,255,255));
}
/** @brief ~Destructor */
WidgetFactory::~WidgetFactory() {

}
/** @brief acces singleton */
WidgetFactory * WidgetFactory::getInstance() {
    if (_instance==0) {
        _instance=new WidgetFactory();
    }
    return _instance;
}

/** @brief Construction widget representant les composants disponibles*/
QWidget * WidgetFactory::buildPluginsBankView(bool isGraphViewHosted) {
    
    QWidget * w=new QWidget();
    QListView * lviewPlugins=new QListView(w);
    lviewPlugins->setModel(new PluginsListModel(lviewPlugins));
    //lviewPlugins->setFixedWidth(150);
    lviewPlugins->setFrameShape(QFrame::NoFrame);
    if (isGraphViewHosted) {
        lviewPlugins->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
        lviewPlugins->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    }
    QListView * lviewComponents=new QListView(w);
    ComponentListModel * componentModel=new ComponentListModel(lviewComponents);
    lviewPlugins->connect(lviewPlugins,SIGNAL(  clicked ( const QModelIndex &)),componentModel,SLOT(onSelectedPluginChanged(const QModelIndex&)));
    lviewComponents->setModel(componentModel);
    //lviewComponents->setFixedWidth(200);
    lviewComponents->setFrameShape(QFrame::NoFrame);
    lviewComponents->setDragEnabled(true); 
    if (isGraphViewHosted) {
        lviewComponents->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
        lviewComponents->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    }
    QHBoxLayout * layout=new QHBoxLayout(w);
    layout->addWidget(lviewComponents);
    layout->addWidget(lviewPlugins);
    layout->setMargin(0);
    layout->setSpacing(2);

    //AnchorableContainer * ac=new AnchorableContainer(0,TOP_RIGHT,QIcon(":/StreamWorkEditor2/comp_toolbox.png"),"Components");
    //ac->setCentralWidget(w,true,false);
    //setGraphViewMode(ac);
    //return ac;
    setGraphViewMode(w);
    return w;
}
/** @brief Construction widget vue en arbre*/
QWidget * WidgetFactory::buildStreamTreeView(QStreamTreeModel * model) {
    QTreeView * w=new QTreeView();
    w->setAlternatingRowColors(true);
    w->setModel(model);
    //setGraphViewMode(w);
    return w;
}
/** @brief Construction widget propriétés*/
PropertiesWidget * WidgetFactory::buildPropertiesWidget() {
    PropertiesWidget * w=new PropertiesWidget();
    return w;
}

/** @brief Modification attribut lorsque le widget est hébergé par un GraphView */
void WidgetFactory::setGraphViewMode(QWidget * w) {
    w->setPalette(_graphPalette);
    //w->setAutoFillBackground(true);
}


