/**
* @file TKeyComp.h
* @brief class TKeyComp mappé sur la table TKeyComp
* @version 1.0
* @date 2011/10/18 - 18:10:2011 - 10:48
* @author AAY
*/


#ifndef TKEYCOMP_H
#define TKEYCOMP_H

#include <QDjangoModel.h>
#include <QDateTime>

class TKeyComp : public QDjangoModel
{
	Q_OBJECT

	Q_PROPERTY(int id_keyword READ getIdKey WRITE setIdKey);
	Q_PROPERTY(int id_component READ getIdComp WRITE setIdComp);
	Q_PROPERTY(QDateTime date_add READ getDateAdd WRITE setDateAdd);

	Q_CLASSINFO("date_add","auto_date=true")	

public:

	int getIdKey() const { return _idKey; }
	void setIdKey(int val) { _idKey = val; }

	int getIdComp() const { return _idComp; }
	void setIdComp(int val) { _idComp = val; }

	QDateTime getDateAdd() const { return _dateAdd; }
	void setDateAdd(QDateTime val) { _dateAdd = val; }

private:
	int _idKey;
	int _idComp;
	QDateTime _dateAdd;
};

#endif // USER_H
