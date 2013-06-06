#include "Gui/facevue2.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);        
        FaceVuee w;
        w.show();
        return a.exec();
}
