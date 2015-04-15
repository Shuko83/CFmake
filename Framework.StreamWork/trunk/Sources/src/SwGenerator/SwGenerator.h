#ifndef SWGENERATOR_H
#define SWGENERATOR_H

#include <QMainWindow>
#include "ui_SWGenerator.h"
#include <QCompleter>
#include <QSettings>
#include <QCTemplate.h>
#include "Component.h"

#include <QProgressDialog>

struct XmlPath
{
	QString pathLib;
	QString pathInclude;
};



#define INFO_LOG QString("<img src=\":/led/green\" /> ")
#define ERROR_LOG QString("<img src=\":/led/red\" /> ")
#define WARNING_LOG QString("<img src=\":/led/orange\" /> ")
#define TAB_LOG QString("&nbsp;&nbsp;&nbsp;&nbsp;")


class SWGenerator : public QMainWindow
{
	Q_OBJECT

public:
	SWGenerator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SWGenerator();

	bool generatePlugin();


protected slots:
	void firstPageChange(const QString&val);
	void nextPage();
	void prevPage();
	void nextPage_2();
	void prevPage_2();

	void closeEvent(QCloseEvent *event);
	void nbComponentChanged(int nbComponent);
	void doWork();
	void openExplorer(const QUrl&);

	void switchMainSW();
	void listAllPlugin();
	void doWork_2();

private:
	void writeSettings();
	void readSettings();
	void readXml();
	void fillListLibWidget();

	bool generatePluginProTpl();
	bool generatePluginPriTpl();
	bool generateGenerateVcprojBatTpl();
	bool generateRessources();
	bool generateSources();
	bool generateComponentSourcesBaseModelConsumer();
	bool generateComponentSourcesBaseModelConsumerOnly();

	QString headerLog(QString val);
	QString subHeaderLog(QString val);
	void testAA();

	QString interfaceToVarName(QString name);
	QStringList findFiles(const QString &startDir, QStringList filters);
	void fillPluginList();
	bool generateComponent();

	void appendRessources();
	void appendPluginPriTpl();
	void appendSources();
	bool generateCompontentCommon(Component *cp,QString componentName, QString dirComponent,QString outputFileNameCPP ,QString outputFileNameH ,QTextBrowser * log);
private:
	Ui::SWGeneratorClass ui;
	QCompleter *_completer;
	QString _outputPath;
	QCTemplate _zTpl;

	int _nbComponents;
	QList<Component*> _listComponent;

	QMap<QString,XmlPath> _libMap;

	QProgressDialog  * _progDial;

	QMap<QString,QString> _pathPlugin;

	
};

#endif // SWGENERATOR_H
