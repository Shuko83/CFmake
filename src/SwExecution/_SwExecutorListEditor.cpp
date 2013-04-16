/*!
 \file _SwExecutorListEditor.cpp
 \brief Objet pour l'edition de la liste
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QtGui>
#include <Qt>
#include <QMimeData>
#include <QDrag>
#include "_SwExecutorListEditor.h"

#define CL_ROW_HEIGHT 16
#define CL_HEADER_HEIGHT 16
#define CL_TEXT_HEADER "Execution order"
#define CL_TEXT_MODE_HEADER "Mode"
#define CL_X_SPACING 10

#define CL_X_ALEFT CL_X_SPACING
#define CL_X_MODE_OFFSET CL_HEADER_HEIGHT+CL_X_SPACING
#define CL_X_ARIGHT CL_X_MODE_OFFSET+40+CL_X_SPACING
#define CL_X_OFFSET CL_X_ARIGHT+CL_HEADER_HEIGHT+CL_X_SPACING

/*! \brief Constructeur */
_SwExecutorListEditor::_SwExecutorListEditor(QWidget * parent,SwComponent_Class * root_component,QList<QString> * liste,QList<_SwExecutionMode> * liste_mode):QWidget(parent) {
    setAcceptDrops(true);
    setMouseTracking(true);
    setMinimumSize(400, 400);
    _root_component=root_component;
    _path_list=liste;
    _mode_list=liste_mode;
    while (_sel_list.count()<_path_list->count()) _sel_list.push_back(false);
    _start_drag_to_do=false;
    setToolTip("Press Delete to remove selected component");
    setFocusPolicy(Qt::StrongFocus);
    _arrowLeft=qApp->style()->standardIcon(QStyle::SP_ArrowLeft);
    _arrowRight=qApp->style()->standardIcon(QStyle::SP_ArrowRight);
    _hoverIndex=-1;
}
/*! \brief Constructeur */
_SwExecutorListEditor::~_SwExecutorListEditor() {

}
/*! \brief UpdateList */
void _SwExecutorListEditor::UpdateList() {
    QStringList list_update(*_path_list);
    emit OnExeAdded(list_update);
}

