#ifndef MY_QTCLASS_A_H
#define MY_QTCLASS_A_H

#include <QObject>

class MyQtClassA : public QObject
{
	Q_OBJECT
public:
	MyQtClassA();
private:
	int _myint;
};

#endif // !MY_CLASS_A_H
