#include "VideoCapturerStateAllocated.h"
#include "VideoCapturerStateNull.h"

VideoCapturerStateAllocated::VideoCapturerStateAllocated(AbstractVideoCapturer *capturer):
	VideoCapturerState(capturer)
{
}

void
VideoCapturerStateAllocated::sendNextFrame()
{
	capturer->sendNextFrame();
}

void
VideoCapturerStateAllocated::deallocate ()
{
	capturer->deallocate();
	VideoCapturerState *newState = new VideoCapturerStateNull(capturer);
	capturer->setState (newState);
}

void
VideoCapturerStateAllocated::allocate ()
{
}

bool
VideoCapturerStateAllocated::isAllocated ()
{
	return true;
}
