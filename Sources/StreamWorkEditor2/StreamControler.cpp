/**
@file StreamControler.cpp
@brief Controleur Stream <-> Scene
@author F.Bighelli
*/

#include <QDomElement>
#include <QMessageBox>

#include <SwApplication.h>
#include <SwMacros.h>
#include <SwSaver_Class.h>
#include <SwLoader_Class.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwPins_Manager.h>
#include <SwPin.h>

#include "StreamControler.h"
#include "MenuManager.h"
#include "WidgetFactory.h"
#include "GraphicsLoaderV1.h"
#include "Arranger.h"
#include "InterestArea.h"
#include "ISwModelHostModifier.h"
#include "ISwModelService.h"
#include "SwSaver_Class.h"
#include "ModelCreatorHelper.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwModel;

#define CL_SCENE_NODE "Scene"
#define CL_SCENE_ATT_X "x"
#define CL_SCENE_ATT_Y "y"
#define CL_SCENE_ATT_WIDTH "width"
#define CL_SCENE_ATT_HEIGHT "height"

#define CL_IA_NODE "IA"
#define CL_IA_ATT_X "x"
#define CL_IA_ATT_Y "y"
#define CL_IA_ATT_W "W"
#define CL_IA_ATT_H "H"
#define CL_IA_ATT_COLOR "color"
#define CL_IA_ATT_ALPHA "alpha"
#define CL_IA_ATT_TEXT "text"

#define CL_CGITEM_NODE "CGItem"
#define CL_CGITEM_ATT_NAME "name"
#define CL_CGITEM_ATT_X "x"
#define CL_CGITEM_ATT_Y "y"
#define CL_CGITEM_ATT_COLOR "color"
#define CL_CGITEM_ATT_TEXT_COLOR "tcolor"
#define CL_CGCNTITEM_NODE "Cnt"
#define CL_CGCNTITEM_ATT_NAME "name"
#define CL_CGCNTITEM_ATT_POS "pos"

namespace
{

	std::tuple<bool, bool, QString, double, double> makeComparableTuple(QGraphicsItem * item)
	{
		ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(item);
		InterestArea * ia = dynamic_cast<InterestArea *>(item);
		QString name;
		if (ia)
			name = ia->getText();
		else if (cgitem)
			name = cgitem->getComponent()->GetName();
		
		// On place les IA en premier puis les ComponentGraphicItem ;
		// Chaque catégorie est trié avec son nom ;
		// S'il y a des doublons (ce qui a peu de chance d'arriver, les noms des Composants devant êtr euniques),
		// on se sert de la position et de la taille pour les distinguer.
		return std::make_tuple(!bool(ia), !bool(cgitem), name, item->x(), item->y());
	}

	bool graphicItemComparator(QGraphicsItem * leftItem, QGraphicsItem * rightItem)
	{
		return makeComparableTuple(leftItem) < makeComparableTuple(rightItem);
	}

}

