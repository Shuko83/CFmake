/*!
\file SwWizardFrontEnd.cpp
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */

#include <SwApplication.h>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
#include <SwAddress_ToolBox.h>
#include "SwWizardFrontEnd.h"
#include "SwWizardNodeTreeView.h"
#include <SwGuiDefaultItemDelegate.h>
#include <QDomDocument>
#include <QTemporaryFile>
#include <QByteArray>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include <QFileInfo>
#include <QProcess>
#include <QtGlobal>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
SwWizardFrontEnd::SwWizardFrontEnd(QWidget *parent)
    : QDialog(parent)
{

    setWindowTitle("Plugin wizard");

    /* Root component */
    _root=dynamic_cast<SwSimpleComponent_Class *>(SW_FACTORIES.CreateComponent("NodeSwFactoryEntry"));

    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(600,600));

    /* Composant de l'ihm */
    QVBoxLayout * main_layout=new QVBoxLayout(this);
    this->setLayout(main_layout);

    //Ajout de l'arbre
    QSplitter * splitter=new QSplitter(Qt::Horizontal,this);
    QSplitter * v_splitter=new QSplitter(Qt::Vertical,this);
    v_splitter->addWidget(splitter);
    main_layout->addWidget(v_splitter);
    NodePropertiesModelImpl *model_property=new NodePropertiesModelImpl(this);
    SwWizardNodeTreeView *view_model=new SwWizardNodeTreeView(this);
    _model=new SwWizardNodeModel(view_model,_root,model_property);
    view_model->setModel(_model);
    view_model->connect(view_model,SIGNAL(OnMenuRequested( const QPoint &,QModelIndex)),_model,SLOT(OnMenuRequested( const QPoint &,QModelIndex)));
    view_model->connect(view_model,SIGNAL(clicked( const QModelIndex & )),_model,SLOT(OnClicked( const QModelIndex & )));
    splitter->addWidget(view_model);
    QTreeView * _treeview=new QTreeView(this);
    _treeview->setModel(model_property);
    _treeview->setItemDelegate(new SwGuiDefaultItemDelegate(_treeview));
    splitter->addWidget(_treeview);
    //Ajout zone de texte
    _tedit=new QTextEdit(this);
    _tedit->setReadOnly(true);
    v_splitter->addWidget(_tedit);
    //Ajout des boutons
    QHBoxLayout * button_layout=new QHBoxLayout(this);
    main_layout->addLayout(button_layout);
    QPushButton *but=new QPushButton(QString("Close"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(accept()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignLeft);
    but=new QPushButton(QString("Generate"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(Generate()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignRight);

    //Definition de l'action d'appel
    _call_action=new QAction("Wizard",this);
    connect(_call_action, SIGNAL(triggered()), this, SLOT(ShowWizard()));

}
/*! \brief Destructeur */
SwWizardFrontEnd::~SwWizardFrontEnd()
{
}
//--------------------------------------------------------------
// Interface ISwAction
//--------------------------------------------------------------
/*! \brief Renvoie le Action
\return le Action */
QAction & SwWizardFrontEnd::GetAction() {
    return (*_call_action);    
}
//--------------------------------------------------------------
// Locals Slots
//--------------------------------------------------------------
/*! \brief slot pour la visualisation de ce wizard */
void SwWizardFrontEnd::ShowWizard() {
    this->exec();
}
/*! \brief slot pour la generation de ce wizard */
void SwWizardFrontEnd::Generate() {
    QDomDocument doc;
    QByteArray stream_desc;
    QTemporaryFile file;
    QString relative_path;

    file.setFileTemplate(QString("plugindata_XXXXXX.xml"));
    _root->Save(doc);
    //Recuperation du stream
    stream_desc="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream_desc=stream_desc+doc.toByteArray(4); //Indentation de quatre espace
    //Ouverture d'un fichier en ecriture
    if (file.open()==false) {
        QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Fail to save stream in file %1").arg(file.fileName()));        
        return;
    }  
    QFileInfo fi(file);
    qDebug(fi.absolutePath().toAscii().data());
    qDebug(file.fileName().toAscii().data());
    //Ecriture du fichier
    file.write(stream_desc);
    //Fermeture du fichier
    file.close();
    //recuperation du fichier temporaire
    relative_path=QDir::toNativeSeparators(fi.absoluteFilePath());
    qDebug(relative_path.toAscii().data());
    _tedit->setText("");
    //Generation (A REVOIR sous LINUX)
    QProcess *myProcess = new QProcess(this);
#ifdef Q_OS_LINUX
    QString program = QDir::toNativeSeparators("wizardgeneration");
#endif
#ifdef Q_WS_WIN
    QString program = QDir::toNativeSeparators("wizardgeneration.bat");
#endif
    QStringList arguments;
    arguments << relative_path;
    myProcess->setWorkingDirectory(qApp->applicationDirPath());
    myProcess->start(program, arguments);
    QString text;
    if(myProcess->waitForFinished() ) {
        text="Succeed:\n";
        text+=myProcess->readAll();
        qDebug("Succeed:");
        qDebug(myProcess->readAll().data());
    } else {
        text="Failed:\n";
        text+=myProcess->errorString();
        qDebug("Failed:");
        qDebug(myProcess->errorString().toAscii().data());
    }
    _tedit->setText(text);
    myProcess->close();
    delete myProcess;
}
