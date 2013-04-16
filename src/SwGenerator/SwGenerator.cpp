#include "SWGenerator.h"
#include <windows.h>
#include <Lmcons.h>
#include <QFileSystemModel>
#include <QCloseEvent>
#include <QDomDocument>
#include <iostream>
#include <QDebug>
#include <QDateTime>

#include "Component.h"
#include "Property.h"
#include "Shortcut.h"
#include "Interface.h"


//-------------------------------------------------------------------------
SWGenerator::SWGenerator(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.TE_log->setAutoFormatting(QTextEdit::AutoAll);
	ui.TE_log_2->setAutoFormatting(QTextEdit::AutoAll);
	ui.PB_Next->setDisabled(true);

	_outputPath = "c:\\tmp";

	_nbComponents = 0;

	_completer = new QCompleter(this);
	_completer->setMaxVisibleItems(10);
	_completer->setWrapAround(true);
	_completer->setCaseSensitivity(Qt::CaseInsensitive);
	_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

	QFileSystemModel *fsModel = new QFileSystemModel(_completer);
	fsModel->setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	fsModel->setRootPath("c:/Projects/");
	_completer->setModel(fsModel);

	ui.LE_OutputDir->setCompleter(_completer);

	connect(ui.LE_namespace1,SIGNAL(textChanged ( const QString & )),this,SLOT(firstPageChange(const QString&)));
	connect(ui.LE_namespace2,SIGNAL(textChanged ( const QString & )),this,SLOT(firstPageChange(const QString&)));
	connect(ui.LE_namespace3,SIGNAL(textChanged ( const QString & )),this,SLOT(firstPageChange(const QString&)));
	connect(ui.LE_OutputDir,SIGNAL(textChanged ( const QString & )),this,SLOT(firstPageChange(const QString&)));
	connect(ui.LE_Author,SIGNAL(textChanged ( const QString & )),this,SLOT(firstPageChange(const QString&)));

	connect(ui.PB_Next,SIGNAL(clicked()),this,SLOT(nextPage()));
	connect(ui.PB_Prev,SIGNAL(clicked()),this,SLOT(prevPage()));

	connect(ui.PB_Next_2,SIGNAL(clicked()),this,SLOT(nextPage_2()));
	connect(ui.PB_Prev_2,SIGNAL(clicked()),this,SLOT(prevPage_2()));

	connect(ui.spinBox,SIGNAL(valueChanged(int)),this,SLOT(nbComponentChanged(int)));

	connect(ui.Pb_make,SIGNAL(clicked()),this,SLOT(doWork()));
	connect(ui.Pb_make_2,SIGNAL(clicked()),this,SLOT(doWork_2()));
	
	connect(ui.TE_log,SIGNAL(anchorClicked (const QUrl&)),this,SLOT(openExplorer(const QUrl&)));
	connect(ui.TE_log_2,SIGNAL(anchorClicked (const QUrl&)),this,SLOT(openExplorer(const QUrl&)));

	connect(ui.pb_plugin,SIGNAL(clicked()),this,SLOT(switchMainSW()));
	connect(ui.pb_component,SIGNAL(clicked()),this,SLOT(switchMainSW()));
	connect(ui.pb_reload,SIGNAL(clicked()),this,SLOT(listAllPlugin()));

	ui.TE_log->setOpenLinks(false);
	ui.TE_log_2->setOpenLinks(false);
	readXml();

	fillListLibWidget();

	Component * test = new Component();
	ui.v_cp->addWidget(test);

	/*Load the setting previously saved*/
	readSettings();

	//On rempli la liste des plugin
	fillPluginList();
	
	_progDial = new QProgressDialog("Find Plugins","Cancel",0,0);
}




//-------------------------------------------------------------------------
SWGenerator::~SWGenerator()
{

}


//-------------------------------------------------------------------------
void SWGenerator::writeSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"SwGenerator", "Config");

	settings.beginGroup("GlobalAPP");
	settings.setValue("size", size()); //save the size of the window
	settings.setValue("pos", pos()); //save the pos of the window
	settings.endGroup();



	settings.beginGroup("PreviousSettings");
	settings.setValue("namespace1", ui.LE_namespace1->text()); 
	settings.setValue("namespace2", ui.LE_namespace2->text()); 
	settings.setValue("namespace3", ui.LE_namespace3->text()); 
	settings.setValue("output_dir", ui.LE_OutputDir->text()); 
	settings.setValue("author", ui.LE_Author->text()); 


	settings.beginWriteArray("SelectedLibs");
	int nbItem = ui.LW_libDependency->count();
	int index = 0;
	for (int i = 0; i < nbItem; ++i) 
	{
		if(ui.LW_libDependency->item(i)->checkState() == Qt::Checked)
		{
			settings.setArrayIndex(index);
			settings.setValue("name", ui.LW_libDependency->item(i)->text());
			index++;
		}
	}
	settings.endArray();


	settings.beginWriteArray("PathPlugins");
	index = 0;
	QMapIterator<QString,QString> it(_pathPlugin);

	while (it.hasNext()) 
	{
		it.next();
		settings.setArrayIndex(index);
		settings.setValue("name", it.key());
		settings.setValue("path", it.value());
		index++;
	}
	settings.endArray();

	//write settings about component
	settings.beginWriteArray("Components");
	for (int i = 0; i < _nbComponents ; i++)
	{
		Component * c = dynamic_cast<Component*>(ui.SW_Plugin->widget(i+2));
		if(c)
		{
			settings.setArrayIndex(i);
			settings.setValue("baseClass",c->ui.Cb_baseClass->currentIndex());
			settings.setValue("name",c->ui.Le_name->text());
			settings.setValue("desc",c->ui.Le_desc->text());
			settings.setValue("icon",c->getIconPath());
			settings.setValue("executable",c->ui.Cb_executable->isChecked());
			settings.setValue("OwnerConfigurable",c->ui.Cb_ownerConfigurable->isChecked());
			settings.setValue("Owner",c->ui.Cb_owner->isChecked());

			//Pins
			settings.setValue("hasPins",c->ui.Gb_pins->isChecked());
			settings.beginWriteArray("pins");
			{
				settings.setArrayIndex(0);
				settings.setValue("inPin",c->ui.Cb_pinIn->isChecked());
				settings.setArrayIndex(1);
				settings.setValue("outPin",c->ui.Cb_pinOut->isChecked());
			}
			settings.endArray();

			//Properties
			settings.setValue("hasProperties",c->ui.Gb_properties->isChecked());
			settings.beginWriteArray("properties");
			for (int i = 0; i < c->ui.Sb_properties->value() ; i++)
			{
				QLayoutItem * li = c->ui.v_properties->itemAt(i);
				if(li)
				{
					Property * p = dynamic_cast<Property*>(li->widget());
					if(p)
					{
						settings.setArrayIndex(i);
						settings.setValue("name",p->ui.Le_name->text());
						settings.setValue("type",p->ui.Cb_type->currentIndex());
						settings.setValue("default",p->ui.le_defaultValue->text());
					}
				}
			}
			settings.endArray();

			settings.setValue("hasShorcuts",c->ui.Gb_shortcuts->isChecked());
			settings.setValue("shortcutComponentName",c->ui.Le_shortcutComponentName->text());
			settings.beginWriteArray("shortcuts");
			for (int i = 0; i < c->ui.Sb_shortcuts->value() ; i++)
			{
				QLayoutItem * li = c->ui.v_shortcuts->itemAt(i);
				if(li)
				{
					Shortcut * s = dynamic_cast<Shortcut*>(li->widget());
					if(s)
					{
						settings.setArrayIndex(i);
						settings.setValue("name",s->ui.Le_displayName->text());
					}
				}
			}
			settings.endArray();

			settings.setValue("hasProvidedInterfaces",c->ui.Gb_IProvided->isChecked());
			settings.beginWriteArray("providedInterfaces");
			for (int i = 0; i < c->ui.Sb_pIntefaces->value() ; i++)
			{
				QLayoutItem * li = c->ui.v_pInterfaces->itemAt(i);
				if(li)
				{
					Interface * it = dynamic_cast<Interface*>(li->widget());
					if(it)
					{
						settings.setArrayIndex(i);
						settings.setValue("name",it->ui.Le_name->text());
						settings.setValue("type",it->ui.Le_type->text());
						settings.setValue("include",it->ui.Le_include->text());
					}
				}
			}
			settings.endArray();

			settings.setValue("hasConsumedInterfaces",c->ui.Gb_IConsummed->isChecked());
			settings.beginWriteArray("consumedInterfaces");
			for (int i = 0; i < c->ui.Sb_cIntefaces->value() ; i++)
			{
				QLayoutItem * li = c->ui.v_cInterfaces->itemAt(i);
				if(li)
				{
					Interface * it = dynamic_cast<Interface*>(li->widget());
					if(it)
					{
						settings.setArrayIndex(i);
						settings.setValue("name",it->ui.Le_name->text());
						settings.setValue("type",it->ui.Le_type->text());
						settings.setValue("include",it->ui.Le_include->text());
					}
				}
			}
			settings.endArray();
		}


	}
	settings.endArray();



	//write settings about component
	settings.beginWriteArray("componentAlone");
	Component * c = dynamic_cast<Component*>(ui.v_cp->itemAt(0)->widget());
	if(c)
	{
		settings.setArrayIndex(0);
		settings.setValue("baseClass",c->ui.Cb_baseClass->currentIndex());
		settings.setValue("name",c->ui.Le_name->text());
		settings.setValue("desc",c->ui.Le_desc->text());
		settings.setValue("icon",c->getIconPath());
		settings.setValue("executable",c->ui.Cb_executable->isChecked());
		settings.setValue("OwnerConfigurable",c->ui.Cb_ownerConfigurable->isChecked());
		settings.setValue("Owner",c->ui.Cb_owner->isChecked());

		//Pins
		settings.setValue("hasPins",c->ui.Gb_pins->isChecked());
		settings.beginWriteArray("pins");
		{
			settings.setArrayIndex(0);
			settings.setValue("inPin",c->ui.Cb_pinIn->isChecked());
			settings.setArrayIndex(1);
			settings.setValue("outPin",c->ui.Cb_pinOut->isChecked());
		}
		settings.endArray();

		//Properties
		settings.setValue("hasProperties",c->ui.Gb_properties->isChecked());
		settings.beginWriteArray("properties");
		for (int i = 0; i < c->ui.Sb_properties->value() ; i++)
		{
			QLayoutItem * li = c->ui.v_properties->itemAt(i);
			if(li)
			{
				Property * p = dynamic_cast<Property*>(li->widget());
				if(p)
				{
					settings.setArrayIndex(i);
					settings.setValue("name",p->ui.Le_name->text());
					settings.setValue("type",p->ui.Cb_type->currentIndex());
					settings.setValue("default",p->ui.le_defaultValue->text());
				}
			}
		}
		settings.endArray();

		settings.setValue("hasShorcuts",c->ui.Gb_shortcuts->isChecked());
		settings.setValue("shortcutComponentName",c->ui.Le_shortcutComponentName->text());
		settings.beginWriteArray("shortcuts");
		for (int i = 0; i < c->ui.Sb_shortcuts->value() ; i++)
		{
			QLayoutItem * li = c->ui.v_shortcuts->itemAt(i);
			if(li)
			{
				Shortcut * s = dynamic_cast<Shortcut*>(li->widget());
				if(s)
				{
					settings.setArrayIndex(i);
					settings.setValue("name",s->ui.Le_displayName->text());
				}
			}
		}
		settings.endArray();

		settings.setValue("hasProvidedInterfaces",c->ui.Gb_IProvided->isChecked());
		settings.beginWriteArray("providedInterfaces");
		for (int i = 0; i < c->ui.Sb_pIntefaces->value() ; i++)
		{
			QLayoutItem * li = c->ui.v_pInterfaces->itemAt(i);
			if(li)
			{
				Interface * it = dynamic_cast<Interface*>(li->widget());
				if(it)
				{
					settings.setArrayIndex(i);
					settings.setValue("name",it->ui.Le_name->text());
					settings.setValue("type",it->ui.Le_type->text());
					settings.setValue("include",it->ui.Le_include->text());
				}
			}
		}
		settings.endArray();

		settings.setValue("hasConsumedInterfaces",c->ui.Gb_IConsummed->isChecked());
		settings.beginWriteArray("consumedInterfaces");
		for (int i = 0; i < c->ui.Sb_cIntefaces->value() ; i++)
		{
			QLayoutItem * li = c->ui.v_cInterfaces->itemAt(i);
			if(li)
			{
				Interface * it = dynamic_cast<Interface*>(li->widget());
				if(it)
				{
					settings.setArrayIndex(i);
					settings.setValue("name",it->ui.Le_name->text());
					settings.setValue("type",it->ui.Le_type->text());
					settings.setValue("include",it->ui.Le_include->text());
				}
			}
		}
		settings.endArray();
	}
	settings.endArray();

	settings.endGroup();

}