//-----------------------------------------------------------------------
StreamControler::StreamControler(PropertiesWidget * propertiesWidget) :QObject()
{
	_isExistingStream = false;
	_enableStreamControlerObservation = false;
	_streamView = 0;
	_creationPosition = QPointF(0.0f, 0.0f);
	_rootComponent = SW_APP->CreateNewStream("NoNamed");
	recursiveConnectToControler(_rootComponent);
	/** @brief Scene */
	_streamScene = new StreamScene();
	_streamScene->setSceneRect(-100.0, -100.0, 100.0, 100.0);
	_streamFileName = "";
	MenuManager::getInstance()->setControler(this);
	//Ajout widget
	//_streamScene->addApplicativeWidget(WidgetFactory::getInstance()->buildPluginsBankView(true));

	_propertiesWidget = propertiesWidget;
	//Connections signaux
	connect(_streamScene, SIGNAL(sceneRectChanged(const QRectF &)), this, SLOT(onSceneRectChanged(const QRectF &)));
	connect(_streamScene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(_streamScene, SIGNAL(onLinkConnectors(ConnectorGraphicItem *, ConnectorGraphicItem *)),this, SLOT(onLinkConnectors(ConnectorGraphicItem *, ConnectorGraphicItem *)));
	_enableStreamControlerObservation = true;
	_stopExecution = false;
}

//-----------------------------------------------------------------------
StreamControler::~StreamControler()
{
	if ( _stopExecution == true )
	{
		SW_APP->StopLaunch();
	}
	if ( MenuManager::getInstance()->getControler() == this )
	{
		MenuManager::getInstance()->setControler(0);
	}
	_propertiesWidget->setSelectedGraphicComponent(0);
	if ( _streamView != 0 )
	{
		//Deconnexion a la vue
		_streamView->setScene(0);
		_streamView->setControler(0);
	}
	disconnect(_streamScene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	disconnect(_streamScene, SIGNAL(sceneRectChanged(const QRectF &)), this, SLOT(onSceneRectChanged(const QRectF &)));
	disconnect(_streamScene, SIGNAL(onLinkConnectors(ConnectorGraphicItem *, ConnectorGraphicItem *)),
			   this, SLOT(onLinkConnectors(ConnectorGraphicItem *, ConnectorGraphicItem *)));
	//Deconnection au modele
	recursiveDisconnectToControler(_rootComponent);
	//Destruction des liens
	destroyLinks();
	//Destruction du streams
	if ( !_isExistingStream )
	{
		SW_APP->DestroyStream(_rootComponent);
		_rootComponent = nullptr;
	}
	//Destruction de la scene
	delete _streamScene;
}

//-----------------------------------------------------------------------
StreamScene * StreamControler::getScene()
{
	return _streamScene;
}

//-----------------------------------------------------------------------
void StreamControler::setView(StreamView * view)
{
	_streamView = view;
	_streamView->setScene(_streamScene);
	_streamView->setControler(this);
}

//-----------------------------------------------------------------------
StreamView *StreamControler::getView()
{
	return _streamView;
}

//-----------------------------------------------------------------------
void StreamControler::loadStream(QString streamFileName)
{
	_streamFileName = streamFileName;
	_enableStreamControlerObservation = false;
	QFile file;
	QDomDocument doc;
	QString xml_error;
	int error_line, error_column;
	SwComponent_ClassPtr root_component;

	try
	{
		QString stream_desc;
		file.setFileName(_streamFileName);
		if ( !file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			QMessageBox::warning(NULL, "StreamWorkEditor alert", QString("Fail to open file stream %1").arg(_streamFileName));
			return;
		}
		stream_desc = QString(file.readAll());
		//parsing xml du document
		if ( !doc.setContent(stream_desc, &xml_error, &error_line, &error_column) )
		{
			QString msg = QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
			QMessageBox::warning(NULL, "StreamWorkEditor alert", msg);
			return;
		}
		SwLoader_Class loader;
		root_component = loader.Load(doc);
		if ( root_component == NULL )
		{
			QMessageBox::warning(NULL, "StreamWorkEditor alert", QString("Unable to load %1").arg(_streamFileName));
			return;
		}
		//Supression de l'ancien
		disconnectFromControler(_rootComponent);
		if ( !_isExistingStream )
		{
			SW_APP->DestroyStream(_rootComponent);
			_rootComponent = nullptr;
		}
		_isExistingStream = false;
		//Enregistrement du composant
		SW_APP->AddNewStream(root_component);
		_rootComponent = root_component;
		//Chargement donnees visuelles
		loadVisualData(doc);

		_streamView->centerOn(_streamScene->itemsBoundingRect().center());

		buildLinks();
		//Connection au modele
		recursiveConnectToControler(_rootComponent);
		//Demarrage si necessaire
		_stopExecution = SW_APP->LaunchAutoStart();
	}
	catch ( SwException & e )
	{
		_enableStreamControlerObservation = true;
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Exception when load %1\n%2").arg(_streamFileName).arg(e.what()));
		return;
	}
	_enableStreamControlerObservation = true;
	streamControlerChanged();
}


//-----------------------------------------------------------------------
void StreamControler::loadExistingStream(QString streamFileName, StreamWork::SwCore::SwComponent_Class * aStream)
{
	_streamFileName = streamFileName;
	_enableStreamControlerObservation = false;
	QFile file;
	QDomDocument doc;
	QString xml_error;
	int error_line, error_column;
	SwComponent_ClassPtr root_component;

	try
	{
		QString stream_desc;
		file.setFileName(_streamFileName);
		if ( !file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			QMessageBox::warning(NULL, "StreamWorkEditor alert", QString("Fail to open file stream %1").arg(_streamFileName));
			return;
		}
		stream_desc = QString(file.readAll());
		//parsing xml du document
		if ( !doc.setContent(stream_desc, &xml_error, &error_line, &error_column) )
		{
			QString msg = QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
			QMessageBox::warning(NULL, "StreamWorkEditor alert", msg);
			return;
		}
		root_component = aStream;
		if ( root_component == NULL )
		{
			QMessageBox::warning(NULL, "StreamWorkEditor alert", QString("Unable to load %1").arg(_streamFileName));
			return;
		}
		//Supression de l'ancien
		disconnectFromControler(_rootComponent);
		if ( !_isExistingStream )
		{
			SW_APP->DestroyStream(_rootComponent);
			_rootComponent = nullptr;
		}
		_isExistingStream = true;
		//Enregistrement du composant
		_rootComponent = root_component;
		//Chargement donnees visuelles
		loadVisualData(doc);

		_streamView->centerOn(_streamScene->itemsBoundingRect().center());

		buildLinks();
		//Connection au modele
		recursiveConnectToControler(_rootComponent);
	}
	catch ( SwException & e )
	{
		_enableStreamControlerObservation = true;
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Exception when load %1\n%2").arg(_streamFileName).arg(e.what()));
		return;
	}
	_enableStreamControlerObservation = true;
	streamControlerChanged();
}



//-----------------------------------------------------------------------
void StreamControler::saveStream()
{
	QFile file(_streamFileName);	
	//Ouverture d'un fichier en ecriture
	if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false )
	{
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Fail to save stream in file %1").arg(_streamFileName));
		return;
	}

	QXmlStreamWriter streamWriter(&file);
	streamWriter.setCodec("UTF-8");
	streamWriter.setAutoFormatting(true);

	//serialisation du stream
	SwSaver_Class saver;
	streamWriter.writeStartDocument();

	saver.Save(_rootComponent, streamWriter);
	//Ajout donnees visuelles
	saveVisualData(streamWriter);

	streamWriter.writeEndDocument();
	//Fermeture du fichier
	file.close();
}


//-----------------------------------------------------------------------
void StreamControler::saveStreamAs(QString streamFileName)
{
	_streamFileName = streamFileName;
	saveStream();
}



//-----------------------------------------------------------------------
QString StreamControler::getStreamFileName()
{
	return _streamFileName;
}

//-----------------------------------------------------------------------
SwComponent_Class * StreamControler::getRootItem()
{
	return _rootComponent;
}

//-----------------------------------------------------------------------
void StreamControler::setSelection(StreamWork::SwCore::SwComponent_Class * component)
{
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(component);
	if ( it != _mapCompToItem.end() )
	{
		_streamScene->clearSelection();
		(*it)->setSelected(true);
		_streamView->centerOn((*it));
	}
}
//------------------------------------------------------------------------------
// Evenements de la vue 
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------
QMenu * StreamControler::getContextualMenu(const QPointF & p)
{
	return MenuManager::getInstance()->buildContextMenu(p);
}

//-----------------------------------------------------------------------
void StreamControler::addComponent(QString & name, const QPointF & p)
{
	_creationPosition = p;
	SwComponent_ClassPtr component = SW_FACTORIES.CreateComponent(name);
	QList<QGraphicsItem *> list = _streamScene->items(p);
	QList<ComponentGraphicItem *> listc;
	for ( int i = 0; i < list.count(); i++ )
	{
		ComponentGraphicItem * gcomponent = dynamic_cast<ComponentGraphicItem *>(list[i]);
		if ( gcomponent != 0 )
			listc.push_back(gcomponent);
	}
	if ( listc.count() == 0 )
	{
		component->SetName(_rootComponent->GetSuggestedNameForChild(name));
		_rootComponent->AddChild(component);
	}
	else
	{
		component->SetName(listc.front()->getComponent()->GetSuggestedNameForChild(name));
		listc.front()->getComponent()->AddChild(component);
	}
}
//------------------------------------------------------------------------------
// Evenements Composant
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------
void StreamControler::showProperties()
{
	//_propertiesWidget->showProperties(_streamView);
}
//------------------------------------------------------------------------------
// Evenements de la scene 
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------
void StreamControler::selectionChanged()
{
	MenuManager::getInstance()->setControler(this);
	MenuManager::getInstance()->selectionChanged();
	QList<QGraphicsItem *> list = _streamScene->selectedItems();
	QList<SwComponent_Class *> sel;
	ComponentGraphicItem * first_gcomponent = 0;
	for ( int i = 0; i < list.count(); i++ )
	{
		ComponentGraphicItem * gcomponent = dynamic_cast<ComponentGraphicItem *>(list[i]);
		if ( gcomponent != 0 )
		{
			if ( first_gcomponent == 0 )
				first_gcomponent = gcomponent;
			sel.push_back(gcomponent->getComponent());
		}
	}
	for ( int i = 0; i < _selectionObservers.count(); i++ )
	{
		_selectionObservers[i]->setSelection(sel);
	}
	if ( first_gcomponent != 0 )
	{
		_propertiesWidget->setSelectedGraphicComponent(first_gcomponent);
		return;
	}
	_propertiesWidget->setSelectedGraphicComponent(0);
}

//-----------------------------------------------------------------------
void StreamControler::streamControlerChanged()
{
	for ( int i = 0; i < _streamControlerObservers.count(); i++ )
	{
		_streamControlerObservers[i]->controlerHasChanged();
	}
}

//-----------------------------------------------------------------------
void StreamControler::onLinkConnectors(ConnectorGraphicItem * src, ConnectorGraphicItem * target)
{
	if ( src->getConnectorType() == CONSUMER )
	{
		//Cas interface
		SwComponent_Class * cconsumer = ((ComponentGraphicItem *) src->parentItem())->getComponent();
		ISwInterfaces_Consumer *iconsumer =
			dynamic_cast<ISwInterfaces_Consumer *>(cconsumer->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
		SwComponent_Class * cprovider = ((ComponentGraphicItem *) target->parentItem())->getComponent();
		ISwInterfaces_Provider *iprovider =
			dynamic_cast<ISwInterfaces_Provider *>(cprovider->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
		try
		{
			iconsumer->AttachProvider(iprovider, src->getName(), target->getName());
		}
		catch ( SwException & /*se*/ )
		{
			//L'application a levé une exception
			//QMessageBox::warning(0,QString("Warning... "),QString(se.what()),QMessageBox::Abort,QMessageBox::NoButton,QMessageBox::NoButton);
		}
	}
	if ( src->getConnectorType() == PIN )
	{
		SwComponent_Class * csource = ((ComponentGraphicItem *) src->parentItem())->getComponent();
		SwComponent_Class * ctarget = ((ComponentGraphicItem *) target->parentItem())->getComponent();
		ISwPins_Manager *lpinManager = dynamic_cast<ISwPins_Manager *>(csource->QueryService(CG_SW_SERVICE_PINS_MANAGER));
		ISwPins_Manager *rpinManager = dynamic_cast<ISwPins_Manager *>(ctarget->QueryService(CG_SW_SERVICE_PINS_MANAGER));
		if ( lpinManager != 0 && rpinManager != 0 && rpinManager != lpinManager )
		{
			try
			{
				lpinManager->ConnectRemotePinToLocalPin(src->getName(), target->getName(), rpinManager);
			}
			catch ( SwException & se )
			{
				//L'application a levé une exception
				QMessageBox::warning(0, QString("Warning... "), QString(se.what()), QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton);
			}
		}
	}
}

//-----------------------------------------------------------------------
void StreamControler::onSceneRectChanged(const QRectF & rect)
{
	// A Faire ->scrollcontentBy de la view
	if ( _streamView != 0 )
	{
		_streamView->externalUpdateSceneRect();
	}
}

//------------------------------------------------------------------------------
// Evenements du modele 
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------
void StreamControler::childAdded(SwComponent_Class * parent, SwComponent_Class *child)
{
	ComponentGraphicItem * c = new ComponentGraphicItem(child, this);
	_lastAddedComponent = c;
	if ( parent != _rootComponent )
	{
		QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
		it = _mapCompToItem.find(parent);
		if ( it != _mapCompToItem.end() )
		{
			c->setParentItem((*it));
		}
		c->setPos((*it)->mapFromScene(_creationPosition));
	}
	else
	{
		c->setPos(_creationPosition);
	}
	_mapCompToItem.insert(child, c);
	connectToControler(child);
	if ( parent == _rootComponent )
	{
		_streamScene->addItem(c);
	}
	buildConnectors(child);
	c->updateAttributs();
	_streamView->update();
	streamControlerChanged();
}

//-----------------------------------------------------------------------
ComponentGraphicItem * StreamControler::getLastAddedComponent()
{
	return _lastAddedComponent;
}

//-----------------------------------------------------------------------
void StreamControler::childRemoved(StreamWork::SwCore::SwComponent_Class * parent,
								   StreamWork::SwCore::SwComponent_Class *child)
{
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(child);
	if ( it != _mapCompToItem.end() )
	{
		(*it)->setParentItem(0);
		_streamScene->removeItem((*it));
		recursiveDisconnectToControler(child);
	}
	streamControlerChanged();
}

//-----------------------------------------------------------------------
void StreamControler::componentNameChanged(StreamWork::SwCore::SwComponent_Class * component)
{
	if ( component == _rootComponent )
		return;
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(component);
	if ( it != _mapCompToItem.end() )
	{
		(*it)->updateAttributs();
	}
	streamControlerChanged();
}

//-----------------------------------------------------------------------
void StreamControler::OnAddInterface(ISwInterfaces_Service * source, QString interface_name)
{
	ISwHost * ihost = dynamic_cast<ISwHost *>(source);
	SwComponent_Class * component = ihost->GetHostComponent();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(component);
	if ( it == _mapCompToItem.end() )
	{
		return;
	}
	ComponentGraphicItem * gitem = (*it);
	QList<ConnectorGraphicItem *> * connectors = gitem->getConnectors();

	ISwInterfaces_Provider *iprovider = dynamic_cast<ISwInterfaces_Provider *>(source);
	if ( iprovider != 0 )
	{
		QString type = iprovider->GetInterfaceType(interface_name);
		ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, interface_name, type, PROVIDER);
		connectors->push_back(gcitem);
		gcitem->setParentItem(gitem);
		gitem->updateAttributs();
		return;
	}
	ISwInterfaces_Consumer *iconsumer = dynamic_cast<ISwInterfaces_Consumer *>(source);
	if ( iconsumer != 0 )
	{
		QString type = iconsumer->GetInterfaceType(interface_name);
		ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, interface_name, type, CONSUMER);
		connectors->push_back(gcitem);
		gcitem->setParentItem(gitem);
		gitem->updateAttributs();
		return;
	}
	return;
}

//-----------------------------------------------------------------------
void StreamControler::OnRemoveInterface(ISwInterfaces_Service * source, QString interface_name)
{
	ISwHost * ihost = dynamic_cast<ISwHost *>(source);
	SwComponent_Class * component = ihost->GetHostComponent();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(component);
	if ( it == _mapCompToItem.end() )
	{
		return;
	}
	ComponentGraphicItem * gitem = (*it);
	QList<ConnectorGraphicItem *> * connectors = gitem->getConnectors();
	for ( int i = 0; i < connectors->count(); i++ )
	{
		if ( (*connectors)[i]->getName() == interface_name )
		{
			delete (*connectors)[i];
			connectors->removeAt(i);
			gitem->updateAttributs();
			return;
		}
	}
}

//-----------------------------------------------------------------------
void StreamControler::OnConnectInterface(ISwInterfaces_Service * source, QString interface_name,
										 ISwInterfaces_Service * remote_source, QString remote_interface_name)
{
	if ( dynamic_cast<ISwInterfaces_Provider *>(source) != 0 )
		return;
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator its;
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator itt;
	its = _mapCompToItem.find(source->GetHostComponent());
	itt = _mapCompToItem.find(remote_source->GetHostComponent());
	if ( its == _mapCompToItem.end() || itt == _mapCompToItem.end() )
	{
		return;
	}
	ConnectorGraphicItem *cgis = its.value()->getConnector(interface_name);
	ConnectorGraphicItem *cgit = itt.value()->getConnector(remote_interface_name);
	LinkGraphicItem *lgi = new LinkGraphicItem(cgis, cgit);
	_streamScene->addItem(lgi);
	lgi->setZValue(-100.0);
}
/*! \brief Sur deconnection d'une interface */
void StreamControler::OnDisconnectInterface(ISwInterfaces_Service * source, QString interface_name,
											ISwInterfaces_Service * remote_source, QString remote_interface_name)
{
	ISwInterfaces_Service * lsource = source;
	QString iname = interface_name;
	/*if (dynamic_cast<ISwInterfaces_Provider *>(source)!=0) {
		return;
		//lsource=remote_source;
		//iname=remote_interface_name;
		}*/
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator its;
	its = _mapCompToItem.find(lsource->GetHostComponent());
	if ( its == _mapCompToItem.end() )
	{
		return;
	}
	ConnectorGraphicItem *cgis = its.value()->getConnector(iname);
	if ( !cgis->getLinks()->isEmpty() )
	{

		// Previous code
		//         LinkGraphicItem *lgi=cgis->getLinks()->front();
		//         cgis->getLinks()->pop_front();
		//         lgi->setParentItem(0);
		//         _streamScene->removeItem(lgi);
		//         delete lgi;

		// New code
		QList<LinkGraphicItem*> list = *(cgis->getLinks());


		//-----------------------------------------------------------------------
		for (LinkGraphicItem * item : list)
		{
			StreamWork::SwCore::SwComponent_Class * tmpC = ((ComponentGraphicItem *) item->getSource()->parentItem())->getComponent();
			StreamWork::SwCore::SwComponent_Class * tmpS = ((ComponentGraphicItem *) item->getTarget()->parentItem())->getComponent();

			if ( remote_source == tmpC->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER)
				|| remote_source == tmpS->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER) )
			{
				item->setParentItem(0);
				_streamScene->removeItem(item);
				delete item;
				item = NULL;
			}
		}
	}
}

