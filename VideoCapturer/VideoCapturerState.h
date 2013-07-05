#ifndef VIDEOCAPTURERSTATE_H
#define VIDEOCAPTURERSTATE_H

#include "AbstractVideoCapturer.h"

class VideoCapturerState
{
public:
	VideoCapturerState(AbstractVideoCapturer *capturer) {this->capturer=capturer;}

	virtual void sendNextFrame () = 0;
	virtual void deallocate () = 0;
	virtual void allocate () = 0;
	virtual bool isAllocated () = 0;


protected:
	AbstractVideoCapturer *capturer;
};



#endif /* end of include guard: VIDEOCAPTURERSTATE_H */
