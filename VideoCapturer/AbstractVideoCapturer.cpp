#include "AbstractVideoCapturer.h"
#include "VideoCapturerStateNull.h"
#include <QThread>
#include <iostream>

AbstractVideoCapturer::AbstractVideoCapturer ()
{
	image = NULL;
	myState = new VideoCapturerStateNull (this);
	mySynchronizer = new ObserverSynchronizer (this);
	myThread = new QThread;

	mySynchronizer->moveToThread(myThread);
	this->moveToThread (myThread);
	myThread->start();
}

/**
 * make sure sync->stop() is called and stopped() signal is already recieved...
 */ 
AbstractVideoCapturer::~AbstractVideoCapturer ()
{
	myThread->exit(0);
	myThread->wait();
	delete myThread;
	delete mySynchronizer;
	delete myState;
}

void
AbstractVideoCapturer::ensureAllocated()
{
	mutex.lock();
	myState->allocate();
	mutex.unlock();
}

void
AbstractVideoCapturer::ensureDeallocated()
{
	mutex.lock();
	myState->deallocate();
	mutex.unlock();
}

ObserverSynchronizer*
AbstractVideoCapturer::getSynchronizer()
{
	return mySynchronizer;
}

void
AbstractVideoCapturer::setState (VideoCapturerState *state)
{
	Q_ASSERT (state != NULL);
	delete myState;
	myState = state;
}

void
AbstractVideoCapturer::sendNextFrame ()
{
	if (image)
	{
		delete image;
		image = NULL;
	}
	obtainNextFrame ();
	Q_ASSERT (image != NULL);
	emit imageAvailable (image);
}

void
AbstractVideoCapturer::timeForNextImage ()
{
	mutex.lock();
	myState->sendNextFrame();
	mutex.unlock();
}

bool
AbstractVideoCapturer::destroy()
{
	mutex.lock();
	bool isAllocated = myState->isAllocated();
	mutex.unlock();
	if (isAllocated)
		return false;
	delete this;
	return true;
}
