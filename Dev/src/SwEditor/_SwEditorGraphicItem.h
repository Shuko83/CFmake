/*!
 \file _SwEditorGraphicItem.h
 \brief Implementation of the Class _SwEditorGraphicItem gerant la representation graphique d'un composant dans une vue de dessin QT 
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/

#ifndef __SwEditorGraphicItem_H
#define __SwEditorGraphicItem_H

/*
  * INCLUDES GLOBAUX
  */
#include <QRectF>
//#include <QGraphicsRectItem>
//#include <QGraphicsTextItem>
#include <QPixmap>
#include <QSet>
/*
  * INCLUDES LOCAUX
  */

#include "_SwEditorInterfacesLayerManager.h"
#include "_SwEditorPinsLayerManager.h"
#include "ISwEditorGraphicItem.h"
#include "ISwEditorStreamNavigation.h"
#include "ISwEditorStreamOperations.h"
#include <ISwObserver.h>
#include "_ISwEditorGraphicItemListener.h"
#include <ISwFinalizer.h>
#include <ISwExecution_Service.h>
#include <ISwExecutable_Service.h>

namespace StreamWork
{
	namespace SwEditor
	{
        class _SwEditorInterfacesGraphicItemManager;
        class _SwEditorPinGraphicItemManager;
        /*!
	        \class _SwEditorGraphicItem 
	        \brief Implementation of the Class _SwEditorGraphicItem gerant la representation graphique d'un composant dans une vue de dessin QT
		*/
        class _SwEditorGraphicItem : public QGraphicsItem, public ISwEditorGraphicItem, public SwCore::ISwObserver, public SwCore::ISwFinalizer {
		protected:
            /*! \brief Composant parent */
            SwCore::SwComponent_Class * _host_comp;
            /*! \brief rectangle englobant */
            QRectF _bbox;
            /*! \brief rectangle englobant precedent */
            QRectF _prec_bbox;
            /*! \brief rectangle ombre */
            QRectF _shadow_bbox;
            /*! \brief rectangle englobant les enfant lors du retaillage */
            QRectF _child_bbox;
            /*! \brief rectangle englobant les freres selectionne lors du retaillage */
            QRectF _brothers_bbox;
            /*! \brief rectangle englobant header*/
            QRectF _header_bbox;
            /*! \brief rectangle de retaillage*/
            QRectF _resizer_bbox;
            /*! \brief le button gauche est presse dans le header*/
            bool _pressed_left_button_on_header;
            /*! \brief le button gauche est presse dans le bouton de retaillage*/
            bool _pressed_left_button_on_resizer;
            /*! \brief navigation*/
            ISwEditorStreamNavigation * _navigation;
            /*! \brief navigation*/
            ISwEditorStreamOperations * _operations;
            /*! \brief Indique que le composant est l'index de navigation*/
            bool _isNavigableIndex;
            /*! \brief Autorise la mise a jour de la selection avec itemChange*/
            bool _itemSelectedChangeToComponentSelected;
            /*! \brief id icon */
            QPixmap _icone;
            /*! \brief id icon_run */
            QPixmap _icone_run;
            /*! \brief id icon_run_disable */
            QPixmap _icone_run_disable;
            /*! \brief liste des observers de l'item */
            QSet<_ISwEditorGraphicItemListener *> _observers;
            /*! \brief maganer graphique des interfaces */
            _SwEditorInterfacesGraphicItemManager * _interface_managers;
            /*! \brief maganer graphique des pins */
            _SwEditorPinGraphicItemManager * _pin_managers;
            /*! \brief Acces a l'interface layer manager*/
            _SwEditorInterfacesLayerManager * _il_manager;
            /*! \brief Acces au pin layer manager*/
            _SwEditorPinsLayerManager * _pl_manager;
            /*! \brief index de finalisation de l'interface manager*/
            quint64 _interface_managers_history_index;
            /*! \brief index de finalisation du pins manager*/
            quint64 _pin_managers_history_index;
            /*! \brief Execution service*/
            SwExecution::ISwExecution_Service * _exe_serv;
            /*! \brief Executable service*/
            SwExecution::ISwExecutable_Service * _exable_serv;
		protected:
            /*! \brief Destructeur */
			~_SwEditorGraphicItem();
            /*! \brief Sur changement de l'item */
		    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
            /*! \brief Sur pression d'un bouton de la sourie */
		    void mousePressEvent(QGraphicsSceneMouseEvent *event);
            /*! \brief Sur liberation d'un bouton de la sourie */
            void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
            /*! \brief Sur liberation d'un bouton de la sourie */
		    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);	
            /*! \brief ajustement de la taille de l'item parent pour qu'il contienne toujours l'item */
            void AdjustParentItemSize();
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(); 
            //---------------------------------------------------------------------
            // Gestion de drag and drop
            //---------------------------------------------------------------------
            /*! \brief Sur propostion d'un element dragge*/           
            virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
             /*! \brief Sur drop event*/           
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
		public:
            /*! \brief Constructeur */
            _SwEditorGraphicItem(SwCore::SwComponent_Class * host_comp);
            /*! \brief Initialisation */
            void Initialize(ISwEditorStreamNavigation * nav,ISwEditorStreamOperations *op,_SwEditorInterfacesLayerManager * il_manager,_SwEditorPinsLayerManager * pl_manager);
             /*! \brief Definition et acces au type de l'item*/
		    enum { Type = UserType + CG_SW_EDITOR_GRAPHIC_ITEM_OFFSET_TYPE };
		    int type() const { return Type; }
            /*! \brief Permet de connaitre le rectangle englobant l'item graphique
            \return renvoie le rectangle*/
		    QRectF boundingRect() const;
           /*! \brief Permet de connaitre la zone de positionnement des connecteurs d'interfaces
            \return renvoie le rectangle*/
		    QRectF connectorsRect() const;
            /*! \brief Dessine l'item*/
		    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
            //---------------------------------------------------------------------
            // Gestion des listeners
            //---------------------------------------------------------------------            
	        /*! \brief Ajout d'un listener */
	        virtual void Attach_Listener(_ISwEditorGraphicItemListener * listener);            
	        /*! \brief Retrait d'un listener */
	        virtual void Detach_Listener(_ISwEditorGraphicItemListener * listener);   
	        /*! \brief Signaler aux listener les changements */
	        virtual void SignalChangeToListeners();   
            //---------------------------------------------------------------------
            // Interface ISwEditorGraphicItem
            //---------------------------------------------------------------------
            /*! \brief Fournit l'item graphique qt correspondant
            \return Renvoie un pointeur sur l'item graphique qt correspondant */
            QGraphicsItem * GetRelatedItem();
            /*! \brief resize de l'item */
            void ResizeItem(QRectF rect,bool propagate_to_parent=false);
            /*! \brief autorise ou non la mise a jour de la selection par itemChange */
            void EnableSelectionUpdateByItemChange(bool enable);
            /*! \brief Acces a l'interface manager */
            ISwEditorInterfacesGraphicItemManager * GetInterfacesManager();
            /*! \brief Acces au pin manager */
            ISwEditorPinGraphicItemManager * GetPinsManager();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief acces a son composant hote */
            SwCore::SwComponent_Class * GetHostComponent();            
            //---------------------------------------------------------------------
            // Interface ISwPersistent
            //---------------------------------------------------------------------
	        /*! \brief methode permettant de charger des donnees */
	        void Load(QDomElement & elt,SwCore::ISwFinalizerManager & finalizer_manager);
	        /*! \brief methode permettant de sauver des donnees */
	        void Save(QDomElement & elt,QDomDocument & doc);
             //---------------------------------------------------------------------
            // Interface ISwFinalizer
            //---------------------------------------------------------------------            
             /*! \brief finalize l'operation correspondant a l'index d'historique
            \return false si la finalisation n'a pas eu lieu et true si ok*/
	        bool Finalize(quint64 historic_index);
            //---------------------------------------------------------------------
            // Sur changement du parent
            //---------------------------------------------------------------------
	        /*! \brief sur changement du parent du composant*/
            void OnComponentParentChange();
            //---------------------------------------------------------------------
            // Sur changement du nom
            //---------------------------------------------------------------------
	        /*! \brief sur changement du nom du composant*/
            void OnComponentNameChange();
            //---------------------------------------------------------------------
            // Sur mise a jour
            //---------------------------------------------------------------------
	        /*! \brief methode appelée par l'observable*/
	        void Update();
	        /*! \brief Active le capacite de selection des items dont le composant est selectionnable*/
	        void SetSelectionModeOn();
	        /*! \brief Desactive le capacite de selection des items dont le composant n'est selectionnable*/
	        void SetSelectionModeOff();
            //---------------------------------------------------------------------
            // Gestion menu
            //---------------------------------------------------------------------
	        /*! \brief Affiche le popup relatif a l'index navigable*/
            void ShowPopup(const QPoint &globalpos);
		};
	}
}

#endif