QT       += core gui

TARGET = facevue 
TEMPLATE = app


SOURCES += main.cpp\
    Core/ProcessThread.cpp \   
    Core/ProcessingMode.cpp\
    Core/RegistrationMode.cpp\
    Core/RecognitionMode.cpp\
    GUI/addimage.cpp \
    GUI/facevue2.cpp

HEADERS  += \
    Core/ProcessThread.h \   
    Core/ProcessingMode.h\
    Core/RegistrationMode.h\
    Core/RecognitionMode.h\
    GUI/addimage.h \
    GUI/facevue2.h\
    FaceVueApplication.h

FORMS    += \
    FormFiles/facevue.ui

RESOURCES += \
     FormFiles/facevue.qrc

INCLUDEPATH +=../FaceVueAPI

# unix-based
unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv
   #QMAKE_CXXFLAGS += -fpermissive
   DEFINES += "FACEVUE_ETC_DIR=/etc/facevue/"
}

# include the facevue lib
LIBS += -L../FaceVueAPI\
	-lFaceVue

# windows-based
win32 {
   INCLUDEPATH +=C:\API\opencv-build\install\include
   #INCLUDEPATH +=C:\Users\Ishmael\Desktop\Projects\FaceVue_api
   LIBS +=$$PWD\opencv_dlls\libopencv_calib3d241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_contrib241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_features2d241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_flann241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_gpu241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_legacy241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_ml241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_nonfree241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_objdetect241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_photo241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_ts241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_video241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_stitching241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_videostab241.dll.a
   #LIBS +=$$PWD\Face_api\libFaceVue.a
   #LIBS +=C:\Users\Ishmael\Desktop\Projects\FaceVue_api\release\libFaceVue.a
   LIBS +=$$PWD\opencv_dlls\libopencv_highgui241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_imgproc241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_calib3d241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_video241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_core241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_imgproc241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_video241.a
   LIBS +=$$PWD\opencv_dlls\liblibjasper.a
   LIBS +=$$PWD\opencv_dlls\liblibtiff.a
   LIBS +=$$PWD\opencv_dlls\libzlib.a
   LIBS +=$$PWD\opencv_dlls\liblibpng.a
   LIBS +=$$PWD\opencv_dlls\liblibjpeg.a
   LIBS +=$$PWD\opencv_dlls\libopencv_contrib241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_features2d241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_flann241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_gpu241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_highgui241.dll.a
   LIBS +=$$PWD\opencv_dlls\libopencv_legacy241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_ml241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_nonfree241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_objdetect241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_photo241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_ts241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_stitching241.a
   LIBS +=$$PWD\opencv_dlls\libopencv_videostab241.a
   LIBS +=$$PWD\opencv_dlls\libvfw32.a
   LIBS +=$$PWD\opencv_dlls\libcomctl32.a
   LIBS +=$$PWD\opencv_dlls\libavcodec.a
   LIBS +=$$PWD\opencv_dlls\libopencv_perf_highgui_pch_dephelp.a
   LIBS +=$$PWD\opencv_dlls\libavformat.a
   LIBS +=$$PWD\opencv_dlls\libavcore64.a
   LIBS +=$$PWD\opencv_dlls\libole32.a
   LIBS +=$$PWD\opencv_dlls\libavifil32.a
   LIBS +=$$PWD\opencv_dlls\libuuid.a
   LIBS +=$$PWD\opencv_dlls\liboleaut32.a
   LIBS +=$$PWD\opencv_dlls\libolepro32.a
}
