#pragma once


#include "SwAssistedComponent.h"
#include "qdialog.h"
#include "qboxlayout.h"
#include "ISwWidget.h"
#include "SwServiceManager_Helper.h"
#include "ISwServiceMainWindow.h"
#include "ISwAction.h"
#include "qevent.h"

#define QWIDGET_TO_QDIALOG_COMPONENT "_SwGuiCompQWidgetToQDialog"




class DialogContainer : public QDialog, public StreamWork::SwGui::ISwWidget
{
public :
	DialogContainer()
	{
		_layout = new QVBoxLayout(this);
		_layout->setContentsMargins(0, 0, 0, 0);
		setLayout(_layout);
		setMinimumSize(100, 100);
	}

	void setContentWidget(QWidget * wid)
	{
		if ( wid != 0 )
		{
			//Enregistrement du noluveau
			resize(wid->size());
			setMinimumSize(wid->minimumSize());
			setWindowTitle(wid->windowTitle());
			setWindowIcon(wid->windowIcon());
			connect(wid, &QWidget::windowTitleChanged, [=](const QString& value) { setWindowTitle(value); });
			_layout->addWidget(wid);
		}
	}

	void keyPressEvent(QKeyEvent *evt)
	{
		if ( evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return || evt->key() == Qt::Key_Escape )
			return;
		QDialog::keyPressEvent(evt);
	}

	virtual QWidget & GetWidget()
	{
		return *this;
	};

private:
	QVBoxLayout * _layout;
};


/**
*	@class _SwGuiCompQWidgetToQDialog
*	@brief Charger de convertir un widget en QDialog parenté
*/
class _SwGuiCompQWidgetToQDialog : public StreamWork::SwFoundation::SwAssistedComponent, public StreamWork::SwGui::ISwAction
{
	Q_OBJECT;
	Q_PROPERTY(bool EnableMaximize READ getEnableMaximize WRITE setEnableMaximize);
	bool getEnableMaximize() const { return _enableMaximize; }
	void setEnableMaximize(bool val) { _enableMaximize = val; }

public:

		/** @brief : Constructeur */
		_SwGuiCompQWidgetToQDialog();

	/** @brief : Destructeur */
	virtual ~_SwGuiCompQWidgetToQDialog();

	/**
	* @brief    : Initialisation du composant
	*/
	virtual void initializeComponent() throw(SwException);

	virtual void interfaceAvailable(QString interfaceName);

	virtual void interfaceUnavailable(QString interfaceName);

	void onService(QString servicename, bool isEnabled);

	virtual QAction & GetAction();

protected:
	DialogContainer _container;
	QWidget *_widget;
	bool _enableMaximize;

	QAction *_action;
	SwServiceManager_Helper<StreamWork::Service::ISwServiceMainWindow, _SwGuiCompQWidgetToQDialog> *_helper;
};

