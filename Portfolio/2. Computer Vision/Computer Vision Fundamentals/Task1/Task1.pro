TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

#=====================OpenCV Includes=======================
INCLUDEPATH += C:\OpenCV45\release\install\include

LIBS += -LC:\OpenCV45\release\lib
LIBS +=    -lopencv_core450 \
    -lopencv_highgui450 \
    -lopencv_imgproc450 \
    -lopencv_features2d450 \
    -lopencv_calib3d450 \
    -lopencv_videoio450 \
    -lopencv_imgcodecs450 \
    -lopencv_face450 \
    -lopencv_objdetect450 \

#======================Dlib Includes========================
#QMAKE_CXXFLAGS_RELEASE += -mavx

#INCLUDEPATH+=C:\AINT308Repository\dlib19_19\include
#LIBS +=-L"C:\AINT308Repository\dlib19_19\lib"
#LIBS+=-ldlib
#LIBS += -lmingw32
#LIBS+=-lOle32
#LIBS+=-lOleaut32
#LIBS+=-lm
#LIBS+= -ldinput8
#LIBS+=-lcomctl32

#LIBS+=-ldxguid
#LIBS+= -ldxerr8
#LIBS+=-luser32
#LIBS+=-lgdi32
#LIBS+=-lwinmm
#LIBS+= -limm32
#LIBS+= -lole32
#LIBS+=-loleaut32
#LIBS+=-lshell32
#LIBS+= -lversion
#LIBS+= -luuid

#LIBS+=-lopengl32
#LIBS+=-lglu32
#LIBS+=-lgdi32
#LIBS+=-lwinmm
#LIBS += -lws2_32

#====================Project Includes======================
SOURCES += \
        main.cpp

HEADERS += \