//-------------------------------------------------------------------------
void SWGenerator::readSettings()
{
	//Get username of the windows sessions
	TCHAR name [ UNLEN + 1 ];
	DWORD size = UNLEN + 1;
	QString username = "";
	if (GetUserName( (TCHAR*)name, &size ))
		username = QString::fromUtf16(name).toUpper();


	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"SwGenerator", "Config");

	settings.beginGroup("GlobalAPP");
	resize(settings.value("size", QSize(400, 400)).toSize()); //load the size of the window
	move(settings.value("pos", QPoint(200, 200)).toPoint());//load the pos of the window
	settings.endGroup();

	settings.beginGroup("PreviousSettings");
	ui.LE_namespace1->setText(settings.value("namespace1", "").toString()); 
	ui.LE_namespace2->setText(settings.value("namespace2", "").toString()); 
	ui.LE_namespace3->setText(settings.value("namespace3", "").toString()); 
	ui.LE_OutputDir->setText(settings.value("output_dir", "").toString()); 
	QString tempDataUsername = settings.value("author", username).toString();
	username = (tempDataUsername==""?username:tempDataUsername);
	ui.LE_Author->setText(username); 


	int sizeL = settings.beginReadArray("SelectedLibs");
	for (int i = 0; i < sizeL; ++i) 
	{
		settings.setArrayIndex(i);
		QList<QListWidgetItem*> list = ui.LW_libDependency->findItems(settings.value("name").toString(),Qt::MatchContains);
		if(!list.isEmpty() && list.count() == 1)
		{
			list.at(0)->setCheckState(Qt::Checked);
		}
	}
	settings.endArray();


	sizeL = settings.beginReadArray("PathPlugins");
	for (int i = 0; i < sizeL; ++i) 
	{
		settings.setArrayIndex(i);
		_pathPlugin.insert(settings.value("name").toString(),settings.value("path").toString());
	}
	settings.endArray();

	//Read settings about component

	int sizeC = settings.beginReadArray("Components");
	ui.spinBox->setValue(sizeC);
	for (int i = 0; i < sizeC ; i++)
	{
		Component * c = dynamic_cast<Component*>(ui.SW_Plugin->widget(i+2));
		if(c)
		{
			settings.setArrayIndex(i);
			c->ui.Cb_baseClass->setCurrentIndex(settings.value("baseClass").toUInt());
			c->ui.Le_name->setText(settings.value("name").toString());
			c->ui.Le_desc->setText(settings.value("desc").toString());
			QString iconTmp = settings.value("icon").toString();
			if(iconTmp != "")
				c->setIconPath(iconTmp);

			c->ui.Cb_executable->setChecked(settings.value("executable").toBool());
			c->ui.Cb_ownerConfigurable->setChecked(settings.value("OwnerConfigurable").toBool());
			c->ui.Cb_owner->setChecked(settings.value("Owner").toBool());
			c->ui.Gb_pins->setChecked(settings.value("hasPins").toBool());

			settings.beginReadArray("Pins");
			settings.setArrayIndex(0);
			c->ui.Cb_pinIn->setChecked(settings.value("inPin").toBool());
			settings.setArrayIndex(1);
			c->ui.Cb_pinOut->setChecked(settings.value("outPin").toBool());
			settings.endArray();

			c->ui.Gb_properties->setChecked(settings.value("hasProperties").toBool());

			int sizeP = settings.beginReadArray("properties");
			c->ui.Sb_properties->setValue(sizeP);
			for (int j = 0; j < sizeP ; j++)
			{
				QLayoutItem * li = c->ui.v_properties->itemAt(j);
				if(li)
				{
					Property * p = dynamic_cast<Property*>(li->widget());
					if(p)
					{
						settings.setArrayIndex(j);
						p->ui.Le_name->setText(settings.value("name").toString());
						p->ui.Cb_type->setCurrentIndex(settings.value("type").toUInt());
						p->ui.le_defaultValue->setText(settings.value("default").toString());
					}
				}
			}
			settings.endArray();


			c->ui.Gb_shortcuts->setChecked(settings.value("hasShorcuts").toBool());
			c->ui.Le_shortcutComponentName->setText(settings.value("shortcutComponentName").toString());

			int sizeS = settings.beginReadArray("shortcuts");
			c->ui.Sb_shortcuts->setValue(sizeS);
			for (int j = 0; j < sizeS ; j++)
			{
				QLayoutItem * li = c->ui.v_shortcuts->itemAt(j);
				if(li)
				{
					Shortcut * p = dynamic_cast<Shortcut*>(li->widget());
					if(p)
					{
						settings.setArrayIndex(j);
						p->ui.Le_displayName->setText(settings.value("name").toString());
					}
				}
			}
			settings.endArray();


			c->ui.Gb_IProvided->setChecked(settings.value("hasProvidedInterfaces").toBool());
			int sizePi = settings.beginReadArray("providedInterfaces");
			c->ui.Sb_pIntefaces->setValue(sizePi);
			for (int j = 0; j < sizePi ; j++)
			{
				QLayoutItem * li = c->ui.v_pInterfaces->itemAt(j);
				if(li)
				{
					Interface * p = dynamic_cast<Interface*>(li->widget());
					if(p)
					{
						settings.setArrayIndex(j);
						p->ui.Le_name->setText(settings.value("name").toString());
						p->ui.Le_type->setText(settings.value("type").toString());
						p->ui.Le_include->setText(settings.value("include").toString());
					}
				}
			}
			settings.endArray();


			c->ui.Gb_IConsummed->setChecked(settings.value("hasConsumedInterfaces").toBool());
			int sizeCi = settings.beginReadArray("consumedInterfaces");
			c->ui.Sb_cIntefaces->setValue(sizeCi);
			for (int j = 0; j < sizeCi ; j++)
			{
				QLayoutItem * li = c->ui.v_cInterfaces->itemAt(j);
				if(li)
				{
					Interface * p = dynamic_cast<Interface*>(li->widget());
					if(p)
					{
						settings.setArrayIndex(j);
						p->ui.Le_name->setText(settings.value("name").toString());
						p->ui.Le_type->setText(settings.value("type").toString());
						p->ui.Le_include->setText(settings.value("include").toString());
					}
				}
			}
			settings.endArray();
		}


	}
	settings.endArray();

	int sizeCA = settings.beginReadArray("componentAlone");
	Component * c = dynamic_cast<Component*>(ui.v_cp->itemAt(0)->widget());
	if(c && sizeCA != 0)
	{
		settings.setArrayIndex(0);
		c->ui.Cb_baseClass->setCurrentIndex(settings.value("baseClass").toUInt());
		c->ui.Le_name->setText(settings.value("name").toString());
		c->ui.Le_desc->setText(settings.value("desc").toString());
		QString iconTmp = settings.value("icon").toString();
		if(iconTmp != "")
			c->setIconPath(iconTmp);

		c->ui.Cb_executable->setChecked(settings.value("executable").toBool());
		c->ui.Cb_ownerConfigurable->setChecked(settings.value("OwnerConfigurable").toBool());
		c->ui.Cb_owner->setChecked(settings.value("Owner").toBool());
		c->ui.Gb_pins->setChecked(settings.value("hasPins").toBool());

		settings.beginReadArray("Pins");
		settings.setArrayIndex(0);
		c->ui.Cb_pinIn->setChecked(settings.value("inPin").toBool());
		settings.setArrayIndex(1);
		c->ui.Cb_pinOut->setChecked(settings.value("outPin").toBool());
		settings.endArray();

		c->ui.Gb_properties->setChecked(settings.value("hasProperties").toBool());

		int sizeP = settings.beginReadArray("properties");
		c->ui.Sb_properties->setValue(sizeP);
		for (int j = 0; j < sizeP ; j++)
		{
			QLayoutItem * li = c->ui.v_properties->itemAt(j);
			if(li)
			{
				Property * p = dynamic_cast<Property*>(li->widget());
				if(p)
				{
					settings.setArrayIndex(j);
					p->ui.Le_name->setText(settings.value("name").toString());
					p->ui.Cb_type->setCurrentIndex(settings.value("type").toUInt());
					p->ui.le_defaultValue->setText(settings.value("default").toString());
				}
			}
		}
		settings.endArray();


		c->ui.Gb_shortcuts->setChecked(settings.value("hasShorcuts").toBool());
		c->ui.Le_shortcutComponentName->setText(settings.value("shortcutComponentName").toString());

		int sizeS = settings.beginReadArray("shortcuts");
		c->ui.Sb_shortcuts->setValue(sizeS);
		for (int j = 0; j < sizeS ; j++)
		{
			QLayoutItem * li = c->ui.v_shortcuts->itemAt(j);
			if(li)
			{
				Shortcut * p = dynamic_cast<Shortcut*>(li->widget());
				if(p)
				{
					settings.setArrayIndex(j);
					p->ui.Le_displayName->setText(settings.value("name").toString());
				}
			}
		}
		settings.endArray();


		c->ui.Gb_IProvided->setChecked(settings.value("hasProvidedInterfaces").toBool());
		int sizePi = settings.beginReadArray("providedInterfaces");
		c->ui.Sb_pIntefaces->setValue(sizePi);
		for (int j = 0; j < sizePi ; j++)
		{
			QLayoutItem * li = c->ui.v_pInterfaces->itemAt(j);
			if(li)
			{
				Interface * p = dynamic_cast<Interface*>(li->widget());
				if(p)
				{
					settings.setArrayIndex(j);
					p->ui.Le_name->setText(settings.value("name").toString());
					p->ui.Le_type->setText(settings.value("type").toString());
					p->ui.Le_include->setText(settings.value("include").toString());
				}
			}
		}
		settings.endArray();


		c->ui.Gb_IConsummed->setChecked(settings.value("hasConsumedInterfaces").toBool());
		int sizeCi = settings.beginReadArray("consumedInterfaces");
		c->ui.Sb_cIntefaces->setValue(sizeCi);
		for (int j = 0; j < sizeCi ; j++)
		{
			QLayoutItem * li = c->ui.v_cInterfaces->itemAt(j);
			if(li)
			{
				Interface * p = dynamic_cast<Interface*>(li->widget());
				if(p)
				{
					settings.setArrayIndex(j);
					p->ui.Le_name->setText(settings.value("name").toString());
					p->ui.Le_type->setText(settings.value("type").toString());
					p->ui.Le_include->setText(settings.value("include").toString());
				}
			}
		}
		settings.endArray();
	}
	settings.endArray();

	settings.endGroup();
}


