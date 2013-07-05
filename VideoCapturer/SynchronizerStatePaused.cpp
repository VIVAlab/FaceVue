#include "SynchronizerStatePaused.h"
#include "SynchronizerStatePlaying.h"
#include "SynchronizerStateStopped.h"
#include "ObserverSynchronizer.h"
#include "AbstractVideoCapturer.h"

SynchronizerStatePaused::SynchronizerStatePaused(ObserverSynchronizer *synchronizer):
	SynchronizerState(synchronizer)
{
}

void 
SynchronizerStatePaused::stop()
{
	if (synchronizer->canProceed())
		synchronizer->capturer->ensureDeallocated();
	synchronizer->setState (new SynchronizerStateStopped(synchronizer));
}

void 
SynchronizerStatePaused::start()
{
	synchronizer->capturer->ensureAllocated();
	if (synchronizer->canProceed()) 
	{
		synchronizer->_proceed();
	}
	synchronizer->setState (new SynchronizerStatePlaying(synchronizer));
}

void 
SynchronizerStatePaused::pause()
{
	synchronizer->capturer->ensureAllocated();
}

void 
SynchronizerStatePaused::imageObserved()
{
	//nothing to do
}

void SynchronizerStatePaused::newObserverAdded()
{
	//nothing to do
}

