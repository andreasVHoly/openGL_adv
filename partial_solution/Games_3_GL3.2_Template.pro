HEADERS       = glheaders.h \
                glwidget.h \
                model.h

SOURCES       = glwidget.cpp \
                main.cpp \
                model.cpp
QT += core gui opengl widgets
LIBS += -lGLEW
RESOURCES += resources.qrc
INCLUDEPATH += glm
QMAKE_CXXFLAGS += --std=c++11
#QMAKE_CXXFLAGS += -DSHOULD_LOAD_SHADERS
# install
target.path = boom
INSTALLS += target