//------------------------------------------------------------------------
void SWGenerator::closeEvent(QCloseEvent *event)
{
	writeSettings();
	event->accept();
}


//-------------------------------------------------------------------------
void SWGenerator::firstPageChange( const QString&/*val*/ )
{
	QString ns1 = ui.LE_namespace1->text().toLower();
	QString ns2 = ui.LE_namespace2->text().toLower();
	QString ns3 = ui.LE_namespace3->text().toLower();

	ui.LE_namespace1->setText(ns1);
	ui.LE_namespace2->setText(ns2);
	ui.LE_namespace3->setText(ns3);

	ns1 = ns1.mid(0,1).toUpper()+ns1.mid(1,ns1.length()-1);
	ns2 = ns2.mid(0,1).toUpper()+ns2.mid(1,ns2.length()-1);
	ns3 = ns3.mid(0,1).toUpper()+ns3.mid(1,ns3.length()-1);

	ui.L_PluginName->setText(ns1+ns2+ns3);

	if(	ui.LE_Author->text() != "" &&
		ui.LE_namespace1->text() != "" && 
		ui.LE_namespace2->text() != "" && 
		ui.LE_namespace3->text() != "" && 
		ui.LE_OutputDir->text() != ""
		)
	{
		ui.PB_Next->setDisabled(false);
	}
	else
	{
		ui.PB_Next->setDisabled(true);
	}
}

//-------------------------------------------------------------------------
void SWGenerator::nextPage()
{
	ui.TE_log->clear();
	int currentIndex = ui.SW_Plugin->currentIndex();
	int maxIndex = ui.SW_Plugin->count();
	int newIndex = 0;
	if(currentIndex+1 > maxIndex-1)
	{
		newIndex = maxIndex-1;
	}
	else
	{
		newIndex = currentIndex+1;
		ui.PB_Prev->setDisabled(false);
		if(newIndex == maxIndex-1)
			ui.PB_Next->setDisabled(true);
	}

	//Action page
	if(currentIndex == 0)
	{
		//generatePlugin();
		if(_nbComponents == 0)
			ui.PB_Next->setDisabled(true);
	}
	if(currentIndex > 1)
	{

	}

	ui.SW_Plugin->setCurrentIndex(newIndex);
}

//-------------------------------------------------------------------------
void SWGenerator::prevPage()
{
	ui.TE_log->clear();
	int currentIndex = ui.SW_Plugin->currentIndex();
	int newIndex = 0;


	if(currentIndex == 0)
	{
		ui.stackedWidget->setCurrentIndex(0);
		return;
	}

	if(currentIndex+1 >= 0)
	{
		newIndex = currentIndex-1;
		ui.PB_Next->setDisabled(false);
	}
	else
	{
		newIndex = currentIndex;
	}

	if(newIndex == 0)
	{
		ui.PB_Next->setDisabled(false);
	}

	ui.SW_Plugin->setCurrentIndex(newIndex);
}


//-------------------------------------------------------------------------
void SWGenerator::nextPage_2()
{
	ui.TE_log_2->clear();
	int currentIndex = ui.sw_component->currentIndex();
	int maxIndex = ui.sw_component->count();
	int newIndex = 0;
	if(currentIndex+1 > maxIndex-1)
	{
		newIndex = maxIndex-1;
	}
	else
	{
		newIndex = currentIndex+1;
		ui.PB_Prev_2->setDisabled(false);
		if(newIndex == maxIndex-1)
			ui.PB_Next_2->setDisabled(true);
	}

	if(newIndex == 1 && ui.lw_listplugin->selectedItems().isEmpty())
	{
		QMessageBox::critical(this,"Selection missmatch","Please select a plugin");
		return;
	}
	
	ui.sw_component->setCurrentIndex(newIndex);
}

//-------------------------------------------------------------------------
void SWGenerator::prevPage_2()
{
	ui.TE_log_2->clear();
	int currentIndex = ui.sw_component->currentIndex();
	int newIndex = 0;

	if(currentIndex == 0)
	{
		ui.stackedWidget->setCurrentIndex(0);
		return;
	}

	if(currentIndex+1 >= 0)
	{
		newIndex = currentIndex-1;
		ui.PB_Next_2->setDisabled(false);
	}
	else
	{
		newIndex = currentIndex;
	}

	if(newIndex == 0)
	{
		ui.PB_Next_2->setDisabled(false);
	}

	ui.sw_component->setCurrentIndex(newIndex);
}

