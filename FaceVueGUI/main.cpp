#include "GUI/facevue2.h"
#include "FaceVueApplication.h"

int main(int argc, char *argv[])
{
        FaceVueApplication a(argc, argv);        
        FaceVuee w;
        w.show();
        return a.exec();
}
