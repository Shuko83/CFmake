#include <exception>

#include <QApplication>
#include <QMessageBox>
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
#include "QsLog.h"
#include "SwTime_ToolBox.h"
#include "ProductLicense.h"
#include "StreamControler.h"

#ifdef Q_OS_WIN
#include <psapi.h>
#endif

using namespace StreamWork::SwCore;

const char * VL_Help = "Usage: %1 [options]\n\
					   -h						this help\n\
					   -v						informations about this application\n\
					   -d						display core actions\n\
					   -ppath path				plugin path\n\
					   -pdesc pathdesc			plugin paths descriptor\n\
					   -stream streamfile		stream will be read in the specified path\n\
					   -models modelFile		file where all models are described\n\
					   -autostart				Start automatically the executor in the stream witch is in Timer/MainEntry mode\n\
					   -appDirPath				The application dir path \n\
					   -log logfile			logs will be write in the specified file\n\
					   -stylesheet styleFile	Add stylesheet to qapplication\n\
					   ";

const char * VL_Version = "%1\n\
						  Version 0.0.1\n\
						  Author Big\n\
						  Description\n\
						  StreamWorkEditor\n\
						  Build on %2 at %3\n";
						  
namespace
{
	struct Parameters
	{
		bool help;
		bool version;
		QString logFile;
		bool verbose;
		QString stream;
		bool noStyle;
		bool logTime;
		QString pluginPath;
		QString pluginPathFile;
		QString appDirPath;
		bool autostart;
		unsigned int restart;
		QString folderToSign;

		Parameters()
			: help(false)
			, version(false)
			, logFile()
			, verbose(false)
			, stream()
			, noStyle(false)
			, logTime(false)
			, appDirPath()
			, pluginPath()
			, pluginPathFile()
			, autostart(false)
			, restart(0)
			, folderToSign()
		{}
	};
}

Parameters readParamaters()
{
	Parameters params;
	QStringList liste_arg = QCoreApplication::instance()->arguments();
	int nb_args = liste_arg.count();
	for (int i = 1; i < nb_args; ++i)
	{
		// Aide
		if (liste_arg[i] == "-h")
			params.help = true;
		// Version
		else if (liste_arg[i] == "-v")
			params.version = true;
		// Definition d'un fichier de log
		else if (liste_arg[i] == "-log" && i + 1 < nb_args)
			params.logFile = liste_arg[++i];
		// Debug
		else if (liste_arg[i] == "-d")
			params.verbose = true;
		// Definition du stream
		else if (liste_arg[i] == "-stream" && i + 1 < nb_args)
			params.stream = liste_arg[++i];
		// Pas de style
		else if (liste_arg[i] == "-NoStreamworkStyle")
			params.stream = true;
		// Log du temps d'execution
		else if (liste_arg[i] == "-checktime")
			params.logTime = true;
		// Definition du plugin path
		else if (liste_arg[i] == "-ppath" && i + 1 < nb_args)
			params.pluginPath = liste_arg[++i];
		// Ajout d'un descripteur de paths
		else if (liste_arg[i] == "-pdesc" && i + 1 < nb_args)
			params.pluginPathFile = liste_arg[++i];
		// Modification de l'application dir path
		else if (liste_arg[i] == "-appDirPath" && i + 1 < nb_args)
			params.appDirPath = liste_arg[++i];
		// Demarrage automatique
		else if (liste_arg[i] == "-autostart")
			params.autostart = true;
		// Demarrage retarde
		else if (liste_arg[i] == "-restart" && i + 1 < nb_args)
			params.restart = liste_arg[++i].toUInt();
		// Permet de signer les fichiers contenu dans le dossier fourni en paramètre
		else if (liste_arg[i] == "-sign" && i + 1 < nb_args)
			params.folderToSign = liste_arg[++i];
	}
	return params;
}

