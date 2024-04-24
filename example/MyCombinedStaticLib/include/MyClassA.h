#ifndef MY_CLASS_A_H
#define MY_CLASS_A_H

#ifdef MYLIBA_LIB
#define MyLibA_EXPORT __declspec(dllexport)
#else
#define MyLibA_EXPORT __declspec(dllimport)
#endif

class MyLibA_EXPORT MyClassA
{
public:
	MyClassA();
private:
	int _myint;
};

#endif // !MY_CLASS_A_H
