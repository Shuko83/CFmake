/**
@file WidgetFactory.cpp
@brief Fabrique de widget
@author F.Bighelli
 */

#include "WidgetFactory.h"

#include "AnchorableContainer.h"
#include <SwPropertiesModelImpl.h>
#include <SwGuiDefaultItemDelegate.h>
#include "PluginOverview.h"


static WidgetFactory * _instance=0;


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

	_doc = new EditDoc();
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


QWidget * WidgetFactory::buildDocBlankView()
{
	return _doc;
}


/** @brief Construction widget representant les composants disponibles*/
QWidget * WidgetFactory::buildPluginsBankView(bool isGraphViewHosted) {
    
	PluginOverview * w = new PluginOverview(_doc,isGraphViewHosted,_graphPalette);
    //QWidget * w=new QWidget();
//     QListView * lviewPlugins=new QListView(w);
//     lviewPlugins->setModel(new PluginsListModel(lviewPlugins));
//     //lviewPlugins->setFixedWidth(150);
//     lviewPlugins->setFrameShape(QFrame::NoFrame);
//     if (isGraphViewHosted) {
//         lviewPlugins->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
//         lviewPlugins->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
//     }
//     QListView * lviewComponents=new QListView(w);
//     ComponentListModel * componentModel=new ComponentListModel(lviewComponents);
// 	lviewPlugins->connect(lviewPlugins,SIGNAL(  clicked ( const QModelIndex &)),componentModel,SLOT(onSelectedPluginChanged(const QModelIndex&)));
// 	lviewPlugins->connect(lviewPlugins->selectionModel(),SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &)),componentModel,SLOT(onSelectedPluginChanged(const QModelIndex&)));
//     lviewComponents->setModel(componentModel);
//     //lviewComponents->setFixedWidth(200);
//     lviewComponents->setFrameShape(QFrame::NoFrame);
//     lviewComponents->setDragEnabled(true); 
//     if (isGraphViewHosted) {
//         lviewComponents->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
//         lviewComponents->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
//     }
// 
// 	lviewComponents->connect(lviewComponents->selectionModel(),SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &)),_doc,SLOT(onSelectedComponentChanged(const QModelIndex&)));
// 	lviewComponents->connect(lviewComponents,SIGNAL(  clicked ( const QModelIndex &)),_doc,SLOT(onSelectedComponentChanged(const QModelIndex&)));
// 
// 	//ADD ACTION TO Edit the component documentation
//  	/*lviewComponents->setContextMenuPolicy(Qt::CustomContextMenu );
//  	QAction *toot = new QAction("ici",lviewComponents);
// 	lviewComponents->addAction(toot);*/
// 	
// 
// 		
// 	QGridLayout * layout=new QGridLayout(w);
// 	
// 	QVBoxLayout * searchLayout = new QVBoxLayout();
// 
// 	QLineEdit * searchLineEdit = new QLineEdit();
// 	searchLayout->addWidget(searchLineEdit);
// 	QPushButton * search = new QPushButton("val");
// 	searchLayout->addWidget(search);
// 
// 
// 
// 	layout->addLayout(searchLayout,0,0);
// 	layout->addWidget(lviewComponents);
//     layout->addWidget(lviewPlugins);
//     layout->setMargin(0);
//     layout->setSpacing(2);

    //AnchorableContainer * ac=new AnchorableContainer(0,TOP_RIGHT,QIcon(":/StreamWorkEditor2/comp_toolbox.png"),"Components");
    //ac->setCentralWidget(w,true,false);
    //setGraphViewMode(ac);
    //return ac;
    //setGraphViewMode(w);
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


