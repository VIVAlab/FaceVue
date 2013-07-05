#include "SynchronizerStateStopped.h"
#include "SynchronizerStatePlaying.h"
#include "SynchronizerStatePaused.h"
#include "AbstractVideoCapturer.h"
#include <QDebug>

SynchronizerStateStopped::SynchronizerStateStopped(ObserverSynchronizer *synchronizer):
	SynchronizerState(synchronizer)
{
}

void
SynchronizerStateStopped::start()
{
	if (synchronizer->hasObservers())
	{
		synchronizer->capturer->ensureAllocated();
		if (synchronizer->canProceed())
			synchronizer->_proceed();
		synchronizer->setState (new SynchronizerStatePlaying(synchronizer));
	} else {
		qDebug () << "Capturer needs at least 1 observer to start";
	}
}

void
SynchronizerStateStopped::stop()
{
}

void
SynchronizerStateStopped::pause()
{
	if (synchronizer->hasObservers())
	{
		synchronizer->capturer->ensureAllocated();
		synchronizer->setState(new SynchronizerStatePaused(synchronizer));
	} else {
		qDebug() << "Cannot change state with no observers";
	}
}

void
SynchronizerStateStopped::newObserverAdded()
{
	//nothing to do
}

void
SynchronizerStateStopped::imageObserved()
{
	if (synchronizer->canProceed())
		synchronizer->capturer->ensureDeallocated();
}
