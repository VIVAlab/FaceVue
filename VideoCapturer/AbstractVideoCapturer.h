#ifndef ABSTRACTVIDEOCAPTURER_H
#define ABSTRACTVIDEOCAPTURER_H

#include <QObject>
#include "ObserverSynchronizer.h"
#include <QThread>

class VideoCapturerState;

class AbstractVideoCapturer : public QObject
{
	Q_OBJECT
	friend class VideoCapturerStateNull;
	friend class VideoCapturerStateAllocated;
	public:
		AbstractVideoCapturer();
		ObserverSynchronizer *getSynchronizer();
		void setState (VideoCapturerState *state);
		void sendNextFrame ();
		void ensureAllocated();
		void ensureDeallocated();
		bool destroy();

	protected:
		virtual void allocate () = 0;
		virtual void deallocate () = 0;
		virtual void obtainNextFrame () = 0;

	signals:
		void imageAvailable (Mat* img);
		
	public slots:
		void timeForNextImage ();

	private:
		ObserverSynchronizer *mySynchronizer;
		VideoCapturerState *myState;
		QThread *myThread;
		QMutex mutex;

	protected:
		Mat* image;
		virtual ~AbstractVideoCapturer();
};


#endif /* end of include guard: ABSTRACTVIDEOCAPTURER_H */
