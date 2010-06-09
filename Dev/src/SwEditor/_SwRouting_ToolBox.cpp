/*!
 \file _SwRouting_ToolBox.cpp
 \brief Implementation of the Class SwAddress_Class
 \date 24-août-2006 12:21:05
 \author F.Bighelli
*/
/*
 * INCLUDES GLOBAUX
 */
#include <QStringList>
/*
 * INCLUDES LOCAUX
 */
#include "_SwRouting_ToolBox.h"

using namespace StreamWork::SwEditor;

#define CL_MIN_STEP 10.0

/*! \brief Constructeur */
_SwRouting_ToolBox::_SwRouting_ToolBox(){
}
/*! \brief Destructeur*/
_SwRouting_ToolBox::~_SwRouting_ToolBox(){
}
/*! \brief Initialize*/
void _SwRouting_ToolBox::Initialize(const QPointF & start,TRoutingDir start_dir,const QPointF & end,TRoutingDir end_dir) {
    _sp=start;
    _sd=start_dir;
    _ep=end;
    _ed=end_dir;
    _path.clear();
    _control_points.clear();
    _path_constraints.clear();
}
/*! \brief Reset*/
void  _SwRouting_ToolBox::Reset() {
    _path.clear();
    _control_points.clear();
    _path_constraints.clear();
}

/*! \brief Initialize*/
void _SwRouting_ToolBox::AddDifficulty(QRectF & area) {
    //Not implemented
}
/*! \brief outils*/
bool _SwRouting_ToolBox::IsOnFront(QPointF & start,TRoutingDir start_dir,QPointF & end) {
    switch(start_dir) {
        case ToTop:
            return end.y()<start.y();
        case ToLeft:
            return end.x()<start.x();
        case ToBottom:
            return end.y()>start.y();
        case ToRight:
            return end.x()>start.x();
        default:
            break;
    }
    return false;
}
bool _SwRouting_ToolBox::IsBehind(QPointF & start,TRoutingDir start_dir,QPointF & end) {
    return !IsOnFront(start,start_dir,end);
}
bool _SwRouting_ToolBox::IsOnLeft(QPointF & start,TRoutingDir start_dir,QPointF & end) {
    switch(start_dir) {
        case ToTop:
            return end.x()<=start.x();
        case ToLeft:
            return end.y()>=start.y();
        case ToBottom:
            return end.x()>=start.x();
        case ToRight:
            return end.y()<=start.y();
        default:
            break;
    }
    return false;
}
bool _SwRouting_ToolBox::IsOnRight(QPointF & start,TRoutingDir start_dir,QPointF & end) {
    return !IsOnLeft(start,start_dir,end);
}
_SwRouting_ToolBox::TRoutingDir _SwRouting_ToolBox::TurnLeft(TRoutingDir dir) {
    switch(dir) {
        case ToTop:
            return ToLeft;
        case ToLeft:
            return ToBottom;
        case ToBottom:
            return ToRight;
        case ToRight:
            return ToTop;
        default:
            break;
    }
    return Undefined;
}
_SwRouting_ToolBox::TRoutingDir _SwRouting_ToolBox::TurnRight(TRoutingDir dir) {
    switch(dir) {
        case ToTop:
            return ToRight;
        case ToRight:
            return ToBottom;
        case ToBottom:
            return ToLeft;
        case ToLeft:
            return ToTop;
        default:
            break;
    }
    return Undefined;
}
QPointF _SwRouting_ToolBox::GoForward(QPointF & start,TRoutingDir start_dir,QPointF & end) {
    QPointF new_point=start;
    switch(start_dir) {
        case ToTop:
            if (!IsOnFront(start,start_dir,end)) {
                return start;
            }
            new_point.setY(end.y());
            break;
        case ToRight:
            if (!IsOnFront(start,start_dir,end)) {
                return start;
            }
            new_point.setX(end.x());
            break;
        case ToBottom:
            if (!IsOnFront(start,start_dir,end)) {
                return start;
            }
            new_point.setY(end.y());
            break;
        case ToLeft:
            if (!IsOnFront(start,start_dir,end)) {
                return start;
            }
            new_point.setX(end.x());
            break;
        default:
            break;
    }
    return start;
}
QPointF _SwRouting_ToolBox::GoForward(QPointF & start,TRoutingDir start_dir,double distance) {
    QPointF new_point=start;
    switch(start_dir) {
        case ToTop:
            new_point=new_point+QPointF(0.0,-distance);
            break;
        case ToRight:
            new_point=new_point+QPointF(distance,0.0);
            break;
        case ToBottom:
            new_point=new_point+QPointF(0.0,distance);
            break;
        case ToLeft:
            new_point=new_point+QPointF(-distance,0.0);
            break;
        default:
            break;
    }
    return start;
}
QPointF _SwRouting_ToolBox::GetMiddlePoint(const QPointF & start,const QPointF & end) {
    return QPointF((start.x()+end.x())/2.0,(start.y()+end.y())/2.0);
}
/*! \brief Contruit la route*/
void _SwRouting_ToolBox::Build() {
    _path.push_back(_sp);
    _path.push_back(_ep);
    _control_points.push_back(GetMiddlePoint(_sp,_ep));
}
/*! \brief Contruit la route*/
const QPolygonF & _SwRouting_ToolBox::GetPath() const {
    return _path;
}
/*! \brief Acces au points de controles*/
const QPolygonF & _SwRouting_ToolBox::GetControlPoints() const{
    return _control_points;
}
/*! \brief Change la position d'un points de controle*/
int _SwRouting_ToolBox::ChangeControlPoint(int index,const QPointF & new_val){
    //regarde si le control point est deja dans le path
    if (index%2!=0) {
        int index_in_path=(index/2+1);
        _path[index_in_path]=new_val;
        _control_points[index]=new_val;
        _control_points[index-1]=GetMiddlePoint(_path[index_in_path-1],_path[index_in_path]);
        _control_points[index+1]=GetMiddlePoint(_path[index_in_path],_path[index_in_path+1]);
        return index;
    } else {
        //On le cree dans le path 
        int index_to_insert=(index/2+1);
        _control_points[index]=new_val;
        _path.insert(index_to_insert,_control_points[index]);
        //et on cree deux nouveaux points de controle intermediaire
        //Celui apres
        _control_points.insert(index+1,GetMiddlePoint(_path[index_to_insert],_path[index_to_insert+1]));
        //Celui avant
        _control_points.insert(index,GetMiddlePoint(_path[index_to_insert-1],_path[index_to_insert]));
        return index+1;
    }
    return -1;
}
/*! \brief Change la position du point de depart*/
void _SwRouting_ToolBox::ChangeStartPoint(const QPointF & new_val){
    _path[0]=new_val;
    _control_points[0]=GetMiddlePoint(new_val,_path[1]);
}
/*! \brief Change la position du point d'arrive*/
void _SwRouting_ToolBox::ChangeEndPoint(const QPointF & new_val){
    _path[_path.count()-1]=new_val;
    _control_points[_control_points.count()-1]=GetMiddlePoint(_path[_path.count()-2],new_val);

}
/*! \brief Definition du path pour le rechargement*/
void _SwRouting_ToolBox::SetPath(QPolygonF & poly) {
    _control_points.clear();
    _path.clear();
    _path=poly;
    for(int i=1;i<_path.count();i++) {
        _control_points.push_back(GetMiddlePoint(_path[i-1],_path[i]));    
        if (i!=_path.count()-1) _control_points.push_back(_path[i]);    
    }
}
