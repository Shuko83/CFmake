#ifndef MY_CLASS_B_H
#define MY_CLASS_B_H

#ifdef MYPLUGIN_LIB
	#define MyPlugin_EXPORT __declspec(dllexport)
#else
	#define MyPlugin_EXPORT __declspec(dllimport)
#endif // MyLibB_LIB

class MyPlugin_EXPORT MyClassB
{
public:
	MyClassB();
private:
	int _myint;
};

#endif // !MY_CLASS_B_H
