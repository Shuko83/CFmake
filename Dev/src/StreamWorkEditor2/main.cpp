#include <exception>

#include <QApplication>
#include <QMessageBox>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include <SwApplication.h>
#include <SwMacros.h>
#include <SwFileLogRecorder_Class.h>
#include "MainWindow.h"
#include "SwSplash.h"
#include "LogView.h"
#include "SwLogger_Class.h"
#include "EditionService.h"

using namespace StreamWork::SwCore;

const char * VL_Help="Usage: %1 [options]\n\
    -h                   this help\n\
    -v                   informations about this application\n\
    -d                   display core actions\n\
    -ppath path          plugin path\n\
    -pdesc pathdesc      plugin paths descriptor\n\
    -stream streamfile   stream will be read in the specified path\n\
	-models modelFile    file where all models are described\n\
	-autostart			 Start automatically the executor in the stream witch is in Timer/MainEntry mode\n\
	-appDirPath			 The application dir path \n\
    -log logfile         logs will be write in the specified file\n\
\n\
";

const char * VL_Version="%1\n\
Version 0.0.1\n\
Author Big\n\
Description\n\
StreamWorkEditor2\n\
Build on %2 at %3\n";

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//QApplication::setStyle(new QPlastiqueStyle);
    QCoreApplication::setApplicationName("StreamWorkEditor2");
    QCoreApplication::setOrganizationName("Diginext");
    QCoreApplication::setOrganizationDomain("diginext.fr");
    QStringList liste_arg;
    int nb_args;
	QString stream_desc;
    SwFileLogRecorder_Class * log_recorder=NULL;
    int result=-1;
    bool doNewStream=true;
    //ISwService * un_service;
    QPixmap pixmap(":/StreamWorkEditor2/splash.png");
    SwSplash splash(pixmap);
    //QSplashScreen splash(pixmap);
    //splash.resize(QSize(800,600));
    splash.show();
    app.processEvents();
    LogView * lView=LogView::getInstance();
    QVector<ISwLogRecorder *> recorders;
    SwLogPreRecorder::PreAttachLogRecorder(&splash);
    SwLogPreRecorder::PreAttachLogRecorder(lView);
    //Test
    //QString test=QString(typeid(un_service).name());
    //QMessageBox::information(0,QString("typeid"),test,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    //Test

    try {
        SW_APP->Verbose();
        liste_arg=QCoreApplication::instance()->arguments();
        nb_args=liste_arg.count();
        for(int i=1;i<nb_args;i++) {
            QString test=liste_arg[i];
            //aide
            if (liste_arg[i]=="-h") {
                QString s=QString(VL_Help).arg(liste_arg[0]);
                QMessageBox::information(0,QString("Help"),s,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
                exit(0);
            }
            //version
            if (liste_arg[i]=="-v") {
                QString s=QString(VL_Version).arg(liste_arg[0]).arg(QString(__DATE__)).arg(QString(__TIME__));
                QMessageBox::information(0,QString("Version"),s,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
                exit(0);
            }           
            //Definition d'un fichier de log
            if (log_recorder==NULL && liste_arg[i]=="-log" && i+1<nb_args) {
        	    log_recorder=new SwFileLogRecorder_Class(liste_arg[i+1]);
                SW_APP->Logger().AttachLogRecorder(log_recorder);
            }
            if (liste_arg[i]=="-stream" ) {
                QFileInfo fi(liste_arg[i+1]);
                if (fi.exists()) {
                    doNewStream=false;
                }
            }

        }
        //Finalisation de l'initialisation
        SW_APP->FinalizeInitialisation();
        SW_APP->RegisterService(new EditionService());
        //Lancement
        MainWindow *window=new MainWindow;
        app.setQuitOnLastWindowClosed(false);
        //app.connect(&app, SIGNAL(lastWindowClosed()), window, SLOT(onQuit()));
        if (doNewStream) {
            window->onNewStream();
        }
        window->show();
        result=app.exec();
        //Destruction du log_recorder
        // Le detachement n'est pas utile puisque l'application est detruite
        delete log_recorder;
    } catch(SwException & se) {
        //L'application a levé une exception
        //Traiter l'exception
        QMessageBox::critical(0,QString("Aborting swlauncher because swexception..."),QString(se.what()),QMessageBox::Abort,QMessageBox::NoButton,QMessageBox::NoButton);
		result=-1;
   } catch(std::exception & e) {
        //L'application a levé une exception
        //Traiter l'exception
        QMessageBox::critical(0,QString("Aborting swlauncher because exception..."),QString(e.what()),QMessageBox::Abort,QMessageBox::NoButton,QMessageBox::NoButton);
		result=-1;
   }
    //Fin
    return result;
}
