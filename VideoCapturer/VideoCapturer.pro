TEMPLATE=lib
CONFIG+= link_pkgconfig staticlib
PKGCONFIG += opencv
HEADERS=\ 
	AbstractVideoCapturer.h\
	ObserverSynchronizer.h\
	VideoCapturerStateAllocated.h\
	VideoCapturerState.h\
	VideoCapturerStateNull.h\
	VideoObserver.h\
	SynchronizerState.h\
	SynchronizerStatePaused.h\
	SynchronizerStatePlaying.h\
	SynchronizerStateStopped.h


SOURCES=\
	AbstractVideoCapturer.cpp\
	ObserverSynchronizer.cpp\
	VideoCapturerStateAllocated.cpp\
	VideoCapturerStateNull.cpp\
	VideoObserver.cpp\
	SynchronizerState.cpp\
	SynchronizerStatePaused.cpp\
	SynchronizerStatePlaying.cpp\
	SynchronizerStateStopped.cpp



