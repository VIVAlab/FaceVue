#ifndef SYNCHRONIZERSTATE_H
#define SYNCHRONIZERSTATE_H

class ObserverSynchronizer;

class SynchronizerState
{
public:
	SynchronizerState(ObserverSynchronizer *synchronizer);

	virtual void start()=0;
	virtual void stop()=0;
	virtual void pause()=0;
	virtual void newObserverAdded()=0;
	virtual void imageObserved()=0;

protected:
	ObserverSynchronizer *synchronizer;

};


#endif /* end of include guard: SYNCHRONIZERSTATE_H */
