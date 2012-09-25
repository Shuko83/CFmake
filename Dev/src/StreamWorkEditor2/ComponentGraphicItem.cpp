/**
@file ComponentGraphicItem.cpp
@brief Item Graphique correspondant à un composant
@author F.Bighelli
 */

#include "ComponentGraphicItem.h"
#include "GraphicsResources.h"
#include "StreamControler.h"
#include "ISwSupportReplay.h"
#include "ISwAdminSetup.h"
#include "ISwSubStream.h"
#include "ISwPluginOverview.h"
#include "MainWindow.h"
#include "ManageLinkColor.h"

#define CL_RADUIS 10.0

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/** @brief Constructor */
ComponentGraphicItem::ComponentGraphicItem(SwComponent_Class * component,StreamControler * controler):
    QGraphicsItem(),_component(component),_controler(controler) {
    _executable=dynamic_cast<ISwExecutable_Service *>(component->QueryService(CG_SW_SERVICE_EXECUTABLE));
    _executor=dynamic_cast<ISwExecution_Service *>(component->QueryService(CG_SW_SERVICE_EXECUTION));

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
#if QT_VERSION >= 0x040600
    setFlag(ItemSendsGeometryChanges);
#endif
    ISwService * service=component->QueryService(CG_SW_SERVICE_PLUGIN_OVERVIEW);
    if (service!=NULL && dynamic_cast<ISwPluginOverview *>(service)!=NULL) {
        ISwPluginOverview * poverview=dynamic_cast<ISwPluginOverview *>(service);
        QString texte=component->GetFactoryComponentName()+ " [Factory: "+poverview->GetPath()+"/"+poverview->GetPluginName()+ "] [Version:"+ poverview->GetPluginVersion() +"]";
    } else {
        setToolTip(component->GetFactoryComponentName());
    }

    _bodyBrush=QBrush(QColor(255,255,255,50));
    _pen=QPen(QColor(0,0,0));
    _pen.setJoinStyle(Qt::RoundJoin);
    _selected_pen=QPen(QColor(255,255,255));
    _selected_pen.setWidthF(3.2);
    _selected_pen.setJoinStyle(Qt::RoundJoin);
    _text_pen=QPen(TEXT_COLOR);
    _icone=GraphicsResources::getInstance()->getComponentIcon(component);
    _bbox=QRectF();
    updateAttributs();
	setColor(HEADER_COLOR);
    _connectionInsertionPositionDisplay=false;

}
/** @brief Acces au composant */
StreamWork::SwCore::SwComponent_Class * ComponentGraphicItem::getComponent() const{
    return _component;
}

