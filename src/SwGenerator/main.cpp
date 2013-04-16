#include "SWGenerator.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SWGenerator w;
	w.show();
	return a.exec();
}
