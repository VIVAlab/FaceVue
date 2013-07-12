#include "ProcessingMode.h"

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
	return (flags & PROCESSING_MODE_RETURN_KEY);
}

ProcessingMode::~ProcessingMode()
{
}

void 
ProcessingMode::setRecognitionFlag (bool set)
{
	flags = set ? (flags | PROCESSING_MODE_FLAG_RECOGNIZED) :  
		      (flags & ~(PROCESSING_MODE_FLAG_RECOGNIZED)); 
}

void 
ProcessingMode::setDetectionFlag (bool set)
{
	flags = set ? (flags | PROCESSING_MODE_FLAG_DETECTED) :
		      (flags & ~(PROCESSING_MODE_FLAG_DETECTED));
}
	
void 
ProcessingMode::setReturnKeyFlag (bool set)
{
	flags = set ? (flags | PROCESSING_MODE_RETURN_KEY) :
		      (flags & ~(PROCESSING_MODE_RETURN_KEY));
}