//-----------------------------------------------------------------------
void StreamControler::OnAddPin(StreamWork::SwCore::SwPin * pin)
{
	ISwPins_Manager *pinManager = pin->GetManager();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(pinManager->GetHostComponent());
	if ( it == _mapCompToItem.end() )
	{
		return;
	}
	ComponentGraphicItem * gitem = (*it);
	QList<ConnectorGraphicItem *> * connectors = gitem->getConnectors();
	ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, pin->GetName(), pin->GetType(), PIN);
	connectors->push_back(gcitem);
	gcitem->setParentItem(gitem);
	gitem->updateAttributs();

}

//-----------------------------------------------------------------------
void StreamControler::OnRemovePin(StreamWork::SwCore::SwPin * pin)
{
	ISwPins_Manager *pinManager = pin->GetManager();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(pinManager->GetHostComponent());
	if ( it == _mapCompToItem.end() )
	{
		return;
	}
	ConnectorGraphicItem * cgis = it.value()->getConnector(pin->GetName());
	//if (!cgis->getLinks()->isEmpty()) {
	//    LinkGraphicItem *lgi=cgis->getLinks()->front();
	//    cgis->getLinks()->pop_front();
	//    lgi->setParentItem(0);
	//    _streamScene->removeItem(lgi);
	//    delete lgi;
	//}
	it.value()->getConnectors()->removeOne(cgis);
	it.value()->updateAttributs();
	delete cgis;

}

