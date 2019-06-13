#include "RTPDemo.h"
#include <iostream>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	std::cout << "Using version " << RTPLibraryVersion::GetVersion().GetVersionString() << std::endl;
	RTPDemo w;
	w.show();
	return a.exec();
}
