/*!
 \file _SwEditorGraphicScene.h
 \brief Implementation of the Class _SwEditorGraphicScene gerant la scene represenant un stream dans une vue de dessin QT 
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/

#ifndef __SwEditorGraphicScene_H
#define __SwEditorGraphicScene_H

/*
  * INCLUDES GLOBAUX
  */
#include <QGraphicsScene>
/*
  * INCLUDES LOCAUX
  */
#include "ISwEditorGraphicScene.h"

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorGraphicScene 
	        \brief Implementation of the Class _SwEditorGraphicScene gerant la scene represenant un stream dans une vue de dessin QT 
		*/
        class _SwEditorGraphicScene : public QGraphicsScene, public ISwEditorGraphicScene {
			Q_OBJECT
		protected:
            /*! \brief Composant parent */
            SwCore::SwComponent_Class * _host_comp;
		protected:
            /*! \brief Destructeur */
			~_SwEditorGraphicScene();
           //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();   
		public:
            /*! \brief Constructeur */
			_SwEditorGraphicScene(SwCore::SwComponent_Class * host_comp);
            //---------------------------------------------------------------------
            // Interface ISwEditorGraphicScene
            //---------------------------------------------------------------------
            /*! \brief Fournit la scene graphique qt correspondant
            \return Renvoie un pointeur sur la scene graphique qt correspondant */
            virtual QGraphicsScene * GetRelatedScene();
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

		};
	}
}

#endif