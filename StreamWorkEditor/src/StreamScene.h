/**
@file StreamScene.h
@brief Scene Stream
@author F.Bighelli
*/

#ifndef _STREAMSCENE_H
#define _STREAMSCENE_H

#include <QtCore>
#include <QtWidgets>
#include <QtOpenGL>
#include <QGraphicsScene>
#include "IAnchorableObserver.h"
#include "IAnchorable.h"
#include "LinkGraphicItem.h"
#include "ComponentGraphicItem.h"
#include "InterestArea.h"

/**
@class StreamScene
@brief Scene Stream
*/
class StreamScene : public QGraphicsScene, virtual public IAnchorableObserver
{
	Q_OBJECT
public:
	/** @brief Constructor */
	StreamScene(QObject * parent = 0);

	/** @brief Fonction de dessin du fond en opengl */
	virtual void drawBackground(QPainter *painter, const QRectF &rect);

	/** @brief Mise a jour de la zone visible */
	void setVisibleArea(const QRectF & visibleArea);

	/** @brief ajout d'un widget */
	IAnchorable * addApplicativeWidget(QWidget *w);
	//--------------------------------------------------------------------------
	// IAnchorableObserver
	//--------------------------------------------------------------------------
	/*@brief indique que l'entité a changé */
	virtual void anchorableAsChanged(QWidget *w);

	//--------------------------------------------------------------------------
	// Evenement de la scene
	//--------------------------------------------------------------------------
	/** @brief sur mouvement de la mouse */
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
	/** @brief sur press de la mouse */
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
	/** @brief sur release de la mouse */
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

	//--------------------------------------------------------------------------
	// Edition
	//--------------------------------------------------------------------------
	/** @brief rearrange */
	void rearrange();

	//--------------------------------------------------------------------------
	// Tools
	//--------------------------------------------------------------------------
	/** @brief renvoie la liste des graphics components sans enfants */
	QList<ComponentGraphicItem *> getMainComponentGraphicItems();
	/** @brief renvoie la liste des graphics components */
	QList<ComponentGraphicItem *> getAllComponentGraphicItems();
	/** @brief renvoie la liste des zones d'interet */
	QList<InterestArea *> getAllInterestAreas();

signals:
	/** @brief sur demande de connection */
	void onLinkConnectors(ConnectorGraphicItem * src, ConnectorGraphicItem * target);

private:
	/** @brief Mise a jour des widgets */
	void updateWidgets();
	/** @brief rends visible seulement les connectors compatibles*/
	void showOnlyCompatiblesItems(ConnectorGraphicItem * itemSource);
	/** @brief rends visible seulement les connectors dont le parent est identiques*/
	void showOnlyItemsWithSameParent(ConnectorGraphicItem * itemSource);
	/** @brief rends tout visible*/
	void showAllItems(ConnectorGraphicItem * itemSource);
	bool eventFilter(QObject *obj, QEvent *event);

private:
	/** @brief Rectangle representant la zone visible de la scene */
	QRectF _visibleArea;
	/** @brief Widget list */
	QList<QGraphicsProxyWidget *> _wList;
	/** @brief desactivation observer */
	bool _observerEnabled;
	/** @brief Widget plugin */
	QGraphicsProxyWidget * _pluginListWidget;
	/** @brief Item Temporaire */
	LinkGraphicItem * _tmpLinkItem;
	/** @brief Connector source of link */
	ConnectorGraphicItem * _linkSrc;
	/** @brief Connector target of link */
	ConnectorGraphicItem * _linkTarget;
	/** @brief Connector de depalcement */
	ConnectorGraphicItem * _connectorToMove;
	/** @brief Connector cible temporaire */
	ConnectorGraphicItem * _tmpTargetConnector;

	QGraphicsLineItem *_lineX;
	QGraphicsLineItem *_lineY;
};

#endif
