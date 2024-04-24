#ifndef MY_CLASS_B_H
#define MY_CLASS_B_H

#ifdef MYLIBB_LIB
	#define MyLibB_EXPORT __declspec(dllexport)
#else
	#define MyLibB_EXPORT __declspec(dllimport)
#endif // MyLibB_LIB


class MyLibB_EXPORT MyClassB
{
public:
	MyClassB();
private:
	int _myint;
};

#endif // !MY_CLASS_B_H