//-------------------------------------------------------------------------
bool SWGenerator::generatePlugin()
{
	ui.TE_log->clear();
	_zTpl.clear();

	ui.TE_log->append(headerLog("Manage Directories"));

	_outputPath = ui.LE_OutputDir->text()+"/"+ui.LE_namespace1->text()+"/"+ui.LE_namespace2->text()+"/"+ui.LE_namespace3->text()+"/";
	_outputPath = _outputPath.replace("/","\\").replace("\\\\","\\");

	if(!QDir::temp().exists(_outputPath))
	{
		QDir::temp().mkpath(_outputPath);
		ui.TE_log->append(INFO_LOG+"Creating directory <b>"+_outputPath+"</b>");
	}
	else
	{
		ui.TE_log->append(INFO_LOG+"Directory <b>"+_outputPath+"</b> already exist.");
	}

	QString ressourceDir = _outputPath+"resources";
	if(!QDir::temp().exists(ressourceDir))
	{
		QDir::temp().mkpath(ressourceDir);
		ui.TE_log->append(INFO_LOG+"Creating directory <b>"+ressourceDir+"</b>");
	}
	else
	{
		ui.TE_log->append(INFO_LOG+"Directory <b>"+ressourceDir+"</b> already exist.");
	}


	//Check if the file ../../../../plugin_common.pri exists

	if(!QFile::exists(_outputPath+"../../../../plugin_common.pri"))
	{
		//Full path
		QStringList tmpList = _outputPath.split("\\");
		for (int i = 0; i < 5 ; i++)
		{
			tmpList.removeLast();
		}
		QString path ="";
		for (int i = 0; i < tmpList.count() ; i++)
		{
			path+=tmpList.at(i)+"\\";
		}

		ui.TE_log->append(WARNING_LOG+"File <b>"+path+"plugin_common.pri"+"</b> is missing.");

	}

	ui.TE_log->append("<br/>"+headerLog("Manage Files"));


	QString ns1 = ui.LE_namespace1->text().toLower();
	QString ns2 = ui.LE_namespace2->text().toLower();
	QString ns3 = ui.LE_namespace3->text().toLower();
	ns1 = ns1.mid(0,1).toUpper()+ns1.mid(1,ns1.length()-1);
	ns2 = ns2.mid(0,1).toUpper()+ns2.mid(1,ns2.length()-1);
	ns3 = ns3.mid(0,1).toUpper()+ns3.mid(1,ns3.length()-1);


	//Variables TPL globale
	_zTpl[ "PLUGIN_NAME" ] = ui.L_PluginName->text();
	_zTpl[ "PLUGIN_NAME_UPPER" ] = ui.L_PluginName->text().toUpper();
	_zTpl[ "DATE" ] = QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm");
	_zTpl[ "AUTHOR" ] = ui.LE_Author->text();
	_zTpl[ "NAMESPACE" ] = ns1.toLower()+"/"+ns2.toLower()+"/"+ns3.toLower();
	_zTpl[ "NAMESPACE_1" ] = ns1.toLower();
	_zTpl[ "NAMESPACE_2" ] = ns2.toLower();
	_zTpl[ "NAMESPACE_3" ] = ns3.toLower();
	_zTpl[ "NAMESPACE_UNDERSCORE_UPPER" ] = ns1.toUpper()+"_"+ns2.toUpper()+"_"+ns3.toUpper();

	//GenerateComponent clear the ZPL var
	if(generatePluginProTpl() && generateGenerateVcprojBatTpl() && generateRessources() && generatePluginPriTpl() && generateSources() && generateComponentSourcesBaseModelConsumer())
		return true;

	return false;
}


//-------------------------------------------------------------------------
bool SWGenerator::generateComponent()
{
	ui.TE_log_2->clear();
	_zTpl.clear();

	if(ui.lw_listplugin->selectedItems().count() > 1 || ui.lw_listplugin->selectedItems().isEmpty())
	{
		ui.TE_log_2->append(ERROR_LOG+"Multiple selection on the list plugin.");
		return false;
	}
	QString namePluginSelected = ui.lw_listplugin->selectedItems().at(0)->text();

	_outputPath = _pathPlugin[namePluginSelected]+"\\";
	_outputPath = _outputPath.replace("/","\\").replace("\\\\","\\");

	ui.TE_log->append("<br/>"+headerLog("Manage Files"));

	QStringList allNamespace;

	QList<int> allIntPos;
	//Try to get namespace from the pluginName
	int index =0;
	while((index = namePluginSelected.indexOf(QRegExp("[A-Z]"),index	)) != -1)
	{
		allIntPos << index;
		index++;
	}
	index = 0;
	allIntPos.removeOne(0);
	foreach(int pos, allIntPos)
	{
		allNamespace << namePluginSelected.mid(index,pos-index);
		index = pos;
	}
	allNamespace << namePluginSelected.mid(index,namePluginSelected.length() - index);
	//QStringList allNamespace = namePluginSelected.split();

	if(allNamespace.count() != 3)
	{
		ui.TE_log_2->append(ERROR_LOG+"Unable to find all namespace on the plugin.");
		return false;
	}

	QString ns1 = allNamespace.at(0);
	QString ns2 = allNamespace.at(1);
	QString ns3 = allNamespace.at(2);

	//Variables TPL globale
	_zTpl[ "PLUGIN_NAME" ] = namePluginSelected;
	_zTpl[ "PLUGIN_NAME_UPPER" ] = namePluginSelected.toUpper();
	_zTpl[ "DATE" ] = QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm");
	_zTpl[ "AUTHOR" ] = ui.LE_Author->text();
	_zTpl[ "NAMESPACE" ] = ns1.toLower()+"/"+ns2.toLower()+"/"+ns3.toLower();
	_zTpl[ "NAMESPACE_1" ] = ns1.toLower();
	_zTpl[ "NAMESPACE_2" ] = ns2.toLower();
	_zTpl[ "NAMESPACE_3" ] = ns3.toLower();
	_zTpl[ "NAMESPACE_UNDERSCORE_UPPER" ] = ns1.toUpper()+"_"+ns2.toUpper()+"_"+ns3.toUpper();

	//GenerateComponent clear the ZPL var
	if( generateComponentSourcesBaseModelConsumerOnly()  )
	{
		appendPluginPriTpl();
		appendRessources();
		appendSources();
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------
void SWGenerator::readXml()
{
	QDomDocument doc;

	QFile f(":/SWGenerator/Resources/libs.xml");
	f.open(QFile::ReadOnly);
	doc.setContent(&f);
	f.close();

	//********************************
	// Parse the DOM tree
	//********************************
	QDomElement root=doc.documentElement();

	// We traverse its children
	QDomElement child=root.firstChildElement("lib");
	while(!child.isNull())
	{
		if (child.tagName() == "lib")
		{
			// We traverse children
			QDomElement grandChild=child.firstChild().toElement();

			QString name="";
			QString pathLib="";
			QString pathInclude="";
			while(!grandChild.isNull())
			{
				if (grandChild.tagName() == "name")
				{
					name = grandChild.text();
				}
				if (grandChild.tagName() == "path")
				{
					pathLib = grandChild.text();
				}
				if (grandChild.tagName() == "pathInclude")
				{
					pathInclude = grandChild.text();
				}
				grandChild = grandChild.nextSibling().toElement();
			}

			if(!name.isEmpty() && !pathLib.isEmpty() && !pathInclude.isEmpty())
			{
				XmlPath tmp;
				tmp.pathInclude = pathInclude;
				tmp.pathLib = pathLib;
				_libMap.insert(name,tmp);
			}
		}
		child = child.nextSiblingElement("lib");
	}
}

//-------------------------------------------------------------------------
void SWGenerator::fillListLibWidget()
{
	QMap<QString,XmlPath>::iterator it = _libMap.begin();
	QMap<QString,XmlPath>::iterator itEnd = _libMap.end();

	for(it; it != itEnd ; it++)
	{
		QListWidgetItem * tmpItem = new QListWidgetItem(it.key());
		tmpItem->setCheckState(Qt::Unchecked);
		ui.LW_libDependency->addItem(tmpItem );
	}

}

//-------------------------------------------------------------------------
bool SWGenerator::generatePluginProTpl()
{

	ui.TE_log->append(subHeaderLog("File <b>plugin.pro</b>"));

	QString tmp="";

	//on parcours les elements checked et si on a pas déja écris on écris la valeur
	QStringList addionntalIncludes;
	QStringList addionntalLibs;
	QStringList addionntalLibsD;
	QStringList addionntalLibsPath;
	QStringList addionntalLibsDPath;

	foreach(Component* cp, _listComponent)
	{
		bool inPin = cp->ui.Cb_pinIn->isChecked();
		bool outPin = cp->ui.Cb_pinOut->isChecked();

		if(cp->ui.Gb_pins->isChecked() && (inPin || outPin) ) 
		{
			QList<QListWidgetItem*> list = ui.LW_libDependency->findItems("tdlcommon",Qt::MatchExactly);
			if(!list.isEmpty() && list.count() == 1)
			{
				list.at(0)->setCheckState(Qt::Checked);
			}
		}

		if(cp->ui.Cb_baseClass->currentText()==QString("Composant assisté"))
		{
			QList<QListWidgetItem*> list = ui.LW_libDependency->findItems("SwFoundation",Qt::MatchExactly);
			if(!list.isEmpty() && list.count() == 1)
			{
				list.at(0)->setCheckState(Qt::Checked);
			}
		}

		if(cp->ui.Cb_baseClass->currentText()==QString("Composant assisté + ISxModel"))
		{
			QList<QListWidgetItem*> list = ui.LW_libDependency->findItems("SxBasicsEntity",Qt::MatchExactly);
			if(!list.isEmpty() && list.count() == 1)
			{
				list.at(0)->setCheckState(Qt::Checked);
			}
		}
	}




	for(int i = 0; i< ui.LW_libDependency->count() ; i++)
	{
		if(ui.LW_libDependency->item(i)->checkState() == Qt::Checked)
		{
			QString libName = ui.LW_libDependency->item(i)->text();
			XmlPath xml = _libMap.value(libName);
			if(!addionntalIncludes.contains(xml.pathInclude))
			{
				_zTpl.enterSection( "FOR_ADDITIONNAL_INCLUDES" );        
				_zTpl[ "CONTENT" ] = xml.pathInclude+" \\\n\t";          
				_zTpl.exitSection();
				addionntalIncludes.append(xml.pathInclude);
			}
			if(!addionntalLibs.contains(libName))
			{
				_zTpl.enterSection( "FOR_ADDITIONNAL_LIBS" );        
				_zTpl[ "CONTENT" ] = "-l"+libName+" \\\n\t";          
				_zTpl.exitSection();

				_zTpl.enterSection( "FOR_ADDITIONNAL_LIBSD" );        
				_zTpl[ "CONTENT" ] = "-l"+libName+"d \\\n\t";          
				_zTpl.exitSection();
				addionntalLibs.append(libName);
			}

			if(!addionntalLibsPath.contains(xml.pathLib))
			{
				_zTpl.enterSection( "FOR_ADDITIONNAL_LIBS_PATH" );        
				tmp = xml.pathLib;
				tmp = tmp.replace("{{CONF}}","release");
				_zTpl[ "CONTENT" ] = "-L\""+tmp+"\" \\\n\t";          
				_zTpl.exitSection();

				_zTpl.enterSection( "FOR_ADDITIONNAL_LIBSD_PATH" );    
				tmp = xml.pathLib;
				tmp = tmp.replace("{{CONF}}","debug");
				_zTpl[ "CONTENT" ] = "-L\""+tmp+"\" \\\n\t";       
				_zTpl.exitSection();

				addionntalLibsPath.append(xml.pathLib);
			}
		}
	}

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+"plugin.pro"))
	{
		if(!QFile::remove(_outputPath+"plugin.pro"))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+"plugin.pro"+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+"plugin.pro"+"</b>");
		}
	}

	QFile pluginPro(_outputPath+"plugin.pro");

	if(pluginPro.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+"plugin.pro"+"</b>");
		QTextStream out(&pluginPro);
		out << _zTpl.expandFile(":/SWGenerator/Resources/plugin.tpl");
		pluginPro.close();
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------
bool SWGenerator::generateGenerateVcprojBatTpl()
{
	ui.TE_log->append(subHeaderLog("File <b>generatevcproj.bat</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+"generatevcproj.bat"))
	{
		if(!QFile::remove(_outputPath+"generatevcproj.bat"))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+"generatevcproj.bat"+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+"generatevcproj.bat"+"</b>");
		}
	}

	QFile generateFile(_outputPath+"generatevcproj.bat");

	if(generateFile.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+"generatevcproj.bat"+"</b>");
		QTextStream out(&generateFile);
		out << _zTpl.expandFile(":/SWGenerator/Resources/generatevcproj.tpl");
		generateFile.close();

		return true;
	}

	return false;
}

