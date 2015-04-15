/**
	 * @file TComponent.h
	 * @brief class TComponent mappé sur la table TComponent
	 * @version 1.0
	 * @date 2011/10/18 - 18:10:2011 - 10:48
	 * @author AAY
	 */


#ifndef TCOMPONENT_H
#define TCOMPONENT_H

#include <QDjangoModel.h>
#include <QDateTime>

class TComponent : public QDjangoModel
{
	Q_OBJECT

	Q_PROPERTY(QString name READ getName WRITE setName);
	Q_PROPERTY(QString desc READ getDesc WRITE setDesc);
	Q_PROPERTY(QDateTime date_upd READ getDateUpd WRITE setDateUpd);
	Q_PROPERTY(QDateTime date_add READ getDateAdd WRITE setDateAdd);

	Q_CLASSINFO("date_add","auto_date=true")	
	Q_CLASSINFO("date_upd","auto_date=true")	


public:
	QString getName() const { return _name; }
	void setName(QString val) { _name = val; }

	QString getDesc() const { return _desc; }
	void setDesc(QString val) { _desc = val; }

	QDateTime getDateAdd() const { return _dateAdd; }
	void setDateAdd(QDateTime val) { _dateAdd = val; }

	QDateTime getDateUpd() const { return _dateUpd; }
	void setDateUpd(QDateTime val) { _dateUpd = val; }

private:
	QString _name;
	QString _desc;
	QDateTime _dateAdd;
	QDateTime _dateUpd;
};

#endif // TComponent_H