/** @brief Definition du rectangle englobant */
QRectF ComponentGraphicItem::boundingRect() const{
    return _bbox;
}
/** @brief Fonction de dessin */
void ComponentGraphicItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->setPen(_selected_pen);
    } else {
        painter->setPen(_pen);
    }
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setBrush(_headerBrush);
    painter->drawPath(_header_path);
    painter->setBrush(_bodyBrush);
    painter->drawPath(_body_path);
    painter->setRenderHints(QPainter::Antialiasing,false);
    painter->drawPixmap(_bbox.topLeft()+QPointF(CL_RADUIS,2.0),_icone);
    painter->setPen(_text_pen);
    painter->drawText(_bbox.topLeft()+QPointF(CL_RADUIS+_icone.width()+2.0,_header_height-6.0),_component->GetName());
    QFontMetrics fm(QApplication::fontMetrics());
    if (_executable!=0 || _executor!=0) {
        if ((_executable!=0 &&_executable->isRunning()) ||
            (_executor!=0 && !_executor->IsExecutionStopped()) 
            ){
            ISwSupportReplay *sreplay=dynamic_cast<ISwSupportReplay *>(_component);
            if (sreplay!=0 && sreplay->getReplayMode()) {
                painter->drawPixmap(
                    _bbox.topLeft()+QPointF(CL_RADUIS+_icone.width()+4.0+(qreal)fm.width(_component->GetName()),3.0),
                    GraphicsResources::getInstance()->getReplayIcon());
            } else {
                painter->drawPixmap(
                    _bbox.topLeft()+QPointF(CL_RADUIS+_icone.width()+4.0+(qreal)fm.width(_component->GetName()),3.0),
                    GraphicsResources::getInstance()->getRunIcon());
            }
        } else {
            painter->drawPixmap(
                _bbox.topLeft()+QPointF(CL_RADUIS+_icone.width()+4.0+(qreal)fm.width(_component->GetName()),3.0),
                GraphicsResources::getInstance()->getRunIconDisabled());
        }
    } 
    if (!_component->isActive()) {
        painter->drawPixmap(
                    _bbox.topLeft()+QPointF(CL_RADUIS+_icone.width()+5.0+(qreal)fm.width(_component->GetName())+(qreal)GraphicsResources::getInstance()->getRunIcon().width(),4.0),
                    GraphicsResources::getInstance()->getDesactiveIcon());
    }
    if (_connectionInsertionPositionDisplay) {
        painter->setPen(_selected_pen);
        painter->drawLine(_connectionInsertionPosition.x()-CL_CONNECTOR_BBSIZE/3.0,
                          _connectionInsertionPosition.y(),
                          _connectionInsertionPosition.x()+CL_CONNECTOR_BBSIZE/3.0,
                          _connectionInsertionPosition.y());
    }
    _dbg_cname=_component->GetName();
}
/** @brief mise a jour des attributs de l'item */
void ComponentGraphicItem::updateAttributs() {
    QPointF p;
    //Calcul de la taille
    qreal width;
    qreal height;
    qreal half_width;
    qreal half_height;


	foreach(ConnectorGraphicItem * connec, _connectors)
	{
		if(connec->getConnectorType() == PROVIDER)
		{
			setColor(ManageLinkColor::getInstance()->getColorForInterface(connec->getModelType()));
			break;
		}
	}

    arrangeConnectors();
    prepareGeometryChange(); 
    QFontMetrics fm(QApplication::fontMetrics());
    //Calcul de la largeur : raduis+icone_composant+icone_run+name_component+raduis
    // Avec un intervalle de 1
    width=2.0*CL_RADUIS+4.0;
    width+=(qreal)_icone.width()+(qreal)GraphicsResources::getInstance()->getRunIcon().width()+1+(qreal)GraphicsResources::getInstance()->getDesactiveIcon().width();
    width+=(qreal)fm.width(_component->GetName());
    //Calcul de la hauteur de l'entete
    _header_height=qMax(fm.height(),_icone.height())+4.0;
    //Calcul de la hauteur
    int max_count=qMax(_leftCount,_rightCount);
	height=_header_height+((max_count<0.5)?CL_RADUIS:3)+CL_CONNECTOR_BBSIZE*(qreal)(max_count);
    //height=_header_height+CL_RADUIS+CL_CONNECTOR_BBSIZE*2.0;

    if (fmod(width,2.0)>0.5) {
        width+=1.0;
    }
    if (fmod(height,2.0)>0.5) {
        height+=1.0;
    }
    half_width=width/2.0;
        
    half_height=height/2.0;
    //Calcul de la bounding box
    _bbox=QRectF(-half_width,-half_height,width,height);
    
	//Change grandiant size
	_grad.setStart(0,-half_height);
	_grad.setFinalStop (0,-half_height+_header_height);
	setColor(_headerBrush.color());

    //Calcul du path de l'entete
    _header_path=QPainterPath();
    _header_path.moveTo(-half_width,-half_height+_header_height);
    _header_path.lineTo(-half_width,-half_height+CL_RADUIS);
    _header_path.quadTo(-half_width,-half_height,-half_width+CL_RADUIS,-half_height);
    _header_path.lineTo(half_width-CL_RADUIS,-half_height);
    _header_path.quadTo(half_width,-half_height,half_width,-half_height+CL_RADUIS);
    _header_path.lineTo(half_width,-half_height+_header_height);
    //_header_path.lineTo(-half_width,-half_height+_header_height);
    //Calcul du path du corps
    _body_path=QPainterPath();
    _body_path.moveTo(half_width,-half_height+_header_height);
    _body_path.lineTo(half_width,half_height-CL_RADUIS);
    _body_path.quadTo(half_width,half_height,half_width-CL_RADUIS,half_height);
    _body_path.lineTo(-half_width+CL_RADUIS,half_height);
    _body_path.quadTo(-half_width,half_height,-half_width,half_height-CL_RADUIS);
    _body_path.lineTo(-half_width,-half_height+_header_height);
    //Positionnement des connecteurs
    int leftIndex=0;
    int rightIndex=0;
    for(int i=0;i<_connectors.count();i++) {
        TConnectorPosition cpos=_connectors[i]->getParentPosition();
        switch(cpos) {
            case LEFT:
                p.setX(-half_width+CL_CONNECTOR_BBSIZE/2.0);
                p.setY(-half_height+_header_height+leftIndex*CL_CONNECTOR_BBSIZE+CL_CONNECTOR_BBSIZE/2.0);
                leftIndex++;
                break;
            case RIGHT:
                p.setX(+half_width-CL_CONNECTOR_BBSIZE/2.0);
                p.setY(-half_height+_header_height+rightIndex*CL_CONNECTOR_BBSIZE+CL_CONNECTOR_BBSIZE/2.0);
                rightIndex++;
                break;
            default:
                break;
        }
        _connectors[i]->setPos(p);
    }
    updateLinks();
}
/** @brief arrange des connecteurs */
void ComponentGraphicItem::arrangeConnectors() {
    _leftCount=0;
    _rightCount=0;
    for(int i=0;i<_connectors.count();i++) {
        TConnectorPosition cpos=_connectors[i]->getParentPosition();
        switch(cpos) {
            case LEFT:
                _leftCount++;
                break;
            case RIGHT:
                _rightCount++;
                break;
            default:
                break;
        }
    }
    for(int i=0;i<_connectors.count();i++) {
        TConnectorPosition cpos=_connectors[i]->getParentPosition();
        switch(cpos) {
            case LEFT:
            case RIGHT:
                break;
            default:
                if (_leftCount<_rightCount) {
                    _connectors[i]->setParentPosition(LEFT);
                    _leftCount++;
                } else {
                    _connectors[i]->setParentPosition(RIGHT);
                    _rightCount++;
                }
                break;
        }
    }
}
/** @brief show connector insertion */
void ComponentGraphicItem::showConnectorInsertion(QPointF pci) {
    _connectionInsertionPositionDisplay=true;
    _connectionInsertionIndex=-1;
    for(int i=0;i<_connectors.count();i++) {
        TConnectorPosition cpos=_connectors[i]->getParentPosition();
        switch(cpos) {
            case LEFT:
                if (pci.x()<0) {
                    _connectionInsertionIndex=i;
                    _connectionInsertionPosition.setX(_connectors[i]->pos().x());
                    if(pci.y()<(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0)) {
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0);
                        update();
                        return;
                    } else {
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()+CL_CONNECTOR_BBSIZE/2.0);
                        _connectionInsertionIndex++;
                    }
                } else {
                    if(_connectionInsertionIndex==-1) {
                        _connectionInsertionIndex=0;
                        _connectionInsertionPosition.setX(-_connectors[i]->pos().x());
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0);
                    }
                }
                break;
            case RIGHT:
                if (pci.x()>=0) {
                    _connectionInsertionIndex=i;
                    _connectionInsertionPosition.setX(_connectors[i]->pos().x());
                    if(pci.y()<(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0)) {
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0);
                        update();
                        return;
                    } else {
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()+CL_CONNECTOR_BBSIZE/2.0);
                        _connectionInsertionIndex++;
                    }
                } else {
                    if(_connectionInsertionIndex==-1) {
                        _connectionInsertionIndex=0;
                        _connectionInsertionPosition.setX(-_connectors[i]->pos().x());
                        _connectionInsertionPosition.setY(_connectors[i]->pos().y()-CL_CONNECTOR_BBSIZE/2.0);
                    }
                }
                break;
            default:
                break;
        }
    }
    update();

}
/** @brief hide connector insertion */
void ComponentGraphicItem::hideConnectorInsertion() {
    if (!_connectionInsertionPositionDisplay)
        return;
    _connectionInsertionPositionDisplay=false;
    update();
}
/** @brief move connector  */
void ComponentGraphicItem::moveConnector(ConnectorGraphicItem *citem) {
    int old_index=_connectors.indexOf(citem);
    if (_connectionInsertionIndex<=old_index) {
        old_index++;
    }
    if (_connectionInsertionIndex>=_connectors.count()) {
        _connectors.push_back(citem);
    } else {
        _connectors.insert(_connectionInsertionIndex,citem);
    }
    _connectors.removeAt(old_index);
    if (_connectionInsertionPosition.x()<0) {
        citem->setParentPosition(LEFT);
    } else {
        citem->setParentPosition(RIGHT);
    }
    updateAttributs();
}
/** @brief acces au connecteurs */
QList<ConnectorGraphicItem *> * ComponentGraphicItem::getConnectors() 
{
    return &_connectors;
}
/** @brief acces au composants enfants */
QList<ComponentGraphicItem *> ComponentGraphicItem::getChildsComponents() {
    QList<QGraphicsItem *> litems=childItems();
    QList<QGraphicsItem *>::iterator it=litems.begin();
    QList<ComponentGraphicItem *> lcitems;
    while (it!=litems.end()) {
        ComponentGraphicItem *citem=dynamic_cast<ComponentGraphicItem *>(*it);
        if (citem!=00) {
            lcitems.push_back(citem);
        }
        it++;
    }
    return lcitems;

}
/** @brief definition de la couleur */
QColor ComponentGraphicItem::getColor() {
    return _headerBrush.color();
}
/** @brief acces a la couleur */
void ComponentGraphicItem::setColor(QColor c) {
    _headerBrush.setColor(c);
	_grad.setColorAt(0, _headerBrush.color());
	_grad.setColorAt(0.5,_headerBrush.color());
	_grad.setColorAt(1,_headerBrush.color().darker());
	_headerBrush=QBrush(_grad);
	_headerBrush.setColor(c);

	if(c.red()*0.3 + 0.59 * c.green() + 0.11 * c.blue() >= 100)
	{
		_text_pen = QPen(QColor("black"));
	}
	else
	{
		_text_pen = QPen(QColor("white"));
	}
	
}
/** @brief definition de la couleur de texte */
QColor ComponentGraphicItem::getTextColor() {
    return _text_pen.color();
}
/** @brief acces a la couleur de texte*/
void ComponentGraphicItem::setTextColor(QColor c) {
     _text_pen=QPen(c);
}
/** @brief sur changement de l'item */
QVariant ComponentGraphicItem::itemChange ( GraphicsItemChange change, const QVariant & value ) {
    QVariant result=QGraphicsItem::itemChange(change, value);

    if ((change == ItemPositionChange || change == ItemPositionHasChanged) && scene()!=0) 
	{
        // value is the new position.
         QRectF irect = mapToScene(_bbox | childrenBoundingRect()).boundingRect();
         QRectF srect = scene()->sceneRect();
         if (!srect.contains(irect)) 
		 {
            srect = srect | irect;
            scene()->setSceneRect(srect);
		 }

         updateLinks();
    }
    return result;
}
/** @brief sur release sourie */
void ComponentGraphicItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mouseReleaseEvent(event);

	//Try to reparent if the item is over InterestArea
	{
		QRectF irect = mapToScene(_bbox | childrenBoundingRect()).boundingRect();
		bool found = false;
		foreach (QGraphicsItem *item, scene()->items()) 
		{
			if (item != parentItem() && item != this && dynamic_cast< InterestArea* >(item) && item->mapRectToScene(item->boundingRect()).contains(irect)) 
			{
				setParentItem(item);
				setPos(item->mapFromScene(pos()));
				//qDebug() << "Reparent";
				found =  true;
				 updateLinks();
			}
		}

		if(found == false && parentItem() != NULL)
		{
			setPos(parentItem()->mapToScene(pos()));
			setParentItem(NULL);
			//qDebug() << "Set parent NULL";
			 updateLinks();
		}

	}
	
    _controler->streamControlerChanged();
}
/** @brief sur double click */
void ComponentGraphicItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mouseDoubleClickEvent(event);
    ISwAdminSetup * admin=
                dynamic_cast<ISwAdminSetup *>(_component);
    if(admin!=0) {
        admin->AdminSetup();    
    }
    ISwSubStream * subStream = dynamic_cast<ISwSubStream *>(_component);
    if(subStream!=0 && !subStream->getSubStreamPath().isEmpty()) 
	{
        MainWindow *window=MainWindow::getEditors()->value(subStream->getSubStream(),0);
        if(window==0) 
		{
            StreamWork::SwCore::SwComponent_Class * rootComponent=_component;
            while (rootComponent->GetParent()!=0) 
			{
                rootComponent=rootComponent->GetParent();
            }
            window=new MainWindow;
            window->onLoadExistingStream(subStream->getSubStream(),subStream->getSubStreamPath(),rootComponent);
            window->showMaximized();
        } 
		else 
		{
            if (window->isMinimized()) 
			{
                window->showMaximized();
            }
        }
        window->activateWindow();
    }

	//possible ajout de show sur double click sur un composant qui provide un widget
}

