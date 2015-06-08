#include "PluginOverview.h"
#include "PluginsListModel.h"
#include <QScrollBar>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrent>
#else
#include <QtConcurrentRun>
#endif

//#include "..\SwDoc\MultiTagCompleter.h"


PluginOverview::PluginOverview(EditDoc* doc,bool isGraphViewHosted,QPalette graphPalette,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.LE_search->setText(DefaultSearchText);

    ui.lviewPlugins->setModel(new PluginsListModel(ui.lviewPlugins));
    //ui.lviewPlugins->setFixedWidth(150);
    ui.lviewPlugins->setFrameShape(QFrame::Box);
	ui.lviewPlugins->viewport()->setToolTip("blabla");
    if (isGraphViewHosted) 
	{
        ui.lviewPlugins->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
        ui.lviewPlugins->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    }

    _componentModel=new ComponentListModel(ui.lviewComponents);
	ui.lviewPlugins->connect(ui.lviewPlugins,SIGNAL(  clicked ( const QModelIndex &)),_componentModel,SLOT(onSelectedPluginChanged(const QModelIndex&)));
	ui.lviewPlugins->connect(ui.lviewPlugins->selectionModel(),SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &)),_componentModel,SLOT(onSelectedPluginChanged(const QModelIndex&)));
    ui.lviewComponents->setModel(_componentModel);

    ui.lviewComponents->setFrameShape(QFrame::Box);
    ui.lviewComponents->setDragEnabled(true); 

    if (isGraphViewHosted) 
	{
        ui.lviewComponents->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
        ui.lviewComponents->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    }

	setPalette(graphPalette);
	setAutoFillBackground(true);
	//ui.LE_search->setPalette(graphPalette);

	//Connect
	connect(ui.PB_clear,SIGNAL(clicked()),this,SLOT(resetSearchText()));
	connect(ui.LE_search,SIGNAL(textEdited ( const QString & )),this,SLOT(doSearch(const QString&)));
	
	ui.LE_search->installEventFilter(this);
	_pal = graphPalette;


	// QObject::connect(&_futureWatcher, SIGNAL(finished()), this, SLOT(setCompleter()));
}

PluginOverview::~PluginOverview()
{

}

//-------------------------------------------------------------------------
void PluginOverview::resetSearchText()
{
	ui.LE_search->setText(DefaultSearchText);

	//Hide plugin view
	QList<int> sizeWid;
	sizeWid << width()/2 << width()/2;
	ui.splitter->setSizes(sizeWid);

	_componentModel->clear();

	ui.lviewPlugins->clearSelection();
}

//-------------------------------------------------------------------------
void PluginOverview::doSearch( const QString&text )
{
	_componentModel->clear();

	//Evol quand on efface pour réafficher la list plugin
	if(text == "")
	{
		//Hide plugin view
		QList<int> sizeWid;
		sizeWid << width()/2 << width()/2;
		ui.splitter->setSizes(sizeWid);

		_componentModel->clear();

		ui.lviewPlugins->clearSelection();
		return;
	}

	// Pour recherche par mot clé
// 	QStringList keywords = text.split(",",QString::SkipEmptyParts);
// 	QStringList::Iterator it = keywords.begin();
// 
// 	//On trimmed les mots clés et on ignore les mots clés inférieur a 3 lettres
// 	for(it ; it != keywords.end() ; )
// 	{
// 		*it = it->trimmed();
// 		if(it->size() < 3)
// 			it = keywords.erase(it);
// 		else
// 			it++;
// 	}

	//Find the text on the database
	// A mettre dans un thread si latence trop importante
	QStringList componentToDisplay;

	QMap<QString, StreamWork::SwCore::SwPluginFactory_Class *> * allPlugins = SW_APP->ComponentsBank().GetAllPlugins();

	QMapIterator<QString, StreamWork::SwCore::SwPluginFactory_Class *> itPlugins(*allPlugins);
	while (itPlugins.hasNext())
	{
		itPlugins.next();
		QSet<QString> components = itPlugins.value()->GetComponentsList();
		for (QString componentName : components)
		{
			if (componentName.contains(text, Qt::CaseInsensitive))
				componentToDisplay << componentName;
		}
	}

	_componentModel->manageList(componentToDisplay);

	//Hide plugin view
	QList<int> sizeWid;
	sizeWid << 100 << 0;
	ui.splitter->setSizes(sizeWid);
}

// QStringList doCompleterFill()
// {
//  	return QStringList(); // resultat pour la completion
// }

//-------------------------------------------------------------------------
bool PluginOverview::eventFilter( QObject *obj, QEvent *event )
{
	bool ret = QObject::eventFilter(obj, event);
	
	if(obj == ui.LE_search)
	{
		if(event->type() == QEvent::FocusOut)
		{
			if(ui.LE_search->text() == "")
				resetSearchText();
		}
		if(event->type() == QEvent::MouseButtonRelease)
		{
			if(ui.LE_search->text() == DefaultSearchText)
				ui.LE_search->setText("");

			// Permet de faire la requete de recherche de tous les composants qu'une fois au click
			// _futureWatcher.setFuture(QtConcurrent::run(doCompleterFill));
		}
	}

	return ret;
}

//-------------------------------------------------------------------------
// void PluginOverview::setCompleter()
// {
//  	MultiTagCompleter *completer = new MultiTagCompleter(_futureWatcher.result(), this);
//  	completer->setCaseSensitivity(Qt::CaseInsensitive);
//  	completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
//  
//  	completer->popup()->setPalette(_pal);
//  	//Make the search and display result to listView
//  	ui.LE_search->setCompleter(completer);
//  
//  	QObject::connect(completer,SIGNAL(activated(const QString&)),this, SLOT(doSearch(const QString&)));
// }