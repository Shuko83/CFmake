/**
@file PropertiesWidget.cpp
@brief Widget d'edition des propriétés
@author F.Bighelli
 */

#include "PropertiesWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//-----------------------------------------------------------------------
PropertiesWidget::PropertiesWidget() :QWidget(0), _pixmapTextColor(10, 10), _pixmapBgColor(10, 10)
{
    //setWindowOpacity(0.9);
    setEnabled(false);
    setWindowModality(Qt::ApplicationModal);
	QGridLayout  *formLayout;
    formLayout = new QGridLayout (this);
    formLayout->setSpacing(5);
    formLayout->setMargin(2);
    formLayout->addWidget(new QLabel("Name:",this),1,0,Qt::AlignRight);
    _nameEdit=new QLineEdit(this);
    connect(_nameEdit,SIGNAL(textChanged ( const QString &)),this,SLOT(nameChanged ( const QString &)));
    connect(_nameEdit,SIGNAL(editingFinished ()),this,SLOT(nameChangedAndValid()));
    formLayout->addWidget(_nameEdit,1,1,1,4);
    _buttonTextColor=new QPushButton(this);
    connect(_buttonTextColor,SIGNAL(clicked()),this,SLOT(onTextColorClick()));
    _buttonBgColor=new QPushButton(this);
    connect(_buttonBgColor,SIGNAL(clicked()),this,SLOT(onColorClick()));
    formLayout->addWidget(new QLabel("Text color:",this),2,0,Qt::AlignRight);
    formLayout->addWidget(_buttonTextColor,2,1);
    formLayout->addWidget(new QLabel("Header color:",this),2,2,Qt::AlignRight);
    formLayout->addWidget(_buttonBgColor,2,3);
    formLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding),2,4);
    _pModel=new SwPropertiesModelImpl(this);
    _pView=new QTreeView(this);
    _pView->setAlternatingRowColors(true);
    _pView->setItemDelegate(new EditorPropertiesItemDelegate(this));
    _pView->setModel(_pModel);
    _pView->viewport()->setAttribute(Qt::WA_Hover,true);
    connect(_pView,SIGNAL( clicked  ( const QModelIndex &)),this,SLOT(onReset( const QModelIndex & )));
    formLayout->addWidget(_pView,3,0,1,5);
    _cgi=0;
}

//-----------------------------------------------------------------------
void PropertiesWidget::setSelectedGraphicComponent(ComponentGraphicItem * cgi)
{
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
        setTextColorToButton(cgi->getTextColor());
        setBgColorToButton(cgi->getColor());
        setEnabled(true);
    } else {
        _nameEdit->setText("");
        _buttonTextColor->setIcon(QIcon());
        _buttonTextColor->setText("");
        _buttonBgColor->setIcon(QIcon());
        _buttonBgColor->setText("");
        setEnabled(false);
    }
    _cgi=cgi;
}

//-----------------------------------------------------------------------
void PropertiesWidget::showProperties(QWidget * srcWidget)
{
    resize(srcWidget->width()-80,srcWidget->height()-80);
    move(srcWidget->mapToGlobal(QPoint(0,0))+QPoint(40,20));
    show();
}

//-----------------------------------------------------------------------
void PropertiesWidget::nameChanged(const QString & text)
{
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

//-----------------------------------------------------------------------
void PropertiesWidget::nameChangedAndValid()
{
    if (_cgi==0)
        return;
    SwComponent_Class * component=_cgi->getComponent();
    SwComponent_Class * parent=component->GetParent();
	if ( parent )
	{
		SwComponent_Class * achild = parent->GetChild(_nameEdit->text());
		if ( achild == 0 && component->CheckNameValidity(_nameEdit->text()) ) {
			component->SetName(_nameEdit->text());
		}
	}
    
}

//-----------------------------------------------------------------------
void PropertiesWidget::onColorClick()
{
    QColor returnColor=QColorDialog::getColor (_cgi->getColor(),0);
    if(returnColor.isValid()) {
        _cgi->setColor(returnColor);
        setBgColorToButton(_cgi->getColor());
    }
}

//-----------------------------------------------------------------------
void PropertiesWidget::onTextColorClick()
{
    QColor returnColor=QColorDialog::getColor (_cgi->getTextColor(),0);
    if(returnColor.isValid()) {
        _cgi->setTextColor(returnColor);
        setTextColorToButton(_cgi->getTextColor());
    }
}

//-----------------------------------------------------------------------
void PropertiesWidget::setTextColorToButton(const QColor & color)
{
    _pixmapTextColor.fill(color);
    QPainter p(&_pixmapTextColor);
    p.setPen(QPen(QColor(Qt::black)));
    p.drawRect(QRect(0,0,_pixmapTextColor.width()-1,_pixmapTextColor.height()-1));
    _buttonTextColor->setIcon(QIcon(_pixmapTextColor));
    _buttonTextColor->setText(color.name().toUpper());
}

//-----------------------------------------------------------------------
void PropertiesWidget::setBgColorToButton(const QColor & color)
{
    _pixmapBgColor.fill(color);
    QPainter p(&_pixmapBgColor);
    p.setPen(QPen(QColor(Qt::black)));
    p.drawRect(QRect(0,0,_pixmapBgColor.width()-1,_pixmapBgColor.height()-1));
    _buttonBgColor->setIcon(QIcon(_pixmapBgColor));
    _buttonBgColor->setText(color.name().toUpper());
}

//-----------------------------------------------------------------------
void PropertiesWidget::onReset(const QModelIndex & index)
{
    if (!index.isValid() || index.column()!=0) 
        return;
    QVariant value = index.model()->data(index, Qt::UserRole);
    void * ptr=value.value<void*>();
    if(ptr==0) {
        return;
    }
    ISwProperty * p= static_cast<ISwProperty *>(ptr);
    if ( !p->HasChanged() || !p->isResettable() ) {
        return;
    }
    p->reset();
    update();
}