/** @brief updateLinks */
void ComponentGraphicItem::updateLinks() {
    QList<QGraphicsItem *> lchilds=childItems();
    QList<QGraphicsItem *>::iterator it=lchilds.begin();
    while(it!=lchilds.end()) {
        ConnectorGraphicItem * citem=dynamic_cast<ConnectorGraphicItem *>(*it);
        if (citem!=0) {
            citem->updateLinks();
        } else {
            ComponentGraphicItem * cpitem=dynamic_cast<ComponentGraphicItem *>(*it);
            if (cpitem!=0) {
                cpitem->updateLinks();
            }
        }
        it++;
    }
}   
/** @brief acces a un connecteur */
ConnectorGraphicItem * ComponentGraphicItem::getConnector(QString name) {
    QList<ConnectorGraphicItem *>::iterator it=_connectors.begin();
    while(it!=_connectors.end()) {
        if ((*it)->getName()==name)
            return (*it);
        it++;
    }
    return 0;
}
//--------------------------------------------------------------------------
// Methodes pour le rangement 
//--------------------------------------------------------------------------
/** @brief est Producteur*/
bool  ComponentGraphicItem::isProducer() {
    return false;
}
/** @brief est Producteur */
bool  ComponentGraphicItem::isProducerButIsntConsumed() {
    return false;
}
