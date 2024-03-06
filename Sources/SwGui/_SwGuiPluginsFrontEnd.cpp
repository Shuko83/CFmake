/*!
\file _SwGuiPluginsFrontEnd.cpp
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */


#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtGlobal>
#include <QTreeView>
#include "Main/SwApplication.h"
#include <QMessageBox>
#include "Main/SwMacros.h"
#include "_SwGuiPluginsFrontEnd.h"

using namespace StreamWork::SwCore;


/*! \brief Constructeur */
_SwGuiPluginsFrontEnd::_SwGuiPluginsFrontEnd(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Plugins List");
    setWindowIcon(QIcon(":/SwGui/plugins.png"));
    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(1000,700));

    //Creation de l'interface principale
    QTreeView * treeview=new QTreeView(this);
	treeview->setModel(SW_APP->ComponentsBank().GetModel());
    treeview->setColumnWidth(0,200);
    //treeview->expandAll();
    /* Composant de l'ihm */
    QVBoxLayout * main_layout=new QVBoxLayout(this);
    this->setLayout(main_layout);
    main_layout->addWidget(treeview);

    //Ajout de l'arbre
    //Ajout des boutons
    QHBoxLayout * button_layout=new QHBoxLayout(this);
    main_layout->addLayout(button_layout);
    QPushButton *but=new QPushButton(QString("Ok"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(accept()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignLeft);
    //but=new QPushButton(QString("Export"),this);
    //but->connect(but,SIGNAL(clicked()),this,SLOT(Export()));
    //button_layout->addWidget(but);
    //button_layout->setAlignment(but,Qt::AlignRight);

}
/*! \brief Destructeur */
_SwGuiPluginsFrontEnd::~_SwGuiPluginsFrontEnd()
{
}
/*! \brief export */
void _SwGuiPluginsFrontEnd::Export() {
    QMessageBox::information(NULL,"Information","Sorry, not implemented",QMessageBox::Ok);    
}