//-----------------------------------------------------------------------
void StreamControler::OnConnectPin(StreamWork::SwCore::SwPin * local_pin, StreamWork::SwCore::SwPin * remote_pin)
{
	SwComponent_Class * lcomponent = local_pin->GetManager()->GetHostComponent();
	SwComponent_Class * rcomponent = remote_pin->GetManager()->GetHostComponent();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator itl;
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator itr;
	itl = _mapCompToItem.find(lcomponent);
	itr = _mapCompToItem.find(rcomponent);
	if ( itl != _mapCompToItem.end() && itr != _mapCompToItem.end() )
	{
		ConnectorGraphicItem * ritem = (*itr)->getConnector(remote_pin->GetName());
		ConnectorGraphicItem * litem = (*itl)->getConnector(local_pin->GetName());
		if ( ritem != 0 && litem != 0 && ritem->getLinks()->isEmpty() )
		{
			//Construction du lien
			LinkGraphicItem *lgi = new LinkGraphicItem(litem, ritem);
			_streamScene->addItem(lgi);
			lgi->setZValue(-100.0);
		}
	}
}

//-----------------------------------------------------------------------
void StreamControler::OnDisconnectPin(StreamWork::SwCore::SwPin * local_pin, StreamWork::SwCore::SwPin * remote_pin)
{
	SwComponent_Class * lcomponent = local_pin->GetManager()->GetHostComponent();
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator itl;
	itl = _mapCompToItem.find(lcomponent);
	if ( itl != _mapCompToItem.end() )
	{
		ConnectorGraphicItem * litem = (*itl)->getConnector(local_pin->GetName());
		if ( litem != 0 && !litem->getLinks()->isEmpty() )
		{
			LinkGraphicItem *lgi = litem->getLinks()->front();
			litem->getLinks()->pop_front();
			lgi->setParentItem(0);
			_streamScene->removeItem(lgi);
			delete lgi;
		}
	}
}
//------------------------------------------------------------------------------
// Liaison modele <-> Controler
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------
void StreamControler::connectToControler(SwComponent_Class * component)
{
	component->OnAddChild.iconnect(*this, &StreamControler::childAdded);
	component->OnRemoveChild.iconnect(*this, &StreamControler::childRemoved);
	component->OnChangeComponentName.iconnect(*this, &StreamControler::componentNameChanged);
	ISwInterfaces_Provider *iprovider = dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	if ( iprovider != 0 )
	{
		iprovider->AttachInterfacesServices_Listener(this);
	}
	ISwInterfaces_Consumer *iconsumer = dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	if ( iconsumer != 0 )
	{
		iconsumer->AttachInterfacesServices_Listener(this);
	}
	ISwPins_Manager *pinManager = dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
	if ( pinManager != 0 )
	{
		pinManager->RegisterListener(this);
	}
}

//-----------------------------------------------------------------------
void StreamControler::recursiveConnectToControler(SwComponent_Class * component)
{
	connectToControler(component);
	unsigned long count = component->GetNbChildren();
	for ( unsigned long i = 0; i < count; i++ )
	{
		recursiveConnectToControler(component->GetChild(i));
	}
}

//-----------------------------------------------------------------------
void StreamControler::disconnectFromControler(SwComponent_Class * component)
{
	component->OnAddChild.idisconnect(*this, &StreamControler::childAdded);
	component->OnRemoveChild.idisconnect(*this, &StreamControler::childRemoved);
	component->OnChangeComponentName.idisconnect(*this, &StreamControler::componentNameChanged);
	//La partie suivante est mise en commentaire pour ctacher les events en cours de destructions
	ISwInterfaces_Provider *iprovider = dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	if ( iprovider != 0 )
	{
		iprovider->DetachInterfacesServices_Listener(this);
	}
	ISwInterfaces_Consumer *iconsumer = dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	if ( iconsumer != 0 )
	{
		iconsumer->DetachInterfacesServices_Listener(this);
	}
	ISwPins_Manager *pinManager = dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
	if ( pinManager != 0 )
	{
		pinManager->UnregisterListener(this);
	}
}

