/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <QApplication>
#include <QWidget>

#include "MyQtClass.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    QWidget w;
    w.show();

    app.exec();

    return 0;
}
