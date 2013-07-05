#ifndef SYNCHRONIZERSTATEPLAYING_H
#define SYNCHRONIZERSTATEPLAYING_H
#include "SynchronizerState.h"

class SynchronizerStatePlaying : public SynchronizerState
{
public:
	SynchronizerStatePlaying(ObserverSynchronizer *synchronizer);
	
	virtual void pause();
	virtual void start();
	virtual void stop();
	virtual void newObserverAdded();
	virtual void imageObserved();
};


#endif /* end of include guard: SYNCHRONIZERSTATEPLAYING_H */
