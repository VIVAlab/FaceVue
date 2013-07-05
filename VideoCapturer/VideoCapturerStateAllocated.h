#include "VideoCapturerState.h"

class VideoCapturerStateAllocated : public VideoCapturerState
{
public:
	VideoCapturerStateAllocated(AbstractVideoCapturer *capturer);

	void sendNextFrame ();
	void deallocate ();
	void allocate ();
	bool isAllocated();
};
