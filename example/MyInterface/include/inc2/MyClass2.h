#ifndef MY_CLASS_H
#define MY_CLASS_H

#ifdef MYLIB_LIB
#define MyLib_EXPORT __declspec(dllexport)
#else
#define MyLib_EXPORT __declspec(dllimport)
#endif

class MyLib_EXPORT MyClass
{
public:
	MyClass();
private:
	int _myint;
};

#endif // !MY_CLASS_H
