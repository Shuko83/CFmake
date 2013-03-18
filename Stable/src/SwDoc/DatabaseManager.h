#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QThread>
#include <QtSql/QSqlDatabase>
class ManageSQL;

static int MaxTryConnection = 5;

class DatabaseManager : public QThread
{
	Q_OBJECT

public:
	DatabaseManager();
	~DatabaseManager();

	bool openDB();

	void initDB();

protected:
	virtual void run();


signals:
	void connectionState(bool);
private:
	QSqlDatabase m_db;  /** the database connection*/
	bool _isInit;
	int _nbTry;

};

#endif // DATABASEMANAGER_H
