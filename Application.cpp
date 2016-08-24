#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setOrganizationName("SvOlli");
   a.setOrganizationDomain("svolli.org");
   a.setApplicationName("asuc");

   MainWindow w;
   w.show();

   return a.exec();
}