//-------------------------------------------------------------------------
bool SWGenerator::generateRessources()
{

	QString ressourceDir = _outputPath+"resources\\";

	//DefaultContent.txt
	QString defaultContentFileName = "DefaultContent.txt";
	ui.TE_log->append(subHeaderLog("File <b>"+defaultContentFileName+"</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(ressourceDir+defaultContentFileName))
	{
		if(!QFile::remove(ressourceDir+defaultContentFileName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+ressourceDir+defaultContentFileName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+ressourceDir+defaultContentFileName+"</b>");
		}
	}

	//Creating DefaultContent.txt with empty content
	QFile defaultContent(ressourceDir+defaultContentFileName);
	if(defaultContent.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+ressourceDir+defaultContentFileName+"</b>");

		QTextStream out(&defaultContent);
		out << "empty";
		defaultContent.close();
	}
	else
	{
		ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+ressourceDir+defaultContentFileName+"</b>");
		return false;
	}

	//ui.L_PluginName->text()Rsc.qrc
	QString pluginName = ui.L_PluginName->text()+"Rsc.qrc";
	ui.TE_log->append(subHeaderLog("File <b>"+pluginName+"</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(ressourceDir+pluginName))
	{
		if(!QFile::remove(ressourceDir+pluginName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+ressourceDir+pluginName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+ressourceDir+pluginName+"</b>");
		}
	}

	foreach(Component* cp, _listComponent)
	{
		_zTpl.enterSection( "FOR_FILE_RESSOURCES" );    
		QFileInfo file(cp->getIconPath());
		if(file.exists())
		{
			_zTpl[ "CONTENT" ] = "<file>"+file.fileName()+"</file>\n\t\t";          
		}
		_zTpl.exitSection();
	}


	//Creating pluginName with empty content
	QFile qrcFile(ressourceDir+pluginName);
	if(qrcFile.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+ressourceDir+pluginName+"</b>");

		QTextStream out(&qrcFile);
		out << _zTpl.expandFile(":/SWGenerator/Resources/rsc.tpl");
		qrcFile.close();
	}
	else
	{
		ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+ressourceDir+pluginName+"</b>");
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------
QString SWGenerator::headerLog( QString val )
{
	QString out = "<font color=\"#3A82FF\">//";
	int lenght = val.count();
	for(int i = 0; i< lenght*2 ; i++)
		out += "-";
	out += "<br>";
	out += "// "+val+"<br>//";
	for(int i = 0; i< lenght*2 ; i++)
		out += "-";
	return out;
}

//-------------------------------------------------------------------------
QString SWGenerator::subHeaderLog( QString val )
{
	QString out = "<br><font color=\"#3ABAFF\">";
	out += "// "+val;
	return out;
}

//-------------------------------------------------------------------------
bool SWGenerator::generateSources()
{

	//PluginFactory_Class.cpp
	QString pluginCPPFileName = ui.L_PluginName->text()+"PluginFactory_Class.cpp";
	ui.TE_log->append(subHeaderLog("File <b>"+pluginCPPFileName+"</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+pluginCPPFileName))
	{
		if(!QFile::remove(_outputPath+pluginCPPFileName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+pluginCPPFileName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+pluginCPPFileName+"</b>");
		}
	}

	//Creating DefaultContent.txt with empty content
	QFile pluginCPP(_outputPath+pluginCPPFileName);
	if(pluginCPP.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+pluginCPPFileName+"</b>");

		QTextStream out(&pluginCPP);

		QString ns1 = ui.LE_namespace1->text().toLower();
		QString ns2 = ui.LE_namespace2->text().toLower();
		QString ns3 = ui.LE_namespace3->text().toLower();

		foreach(Component* cp, _listComponent)
		{
			if(cp->ui.Le_name->text() == "")
			{
				ui.TE_log->append(TAB_LOG+TAB_LOG+ERROR_LOG+"Component [<b>"+QString::number(_listComponent.indexOf(cp))+"</b>] has no name");
				return false;
			}
			if(cp->ui.Le_desc->text() == "")
				ui.TE_log->append(TAB_LOG+TAB_LOG+WARNING_LOG+"Component [<b>"+QString::number(_listComponent.indexOf(cp))+"</b>] has no description");


			QString componentName = cp->ui.Le_name->text();
			QString dirComponent = componentName+"/";
			QString outputFileNameCPP = componentName+".cpp";
			QString outputFileNameH = componentName+".h";

			_zTpl.enterSection( "FOR_REGISTER_COMPONENT" );    
			_zTpl[ "CONTENT" ] = "RegisterComponent(\""+cp->ui.Le_name->text()+"\",\""+cp->ui.Le_desc->text()+"\"); \n\t";          
			_zTpl.exitSection();


			_zTpl.enterSection( "FOR_CREATEINSTANCEOF" );    
			_zTpl[ "CONTENT" ] = "if (name==\""+cp->ui.Le_name->text()+"\")\n\t{\n\t\treturn new "+ns1+"::"+ns2+"::"+ns3+"::"+cp->ui.Le_name->text()+";\n\t}\n\t";          
			_zTpl.exitSection();

			_zTpl.enterSection( "SECTION_INCLUDE_CLASS" );    
			_zTpl[ "INCLUDE_FILE" ] = dirComponent+outputFileNameH;          
			_zTpl.exitSection();

			_zTpl.enterSection( "FOR_CREATEICONOF" );    
			QFileInfo file(cp->getIconPath());
			if(file.exists())
			{
				//Copier le fichier
				_zTpl[ "CONTENT" ] = "if (name==\""+cp->ui.Le_name->text()+"\")\n\t{\n\t\treturn QIcon(\":/"+ns1+"/"+ns2+"/"+ns3+"/"+file.fileName()+"\");\n\t}\n\t";          

				if(QFile::exists(_outputPath+"resources/"+file.fileName()))
				{
					QFile::remove(_outputPath+"resources/"+file.fileName());
				}

				if(QFile::copy(cp->getIconPath(),_outputPath+"resources/"+file.fileName()))
				{
					ui.TE_log->append(TAB_LOG+TAB_LOG+INFO_LOG+"Copy icon <b>"+file.fileName()+"</b>.");
				}
				else
				{
					ui.TE_log->append(TAB_LOG+TAB_LOG+WARNING_LOG+"Unable to copy the icon of the component \
																  [<b>"+QString::number(_listComponent.indexOf(cp))+"</b>] : <b>"+file.fileName()+"</b> from\
																  <a href=\""+file.absolutePath()+"\">InputDir</a> to <a href=\""+_outputPath+"resources\\\">OutDir</a>.");
				}
			}
			else
			{
				_zTpl[ "CONTENT" ] = "if (name==\""+cp->ui.Le_name->text()+"\")\n\t{\n\t\treturn ico;\n\t}\n\t";          
			}
			_zTpl.exitSection();

		}

		out << _zTpl.expandFile(":/SWGenerator/Resources/PluginFactory_ClassCPP.tpl");
		pluginCPP.close();
	}
	else
	{
		ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+_outputPath+pluginCPPFileName+"</b>");
		return false;
	}


	//PluginFactory_Class.h
	QString pluginHFileName = ui.L_PluginName->text()+"PluginFactory_Class.h";
	ui.TE_log->append(subHeaderLog("File <b>"+pluginHFileName+"</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+pluginHFileName))
	{
		if(!QFile::remove(_outputPath+pluginHFileName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+pluginHFileName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+pluginHFileName+"</b>");
		}
	}

	//Creating DefaultContent.txt with empty content
	QFile pluginH(_outputPath+pluginHFileName);
	if(pluginH.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+pluginHFileName+"</b>");

		QTextStream out(&pluginH);
		out << _zTpl.expandFile(":/SWGenerator/Resources/PluginFactory_ClassH.tpl");
		pluginCPP.close();
	}
	else
	{
		ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+_outputPath+pluginHFileName+"</b>");
		return false;
	}


	//Constantes.h
	QString ContantesHFileName = ui.L_PluginName->text()+"Constantes.h";
	ui.TE_log->append(subHeaderLog("File <b>"+ContantesHFileName+"</b>"));

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+ContantesHFileName))
	{
		if(!QFile::remove(_outputPath+ContantesHFileName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+ContantesHFileName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+ContantesHFileName+"</b>");
		}
	}

	//Creating DefaultContent.txt with empty content
	QFile constantesH(_outputPath+ContantesHFileName);
	if(constantesH.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+ContantesHFileName+"</b>");

		QTextStream out(&constantesH);
		out << _zTpl.expandFile(":/SWGenerator/Resources/Constantes.tpl");
		constantesH.close();
	}
	else
	{
		ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+_outputPath+ContantesHFileName+"</b>");
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------
void SWGenerator::nbComponentChanged( int nbComponent )
{
	if(nbComponent == 0)
	{
		ui.PB_Next->setDisabled(true);
		return;
	}

	ui.PB_Next->setDisabled(false);

	if(nbComponent > _nbComponents)
	{
		for (int i = 0; i < nbComponent - _nbComponents ; i++)
		{
			Component * test = new Component();
			_listComponent.append(test);
			ui.SW_Plugin->insertWidget(ui.SW_Plugin->count()-1,test);
		}
	}
	else
	{
		for (int i = _nbComponents-1 ; i > nbComponent-1; i--)
		{
			QWidget *w = ui.SW_Plugin->widget(i+1);
			if(w)
			{
				ui.SW_Plugin->removeWidget(w);
				_listComponent.removeOne((Component*)(w));
				delete w;
			}
		}
	}
	_nbComponents = nbComponent;
}

