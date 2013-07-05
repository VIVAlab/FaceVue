#include "SynchronizerStatePlaying.h"
#include "SynchronizerStatePaused.h"
#include "SynchronizerStateStopped.h"
#include "ObserverSynchronizer.h"
#include "AbstractVideoCapturer.h"

SynchronizerStatePlaying::SynchronizerStatePlaying (ObserverSynchronizer *synchronizer):
	SynchronizerState (synchronizer)
{
}

void
SynchronizerStatePlaying::pause()
{
	synchronizer->setState (new SynchronizerStatePaused(synchronizer));
}

void
SynchronizerStatePlaying::start()
{
	//nothing to do
}

void
SynchronizerStatePlaying::stop()
{
	synchronizer->setState (new SynchronizerStateStopped(synchronizer));
}

void
SynchronizerStatePlaying::newObserverAdded()
{
	if (synchronizer->canProceed())
		synchronizer->_proceed();
}

void
SynchronizerStatePlaying::imageObserved()
{
	if (synchronizer->canProceed())
		synchronizer->_proceed();
}

