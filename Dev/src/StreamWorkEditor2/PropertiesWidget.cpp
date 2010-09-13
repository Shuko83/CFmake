/**
@file PropertiesWidget.cpp
@brief Widget d'edition des propriétés
@author F.Bighelli
 */

#include "PropertiesWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/** @brief Constructor */
PropertiesWidget::PropertiesWidget():QWidget(0) {
    //setWindowOpacity(0.9);
    setEnabled(false);
    setWindowModality(Qt::ApplicationModal);
	QGridLayout  *formLayout;
    formLayout = new QGridLayout (this);
    formLayout->setSpacing(5);
    formLayout->setMargin(2);
    formLayout->addWidget(new QLabel("Name:",this),1,0);
    _nameEdit=new QLineEdit(this);
    connect(_nameEdit,SIGNAL(textChanged ( const QString &)),this,SLOT(nameChanged ( const QString &)));
    connect(_nameEdit,SIGNAL(editingFinished ()),this,SLOT(nameChangedAndValid()));
    formLayout->addWidget(_nameEdit,1,1);
    _pModel=new SwPropertiesModelImpl(this);
    _pView=new QTreeView(this);
    _pView->setAlternatingRowColors(true);
    _pView->setItemDelegate(new SwGuiDefaultItemDelegate(this));
    _pView->setModel(_pModel);
    formLayout->addWidget(_pView,2,0,1,2);
    _cgi=0;
}
/** @brief Composant en cours d'edition*/
void PropertiesWidget::setSelectedGraphicComponent(ComponentGraphicItem * cgi) {
    if (_cgi!=0) {
        nameChangedAndValid();
    }
    _pModel->SetProperties(0);
    _cgi=0;
    if (cgi!=0) {
        _nameEdit->setText(cgi->getComponent()->GetName());
        _pModel->SetProperties(dynamic_cast<ISwProperties *>(cgi->getComponent()->QueryService(CG_SW_SERVICE_PROPERTIES)));
        _pView->expandAll();
        _pView->setColumnWidth(0,200);
        setEnabled(true);
    } else {
        _nameEdit->setText("");
        setEnabled(false);
    }
    _cgi=cgi;
}
/** @brief montre le widget */
void PropertiesWidget::showProperties(QWidget * srcWidget) {
    resize(srcWidget->width()-80,srcWidget->height()-80);
    move(srcWidget->mapToGlobal(QPoint(0,0))+QPoint(40,20));
    show();
}
/** @brief sur changement du nom */
void PropertiesWidget::nameChanged ( const QString & text ) {
    if (_cgi==0)
        return;
    SwComponent_Class * component=_cgi->getComponent();
    SwComponent_Class * parent=component->GetParent();
    SwComponent_Class * achild=parent->GetChild(text);
    if ((achild==0 || achild==component) && component->CheckNameValidity(text)) {
        _nameEdit->setStyleSheet(QString());
    } else {
        _nameEdit->setStyleSheet("QLineEdit {color:red;}");
    }

}   
/** @brief sur fin changement du nom */
void PropertiesWidget::nameChangedAndValid() {
    if (_cgi==0)
        return;
    SwComponent_Class * component=_cgi->getComponent();
    SwComponent_Class * parent=component->GetParent();
    SwComponent_Class * achild=parent->GetChild(_nameEdit->text());
    if (achild==0 && component->CheckNameValidity(_nameEdit->text())) {
        component->SetName(_nameEdit->text());
    }
}
