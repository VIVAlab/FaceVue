#include "VideoCapturerStateNull.h"
#include "VideoCapturerStateAllocated.h"

VideoCapturerStateNull::VideoCapturerStateNull (AbstractVideoCapturer *capturer):
	VideoCapturerState (capturer)
{
}

void
VideoCapturerStateNull::sendNextFrame ()
{
/*	capturer->allocate();
	VideoCapturerState* newState = new VideoCapturerStateAllocated(capturer);
	capturer->setState (newState);
	newState->sendNextFrame();
*/}

void
VideoCapturerStateNull::deallocate()
{
	//capturer is already deallocated
}

void
VideoCapturerStateNull::allocate()
{
	capturer->allocate();
	VideoCapturerState* newState = new VideoCapturerStateAllocated(capturer);
	capturer->setState (newState);
}

bool
VideoCapturerStateNull::isAllocated()
{
	return false;
}
