/**
	 * @file TUser.h
	 * @brief class TUser mappé sur la table TUser
	 * @version 1.0
	 * @date 2011/10/18 - 18:10:2011 - 10:48
	 * @author AAY
	 */


#ifndef TUSER_H
#define TUSER_H

#include <QDjangoModel>
#include <QDateTime>

class TUser : public QDjangoModel
{
	Q_OBJECT

	Q_PROPERTY(QString username READ getUserName WRITE setUserName);
	Q_PROPERTY(QString password READ getPassword WRITE setPassword);
	Q_PROPERTY(QDateTime date_add READ getDateAdd WRITE setDateAdd);


	Q_CLASSINFO("userName","max_length=128")
	Q_CLASSINFO("password","max_length=128")	
	Q_CLASSINFO("date_add","auto_date=true")	

public:

	QString getUserName() const { return _userName; }
	void setUserName(QString val) { _userName = val; }


	QString getPassword() const { return _password; }
	void setPassword(QString val) { _password = val; }


	QDateTime getDateAdd() const { return _dateAdd; }
	void setDateAdd(QDateTime val) { _dateAdd = val; }

private:
	QString _userName;
	QString _password;
	QDateTime _dateAdd;
};

#endif // TUser_H
