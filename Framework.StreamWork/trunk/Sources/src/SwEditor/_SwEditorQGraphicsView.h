/*!
 \file _SwEditorQGraphicsView.h
 \brief implementation QGraphicsView pour l'editeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorQGraphicsView_H
#define _SwEditorQGraphicsView_H

#include <QGraphicsView>

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorQGraphicsView 
	        \brief implementation QGraphicsView pour l'editeur
        */
        class _SwEditorQGraphicsView : public QGraphicsView
        {
	        Q_OBJECT
        private:
            /* \brief Evenement pression touche */
            QString _help;
            /* \brief Position */
            QPoint _last_pos;
            /* \brief indicateur d'evenement interne */
            bool _internalEvent;
        public:
            _SwEditorQGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);
            ~_SwEditorQGraphicsView();
            /** mise a jour de la scene */
            void sceneUpdated();
        public slots:
            /* \brief sur changement de selection */
            void onSelectionChanged();
        protected:
            /* \brief Evenement pression touche */
            void keyPressEvent ( QKeyEvent * event );
            /* \brief Evenement sourie en deplacement */
            virtual void mouseMoveEvent ( QMouseEvent * e );
            /* \brief Evenement sourie presse */
            virtual void mousePressEvent ( QMouseEvent * e );
            /* \brief Evenement sourie libere */
            virtual void mouseReleaseEvent ( QMouseEvent * e );
            /* \brief Reception d'un evenement roulette de souris */
            void wheelEvent(QWheelEvent *event);
            /* \brief Retaillage de la vue */
            void scaleView(qreal scaleFactor);
        };

    }
}

#endif // _SwEditorQGraphicsView_H
