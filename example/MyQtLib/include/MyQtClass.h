#ifndef MY_QTCLASS_H
#define MY_QTCLASS_H

#ifdef MYQTLIB_LIB
#define MyQtLib_EXPORT __declspec(dllexport)
#else
#define MyQtLib_EXPORT __declspec(dllimport)
#endif

#include <QObject>

class MyQtLib_EXPORT MyQtClass : public QObject
{
	Q_OBJECT
public:
	MyQtClass();
private:
	int _myint;
};

#endif // !MY_QTCLASS_H