//-------------------------------------------------------------------------
void SWGenerator::doWork()
{
	generatePlugin();

	ui.TE_log->append("<br/>"+INFO_LOG + "<a href=\"/select,"+_outputPath+"generatevcproj.bat\"> QMAKE</a>");

}

//-------------------------------------------------------------------------
void SWGenerator::doWork_2()
{
	generateComponent();

	ui.TE_log_2->append("<br/>"+INFO_LOG + "<a href=\"/select,"+_outputPath+"generatevcproj.bat\"> QMAKE</a>");

}


//-------------------------------------------------------------------------
void SWGenerator::openExplorer( const QUrl& val)
{
	QProcess::execute("explorer.exe "+val.path().replace("/","\\").replace("\\select","/select"));
}


void SWGenerator::testAA()
{

	QString toto  = "{{#SECTION1}}moncontenu{{/SECTION1}} mon test2{{#SECTION1}}bis{{/SECTION1}}";
	//this->_zTpl.showSection("SECTION1");

	//qDebugAA
	qDebug() << this->_zTpl.expandString(toto);
}

//-------------------------------------------------------------------------
bool SWGenerator::generateComponentSourcesBaseModelConsumer()
{
	ui.TE_log->append("<br/>"+headerLog("Manage component sources"));

	QString dirComponent = "";
	QString componentName = "";
	QString outputFileNameCPP = "";
	QString outputFileNameH = "";

	foreach(Component * cp , _listComponent)
	{
		
		//temp var
		componentName = cp->ui.Le_name->text();
		dirComponent = _outputPath+componentName+"\\";
		outputFileNameCPP = componentName+".cpp";
		outputFileNameH = componentName+".h";

		QString ns1 = ui.LE_namespace1->text().toLower();
		QString ns2 = ui.LE_namespace2->text().toLower();
		QString ns3 = ui.LE_namespace3->text().toLower();

		_zTpl.clear();
		_zTpl[ "DATE" ] = QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm");
		_zTpl[ "AUTHOR" ] = ui.LE_Author->text();
		_zTpl[ "NAMESPACE_1" ] = ns1;
		_zTpl[ "NAMESPACE_2" ] = ns2;
		_zTpl[ "NAMESPACE_3" ] = ns3;


		if(!generateCompontentCommon(cp,componentName,dirComponent,outputFileNameCPP,outputFileNameH,ui.TE_log))
			return false;
	}

	return true;

}

//-------------------------------------------------------------------------
QString SWGenerator::interfaceToVarName( QString name )
{
	QString returnString = "_";

	name = name.toLower();
	QStringList list = name.split(" ");

	int index = 0;
	foreach(QString st, list)
	{
		if(index == 0)
			returnString+=st;
		else
			returnString+=st.mid(0,1).toUpper()+st.mid(1,st.length()-1).toLower();
		index++;
	}
	return returnString;
	
}

