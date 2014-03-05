#ifndef MAINDOCKCONFIGURATION_H
#define MAINDOCKCONFIGURATION_H

#include <QMenu>
#include <QRadioButton>
#include <QGridLayout>

class SwDockWidget_MainDockConfiguration : public QMenu
{
	Q_OBJECT
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

signals:
	void changeConf(SwDockWidget_MainDockConfiguration::ConfigurationIndex index);

protected slots:
	void updateConf(bool state = false);

private:
	QRadioButton * addConfiguration(QString iconPath, ConfigurationIndex index = BLTR);

private:
	QMap<QRadioButton*, ConfigurationIndex> _listBtn;
	ConfigurationIndex _activeConf;
	QGridLayout * _layout;
};









#endif