/*!
 \file _SwEditorGraphicScene.h
 \brief Implementation of the Class _SwEditorGraphicScene gerant la scene represenant un stream dans une vue de dessin QT 
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/

#include <QRectF>
#include "ISwEditorGraphicItem.h"
#include "_SwEditorGraphicScene.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorGraphicScene::_SwEditorGraphicScene(SwComponent_Class * host_comp):QGraphicsScene(0) {
    ISwEditorGraphicItem * item;
    _host_comp=host_comp;
    item=dynamic_cast<ISwEditorGraphicItem *>(host_comp->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
    if (item!=NULL) {
        this->addItem(item->GetRelatedItem());
    }
    //A revoir
    setBackgroundBrush(QColor(230,230,230));
    //setItemIndexMethod(QGraphicsScene::NoIndex);
}
/*! \brief Destructeur */
_SwEditorGraphicScene::~_SwEditorGraphicScene() {

}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwEditorGraphicScene::Liberate() {

}
//---------------------------------------------------------------------
// Interface ISwEditorGraphicScene
//---------------------------------------------------------------------
/*! \brief Fournit la scene graphique qt correspondant
\return Renvoie un pointeur sur la scene graphique qt correspondant */
QGraphicsScene * _SwEditorGraphicScene::GetRelatedScene(){
    return this;
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwEditorGraphicScene::GetHostComponent(){
    return _host_comp;
}            
//---------------------------------------------------------------------
// Interface ISwPersistent
//---------------------------------------------------------------------
#define CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE "gscene"
#define CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_WIDTH "width"
#define CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_HEIGHT "height"
#define CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_X "x"
#define CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_Y "y"
/*! \brief methode permettant de charger des donnees */
void _SwEditorGraphicScene::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager){
    double px,py,pwidth,pheight;
    bool result=true;

    //To do load
    QDomElement item_node = elt.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE));
    if (item_node.isNull()) {
        return;
    }
    if (item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_WIDTH) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_HEIGHT) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_X) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_Y)) {
        px=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_X).toDouble(&result);
        if (!result) return;
        py=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_Y).toDouble(&result);
        if (!result) return;
        pwidth=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_WIDTH).toDouble(&result);
        if (!result) return;
        pheight=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_HEIGHT).toDouble(&result);
        if (!result) return;
        //setSceneRect(px,py,pwidth,pheight);
    }

}

/*! \brief methode permettant de sauver des donnees */
void StreamWork::SwEditor::_SwEditorGraphicScene::Save(QXmlStreamWriter& writer)
{
	writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE);
	//taille
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_WIDTH, QString::number(sceneRect().width()));
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_HEIGHT, QString::number(sceneRect().height()));
	//position
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_X, QString::number(sceneRect().x()));
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GSCENE_NODE_Y, QString::number(sceneRect().y()));
	writer.writeEndElement();
}