//-----------------------------------------------------------------------
void StreamControler::recursiveDisconnectToControler(SwComponent_Class * component)
{
	disconnectFromControler(component);
	unsigned long count = component->GetNbChildren();
	for ( unsigned long i = 0; i < count; i++ )
	{
		recursiveDisconnectToControler(component->GetChild(i));
	}
}

//-----------------------------------------------------------------------
void StreamControler::createModelFromSelection(QList<SwComponent_Class *> & components, QString modelName)
{
	ModelCreatorHelper modelCreatorHelper;

	//Recuperation du service
	ISwModelService * serviceModel = dynamic_cast<ISwModelService *>(SW_APP->QueryService(CG_SW_SERVICE_MODEL));
	if ( serviceModel == 0 )
		return;

	if ( serviceModel->checkModelName(modelName) )
	{
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Model name already exist %1").arg(modelName));
		return;
	}


	//Calcul position
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	QPointF targetpos;

	for (SwComponent_Class * comp : components)
	{
		it = _mapCompToItem.find(comp);
		targetpos += it.value()->pos();
		comp->setParent(nullptr);
	}
	targetpos /= (qreal) components.count();
	_creationPosition = targetpos;

	QString modelHostName = "__host";
	//Ajout model host
	SwComponent_Class * modelHost = SW_APP->ComponentsBank().CreateComponent("SwModel2Host");
	modelHost->SetName(modelHostName);

	SwComponent_Class *cp = _rootComponent->GetChild(modelHostName);
	while ( cp != nullptr )
	{
		modelHostName += "t";
		cp = _rootComponent->GetChild(modelHostName);
	}

	modelHost->SetName(modelHostName);

	_rootComponent->AddChild(modelHost);
	//Ajout des interfaces qui sont externes a la selection dans le model host
	ISwModelHostModifier * modelModifier = dynamic_cast<ISwModelHostModifier *>(modelHost);

	QStringList interfaceExposed;

	QMap<SwComponent_Class*, QStringList> allreadyConsumed;
	QList<ModelCreatorHelper::ConsumerKey*> toremove;
	for (SwComponent_Class * comp : components)
	{
		//pour chaque interface produite linké a un composant exterieur a la selection
		//On la rajoute au model host
		ISwInterfaces_Provider *provider_handle = dynamic_cast<ISwInterfaces_Provider *>(comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
		if ( provider_handle != NULL )
		{
			QString interface_name;
			QString m_interface_name;
			QString remote_interface_name;
			interface_name = provider_handle->GetFirstInterface();
			while ( !interface_name.isEmpty() )
			{
				m_interface_name = "";
				ISwInterfaces_Consumer * consumer = provider_handle->GetFirstConsumer(interface_name, &remote_interface_name);
				while ( consumer != 0 )
				{
					if ( components.indexOf(consumer->GetHostComponent()) == -1 )
					{
						if ( m_interface_name.isEmpty() )
							m_interface_name = modelModifier->addProviderInterface(interface_name, comp, provider_handle->GetInterfaceType(interface_name));

						modelCreatorHelper.addProviderLink(provider_handle, interface_name, m_interface_name, consumer, remote_interface_name);

						consumer = 0;
					}
					consumer = provider_handle->GetNextConsumer(&remote_interface_name);
				}
				interface_name = provider_handle->GetNextInterface();
			}
		}

		//pour chaque interface consommé linké a un composant exterieur a la selection
		//On la rajoute au model host
		ISwInterfaces_Consumer *consumer_handle = dynamic_cast<ISwInterfaces_Consumer *>(comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
		if ( consumer_handle != NULL )
		{
			QString interface_name;
			QString m_interface_name;
			QString remote_interface_name;
			ISwInterfaces_Provider * pt_provider;
			interface_name = consumer_handle->GetFirstInterface(NULL, &pt_provider, &remote_interface_name);
			while ( !interface_name.isEmpty() )
			{
				if ( pt_provider != 0 )
				{
					if ( components.indexOf(pt_provider->GetHostComponent()) == -1 )
					{
						if ( allreadyConsumed[pt_provider->GetHostComponent()].contains(interface_name) )
						{
							toremove.append(modelCreatorHelper.addConsumerLink(consumer_handle, interface_name, interface_name, pt_provider, remote_interface_name));
						}
						else
						{
							allreadyConsumed[pt_provider->GetHostComponent()].append(interface_name);
							m_interface_name = modelModifier->addConsumerInterface(interface_name, comp, consumer_handle->GetInterfaceType(interface_name));
							modelCreatorHelper.addConsumerLink(consumer_handle, interface_name, m_interface_name, pt_provider, remote_interface_name);
						}
					}
				}
				interface_name = consumer_handle->GetNextInterface(NULL, &pt_provider, &remote_interface_name);
			}
		}

		//pour chaque connecteur consommé linké a un composant exterieur a la selection
		//On la rajoute au model host
		ISwPins_Manager *pins_manager_handle = dynamic_cast<ISwPins_Manager *>(comp->QueryService(CG_SW_SERVICE_PINS_MANAGER));
		if ( pins_manager_handle != NULL )
		{
			QList<SwPin *> pins = pins_manager_handle->GetPinList();
			for ( int i = 0; i < pins.count(); i++ )
			{
				QString m_connector_name;
				SwPin * rpin = pins[i]->GetConnected();
				QString pinname = pins[i]->GetName();
				if ( rpin != 0 )
				{
					if ( components.indexOf(rpin->GetManager()->GetHostComponent()) == -1 )
					{
						QString oldInterfaceName = pinname;
						while ( interfaceExposed.contains(pinname) )
							pinname += "_";

						interfaceExposed.append(pinname);

						m_connector_name = modelModifier->addConnector(pinname, comp, pins[i]->GetType());
						modelCreatorHelper.addConnectorLink(pins[i], rpin, m_connector_name);
					}
				}
			}
		}

	}
	modelModifier->updateModelHost();

	//Link selection to model host
	modelCreatorHelper.connectInternalToModelHost(modelHost);
	//Sauvegarde de la selection
	QList<SwComponent_Class *> components_and_model_host = components;
	components_and_model_host.push_back(modelHost);
	QString models_path = serviceModel->getModelsDirectory();
	QString _streamFileName = models_path + "/" + modelName + ".xml";
	QFile file;
	//Ouverture d'un fichier en ecriture
	file.setFileName(_streamFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false)
	{
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Fail to save stream in file %1").arg(_streamFileName));
		return;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormattingIndent(4);
	writer.setAutoFormatting(true);
	//serialisation du stream
	SwSaver_Class saver;
	saver.SaveModel(components_and_model_host, writer);
	//Ajout donnees visuelles
	saveVisualDataFromSelection(writer, components_and_model_host);
	writer.writeEndDocument();

	//Modification de la liste de model
	serviceModel->addModel(modelHostName, modelName);
	//Suppression de la selection
	for ( int i = 0; i < components_and_model_host.count(); i++ )
	{
		SwComponent_Class * component = components_and_model_host[i]->GetParent();
		if ( component != 0 )
		{
			component->RemoveChild(components_and_model_host[i]);
		}
	}


	// Insertion du model final
	SwComponent_Class * model = SW_APP->ComponentsBank().CreateComponent(modelName);
	model->SetName(modelName + "_instance");

	_creationPosition = targetpos;
	_rootComponent->AddChild(model);

	//Remove non fully integraded consumer
	for (ModelCreatorHelper::ConsumerKey* key : toremove)
	{
		modelCreatorHelper.removeConsumerLink(key);
	}
	

	modelCreatorHelper.connectModelToExternal(model);
}

//--------------------------------------------------------------------------
// Gestion persistence des données
//--------------------------------------------------------------------------
#define CL_IA_NODE "IA"
#define CL_IA_ATT_X "x"
#define CL_IA_ATT_Y "y"
#define CL_IA_ATT_W "W"
#define CL_IA_ATT_H "H"
#define CL_IA_ATT_COLOR "color"
#define CL_IA_ATT_ALPHA "alpha"
#define CL_IA_ATT_TEXT "text"


//-----------------------------------------------------------------------
void StreamControler::saveVisualData(QXmlStreamWriter & writer)
{
	writer.writeStartElement(CL_SCENE_NODE);
	QRectF srect = _streamScene->sceneRect();
	writer.writeAttribute(CL_SCENE_ATT_X, QString::number(srect.topLeft().x(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_Y, QString::number(srect.topLeft().y(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_WIDTH, QString::number(srect.width(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_HEIGHT, QString::number(srect.height(), 'g', 12));
	
	QList<QGraphicsItem *> items = _streamScene->items();
	std::sort(items.begin(), items.end(), graphicItemComparator);

	for (QGraphicsItem * item : items)
	{
		if (item->parentItem() == 0)
		{
			ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(item);
			if (cgitem != 0)
			{
				saveVisualItem(cgitem, writer);
			}
			InterestArea * ia = dynamic_cast<InterestArea *>(item);
			if (ia != 0)
			{
				writer.writeStartElement(CL_IA_NODE);
				writer.writeAttribute(CL_IA_ATT_X, QString::number(ia->pos().x(), 'g', 12));
				writer.writeAttribute(CL_IA_ATT_Y, QString::number(ia->pos().y(), 'g', 12));
				QRectF r = ia->boundingRect();
				writer.writeAttribute(CL_IA_ATT_W, QString::number(r.width(), 'g', 12));
				writer.writeAttribute(CL_IA_ATT_H, QString::number(r.height(), 'g', 12));
				writer.writeAttribute(CL_IA_ATT_COLOR, ia->getColor().name());
				writer.writeAttribute(CL_IA_ATT_ALPHA, QString::number(ia->getColor().alpha()));
				writer.writeAttribute(CL_IA_ATT_TEXT, ia->getText());
				
				QList<QGraphicsItem *> subItems = item->childItems();
				std::sort(subItems.begin(), subItems.end(), graphicItemComparator);
				for (QGraphicsItem* subItem : subItems)
				{
					ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(subItem);
					if (cgitem != 0)
					{
						saveVisualItem(cgitem, writer);
					}
				}
				writer.writeEndElement();
			}
		}
	}
	writer.writeEndElement();
}

//-----------------------------------------------------------------------
void StreamControler::saveVisualDataFromSelection(QXmlStreamWriter & writer, QList<StreamWork::SwCore::SwComponent_Class *> & components)
{
	writer.writeStartElement(CL_SCENE_NODE);
	QRectF srect = _streamScene->sceneRect();
	writer.writeAttribute(CL_SCENE_ATT_X, QString::number(srect.topLeft().x(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_Y, QString::number(srect.topLeft().y(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_WIDTH, QString::number(srect.width(), 'g', 12));
	writer.writeAttribute(CL_SCENE_ATT_HEIGHT, QString::number(srect.height(), 'g', 12));

	QList<QGraphicsItem *> items = _streamScene->items();
	int count = items.count();
	for (int i = 0; i < count; i++)
	{
		QGraphicsItem * item = items[i];
		ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(item);
		if (cgitem != 0 && components.indexOf(cgitem->getComponent()) != -1)
		{
			// Redondance mais pour sécurité
			QPointF pt = item->scenePos();
			item->setParentItem(NULL);
			item->setPos(pt);

			saveVisualItem(cgitem, writer);
		}
	}
	writer.writeEndElement();
}

//-----------------------------------------------------------------------
void StreamControler::saveVisualItem(ComponentGraphicItem * item, QXmlStreamWriter & writer)
{
	writer.writeStartElement(CL_CGITEM_NODE);
	writer.writeAttribute(CL_CGITEM_ATT_NAME, item->getComponent()->GetName());
	writer.writeAttribute(CL_CGITEM_ATT_X, QString::number(item->pos().x(), 'g', 12));
	writer.writeAttribute(CL_CGITEM_ATT_Y, QString::number(item->pos().y(), 'g', 12));
	writer.writeAttribute(CL_CGITEM_ATT_COLOR, item->getColor().name());
	writer.writeAttribute(CL_CGITEM_ATT_TEXT_COLOR,item->getTextColor().name());
	
	//Connector
	QList<ConnectorGraphicItem *> * clist = item->getConnectors();
	for (int i = 0; i < clist->count(); i++)
	{
		writer.writeStartElement(CL_CGCNTITEM_NODE);
		writer.writeAttribute(CL_CGCNTITEM_ATT_NAME, (*clist)[i]->getName());
		writer.writeAttribute(CL_CGCNTITEM_ATT_POS, (*clist)[i]->getParentPosition() == LEFT ? "left" : "right");
		writer.writeEndElement();
	}

	//Enfant
	QList<QGraphicsItem *> items = item->childItems();
	int count = items.count();
	for (int i = 0; i < count; i++)
	{
		ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(items[i]);
		if (cgitem != 0)
		{
			saveVisualItem(cgitem, writer);
		}
	}

	writer.writeEndElement();
}

//-----------------------------------------------------------------------
void StreamControler::loadVisualData(QDomDocument & doc)
{
	QDomElement sceneNode = doc.documentElement().firstChildElement(QString(CL_SCENE_NODE));
	if ( sceneNode.isNull() )
	{
		buildComponents(_rootComponent);
		QList<ComponentGraphicItem *> mainItems = _streamScene->getAllComponentGraphicItems();
		GraphicsLoaderV1::loadGraphicsInfoFromDom(doc, mainItems, _streamScene);
		Arranger arranger;
		arranger.arrangeConnectors(&mainItems);
		return;
	}
	QRectF srect;
	srect.setLeft(sceneNode.attribute(CL_SCENE_ATT_X).toDouble());
	srect.setTop(sceneNode.attribute(CL_SCENE_ATT_Y).toDouble());
	srect.setWidth(sceneNode.attribute(CL_SCENE_ATT_WIDTH).toDouble());
	srect.setHeight(sceneNode.attribute(CL_SCENE_ATT_HEIGHT).toDouble());
	_streamScene->setSceneRect(srect);

	for ( QDomElement node = sceneNode.firstChildElement(QString(CL_CGITEM_NODE));
		 !node.isNull();
		 node = node.nextSiblingElement(QString(CL_CGITEM_NODE)) )
	{
		loadVisualItem(doc, node, _rootComponent, 0);
	}

	for ( QDomElement node = sceneNode.firstChildElement(QString(CL_IA_NODE));
		 !node.isNull();
		 node = node.nextSiblingElement(QString(CL_IA_NODE)) )
	{
		InterestArea * ia = new InterestArea(this);
		QPointF p(node.attribute(CL_IA_ATT_X).toDouble(), node.attribute(CL_IA_ATT_Y).toDouble());
		ia->setPos(p);
		QRectF bbox(0, 0, node.attribute(CL_IA_ATT_W).toDouble(), node.attribute(CL_IA_ATT_H).toDouble());
		ia->setBoundingRect(bbox);
		QColor tmp = QColor(node.attribute(CL_IA_ATT_COLOR));
		int alpha = node.attribute(CL_IA_ATT_ALPHA).toInt();
		if ( alpha == 0 )
			alpha = 150;

		tmp.setAlpha(alpha);
		ia->setColor(tmp);

		ia->setText(node.attribute(CL_IA_ATT_TEXT));
		_streamScene->addItem(ia);


		//Chargement item enfant
		for ( QDomElement cnode = node.firstChildElement(QString(CL_CGITEM_NODE)); !cnode.isNull(); cnode = cnode.nextSiblingElement(QString(CL_CGITEM_NODE)) )
		{
			loadVisualItem(doc, cnode, _rootComponent, ia);
		}
	}
	QRectF bouding = _streamScene->itemsBoundingRect();
	bouding.adjust(-1000, -1000, 1000, 1000);
	_streamScene->setSceneRect(bouding);
}

//-----------------------------------------------------------------------
void StreamControler::loadVisualItem(QDomDocument & doc, QDomElement &node, SwComponent_Class * parentComponent, QGraphicsItem * parentItem)
{
	//Chargement item courant
	QString cname = node.attribute(CL_CGITEM_ATT_NAME);
	SwComponent_Class * component = parentComponent->GetChild(cname);
	if ( component == 0 )
	{
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Fail to find component %1").arg(cname));
		return;
	}

	ComponentGraphicItem * cgitem = new ComponentGraphicItem(component, this);
	QPointF pos;
	pos.setX(node.attribute(CL_CGITEM_ATT_X).toDouble());
	pos.setY(node.attribute(CL_CGITEM_ATT_Y).toDouble());
	cgitem->setPos(pos);
	cgitem->setColor(QColor(node.attribute(CL_CGITEM_ATT_COLOR)));
	cgitem->setTextColor(QColor(node.attribute(CL_CGITEM_ATT_TEXT_COLOR)));

	if ( parentItem )
		cgitem->setParentItem(parentItem);

	if ( parentComponent != _rootComponent )
	{
		QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
		it = _mapCompToItem.find(parentComponent);
		if ( it != _mapCompToItem.end() )
		{
			cgitem->setParentItem((*it));
		}
	}
	_mapCompToItem.insert(component, cgitem);

	if ( parentComponent == _rootComponent && !_streamScene->items().contains(cgitem) )
		_streamScene->addItem(cgitem);

	buildConnectors(component);

	//Chargement de l'ordre des connectors
	QList<QString> labelConnectors;
	QList<TConnectorPosition> labelPositions;
	for ( QDomElement cntnode = node.firstChildElement(QString(CL_CGCNTITEM_NODE));!cntnode.isNull();cntnode = cntnode.nextSiblingElement(QString(CL_CGCNTITEM_NODE)) )
	{
		if (! labelConnectors.contains(cntnode.attribute(CL_CGCNTITEM_ATT_NAME)) )
		{
			labelConnectors.push_back(cntnode.attribute(CL_CGCNTITEM_ATT_NAME));
			labelPositions.push_back(cntnode.attribute(CL_CGCNTITEM_ATT_POS) == "right" ? RIGHT : LEFT);
		}
	}

	QList<ConnectorGraphicItem *> * clist = cgitem->getConnectors();
	QList<ConnectorGraphicItem *> newConnector;



	QList<ConnectorGraphicItem *> original_list = (*clist);
	(*clist).clear();

	for ( int i = 0; i < labelConnectors.count(); i++ )
	{
		for ( int j = 0; j < original_list.count(); j++ )
		{
			if ( original_list[j]->getName() == labelConnectors[i] )
			{
				(*clist).append(original_list[j]);
				(*clist).last()->setParentPosition(labelPositions[i]);
			}
		}
	}

	for ( int j = 0; j < original_list.count(); j++ )
	{
		if ( !(*clist).contains(original_list[j]) )
		{
			original_list[j]->setParentPosition(UNDEFINED);
			(*clist).append(original_list[j]);
		}
	}

// 	QList<int> indexToRemove;
// 	if ( labelConnectors.count() != clist->count() )
// 	{
// 		//On remove les items qui sont pas contenu dedans et on les stock pour la suite
// 		for ( int i = 0; i < clist->count(); i++ )
// 		{
// 			int j = labelConnectors.indexOf((*clist)[i]->getName());
// 			if ( j < 0 )
// 			{
// 				newConnector.append((*clist)[i]);
// 				indexToRemove.push_front(i);
// 			}
// 		}
// 	}
// 
// 	for (auto i : indexToRemove )
// 		(*clist).removeAt(i);
// 
// 
// 	QList<ConnectorGraphicItem *> original_list = (*clist);
// 	(*clist).clear();
// 	for ( int i = 0; i < original_list.count(); i++ )
// 	{
// 		int j = labelConnectors.indexOf(original_list[i]->getName());
// 		if ( j >= 0 )
// 		{
// 			(*clist).last()->setParentPosition(labelPositions[j]);
// 		}
// 		else
// 		{
// 			//newConnector.append(original_list[i]);
// 		}
// 	}
// 
// 	for ( auto * item : newConnector )
// 	{
// 		item->setParentPosition(UNDEFINED);
// 		(*clist).append(item);
// 	}

	cgitem->updateAttributs();

	//Chargement item enfant
	for ( QDomElement cnode = node.firstChildElement(QString(CL_CGITEM_NODE));!cnode.isNull();cnode = cnode.nextSiblingElement(QString(CL_CGITEM_NODE)) )
		loadVisualItem(doc, cnode, component, cgitem);
}

//--------------------------------------------------------------------------
// Tools
//--------------------------------------------------------------------------

//-----------------------------------------------------------------------
ComponentGraphicItem * StreamControler::buildComponents(StreamWork::SwCore::SwComponent_Class * component)
{
	ComponentGraphicItem * cgitem = 0;

	if ( component != _rootComponent )
	{
		cgitem = new ComponentGraphicItem(component, this);
		QPointF pos;
		pos.setX(0.0);
		pos.setY(0.0);
		cgitem->setPos(pos);
		cgitem->setColor(QColor(0, 0, 128));
		cgitem->setTextColor(QColor(255, 255, 255));

		StreamWork::SwCore::SwComponent_Class * parentComponent = component->GetParent();
		QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
		it = _mapCompToItem.find(parentComponent);
		if ( it != _mapCompToItem.end() )
		{
			cgitem->setParentItem((*it));
		}
		_mapCompToItem.insert(component, cgitem);
		if ( parentComponent == _rootComponent )
		{
			_streamScene->addItem(cgitem);
		}
		buildConnectors(component);
	}
	SwComponent_Class * scomponent = component->GetFirstChild();
	int nbChilds = component->GetNbChildren();
	int index = 0;
	ComponentGraphicItem * scgitem;
	while ( scomponent != 0 )
	{
		scgitem = buildComponents(scomponent);
		if ( nbChilds > 1 )
		{
			QPointF pos;
			pos.setX(300.0*cos(3.14*(double) (index) / (double) nbChilds));
			pos.setY(300.0*sin(3.14*(double) (index) / (double) nbChilds));
			scgitem->setPos(pos);
		}
		scomponent = component->GetNextChild();
		index++;
	}
	return cgitem;
}

//-----------------------------------------------------------------------
void StreamControler::buildConnectors(StreamWork::SwCore::SwComponent_Class * component)
{
	//return;
	QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
	it = _mapCompToItem.find(component);
	if ( it == _mapCompToItem.end() )
	{
		return;
	}
	ComponentGraphicItem * gitem = (*it);
	QList<ConnectorGraphicItem *> * connectors = gitem->getConnectors();

	ISwInterfaces_Provider *iprovider = dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	if ( iprovider != 0 )
	{
		QString name = iprovider->GetFirstInterface();
		while ( !name.isNull() )
		{
			QString type = iprovider->GetInterfaceType(name);
			ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, name, type, PROVIDER);
			connectors->push_back(gcitem);
			gcitem->setParentItem(gitem);
			name = iprovider->GetNextInterface();
		}
	}

	ISwInterfaces_Consumer *iconsumer = dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	if ( iconsumer != 0 )
	{
		QString type;
		QString name = iconsumer->GetFirstInterface(&type, 0, 0);
		while ( !name.isNull() )
		{
			ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, name, type, CONSUMER);
			connectors->push_back(gcitem);
			gcitem->setParentItem(gitem);
			name = iconsumer->GetNextInterface(&type, 0, 0);
		}
	}

	ISwPins_Manager *pinManager = dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
	if ( pinManager != 0 )
	{
		QList<SwPin *> plist = pinManager->GetPinList();
		QList<SwPin *>::iterator it = plist.begin();
		while ( it != plist.end() )
		{
			SwPin * pin = (*it);
			ConnectorGraphicItem * gcitem = new ConnectorGraphicItem(gitem, pin->GetName(), pin->GetType(), PIN);
			connectors->push_back(gcitem);
			gcitem->setParentItem(gitem);
			it++;
		}
	}

	gitem->updateAttributs();
}

//-----------------------------------------------------------------------
void StreamControler::buildLinks()
{
	QList<QGraphicsItem *> litems = _streamScene->items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	while ( it != litems.end() )
	{
		ConnectorGraphicItem * citem = dynamic_cast<ConnectorGraphicItem *>(*it);
		if ( citem != 0 )
		{
			//Cas connecteur consommateur
			if ( citem->getConnectorType() == CONSUMER )
			{
				//Recherche de l'interface produite si elle existe
				SwComponent_Class * component = ((ComponentGraphicItem *) citem->parentItem())->getComponent();
				ISwInterfaces_Consumer *iconsumer =
					dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
				if ( iconsumer != 0 )
				{
					ISwInterfaces_Provider * pt_provider = 0;
					QString interface_provider;
					QString model_type;
					QString interface_consumer = iconsumer->GetFirstInterface(&model_type, &pt_provider, &interface_provider);
					while ( !interface_consumer.isNull() && interface_consumer != citem->getName() )
					{
						pt_provider = 0;
						interface_consumer = iconsumer->GetNextInterface(&model_type, &pt_provider, &interface_provider);
					}
					if ( !interface_consumer.isNull() && pt_provider != 0 && citem->getModelType() == model_type )
					{
						//Interface produite trouvé -> recuperation du connecteur associé
						SwComponent_Class * pcomponent = pt_provider->GetHostComponent();
						QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
						it = _mapCompToItem.find(pcomponent);
						if ( it != _mapCompToItem.end() )
						{
							ComponentGraphicItem * gitem = (*it);
							ConnectorGraphicItem * pitem = gitem->getConnector(interface_provider);
							if ( pitem != 0 )
							{
								//Construction du lien
								LinkGraphicItem *lgi = new LinkGraphicItem(citem, pitem);
								_streamScene->addItem(lgi);
								lgi->setZValue(-100.0);
							}
						}
					}
				}
			}
			if ( citem->getConnectorType() == PIN && citem->getLinks()->isEmpty() )
			{
				SwComponent_Class * component = ((ComponentGraphicItem *) citem->parentItem())->getComponent();
				ISwPins_Manager *pinManager = dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
				if ( pinManager != 0 )
				{
					SwPin * pin = pinManager->GetPinByName(citem->getName());
					SwPin * rpin = pin->GetConnected();
					if ( rpin != 0 )
					{
						SwComponent_Class * rcomponent = rpin->GetManager()->GetHostComponent();
						QMap<StreamWork::SwCore::SwComponent_Class *, ComponentGraphicItem *>::iterator it;
						it = _mapCompToItem.find(rcomponent);
						if ( it != _mapCompToItem.end() )
						{
							ConnectorGraphicItem * pitem = (*it)->getConnector(rpin->GetName());
							if ( pitem != 0 )
							{
								//Construction du lien
								LinkGraphicItem *lgi = new LinkGraphicItem(citem, pitem);
								_streamScene->addItem(lgi);
								lgi->setZValue(-100.0);
							}
						}
					}
				}
			}
		}
		it++;
	}
}

//-----------------------------------------------------------------------
void StreamControler::destroyLinks()
{
	QList<QGraphicsItem *> litems = _streamScene->items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	while ( it != litems.end() )
	{
		LinkGraphicItem * lkitem = dynamic_cast<LinkGraphicItem *>(*it);
		if ( lkitem != 0 )
		{
			delete lkitem;
		}
		it++;
	}
}
//-----------------------------------------------------------------------
void StreamControler::addSelectionObserver(ISelectionObserver * observer)
{
	_selectionObservers.push_back(observer);
}

//-----------------------------------------------------------------------
void StreamControler::removeSelectionObserver(ISelectionObserver * observer)
{
	_selectionObservers.removeOne(observer);
}
//-----------------------------------------------------------------------
void StreamControler::addStreamControlerObserver(IStreamControlerObserver * observer)
{
	_streamControlerObservers.push_back(observer);
}

//-----------------------------------------------------------------------
void StreamControler::removeStreamControlerObserver(IStreamControlerObserver * observer)
{
	_streamControlerObservers.removeOne(observer);
}
