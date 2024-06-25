#ifndef SWDOCKWIDGET_MAINDOCKCONFIGURATION_H
#define SWDOCKWIDGET_MAINDOCKCONFIGURATION_H

#include <QMenu>
#include <QPushButton>
#include <QGridLayout>

class SwDockWidget_MainDockConfiguration : public QMenu
{
	Q_OBJECT;
	Q_ENUMS(ConfigurationIndex);

public :
	SwDockWidget_MainDockConfiguration(QString title, QWidget * parent = 0);
	~SwDockWidget_MainDockConfiguration();

	typedef enum
	{
		BLTR,
		BRLT,
		BRTL,
		LBRT,
		LTBR,
		LTRB,
		RBLT,
		RLTB,
		RTBL,
		RTLB,
		TBRL,
		TLBR,
		TRBL,
		TRLB,

		NB_CONF
	}ConfigurationIndex;

	void setActiveConfiguration(ConfigurationIndex index);
	void setActiveConfiguration(QString conf);
	ConfigurationIndex getActiveConfiguration();
	QString getActiveConfigurationInString();
	QString getConfigurationInString(int index);

signals:
	void changeConf(SwDockWidget_MainDockConfiguration::ConfigurationIndex index);

protected slots:
	void updateConf(bool state = false);
	void showMore();

protected:
	virtual void hideEvent(QHideEvent * event);

private:
	QPushButton * addConfiguration(QString iconPath, ConfigurationIndex index = BLTR, bool visible = true);
	SwDockWidget_MainDockConfiguration::ConfigurationIndex getExistingConf(QString conf, int nbDocks);
	QPushButton * addSecondaryConfOption(QString iconPath);

private:
	QMap<QPushButton*, ConfigurationIndex> _listBtn;
	ConfigurationIndex _activeConf;
	QGridLayout * _layout;
	QPushButton * _btnMore;
};

#endif
