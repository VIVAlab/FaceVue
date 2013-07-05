#include <QObject>
#include "ObserverSynchronizer.h"
#include "AbstractVideoCapturer.h"
#include "SynchronizerStateStopped.h"

ObserverSynchronizer::ObserverSynchronizer (AbstractVideoCapturer *capturer)
{
	this->capturer = capturer;
	myState = new SynchronizerStateStopped(this);
	connect (this, SIGNAL (proceed()), capturer, SLOT (timeForNextImage()), Qt::QueuedConnection);
}

void
ObserverSynchronizer::setState (SynchronizerState *state)
{
	delete myState;
	myState = state;
}

void 
ObserverSynchronizer::addObserver (VideoObserver *observer)
{
	observerListMutex.lock();
	observers[observer] = true;
	QObject::connect (observer, 
			  SIGNAL (imageObserved(VideoObserver*)), 
			  this, 
			  SLOT (imageObserved(VideoObserver*)), 
			  Qt::QueuedConnection);
	QObject::connect (capturer, 
			  SIGNAL (imageAvailable (Mat *)), 
			  observer, 
			  SLOT (imageAvailable (Mat *)), 
			  Qt::QueuedConnection);
	observerListMutex.unlock();

	stateMutex.lock();
	myState->newObserverAdded();
	stateMutex.unlock();
}

void
ObserverSynchronizer::start ()
{
	stateMutex.lock();
	myState->start();
	stateMutex.unlock();
}

bool
ObserverSynchronizer::hasObservers()
{
	observerListMutex.lock();
	bool retVal = (observers.size() != 0);
	observerListMutex.unlock();
	return retVal;
}

bool
ObserverSynchronizer::canProceed()
{
	observerListMutex.lock();
	bool retVal = true;
	for (QMap<VideoObserver*, bool>::iterator iter = observers.begin(); 
	     iter != observers.end() && retVal;
	     iter++)
	{
		if (iter.value() == false)
			retVal = false;
	}
	observerListMutex.unlock();
	return retVal;
}

void
ObserverSynchronizer::_proceed()
{
	observerListMutex.lock();
	for (QMap<VideoObserver*, bool>::iterator iter = observers.begin(); 
	     iter != observers.end();
	     iter++)
	{
		bool & val = iter.value();
		val = false;
	}
	observerListMutex.unlock();
	emit proceed();
}

void
ObserverSynchronizer::stop()
{
	stateMutex.lock();
	myState->stop();
	stateMutex.unlock();
}

void
ObserverSynchronizer::pause ()
{
	stateMutex.lock();
	myState->pause();
	stateMutex.unlock();
}

void
ObserverSynchronizer::imageObserved (VideoObserver *observer)
{
	observerListMutex.lock();
	observers[observer] = true;
	observerListMutex.unlock();
	myState->imageObserved();
}

ObserverSynchronizer::~ObserverSynchronizer()
{
	stateMutex.lock();
	delete myState;
	myState=NULL;
	stateMutex.unlock();
}
