/*!
 \file _SwRouting_ToolBox.h
 \brief outil de line-routing
 \version 1.0
 \date 24-août-2006 12:21:05
 \author F.Bighelli
*/

#ifndef _SwAddress_Class_H
#define _SwAddress_Class_H

/*
  * INCLUDES LOCAUX
  */
#include <QPointF>
#include <QRectF>
#include <QVector>
#include <QPolygonF>

namespace StreamWork
{
	namespace SwEditor
	{
        
		/*!
		 \class _SwRouting_ToolBox 
		 \brief outil de line-routing
		*/
		class _SwRouting_ToolBox
		{
        public:
			/*! \brief Enumere de direction */
			typedef enum {
				Undefined=-1,
				ToTop=0,
				ToRight=1,
				ToBottom=2,
				ToLeft=4,
			} TRoutingDir;
            /*! \brief start point */
            QPointF _sp;
            /*! \brief start dir */
            TRoutingDir _sd;
            /*! \brief end point */
            QPointF _ep;
            /*! \brief end dir */
            TRoutingDir _ed;
            /*! \brief path */
            QPolygonF _path;
            /*! \brief control points */
            QPolygonF _control_points;
            /*! \brief control points direction */
            QVector<TRoutingDir> _path_constraints;
        private:
            /*! \brief outils*/
            bool IsOnFront(QPointF & start,TRoutingDir start_dir,QPointF & end);
            bool IsBehind(QPointF & start,TRoutingDir start_dir,QPointF & end);
            bool IsOnLeft(QPointF & start,TRoutingDir start_dir,QPointF & end);
            bool IsOnRight(QPointF & start,TRoutingDir start_dir,QPointF & end);
            TRoutingDir TurnLeft(TRoutingDir dir);
            TRoutingDir TurnRight(TRoutingDir dir);
            QPointF GoForward(QPointF & start,TRoutingDir start_dir,QPointF & end);
            QPointF GoForward(QPointF & start,TRoutingDir start_dir,double distance);
            QPointF GetMiddlePoint(const QPointF & start,const QPointF & end);
        public:
            /*! \brief Constructeur */
            _SwRouting_ToolBox();
            /*! \brief Destructeur*/
            ~_SwRouting_ToolBox();
            /*! \brief Initialize*/
            void Initialize(const QPointF & start,TRoutingDir start_dir,const QPointF & end,TRoutingDir end_dir);
            /*! \brief Reset*/
            void Reset();
            /*! \brief Initialize*/
            void AddDifficulty(QRectF & area);
			/*! \brief Contruit la route*/
            void Build();
			/*! \brief Acces au path*/
            const QPolygonF & GetPath() const;
			/*! \brief Acces au points de controles*/
            const QPolygonF & GetControlPoints() const;
			/*! \brief Change la position d'un points de controle
            \return le nouvel index*/
            int ChangeControlPoint(int index,const QPointF & new_val);
			/*! \brief Change la position du point de depart*/
            void ChangeStartPoint(const QPointF & new_val);
			/*! \brief Change la position du point d'arrive*/
            void ChangeEndPoint(const QPointF & new_val);
			/*! \brief Definition du path pour le rechargement*/
            void SetPath(QPolygonF & poly);

       };
	}
}
#endif 
