#ifndef SYNCHRONIZERSTATEPAUSED_H
#define SYNCHRONIZERSTATEPAUSED_H

#include "SynchronizerState.h"

class SynchronizerStatePaused : public SynchronizerState
{
public:
	SynchronizerStatePaused(ObserverSynchronizer *synchronizer);

	virtual void stop();
	virtual void start();
	virtual void pause();
	virtual void imageObserved();
	virtual void newObserverAdded();
};

#endif /* end of include guard: SYNCHRONIZERSTATEPAUSED_H */
