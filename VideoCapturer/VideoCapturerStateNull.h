#ifndef VIDEOCAPTURERSTATENULL_H
#define VIDEOCAPTURERSTATENULL_H

#include "VideoCapturerState.h"

class VideoCapturerStateNull : public VideoCapturerState
{
	public:
		VideoCapturerStateNull (AbstractVideoCapturer *capturer);
		virtual void sendNextFrame ();
		virtual void deallocate();
		virtual void allocate();
		virtual bool isAllocated();
};

#endif /* end of include guard: VIDEOCAPTURERSTATENULL_H */
