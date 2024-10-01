#ifndef MY_PLUGINCLASS_H
#define MY_PLUGINCLASS_H

#ifdef MYPLUGIN_LIB
	#define MyPlugin_EXPORT __declspec(dllexport)
#else
	#define MyPlugin_EXPORT __declspec(dllimport)
#endif // MyLibB_LIB

class MyPlugin_EXPORT MyPluginClass
{
public:
	MyPluginClass();
private:
	int _myint;
};

#endif // !MY_PLUGINCLASS_H