int main(int argc, char *argv[])
{
	//Log Color
	QsLogger->start();

	// Qt Core Application
	QCoreApplication::setApplicationName("StreamWorkEditor");
	QCoreApplication::setOrganizationName("Diginext");
	QCoreApplication::setOrganizationDomain("diginext.fr");
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication app(argc, argv);

	// Licence
	ProductLicense productLicense(licenseId::ProductId::Product_STREAMWORK);
	bool licenseLost = false;

	QObject::connect(&productLicense, &ProductLicense::error, [&licenseLost]() {licenseLost = true; qApp->exit(EXIT_FAILURE); });
	if (!productLicense.takeFeatures({licenseId::FunctionId::Function_CORE, licenseId::FunctionId::Function_DEVELOPPER}))
	{
		QMessageBox::information(0, QStringLiteral("No license"), QStringLiteral("No licence for StreamWork could be found."), QMessageBox::Ok);
		return EXIT_FAILURE;
	}

	// SplashScreedn
	SwSplash splash(QPixmap(":/StreamWorkEditor/splash.png"));
	splash.show();
	app.processEvents();

	// Log Recorder
	SwLogPreRecorder::PreAttachLogRecorder(&splash);
	SwLogPreRecorder::PreAttachLogRecorder(LogView::getInstance());

	// SwApplication
	Parameters params = readParamaters();
	
	// Application des arguments
	// Aide
	if(params.help)
	{
		QString s = QString(VL_Help).arg(QCoreApplication::instance()->arguments().first());
		QMessageBox::information(0, QString("Help"), s, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		exit(0);
	}

	//Signature des streams
	if (!params.folderToSign.isEmpty())
	{
		QFileInfoList xmlInfoList;
		QDirIterator it(params.folderToSign, { QString("*.xml") }, QDir::Files | QDir::Readable, QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			xmlInfoList.append(it.fileInfo());
			it.next();
		}
 		for (QFileInfo fileInfo : xmlInfoList)
 		{
			QFile file(fileInfo.absoluteFilePath());
			StreamControler::SaveStream(file);
 		}
		exit(0);
	}
	// Version
	if (params.version)
	{
		QString s = QString(VL_Version).arg(QCoreApplication::instance()->arguments().first()).arg(QString(__DATE__)).arg(QString(__TIME__));
		QMessageBox::information(0, QString("Version"), s, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		exit(0);
	}
	// Definition d'un fichier de log
	SwFileLogRecorder_Class *log_recorder = nullptr;
	if (!params.logFile.isEmpty())
	{
		log_recorder = new SwFileLogRecorder_Class(params.logFile);
		SW_APP->Logger().AttachLogRecorder(log_recorder);
	}
	// Debug
	if (params.verbose)
	{
		SW_APP->Verbose();
	}
	// Pas de style
	if (!params.noStyle)
	{
		QFile res(":/StreamWorkEditor/css_file");
		res.open(QIODevice::ReadOnly | QIODevice::Text);
		qApp->setStyleSheet(res.readAll());
	}
	// Log du temps d'execution
	if (params.logTime)
	{
		QFile debugFile("log.csv");
		debugFile.open(QIODevice::WriteOnly);
		debugFile.write(QString("Component Name;InitializeResources;Execute;BeforeInterfaceAvailabilityChange;AfterInterfaceAvailabilityChange\n").toLatin1());
		debugFile.close();
		SW_APP->enableLogTime();
	}
	// Demarrage automatique
	if (params.autostart)
	{
		SW_APP->enableAutoStart();
	}
	// Modification de l'application dir path
	// Doit être fait avant pluginPath et pluginPathFile
	if (!params.appDirPath.isEmpty())
	{
		QDir dir(params.appDirPath);
		if (!dir.exists())
			qCritical() << QString("application dir path %1 doesn't exist").arg(params.appDirPath);
		else
			SW_APP->SetApplicationDirPath(dir.path());
	}
	// Definition du plugin path
	if (!params.pluginPath.isEmpty())
	{
		QDir dir(params.pluginPath);
		if (!dir.exists())
			qCritical() << QString("Plugin path %1 doesn't exist").arg(params.pluginPath);
		else
			SW_APP->ComponentsBank().AddPath(dir.path());
	}
	// Ajout d'un descripteur de paths
	if(!params.pluginPathFile.isEmpty())
	{
		if (!QFileInfo::exists(params.pluginPathFile))
			qCritical() << QString("PluginPathFile %1 doesn't exist").arg(params.pluginPathFile);
		else
			SW_APP->ComponentsBank().AddPaths(params.pluginPathFile);
	}
	// Demarrage retarde
	if(params.restart != 0)
	{
#ifdef Q_OS_WIN
		//Recherche du process origine,
		//Si trouve on attends et sinon, on contenu
		DWORD aProcesses[1024], cbNeeded, cProcesses;
		bool start = false;
		SW_DEBUG("Waiting to restart");
		while (!start)
		{
			start = true;
			SwTime_ToolBox::InternalSleep((unsigned long)50); //50 ms d'attente  
			if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
				continue;

			// Calculate how many process identifiers were returned.
			cProcesses = cbNeeded / sizeof(DWORD);
			// Print the name and process identifier for each process.
			for (unsigned int i = 0; i < cProcesses; ++i)
			{
				if (aProcesses[i] == params.restart)
					start = false;
			}
		}
#endif
	}
	
	SW_APP->enableDeveloperMode();

	// Finalisation de l'initialisation
	SW_APP->FinalizeInitialisation();
	
	EditionService *editionService = new EditionService();
	SW_APP->RegisterService(editionService);

	// Lancement
	MainWindow *window = new MainWindow(params.stream);
	window->setIsMasterWindow(true);
	app.setQuitOnLastWindowClosed(false);
	window->show();
	int result = app.exec();

	// Destruction du log_recorder
	delete log_recorder;
	
	// Destruction du service d'édition
	SW_APP->UnregisterService(editionService->GetServiceName());
    delete editionService;

	return result;
}
