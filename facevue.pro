TEMPLATE = subdirs

#first compile the library
SUBDIRS = VideoCapturer FaceVueAPI 

#then compile the executable
CONFIG += ordered
SUBDIRS += FaceVueGUI
