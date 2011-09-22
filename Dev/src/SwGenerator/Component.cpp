#include "Component.h"
#include "Property.h"
#include "Shortcut.h"
#include "Interface.h"


Component::Component(QWidget *parent)
	: QWidget(parent)
{

	_nbCInterfaces = 0;
	_nbPInterfaces = 0;
	_nbProperties = 0;
	_nbShortcuts = 0;

	ui.setupUi(this);

	QRegExp rx("([A-Z]|[a-z]|\\d|\\s|_)*");
	v= new QRegExpValidator(rx, 0);

	ui.Le_name->setValidator(v);

	connect(ui.Gb_pins, SIGNAL(toggled ( bool )), this,SLOT(gbToggled(bool)));
	connect(ui.Gb_IConsummed, SIGNAL(toggled ( bool )), this,SLOT(gbToggled(bool)));
	connect(ui.Gb_IProvided, SIGNAL(toggled ( bool )), this,SLOT(gbToggled(bool)));
	connect(ui.Gb_properties, SIGNAL(toggled ( bool )), this,SLOT(gbToggled(bool)));
	connect(ui.Gb_shortcuts, SIGNAL(toggled ( bool )), this,SLOT(gbToggled(bool)));

	connect(ui.Pb_icon,SIGNAL(clicked()),this,SLOT(lookForIcon()));

	foreach(QWidget *test,this->findChildren<QWidget*>(QRegExp("Gb_*")))
	{
		test->setStyleSheet("QGroupBox{border-top: 1px solid gray;border-radius: 0px;margin-top: 2ex;}QGroupBox::title {subcontrol-origin: margin;subcontrol-position: left top;left:10px}");
		QList<QWidget*> list =  test->findChildren<QWidget*>(QRegExp("w_*"));
		if(!list.isEmpty())
		{
			list.at(0)->hide();
		}
	}


	connect(ui.Sb_properties,SIGNAL(valueChanged(int)),this,SLOT(nbPropertiesChanged(int)));
	connect(ui.Sb_cIntefaces,SIGNAL(valueChanged(int)),this,SLOT(nbCInterfacesChanged(int)));
	connect(ui.Sb_pIntefaces,SIGNAL(valueChanged(int)),this,SLOT(nbPInterfacesChanged(int)));
	connect(ui.Sb_shortcuts,SIGNAL(valueChanged(int)),this,SLOT(nbShortcutChanged(int)));

	connect(ui.Le_name,SIGNAL(textChanged ( const QString & )),this,SLOT(nameChanged(const QString&)));
}

Component::~Component()
{

}

//-------------------------------------------------------------------------
void Component::gbToggled( bool val)
{
	QGroupBox * item = qobject_cast<QGroupBox *> (sender());
	QList<QWidget*> list =  item->findChildren<QWidget*>(QRegExp("w_*"));

	if(val)
	{

		item->setStyleSheet("QGroupBox{border: 1px solid gray;border-radius: 3px;margin-top: 2ex;}QGroupBox::title {subcontrol-origin: margin;subcontrol-position: left top;left:10px}");
		if(!list.isEmpty())
		{
			list.at(0)->show();
		}
	}
	else
	{
		item->setStyleSheet("QGroupBox{border-top: 1px solid gray;border-radius: 0px;margin-top: 2ex;}QGroupBox::title {subcontrol-origin: margin;subcontrol-position: left top;left:10px}");
		if(!list.isEmpty())
		{
			list.at(0)->hide();
		}
	}
}

//-------------------------------------------------------------------------
void Component::nbPropertiesChanged( int val )
{
	if(val > _nbProperties)
	{
		for (int i = 0; i < val - _nbProperties ; i++)
		{
			Property * P =new Property();
			ui.v_properties->addWidget(P);
		}
	}
	else
	{
		for (int i = _nbProperties-1; i > val-1; i--)
		{
			QLayoutItem *li = ui.v_properties->itemAt(i);
			if(li)
			{
				QWidget *w = li->widget();
				if(w)
				{
					ui.v_properties->removeWidget(w);
					delete w;
				}
			}
		}
	}

	_nbProperties = val;
}


//-------------------------------------------------------------------------
void Component::nbPInterfacesChanged( int val )
{
	if(val > _nbPInterfaces)
	{
		for (int i = 0; i < val - _nbPInterfaces ; i++)
		{
			Interface * P = new Interface();
			ui.v_pInterfaces->addWidget(P);
		}
	}
	else
	{
		for (int i = _nbPInterfaces-1; i > val-1; i--)
		{
			QLayoutItem *li = ui.v_pInterfaces->itemAt(i);
			if(li)
			{
				QWidget *w = li->widget();
				if(w)
				{
					ui.v_pInterfaces->removeWidget(w);
					delete w;
				}
			}
		}
	}

	_nbPInterfaces = val;
}


//-------------------------------------------------------------------------
void Component::nbCInterfacesChanged( int val )
{
	if(val > _nbCInterfaces)
	{
		for (int i = 0; i < val - _nbCInterfaces ; i++)
		{
			Interface * P = new Interface();
			ui.v_cInterfaces->addWidget(P);
		}
	}
	else
	{
		for (int i = _nbCInterfaces-1; i > val-1; i--)
		{
			QLayoutItem *li = ui.v_cInterfaces->itemAt(i);
			if(li)
			{
				QWidget *w = li->widget();
				if(w)
				{
					ui.v_cInterfaces->removeWidget(w);
					delete w;
				}
			}
		}
	}

	_nbCInterfaces = val;
}


//-------------------------------------------------------------------------
void Component::nbShortcutChanged( int val )
{
	if(val > _nbShortcuts)
	{
		for (int i = 0; i < val - _nbShortcuts ; i++)
		{
			Shortcut * P = new Shortcut();
			ui.v_shortcuts->addWidget(P);
		}
	}
	else
	{
		for (int i = _nbShortcuts-1; i > val-1; i--)
		{
			QLayoutItem *li = ui.v_shortcuts->itemAt(i);
			if(li)
			{
				QWidget *w = li->widget();
				if(w)
				{
					ui.v_shortcuts->removeWidget(w);
					delete w;
				}
			}
		}
	}
	_nbShortcuts = val;
}


//-------------------------------------------------------------------------
void Component::setIconPath(QString val) 
{ 
	_iconPath = val; 
	if(QFile::exists(_iconPath))
		ui.Pb_icon->setIcon(QIcon(_iconPath));
	ui.Pb_icon->setText(_iconPath);

}

//-------------------------------------------------------------------------
void Component::lookForIcon()
{
	QString test = QFileDialog::getOpenFileName(this,"Icon","c:\\","png file(*.png)");
	setIconPath(test);

}

//-------------------------------------------------------------------------
void Component::nameChanged( const QString &val )
{
	QString tmp = val;
	tmp.replace(QString(" "),QString("_"));
	QString name = tmp.toLower();

	name = name.mid(0,1).toUpper()+tmp.mid(1,tmp.length()-1);
	ui.Le_name->setText(name);
	ui.groupBox->setTitle(name);
}

