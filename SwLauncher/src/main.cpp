#include <exception>

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "Tools/Log/SwFileLogRecorder_Class.h"
//#include <QMainWindow>
#include "Tools/Exception/SwException.h"
using namespace StreamWork::SwCore;

const char * VL_Help="Usage: %1 [options]\n\
    -h                   this help\n\
    -v                   informations about this application\n\
    -d                   display core actions\n\
    -ppath path          plugin path\n\
    -pdesc pathdesc      plugin paths descriptor\n\
    -stream streamfile   stream will be read in the specified path\n\
    -log logfile         logs will be write in the specified file\n";

const char * VL_Version="%1\n\
Version 1.0.0\n\
Author Big\n\
Description\n\
GUI stream execution\n\
Build on %2 at %3\n";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	//QApplication::setStyle(new QPlastiqueStyle);
    QStringList liste_arg;
    int nb_args;
	QString stream_desc;
    SwFileLogRecorder_Class * log_recorder=NULL;
    int result=-1;
    /*ISwService * un_service;*/

    //Test
    //QString test=QString(typeid(un_service).name());
    //QMessageBox::information(0,QString("typeid"),test,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    //Test

    try {

        liste_arg=QCoreApplication::instance()->arguments();
        nb_args=liste_arg.count();
        for(int i=1;i+1<nb_args;i++) {
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
            //display
            if (liste_arg[i]=="-d") {
                SW_APP->Verbose();
            }
            //Ajout d'un path
            if (liste_arg[i]=="-ppath" && i+1<nb_args) {
                QDir dir(liste_arg[i+1]);
                if (!dir.exists()) {
                    //LAUNCH_SWEXCEPTION("swlauncher",QString("Plugin path %1 doesn't exist").arg(liste_arg[i+1]));
                    SwException _owner(QString("swlauncher"),QString(__FILE__),__LINE__,QString(QString("Plugin path %1 doesn't exist").arg(liste_arg[i+1])));throw(_owner);
                } else {
                    SW_APP->ComponentsBank().AddPath(dir.path());
                }
            }
            //Ajout d'un descripteur de paths
            if (liste_arg[i]=="-pdesc" && i+1<nb_args) {
                QFile file(liste_arg[i+1]);
                if (!file.exists()) {
                    LAUNCH_SWEXCEPTION("swlauncher",QString("Plugin path %1 doesn't exist").arg(liste_arg[i+1]));
                } else {
                    SW_APP->ComponentsBank().AddPaths(liste_arg[i+1]);
                }
            }
            //Specification d'un stream
            if (stream_desc.isEmpty() && liste_arg[i]=="-stream" && i+1<nb_args) {
        	    QFile file;
                file.setFileName(liste_arg[i+1]);
                if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    LAUNCH_SWEXCEPTION("swlauncher",QString("File stream %1 doesn't exist in %2 or can't be opened").arg(liste_arg[i+1]).arg(QDir::currentPath()));
        	    }
        	    stream_desc=QString(file.readAll());
            }
            //Definition d'un fichier de log
            if (log_recorder==NULL && liste_arg[i]=="-log" && i+1<nb_args) {
        	    log_recorder=new SwFileLogRecorder_Class(liste_arg[i+1]);
                SW_APP->Logger().AttachLogRecorder(log_recorder);
            }

        }
        //Lancement
        result=SW_APP->Launch(stream_desc);
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