//-------------------------------------------------------------------------
bool SWGenerator::generatePluginPriTpl()
{
	ui.TE_log->append(subHeaderLog("File <b>plugin_files.pri</b>"));

	QString tmp="";

	//on parcours les elements checked et si on a pas déja écris on écris la valeur
	QStringList headerInclude;
	QStringList sourceInclude;

	QString pluginCPPFileName = ui.L_PluginName->text()+"PluginFactory_Class.cpp";
	QString pluginHFileName = ui.L_PluginName->text()+"PluginFactory_Class.h";
	QString ContantesHFileName = ui.L_PluginName->text()+"Constantes.h";

	headerInclude.append(pluginHFileName);
	headerInclude.append(ContantesHFileName);
	sourceInclude.append(pluginCPPFileName);

	foreach(Component* cp, _listComponent)
	{
		QString componentName = cp->ui.Le_name->text();
		QString dirComponent = componentName+"/";
		QString outputFileNameCPP = componentName+".cpp";
		QString outputFileNameH = componentName+".h";

		if(cp->ui.Le_name->text() == "")
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Component [<b>"+QString::number(_listComponent.indexOf(cp))+"</b>] has no name");
			return false;
		}

		headerInclude.append(dirComponent+outputFileNameH);
		sourceInclude.append(dirComponent+outputFileNameCPP);
	}
	
	QString fileName = "plugin_files.pri";

	//On efface le fichier si il existe
	if(QFile::exists(_outputPath+fileName))
	{
		if(!QFile::remove(_outputPath+fileName))
		{
			ui.TE_log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+_outputPath+fileName+"</b>");
			return false;
		}
		else
		{
			ui.TE_log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+_outputPath+fileName+"</b>");
		}
	}

	QFile file(_outputPath+fileName);

	if(file.open(QFile::ReadWrite))
	{
		ui.TE_log->append(TAB_LOG+INFO_LOG+"Write file <b>"+_outputPath+fileName+"</b>");

		foreach(QString val, headerInclude)
		{
			_zTpl.enterSection( "SECTION_HEADER" );    
			_zTpl[ "CONTENT" ] = val;       
			_zTpl.exitSection();	
		}

		foreach(QString val, sourceInclude)
		{
			_zTpl.enterSection( "SECTION_SOURCE" );    
			_zTpl[ "CONTENT" ] = val;       
			_zTpl.exitSection();	
		}

		QTextStream out(&file);
		out << _zTpl.expandFile(":/SWGenerator/Resources/pluginPri.tpl");
		file.close();
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------
void SWGenerator::switchMainSW()
{
	QPushButton * item = qobject_cast<QPushButton *> (sender());

	if(item == ui.pb_plugin)
		ui.stackedWidget->setCurrentIndex(1);

	if(item == ui.pb_component)
	{
		if(_pathPlugin.isEmpty())
		{
			listAllPlugin();
		}

		ui.stackedWidget->setCurrentIndex(2);
	}

}

//-------------------------------------------------------------------------
void SWGenerator::listAllPlugin()
{
	_pathPlugin.clear();
	_progDial->show();
	QStringList test2 = findFiles ("c:\\projects\\",QStringList("plugin_files.pri"));

	foreach(QString plugin, test2)
	{
		QFileInfo fi(plugin);
		QString pluginName = "";
		QString path = fi.absolutePath().replace("\\","/").replace("//","/");
		QStringList list = path.split("/");
		for (int i = list.count()-3; i <=list.count()-1  ; i++)
		{
				pluginName+=list.at(i).mid(0,1).toUpper()+list.at(i).mid(1,list.at(i).length()-1).toLower();
		}
		if(!_pathPlugin.contains(pluginName))
		{
			_pathPlugin.insert(pluginName,path);
		}
	}

	fillPluginList();
	_progDial->reset();
}


//-------------------------------------------------------------------------
QStringList SWGenerator::findFiles(const QString &startDir, QStringList filters)
{
	QStringList names;
	QDir dir(startDir);

	if(_progDial->wasCanceled())
		return names;

	qApp->processEvents();
	
	foreach (QString file, dir.entryList(filters, QDir::Files))
		names += startDir + "/" + file;

	foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
	{
		//Display Max 3 niveau
		QString maxDir = QString(startDir + "/" + subdir).replace("\\","/").replace("//","/");
		QStringList list = maxDir.split("/");
		maxDir = "";
		for (int i = 0; i < 4 ; i++)
		{
			if(list.count()-1 >= i)
				maxDir+=list.at(i)+"\\";
		}
		_progDial->setLabelText("Looking dir : "+maxDir);
		names += findFiles(startDir + "/" + subdir, filters);
	}
	return names;
}


//-------------------------------------------------------------------------
void SWGenerator::fillPluginList()
{
	ui.lw_listplugin->clear();
	QMapIterator<QString,QString> it(_pathPlugin);

	while (it.hasNext()) 
	{
		it.next();
		ui.lw_listplugin->addItem(it.key());
	}

}

//-------------------------------------------------------------------------
bool SWGenerator::generateComponentSourcesBaseModelConsumerOnly()
{
	ui.TE_log_2->append("<br/>"+headerLog("Manage component sources"));

	QString dirComponent = "";
	QString componentName = "";
	QString outputFileNameCPP = "";
	QString outputFileNameH = "";
	//QFile currentFileCPP;
	//QFile currentFileH;

	Component *cp = qobject_cast<Component*>(ui.v_cp->itemAt(0)->widget());
	
	//temp var
	componentName = cp->ui.Le_name->text();
	dirComponent = _outputPath+componentName+"\\";
	outputFileNameCPP = componentName+".cpp";
	outputFileNameH = componentName+".h";

	if(!generateCompontentCommon(cp,componentName,dirComponent,outputFileNameCPP,outputFileNameH,ui.TE_log_2))
		return false;

	return true;
}

//-------------------------------------------------------------------------
void SWGenerator::appendRessources()
{
	Component *cp = qobject_cast<Component*>(ui.v_cp->itemAt(0)->widget());

	QString componentName = cp->ui.Le_name->text();
	QString componentDesc = cp->ui.Le_desc->text();
	QString dirComponent =  componentName+"\\";
	QString outputFileNameCPP = componentName+".cpp";
	QString outputFileNameH = componentName+".h";
	QString pluginName = _zTpl[ "PLUGIN_NAME" ];

	QString fileName = pluginName+"Rsc.qrc";


	QFile file(_outputPath+"resources/"+fileName);

	if(file.open(QFile::ReadWrite))
	{
		ui.TE_log_2->append(TAB_LOG+INFO_LOG+"open file <b>"+_outputPath+"resources/"+fileName+"</b>");
		QTextStream out(&file);

		bool includeDone =false;
		QString outF;
		QString line;
		do 
		{
			line = out.readLine();
			outF.append(line+"\n");
			if(line.contains(_zTpl[ "NAMESPACE_1" ]+"/"+_zTpl[ "NAMESPACE_2" ]+"/"+_zTpl[ "NAMESPACE_3"],Qt::CaseSensitive) && !includeDone)
			{
				QFileInfo fileIcon(cp->getIconPath());
				if(fileIcon.exists())
					outF.append("\t\t<file>"+fileIcon.fileName()+"</file>\n");
				includeDone = true;
			}

			if(line.contains("</RCC>",Qt::CaseSensitive) && !includeDone)
			{
				QFileInfo fileIcon(cp->getIconPath());
				if(fileIcon.exists())
				{
					outF.remove("</RCC>");
					outF.append("\t<qresource prefix=\""+_zTpl[ "NAMESPACE_1" ]+"/"+_zTpl[ "NAMESPACE_2" ]+"/"+_zTpl[ "NAMESPACE_3"]+"\">\n");
					outF.append("\t\t<file>"+fileIcon.fileName()+"</file>\n");
					outF.append("\t</qresource>\n");
					outF.append("</RCC>\n");
				}
				includeDone = true;
			}

		} while (!line.isNull());


		out.seek(0);
		out << outF ;
		file.close();
	}
	else
	{
		ui.TE_log_2->append(TAB_LOG+ERROR_LOG+"Unable to open file <b>"+_outputPath+fileName+"</b>");

	}
}

//-------------------------------------------------------------------------
void SWGenerator::appendPluginPriTpl()
{
	QString dirComponent = "";
	QString componentName = "";
	QString outputFileNameCPP = "";
	QString outputFileNameH = "";

	//Try to append to plugi
	QString fileName = "plugin_files.pri";

	Component *cp = qobject_cast<Component*>(ui.v_cp->itemAt(0)->widget());
	//temp var
	componentName = cp->ui.Le_name->text();
	dirComponent = componentName+"/";
	outputFileNameCPP = componentName+".cpp";
	outputFileNameH = componentName+".h";

	QFile file(_outputPath+fileName);

	if(file.open(QFile::ReadWrite))
	{
		ui.TE_log_2->append(TAB_LOG+INFO_LOG+"open file <b>"+_outputPath+fileName+"</b>");
		QTextStream out(&file);

		QStringList lineList;
		QString outF;
		QString line;
		do 
		{
			line = out.readLine();
			outF.append(line+"\n");
			if(line.contains("HEADERS",Qt::CaseSensitive))
				outF.append("\t"+dirComponent+outputFileNameH + " \\\n");
			if(line.contains("SOURCES",Qt::CaseSensitive))
				outF.append("\t"+dirComponent+outputFileNameCPP + " \\\n");
		} while (!line.isNull());

		out.seek(0);
		out << outF ;
		file.close();
	}
	else
	{
		ui.TE_log_2->append(TAB_LOG+ERROR_LOG+"Unable to open file <b>"+_outputPath+fileName+"</b>");

	}
}

//-------------------------------------------------------------------------
void SWGenerator::appendSources()
{
	Component *cp = qobject_cast<Component*>(ui.v_cp->itemAt(0)->widget());

	QString componentName = cp->ui.Le_name->text();
	QString componentDesc = cp->ui.Le_desc->text();
	QString dirComponent =  componentName+"\\";
	QString outputFileNameCPP = componentName+".cpp";
	QString outputFileNameH = componentName+".h";
	QString pluginName = _zTpl[ "PLUGIN_NAME" ];

	QString fileName = pluginName+"PluginFactory_Class.cpp";


	QFile file(_outputPath+fileName);

	if(file.open(QFile::ReadWrite))
	{
		ui.TE_log_2->append(TAB_LOG+INFO_LOG+"open file <b>"+_outputPath+fileName+"</b>");
		QTextStream out(&file);

		bool includeDone =false;
		bool createInstanceOfDone =false;
		bool createIconOfDone =false;
		bool registerDone =false;
		QString outF;
		QString line;
		do 
		{
			line = out.readLine();
			outF.append(line+"\n");
			if(line.contains("#include",Qt::CaseSensitive) && !includeDone)
			{
				outF.append("#include \""+dirComponent+outputFileNameH + "\"\n");
				includeDone = true;
			}

			if(line.contains("void",Qt::CaseSensitive) && line.contains("Initialize",Qt::CaseSensitive) && !registerDone)
			{
				while(!line.contains("{"))
				{
					line = out.readLine();
					outF.append(line);
				}
				outF.append("\n\tRegisterComponent(\""+componentName+"\",\""+componentDesc+"\");\n");
				registerDone = true;
			}

			if(line.contains("QIcon ico",Qt::CaseSensitive)&& !createIconOfDone)
			{
				QFileInfo fileIcon(cp->getIconPath());
				if(fileIcon.exists())
				{
					outF.append("\n\tif (name==\""+componentName+"\")\n\t{\n\t\treturn QIcon(\":/"+_zTpl[ "NAMESPACE_1" ]+"/"+_zTpl[ "NAMESPACE_2" ]+"/"+_zTpl[ "NAMESPACE_3" ]+"/"+fileIcon.fileName()+"\");\n\t}\n\t");          

					if(QFile::copy(cp->getIconPath(),_outputPath+"resources/"+fileIcon.fileName()))
					{
						ui.TE_log_2->append(TAB_LOG+TAB_LOG+INFO_LOG+"Copy icon <b>"+fileIcon.fileName()+"</b>.");
					}
				}
				else
				{
					outF.append("\n\tif (name==\""+componentName+"\")\n\t{\n\t\treturn ico;\n\t}\n\t");
				}
				createIconOfDone = true;
			}



			if(line.contains("SwComponent_Class",Qt::CaseSensitive) && line.contains("CreateInstanceOf",Qt::CaseSensitive) && !createInstanceOfDone)
			{
				while(!line.contains("{"))
				{
					line = out.readLine();
					outF.append(line);
				}
				outF.append("\n\tif (name==\""+componentName+"\")\n");
				outF.append("\t{\n\t\treturn new "+_zTpl[ "NAMESPACE_1" ]+"::"+_zTpl[ "NAMESPACE_2" ]+"::"+_zTpl[ "NAMESPACE_3" ]+"::"+componentName+";\n\t}\n");
				
				createInstanceOfDone = true;
			}

		} while (!line.isNull());

		out.seek(0);
		out << outF ;
		file.close();
	}
	else
	{
		ui.TE_log_2->append(TAB_LOG+ERROR_LOG+"Unable to open file <b>"+_outputPath+fileName+"</b>");

	}
}


//-------------------------------------------------------------------------
bool SWGenerator::generateCompontentCommon(Component *cp,QString componentName, QString dirComponent,QString outputFileNameCPP ,QString outputFileNameH ,QTextBrowser * log)
{
	QStringList _includeList;
	QFile currentFileCPP;
	QFile currentFileH;

	log->append(subHeaderLog("Component <b>"+componentName+"</b>"));

	//Create a directory for each component
	if(!QDir::temp().exists(dirComponent))
	{
		QDir::temp().mkpath(dirComponent);
		log->append(TAB_LOG+INFO_LOG+"Creating directory <b>"+dirComponent+"</b>");
	}
	else
	{
		log->append(TAB_LOG+INFO_LOG+"Directory <b>"+dirComponent+"</b> already exist.");
	}

	//On efface le fichier si il existe
	if(QFile::exists(dirComponent+outputFileNameCPP))
	{
		if(!QFile::remove(dirComponent+outputFileNameCPP))
		{
			log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+dirComponent+outputFileNameCPP+"</b>");
			return false;
		}
		else
		{
			log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+dirComponent+outputFileNameCPP+"</b>");
		}
	}

	//On efface le fichier si il existe
	if(QFile::exists(dirComponent+outputFileNameH))
	{
		if(!QFile::remove(dirComponent+outputFileNameH))
		{
			log->append(TAB_LOG+ERROR_LOG+"Unable to remove file <b>"+dirComponent+outputFileNameH+"</b>");
			return false;
		}
		else
		{
			log->append(TAB_LOG+INFO_LOG+"Removing file <b>"+dirComponent+outputFileNameH+"</b>");
		}
	}

	currentFileCPP.setFileName(dirComponent+outputFileNameCPP);
	currentFileH.setFileName(dirComponent+outputFileNameH);
	if(currentFileCPP.open(QFile::ReadWrite) && currentFileH.open(QFile::ReadWrite))
	{
		log->append(TAB_LOG+INFO_LOG+"Write class <a href=\""+dirComponent+"\"><b>"+dirComponent+componentName+"</b></a>");

		QTextStream outCPP(&currentFileCPP);
		QTextStream outH(&currentFileH);

		if(cp->ui.Le_name->text() == "")
		{
			log->append(TAB_LOG+TAB_LOG+ERROR_LOG+"Component has no name");
			return false;
		}

		if(cp->ui.Le_desc->text() == "")
			log->append(TAB_LOG+TAB_LOG+WARNING_LOG+"Component has no description");


		_zTpl[ "COMPONENT_NAME" ] = componentName;          

		bool inPin = cp->ui.Cb_pinIn->isChecked();
		bool outPin = cp->ui.Cb_pinOut->isChecked();

		if ( cp->ui.Gb_pins->isChecked() && (inPin || outPin) )
		{
			_zTpl.showSection("SECTION_HAS_PIN"); 

			if(inPin)
				_zTpl.showSection("SECTION_ISWPIN_LISTENER");

			if(outPin)
				_zTpl.showSection("SECTION_ISWPIN_OUTPUT");
		}

		if ( cp->ui.Gb_shortcuts->isChecked() )
		{
			_zTpl.showSection("SECTION_ISWSHORTCUT"); 
			_zTpl["SHORTCUT_COMPONENT_NAME"] = (cp->ui.Le_shortcutComponentName->text() != "" )?cp->ui.Le_shortcutComponentName->text() :componentName;

			for (int i = 0; i < cp->ui.Sb_shortcuts->value() ; i++)
			{
				QLayoutItem * item = cp->ui.v_shortcuts->itemAt(i);
				if(item)
				{
					QWidget * w = item->widget();
					if(w)
					{
						Shortcut * sc = dynamic_cast<Shortcut*>(w);
						if(sc)
						{
							_zTpl.enterSection( "SECTION_ADD_SHORTCUT" );    
							_zTpl[ "SHORTCUT_NAME" ] = sc->ui.Le_displayName->text();          
							_zTpl.exitSection();	
						}
					}
				}
			}
		}

		if ( cp->ui.Gb_IConsummed->isChecked() || cp->ui.Gb_IProvided->isChecked())
		{
			_zTpl.showSection("SECTION_HAS_INTERFACES");
		}

		if ( cp->ui.Gb_IConsummed->isChecked() )
		{
			_zTpl.showSection("SECTION_ISCONSUMED"); 

			for (int i = 0; i < cp->ui.Sb_cIntefaces->value() ; i++)
			{
				QLayoutItem * item = cp->ui.v_cInterfaces->itemAt(i);
				if(item)
				{
					QWidget * w = item->widget();
					if(w)
					{
						Interface * sc = dynamic_cast<Interface*>(w);
						if(sc)
						{
							QString varName = interfaceToVarName(sc->ui.Le_name->text());
							_zTpl.enterSection( "SECTION_INTERFACECONSUMED" );    
							_zTpl[ "INTERFACE_TYPE" ] = sc->ui.Le_type->text();          
							_zTpl[ "INTERFACE_NAME" ] = sc->ui.Le_name->text();       
							_includeList.append(sc->ui.Le_include->text());
							_zTpl[ "CONSUMED_VAR_NAME" ] = varName;       
							_zTpl.exitSection();	
						}
					}
				}
			}
		}


		if ( cp->ui.Gb_IProvided->isChecked() )
		{
			_zTpl.showSection("SECTION_ISCONSUMED"); 

			for (int i = 0; i < cp->ui.Sb_pIntefaces->value() ; i++)
			{
				QLayoutItem * item = cp->ui.v_pInterfaces->itemAt(i);
				if(item)
				{
					QWidget * w = item->widget();
					if(w)
					{
						Interface * sc = dynamic_cast<Interface*>(w);
						if(sc)
						{
							QString varName = interfaceToVarName(sc->ui.Le_name->text());
							_zTpl.enterSection( "SECTION_INTERFACEPRODUCTED" );    
							_zTpl[ "INTERFACE_TYPE" ] = sc->ui.Le_type->text();       
							_includeList.append(sc->ui.Le_include->text());
							_zTpl[ "INTERFACE_NAME" ] = sc->ui.Le_name->text();       
							_zTpl[ "PRODUCTED_VAR_NAME" ] = varName;       
							_zTpl.exitSection();	
						}
					}
				}
			}
		}

		if ( cp->ui.Gb_properties->isChecked() )
		{
			_zTpl.showSection("SECTION_HAS_PROPERTIES"); 
			for (int i = 0; i < cp->ui.Sb_properties->value() ; i++)
			{
				QLayoutItem * item = cp->ui.v_properties->itemAt(i);
				if(item)
				{
					QWidget * w = item->widget();
					if(w)
					{
						Property * sc = dynamic_cast<Property*>(w);
						if(sc)
						{
							QString varName = interfaceToVarName(sc->ui.Le_name->text());
							QString tmpName = sc->ui.Le_name->text();
							tmpName = tmpName.mid(0,1).toUpper()+tmpName.mid(1,tmpName.length()-1);

							_zTpl.enterSection( "FOR_COMPONENT_PROPERTIES" );    
							_zTpl[ "CONTENT" ] = "Q_PROPERTY("+sc->ui.Cb_type->currentText()+" "+sc->ui.Le_name->text()+" READ get"+tmpName+" WRITE set"+tmpName+")\n\t\t\t\t";       
							_zTpl[ "DECLARE" ] = sc->ui.Cb_type->currentText()+" "+varName+";";       
							if(sc->ui.Cb_type->currentText() == "QString")
							{
								_zTpl[ "INIT" ] = varName+" = \""+sc->ui.le_defaultValue->text()+"\";";       
							}
							else
							{
								_zTpl[ "INIT" ] = varName+" = "+sc->ui.le_defaultValue->text()+";";       
							}
							_zTpl.exitSection();

							QString accessors = "\n\t\t\t\t/** @brief Property value "+sc->ui.Le_name->text()+" */\n";
							accessors += "\t\t\t\t"+sc->ui.Cb_type->currentText()+" get"+tmpName+"(){ return "+varName+";}\n";
							accessors += "\t\t\t\tvoid set"+tmpName+"("+sc->ui.Cb_type->currentText()+" val){ "+varName+" = val;}";

							_zTpl.enterSection( "SECTION_ACCESSOR" );    
							_zTpl[ "CONTENT" ] = accessors;       
							_zTpl.exitSection();	

						}
					}
				}
			}
		}

		_includeList.removeDuplicates();
		foreach(QString val, _includeList)
		{
			_zTpl.enterSection( "SECTION_INCLUDE" );    
			_zTpl[ "INCLUDE_FILE" ] = val;       
			_zTpl.exitSection();	
		}

		if(cp->ui.Cb_owner->isChecked())
		{
			_zTpl.showSection("SECTION_ISWPERSISTENT"); 
		}
		if(cp->ui.Cb_ownerConfigurable->isChecked())
		{
			_zTpl.showSection("SECTION_ISWPERSISTENTCONFIGURABLE"); 
		}

		if(cp->ui.Cb_executable->isChecked())
		{
			_zTpl.showSection("SECTION_ISWEXECUTABLE_SERVICE"); 
		}

		if(cp->ui.Cb_baseClass->currentText() == QString("Composant assisté + ISxModel"))
		{
			outCPP << _zTpl.expandFile(":/SWGenerator/Resources/BaseModelConsumerAssistedCPP.tpl");
			outH << _zTpl.expandFile(":/SWGenerator/Resources/BaseModelConsumerAssistedH.tpl");
		}
		else
		{
			outCPP << _zTpl.expandFile(":/SWGenerator/Resources/SwAssistedComponentCPP.tpl");
			outH << _zTpl.expandFile(":/SWGenerator/Resources/SwAssistedComponentH.tpl");
		}
		currentFileCPP.close();
		currentFileH.close();
	}
	else
	{
		log->append(TAB_LOG+ERROR_LOG+"Unable to write file <b>"+dirComponent+outputFileNameCPP+"</b>");
		return false;
	}


	return true;
}