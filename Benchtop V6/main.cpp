#include "benchtop.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Benchtop w;
	w.show();
	return a.exec();
}
