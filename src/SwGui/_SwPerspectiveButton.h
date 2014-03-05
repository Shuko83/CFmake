/**
@file _SwPerspectiveButton.h
@brief Perspective button
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI__SWPERSPECTIVEBUTTON_H
#define _STREAMWORK_SWGUI__SWPERSPECTIVEBUTTON_H

#include <QtGui>
#include "ISwPerspective.h"
#include "ISwPerspectiveObserver.h"



namespace StreamWork {

    namespace SwGui {
    
        class _SwPerspectivesManager;

        #define CG_PERSPECTIVE_BUTTON_SIZE 100
        #define CG_PERSPECTIVE_MARGING 5
        /**
        @class _SwPerspectiveButton
        @brief Perspective button
        */
        class _SwPerspectiveButton : public QFrame,
                                     public ISwPerspectiveObserver {
            Q_OBJECT
        public:
	        /** @brief Constructor */
	        _SwPerspectiveButton(QWidget * parent,_SwPerspectivesManager *manager);
	        /** @brief setter perspective */
	        void setPerspective(ISwPerspective * perspective);
	        /** @brief getter perspective */
	        ISwPerspective * getPerspective();
	        /** @brief setter checked */
	        void setChecked(bool check);
	        /** @brief getter checked */
	        bool checked();			
			/** @brief changement du style sheet du bouton */
			void setButtonStyleSheet(const QString & stylesheet);

            //---------------------------------------------------------------------
            // Interface ISwPerspectiveObserver
            //---------------------------------------------------------------------
            /** @brief sur changement de l'activation d'une perspective*/
            virtual void OnEnablePerspectiveChange(ISwPerspective * perspective);
             /** @brief sur changement de le la visibilite d'une perspective*/
            virtual void OnVisiblePerspectiveChange(ISwPerspective *);
       
        signals:
            /** @brief sur changement de checked*/
            /*virtual */void onPerspectiveToggle(ISwPerspective * perspective,bool checked);

        protected:
            /** @brief calcul des valeurs pour le dessin */
            virtual void processDrawProperties();
            /** @brief evenement de paint */
            virtual void paintEvent ( QPaintEvent * event );
            /** @brief evenement entree */
            virtual void enterEvent ( QEvent * event );
            /** @brief evenement sortie */
            virtual void leaveEvent ( QEvent * event );
            /** @brief event mouse press */
            virtual void mousePressEvent ( QMouseEvent * event );
            /** @brief event mouse release */
            virtual void mouseReleaseEvent ( QMouseEvent * event );

        private:
            /** @brief Perspective */
            ISwPerspective * _perspective;
            /** @brief icone */
            QIcon _icon;
            /** @brief label */
            QString _label;
            /** @brief icone */
            QString _description;
            /** @brief etat */
            bool _checked;
            bool _hover;
            /** @brief manager */
            _SwPerspectivesManager *_manager;
            /** @brief Propriété pour le dessin */
            QRect _textRect;
            QString _textToDraw;
            QRect _iconRect;
            QRect _iconRectSelected;
            QPixmap _icoPixmap;
            QPixmap _icoPixmapDisabled;			
			/* @brief stylesheet du bouton*/
			QString	_stylesheet;
        };

    }
    
}
#endif
