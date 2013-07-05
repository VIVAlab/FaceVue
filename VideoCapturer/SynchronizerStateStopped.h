#ifndef SYNCHRONIZERSTATESTOPPED_H
#define SYNCHRONIZERSTATESTOPPED_H
#include "SynchronizerState.h"

class SynchronizerStateStopped : public SynchronizerState
{
	friend class ObserverSynchronizer;
	public:
	SynchronizerStateStopped(ObserverSynchronizer *synchronizer);

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void newObserverAdded();
	virtual void imageObserved();
};


#endif /* end of include guard: SYNCHRONIZERSTATESTOPPED_H */
