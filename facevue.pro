TEMPLATE = subdirs

#first compile the library
SUBDIRS = FaceVueAPI 

#then compile the executable
CONFIG += ordered
SUBDIRS += FaceVueGUI