/*! \brief dragEnterEvent */
void _SwExecutorListEditor::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/exe_instance")) {
        _show_pos_drop=true;
        _pos_drop_index=GetIndex(event->pos());
        if (_pos_drop_index==-1) _pos_drop_index=0;
        if (event->source()==this) {
            event->setDropAction(Qt::MoveAction);
        } else {
            event->setDropAction(Qt::CopyAction);
        }
        event->accept();
        update();
    }else
        event->ignore();

}
/*! \brief dragLeaveEvent */
void _SwExecutorListEditor::dragLeaveEvent(QDragLeaveEvent *event) {
    _show_pos_drop=false;
    event->accept();
    update();
}
/*! \brief dragMoveEvent */
void _SwExecutorListEditor::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/exe_instance")) {
        _pos_drop_index=GetIndex(event->pos());
        if (_pos_drop_index==-1) _pos_drop_index=0;
        if (event->source()==this) {
            event->setDropAction(Qt::MoveAction);
        } else {
            event->setDropAction(Qt::CopyAction);
        }
        event->accept();
        update();
        return;
    }
    event->accept();
}
/*! \brief dropEvent */
void _SwExecutorListEditor::dropEvent(QDropEvent *event) {

    if (event->mimeData()->hasFormat("application/exe_instance")) {
        QString comps;
        QStringList allNewItems;
        QStringList newItems;
        QStringList newItemsModeString;
        QList<_SwExecutionMode> newItemsMode;
        QString item;
        comps=QString(event->mimeData()->data("application/exe_instance").data());
        allNewItems=comps.split("\n",QString::SkipEmptyParts);
        newItems=allNewItems[0].split(";",QString::SkipEmptyParts);
        if(allNewItems.count()==2) {
            newItemsModeString=allNewItems[1].split(";",QString::SkipEmptyParts);
            foreach(QString itemModeString,newItemsModeString) {
                if(itemModeString=="N") {
                    newItemsMode.push_back(Normal_mode);
                } else if(itemModeString=="R") {
                    newItemsMode.push_back(Replay_mode);
                } else {
                    newItemsMode.push_back(Both_mode);
                }
            }
        }
        //Definition de l'endroit ou on pointe
        int index=GetIndex(event->pos());
        //Si c'est un deplacement
        if (event->source()==this) {
            foreach(item,newItems) {
                int item_to_remove=_path_list->indexOf(item);
                if (item_to_remove<index)
                    index--;
                _path_list->removeAt(item_to_remove);
                _mode_list->removeAt(item_to_remove);
                _sel_list.removeAt(item_to_remove);
            }
        } else {
            emit OnExeAdded(newItems);
        }
        if (index==-1) {
            while (newItems.count()>0) {
                _path_list->push_front(newItems.back());
                if (newItemsMode.count()==0)
                    _mode_list->push_front(Both_mode);
                else {
                    _mode_list->push_front(newItemsMode.back());
                    newItemsMode.pop_back();
                }
                _sel_list.push_front(true);
                newItems.pop_back();
            }
        }
        if (index==_path_list->count()) {
            while (newItems.count()>0) {
                _path_list->push_back(newItems.front());
                if (newItemsMode.count()==0)
                    _mode_list->push_back(Both_mode);
                else {
                    _mode_list->push_back(newItemsMode.front());
                    newItemsMode.pop_front();
                }
                _sel_list.push_back(true);
                newItems.pop_front();
            }
        } else {
             while (newItems.count()>0) {
                _path_list->insert(index,newItems.back());
                if (newItemsMode.count()==0)
                    _mode_list->insert(index,Both_mode);
                else {
                    _mode_list->insert(index,newItemsMode.back());
                    newItemsMode.pop_back();
                }
                _sel_list.insert(index,true);
                newItems.pop_back();
            }
        }
         _show_pos_drop=false;
        update();
    }
}
/*! \brief mousePressEvent */
void _SwExecutorListEditor::mousePressEvent(QMouseEvent *event) {
    _click_pos=event->pos();
    _start_drag_to_do=true;

    int oldHoverIndex=_hoverIndex;
    _hoverIndex=GetIndex(event->pos());
    if (oldHoverIndex!=_hoverIndex)
        update();
    if(_hoverIndex!=-1 && _hoverIndex<_path_list->count()) {
        if(_click_pos.x()>=CL_X_ALEFT && _click_pos.x()<CL_X_MODE_OFFSET) {
            if ((*_mode_list)[_hoverIndex]==Normal_mode) {
                (*_mode_list)[_hoverIndex]=Both_mode;
            } else if ((*_mode_list)[_hoverIndex]==Both_mode) {
                (*_mode_list)[_hoverIndex]=Replay_mode;
            } else if ((*_mode_list)[_hoverIndex]==Replay_mode) {
                (*_mode_list)[_hoverIndex]=Normal_mode;
            }
            update();
        } else if(_click_pos.x()>=CL_X_ARIGHT && _click_pos.x()<CL_X_OFFSET) {
            if ((*_mode_list)[_hoverIndex]==Normal_mode) {
                (*_mode_list)[_hoverIndex]=Replay_mode;
            } else if ((*_mode_list)[_hoverIndex]==Replay_mode) {
                (*_mode_list)[_hoverIndex]=Both_mode;
            } else if ((*_mode_list)[_hoverIndex]==Both_mode) {
                (*_mode_list)[_hoverIndex]=Normal_mode;
            }
            update();
        }
    }
}
/*! \brief mouseMoveEvent */
void _SwExecutorListEditor::mouseMoveEvent ( QMouseEvent * event ) {
    if (_start_drag_to_do) {
        //Gestion du drag and drop
        QString text;

        for(int i=0;i<_path_list->count();i++) {
            if (_sel_list[i]==true) {
                text=text+(*_path_list)[i];
                text+=";";
            }
        }
        text+="\n";
        for(int i=0;i<_mode_list->count();i++) {
            if (_sel_list[i]==true) {
                switch ((*_mode_list)[i]) {
                    case Normal_mode:
                        text=text+"N";
                        break;
                    case Replay_mode:
                        text=text+"R";
                        break;
                    default:
                        text=text+"B";
                        break;
                }
                text+=";";
            }
        }

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/exe_instance", text.toLatin1());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->start(Qt::MoveAction);
        _start_drag_to_do=false;
    }
    int oldHoverIndex=_hoverIndex;
    _hoverIndex=GetIndex(event->pos());
    if (oldHoverIndex!=_hoverIndex)
        update();
}
/*! \brief mouseReleaseEvent */
void _SwExecutorListEditor::mouseReleaseEvent ( QMouseEvent * event ) {
    _start_drag_to_do=false;
    if (_click_pos==event->pos()) {
        //Gestion de la selection
        int index=GetIndex(event->pos());
        if (index==-1 || index==_path_list->count())
            return;

        if ((event->modifiers() & Qt::ControlModifier)&& _sel_list.indexOf(index)!=-1) {
            _sel_list[index]=!_sel_list[index];
            update();
        } else {
            for(int i=0;i<_sel_list.count();i++)
                _sel_list[i]=false;
            _sel_list[index]=true;
            update();
        }
    }
}
/*! \brief keyPressEvent */
void _SwExecutorListEditor::keyPressEvent ( QKeyEvent * event ) {
    if (event->matches(QKeySequence::Delete)) {
        QStringList ltext;
        QString item;
        int item_to_remove;

        for(int i=0;i<_path_list->count();i++) {
            if (_sel_list[i]==true) {
                ltext.push_back((*_path_list)[i]);
            }
        }
        emit OnExeRemoved(ltext);
        foreach(item,ltext) {
            item_to_remove=_path_list->indexOf(item);
            _path_list->removeAt(item_to_remove);
            _sel_list.removeAt(item_to_remove);
        }
        update();
    }
}
/*! \brief paintEvent */
void _SwExecutorListEditor::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

    painter.fillRect(QRect(0,0,this->width(),CL_HEADER_HEIGHT),Qt::gray);
    painter.drawText(CL_X_OFFSET,0,this->width()-CL_X_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,QString(CL_TEXT_HEADER));
    painter.drawText(CL_X_MODE_OFFSET,0,CL_X_OFFSET-CL_X_MODE_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,QString(CL_TEXT_MODE_HEADER));
    int i;
    for (i=0;i<_path_list->count();i++) {
        if (_sel_list[i]) {
            painter.fillRect(QRect(0,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,this->width(),CL_HEADER_HEIGHT),Qt::blue);
            painter.setPen(QColor(Qt::white));
        }
        painter.drawText(CL_X_OFFSET,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,this->width()-CL_X_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,(*_path_list)[i]);
        switch ((*_mode_list)[i]) {
            case Normal_mode:
                painter.drawText(CL_X_MODE_OFFSET,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,CL_X_OFFSET-CL_X_MODE_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,"Normal");
                break;
            case Replay_mode:
                painter.drawText(CL_X_MODE_OFFSET,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,CL_X_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,"Replay");
                break;
            default:
                painter.drawText(CL_X_MODE_OFFSET,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,CL_X_OFFSET,CL_HEADER_HEIGHT,Qt::TextSingleLine,"Both");
                break;
        }
        if(_hoverIndex==i) {
            _arrowLeft.paint(&painter,QRect(CL_X_ALEFT,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,CL_HEADER_HEIGHT,CL_HEADER_HEIGHT));
            _arrowRight.paint(&painter,QRect(CL_X_ARIGHT,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,CL_HEADER_HEIGHT,CL_HEADER_HEIGHT));
        }
        painter.setPen(QColor(Qt::black));
        if (_show_pos_drop && _pos_drop_index==i) {
            painter.drawLine(0,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,this->width(),CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT);
        }
    }
    if (_show_pos_drop && _pos_drop_index==_path_list->count()) {
        painter.drawLine(0,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,this->width(),CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT);
    }

    //Resize
    QRect r=painter.boundingRect(CL_X_OFFSET,0,this->width(),CL_HEADER_HEIGHT,Qt::TextSingleLine,QString(CL_TEXT_HEADER));
    for (int i=0;i<_path_list->count();i++) {
        r|=painter.boundingRect(CL_X_OFFSET,CL_HEADER_HEIGHT+i*CL_ROW_HEIGHT,this->width(),CL_HEADER_HEIGHT,Qt::TextSingleLine,(*_path_list)[i]);
    }
    resize(r.width(),r.height()+2);
    painter.end();
}
/*! \brief Index de l'item pointé*/
int _SwExecutorListEditor::GetIndex(const QPoint & p) {

    if (p.y()<CL_HEADER_HEIGHT)
        return -1;
    if (p.y()>=(CL_HEADER_HEIGHT+_path_list->count()*CL_ROW_HEIGHT))
        return _path_list->count();
    return (p.y()-CL_HEADER_HEIGHT)/CL_ROW_HEIGHT;
}
