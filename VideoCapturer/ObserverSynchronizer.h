#ifndef OBSERVERSYNCHRONIZER_H
#define OBSERVERSYNCHRONIZER_H

#include <QObject>
#include <QMap>
#include <QMutex>
#include "VideoObserver.h"
#include "SynchronizerState.h"

class AbstractVideoCapturer;  //foreward declaration

class ObserverSynchronizer : public QObject
{
	Q_OBJECT
		friend class SynchronizerStatePaused;
		friend class SynchronizerStatePlaying;
		friend class SynchronizerStateStopped;
	public:
	ObserverSynchronizer(AbstractVideoCapturer *capturer);
	~ObserverSynchronizer();
	void addObserver (VideoObserver *observer);
	void start ();
	void stop ();
	void pause ();



	public slots:
		void imageObserved (VideoObserver *observer);

signals:
	void proceed();


private:
	QMap<VideoObserver*, bool> observers;
	AbstractVideoCapturer *capturer;
	QMutex observerListMutex;
	QMutex stateMutex;
	SynchronizerState *myState;

	void setState (SynchronizerState *state);
	void _proceed();
	bool hasObservers();
	bool canProceed();
};


#endif /* end of include guard: OBSERVERSYNCHRONIZER_H */
