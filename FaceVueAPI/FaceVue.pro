QT       -= gui

TARGET = FaceVue
TEMPLATE = lib
CONFIG += staticlib

SOURCES += facevue.cpp \
    flandmark_detector.cpp \
    cascadedetect.cpp \
    faceRecognition.cpp

HEADERS += facevue.h \
    flandmark_detector.h \
    cascadedetect.hpp \
    faceRecognition.h

# Unix-based systems 
unix {
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}


# windows-based 
win32  {
    INCLUDEPATH += \
    "C:\API\opencv-build\install\include" \

    LIBS += \
    "C:\API\lib\libopencv_highgui241.a" \
    "C:\API\lib\libopencv_imgproc241.a" \
    "C:\API\lib\libopencv_calib3d241.a" \
    "C:\API\lib\libopencv_video241.a" \
    "C:\API\lib\libopencv_core241.a" \
    "C:\API\lib\libopencv_imgproc241.a" \
    "C:\API\lib\libopencv_calib3d241.a" \
    "C:\API\lib\libopencv_video241.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibjasper.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibtiff.a" \
    "C:\API\opencv-build\3rdparty\lib\libzlib.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibpng.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibjpeg.a" \
    "C:\API\lib\libopencv_contrib241.a" \
    "C:\API\lib\libopencv_features2d241.a" \
    "C:\API\lib\libopencv_flann241.a" \
    "C:\API\lib\libopencv_gpu241.a" \
    "C:\API\opencv-build\lib\libopencv_highgui241.dll.a" \
    "C:\API\lib\libopencv_legacy241.a" \
    "C:\API\lib\libopencv_ml241.a" \
    "C:\API\lib\libopencv_nonfree241.a" \
    "C:\API\lib\libopencv_objdetect241.a" \
    "C:\API\lib\libopencv_photo241.a" \
    "C:\API\lib\libopencv_ts241.a" \
    "C:\API\lib\libopencv_stitching241.a" \
    "C:\API\lib\libopencv_videostab241.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibjpeg.a" \
    "C:\API\opencv-build\lib\libopencv_perf_highgui_pch_dephelp.a"\ #// my_version
    "C:\API\opencv-build\3rdparty\lib\liblibpng.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibtiff.a" \
    "C:\API\opencv-build\3rdparty\lib\liblibjasper.a" \
    "C:\API\opencv-build\3rdparty\lib\libzlib.a" \
    "C:\Qt\2010.04\mingw\lib\libvfw32.a" \
    "C:\Qt\2010.04\mingw\lib\libcomctl32.a"\
    "C:\API\opencv2.4.1\opencv\3rdparty\lib\libavcodec.a" \
    "C:\API\opencv2.4.1\opencv\3rdparty\lib\libavformat.a" \
    "C:\API\opencv2.4.1\opencv\3rdparty\lib\libavcore64.a"\
    "C:\Qt\2010.04\mingw\lib\libole32.a"\
    "C:\Qt\2010.04\mingw\lib\libavifil32.a" \
    "C:\Qt\2010.04\mingw\lib\libuuid.a" \
    "C:\Qt\2010.04\mingw\lib\liboleaut32.a"\
    "C:\Qt\2010.04\mingw\lib\libolepro32.a"
    }
