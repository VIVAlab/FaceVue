#ifndef _FACE_VUE_APPLICATION_H_
#define _FACE_VUE_APPLICATION_H_

#include <QtGui/QApplication>

class FaceVueApplication : public QApplication
{
	public:
		FaceVueApplication(int &argc, char **argv):QApplication(argc, argv) {}
		~FaceVueApplication() {}
		bool notify(QObject *rec, QEvent *ev)
		{
			printf ("::notify function is called\n");
			return QApplication::notify(rec, ev);
		}
};


#endif
