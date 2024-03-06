/*!
 \file _SwEditorPinGraphicItem.h
 \brief  Class _SwEditorPinGraphicItem gerant la representation graphique d'un pin dans une vue de dessin QT 
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/

#ifndef __SwEditorPinGraphicItem_H
#define __SwEditorPinGraphicItem_H

/*
  * INCLUDES GLOBAUX
  */
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QSet>
#include <QVector>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "_SwEditorGraphicItem.h"
#include "_ISwEditorGraphicItemListener.h"
#include "_ISwEditorPinGraphicItemListener.h"
#include "_SwEditorInterfacesLayerManager.h"
#include "Component/Services/ISwPins_Manager.h"
#include "_SwRouting_ToolBox.h"

#define CG_SW_EDITOR_PIN_GRAPHIC_ITEM_OFFSET_TYPE 10002

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorPinGraphicItem 
	        \brief Class _SwEditorPinGraphicItem gerant la representation graphique d'un pin dans une vue de dessin QT 
		*/
        class _SwEditorPinGraphicItem : public QGraphicsItem, public _ISwEditorGraphicItemListener, public _ISwEditorPinGraphicItemListener{
        protected:
            SwCore::SwComponent_Class * _comp_host;
            /*! \brief Item hote */
            _SwEditorGraphicItem * _host;
            /*! \brief Nom du pin */
            QString _name;
            /*! \brief Type du pin */
            QString _type_name;
            /*! \brief liste des observers de l'item */
            QSet<_ISwEditorPinGraphicItemListener *> _observers;
            /*! \brief remote item */
            _SwEditorPinGraphicItem * _remote_item;
            /*! \brief StartPort */
            QRectF _start_port;
            /*! \brief StartPort position (centre)*/
            QPointF _start_port_position;
            /*! \brief StartPort position linaire */
            double _start_port_linear_prec_position;
            /*! \brief StartPort position linaire */
            double _start_port_linear_position;
            /*! \brief EndPort position (bord port icone)*/
            QPointF _symbol_start_port_position;
            /*! \brief start port edition*/
            bool _is_start_port_editing;
            /*! \brief EndPort */
            QRectF _end_port;
            /*! \brief EndPort position (centre)*/
            QPointF _end_port_position;
            /*! \brief EndPort position (bord port icone)*/
            QPointF _symbol_end_port_position;
            /*! \brief grabbing mode */
            bool _connecting_mode;
            /*! \brief drawing connection responsable */
            bool _is_connection_draw_responsable;
            /*! \brief zone de texte*/
            QRectF _text_area;
            /*! \brief end port edition*/
            QGraphicsScene * _scene;
            /*! \brief Acces au pins layer manager*/
            _SwEditorPinsLayerManager * _pl_manager;
            /*! \brief flag de mise en evidence*/
            bool _highlight_flag;
            /*! \brief handle sur le service qui permet d'acceder a l'interface */  
            SwCore::ISwPins_Manager * _hosting_service;
            /*! \brief liste des positions des points de connexions*/
            QList<QPointF> _connectables_positions;
            /*! \brief routing*/
            _SwRouting_ToolBox _routing;
            /*! \brief index du point de controle en cours d'edition*/
            int _control_index;
            /*! \brief Position initiale du point de controle*/
            QPointF _initial_control_point;
		public:
            /*! \brief Constructeur */
			_SwEditorPinGraphicItem(_SwEditorGraphicItem * host,QString name,QString type_name,_SwEditorPinsLayerManager * pl_manager,SwCore::ISwPins_Manager * source);
            /*! \brief Destructeur */
			~_SwEditorPinGraphicItem();
             /*! \brief Definition et acces au type de l'item*/
		    enum { Type = UserType + CG_SW_EDITOR_PIN_GRAPHIC_ITEM_OFFSET_TYPE };
		    int type() const { return Type; }
            /*! \brief Permet de connaitre le rectangle englobant l'item graphique
			\return renvoie le rectangle*/
		    QRectF boundingRect() const;
            /*! \brief Dessine l'item*/
		    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
            /*! \brief Sur changement de l'item */
		    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
            /*! \brief Sur pression d'un bouton de la sourie */
		    void mousePressEvent(QGraphicsSceneMouseEvent *event);
            /*! \brief Sur liberation d'un bouton de la sourie */
            void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
            /*! \brief Sur liberation d'un bouton de la sourie */
		    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);	
            /*! \brief Liberation de l'item */
		    void Liberate();	
            /*! \brief Acces au nom du pin */
		    QString GetPinName();	
            /*! \brief Acces au services de pins relatif a ce pin*/
		    SwCore::ISwPins_Manager * GetPinManager();	
		    //-------------------------------------------------------------------
            // Interface _ISwEditorGraphicItemListener
		    //-------------------------------------------------------------------
            /*! \brief Sur  changement de l'item representant le composant*/
	        virtual void OnItemChanged();            
		    //-------------------------------------------------------------------
            // Interface _ISwEditorPinGraphicItemListener
		    //-------------------------------------------------------------------
	        /*! \brief Sur  changement de l'item representant le pin*/
	        virtual void OnPinItemChange(_SwEditorPinGraphicItem * source);            
            //---------------------------------------------------------------------
            // Definition du remote interface item
            //---------------------------------------------------------------------            
            /*! \brief Definition du remote interface item */
            void SetRemoteItem(_SwEditorPinGraphicItem * remote_item);
            /*! \brief Acces au remote pin item */
            _SwEditorPinGraphicItem * GetRemoteItem();
            //---------------------------------------------------------------------
            // Gestion des listeners
            //---------------------------------------------------------------------            
            /*! \brief Ajout d'un listener */
            void Attach_Listener(_ISwEditorPinGraphicItemListener * listener);
            /*! \brief Retrait d'un listener */
            void Detach_Listener(_ISwEditorPinGraphicItemListener * listener);
            /*! \brief Signaler aux listener les changements */
            void SignalChangeToListeners(_SwEditorPinGraphicItem * source);
            //---------------------------------------------------------------------
            // Gestion du positionnement
            //---------------------------------------------------------------------
            /*! \brief Permet de definir la position linaire du port de l'interface (sur le bord du composant)
            \param[in] pos Position linaire (comprise entre 0 et 1)*/
            void SetPortLinearPosition(double pos);
            /*! \brief Permet de connaitre la position linaire du port de l'interface (sur le bord du composant)
            \return Position linaire (comprise entre 0 et 1)*/
            double GetPortLinearPosition();
            /*! \brief Permet de connaitre la position linaire du port de l'interface s'approchant le plus
            de la position passé en parametres*/
            QPointF GetPortNearestValidPosition(const QPointF & pos,double * reminding_linear_position);
            /*! \brief Permet de definir la position avec recalculation de la position linaire*/
            void SetPortPosition(const QPointF & pos, bool reprocess_linear_position=true);
            /*! \brief Permet de definir la position du end port*/
            void SetEndPosition(const QPointF & pos,_SwEditorPinGraphicItem * potential_remote_item=NULL);
            /*! \brief Permet de definir la position du end port en fonction de l'item distant*/
            void SetEndPositionDeduceFromRemoteItem();
            /*! \brief Permet de resetter la position de end*/
            void ResetEndPosition();
            /*! \brief Permet de connaitre la position finale correspondante*/
            QPointF GetEndPosition();
            /*! \brief Permet de definir le dessin du chemin de connexion*/
            void BuildDrawPath(const QPointF & pos,_SwEditorPinGraphicItem * potential_remote_item=NULL);
            /*! \brief Permet de mettre a jour le dessin du chemin de connexion*/
            void UpdateDrawPath(const QPointF & pos);
            /*! \brief Acces au données de routage*/
            _SwRouting_ToolBox * GetRouting();
            //---------------------------------------------------------------------
            // Evenement visibilite interface
            //---------------------------------------------------------------------
            /*! \brief Reception de la visibilite d'un type de pin*/
            void OnPinVisibilityChange(const QString * pin_type_name,bool new_visibility);
            /*! \brief Reception de la visibilite d'un type de pin*/
            void OnPinHighlightChange(const QString * pin_type_name,bool set_highlight);
            //---------------------------------------------------------------------
            // Gestion menu
            //---------------------------------------------------------------------
	        /*! \brief Affiche le popup relatif a l'index navigable*/
            void ShowPopup(const QPoint &globalpos);
    	};
	}
}

#endif
