#include "ProcessingMode.h"

ProcessingMode::ProcessingMode (FaceVuee *gui, FaceVue *facevue)
{
	this->facevue = facevue;
	this->gui = gui;
	recognitionPalette.setColor (QPalette::Window, Qt::red);
	registrationPalette.setColor (QPalette::Window, Qt::red);
}

QPalette
ProcessingMode::getRecognitionLabelPalette () const
{
	return recognitionPalette;
}

QPalette
ProcessingMode::getDetectionLabelPalette () const
{
	return registrationPalette;
}

ProcessingMode::~ProcessingMode()
{
}
