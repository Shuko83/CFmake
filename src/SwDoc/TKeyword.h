/**
	 * @file TKeyword.h
	 * @brief class TKeyword mappé sur la table TKeyword
	 * @version 1.0
	 * @date 2011/10/18 - 18:10:2011 - 10:48
	 * @author AAY
	 */


#ifndef TKEYWORD_H
#define TKEYWORD_H

#include <QDjangoModel>
#include <QDateTime>

class TKeyword : public QDjangoModel
{
	Q_OBJECT

	Q_PROPERTY(QString name READ getName WRITE setName);
	Q_PROPERTY(QDateTime date_add READ getDateAdd WRITE setDateAdd);
	Q_PROPERTY(int use READ getUse WRITE setUse);

	Q_CLASSINFO("date_add","auto_date=true")	

public:

	QString getName() const { return _name; }
	void setName(QString val) { _name = val; }

	QDateTime getDateAdd() const { return _dateAdd; }
	void setDateAdd(QDateTime val) { _dateAdd = val; }

	int getUse() const { return _use; }
	void setUse(int val) { _use = val; }

private:
	QString _name;
	int _use;
	QDateTime _dateAdd;
};

#endif // USER_H
