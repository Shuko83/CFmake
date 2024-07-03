#ifndef MY_CLASS_A_H
#define MY_CLASS_A_H

#ifdef MYLIB_LIB
#define MyLib_EXPORT __declspec(dllexport)
#else
#define MyLib_EXPORT __declspec(dllimport)
#endif

class MyLib_EXPORT MyClassA
{
public:
	MyClassA();
private:
	int _myint;
};

#endif // !MY_CLASS_A_H
