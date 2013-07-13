#include "ProcessingMode.h"

//this macro helps set/unset a flag
#define SET_FLAG(flags,flag,set) (flags=set?(flags | flag):(flags & ~(flag)))

ProcessingMode::ProcessingMode (FaceVuee *gui, FaceVue *facevue)
{
	this->facevue = facevue;
	this->gui = gui;
	flags = 0; //no flags are set
}

bool
ProcessingMode::isRecognized() const
{
	return (flags & PROCESSING_MODE_FLAG_RECOGNIZED);
}

bool
ProcessingMode::isDetected() const
{
	return (flags & PROCESSING_MODE_FLAG_DETECTED);
}


bool 
ProcessingMode::isReturnKeyPressed() const
{
	return (flags & PROCESSING_MODE_FLAG_RETURN_KEY);
}

bool
ProcessingMode::drawsOverlay () const
{
	return (flags & PROCESSING_MODE_FLAG_DRAW_OVERLAY);
}

ProcessingMode::~ProcessingMode()
{
}

void 
ProcessingMode::setRecognitionFlag (bool set)
{
	SET_FLAG(flags, PROCESSING_MODE_FLAG_RECOGNIZED, set); 
}

void 
ProcessingMode::setDetectionFlag (bool set)
{
	SET_FLAG(flags, PROCESSING_MODE_FLAG_DETECTED, set); 
}
	
void 
ProcessingMode::setReturnKeyFlag (bool set)
{
	SET_FLAG(flags, PROCESSING_MODE_FLAG_RETURN_KEY, set); 
}

void
ProcessingMode::setDrawOverlayFlag (bool set)
{
	SET_FLAG(flags, PROCESSING_MODE_FLAG_DRAW_OVERLAY, set);
}
