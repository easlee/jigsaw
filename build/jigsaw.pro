#-------------------------------------------------
#
# Project created by QtCreator 2017-09-27T19:13:41
#
#-------------------------------------------------

QT       += core gui widgets uitools

TARGET = jigsaw
TEMPLATE = app
CONFIG += c++11

DESTDIR = ../_bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../inc

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/jigsaw/composite/app.cpp \
    ../src/jigsaw/composite/CompositeAreaInput.cpp \
    ../src/jigsaw/composite/CompositeCombobox.cpp \
    ../src/jigsaw/composite/CompositeDrawer.cpp \
    ../src/jigsaw/composite/CompositeFlow.cpp \
    ../src/jigsaw/composite/CompositeFollowBar.cpp \
    ../src/jigsaw/composite/CompositeFrame.cpp \
    ../src/jigsaw/composite/CompositeIconButton.cpp \
    ../src/jigsaw/composite/CompositeIconRadio.cpp \
    ../src/jigsaw/composite/CompositeImageButton.cpp \
    ../src/jigsaw/composite/CompositeLineInput.cpp \
    ../src/jigsaw/composite/CompositeMask.cpp \
    ../src/jigsaw/composite/CompositePage.cpp \
    ../src/jigsaw/composite/CompositePalette.cpp \
    ../src/jigsaw/composite/CompositePane.cpp \
    ../src/jigsaw/composite/CompositePicture.cpp \
    ../src/jigsaw/composite/CompositeScroll.cpp \
    ../src/jigsaw/composite/CompositeStack.cpp \
    ../src/jigsaw/composite/CompositeText.cpp \
    ../src/jigsaw/composite/CompositeTextButton.cpp \
    ../src/jigsaw/core/ColorPalette.cpp \
    ../src/jigsaw/core/Composite.cpp \
    ../src/jigsaw/core/MemDB.cpp \
    ../src/jigsaw/core/NotifyCenter.cpp \
    ../src/jigsaw/core/Translator.cpp \
    ../main/main.cpp

HEADERS += \
    ../inc/jigsaw/composite/app.h \
    ../inc/jigsaw/composite/CompositeAreaInput.h \
    ../inc/jigsaw/composite/CompositeCombobox.h \
    ../inc/jigsaw/composite/CompositeDrawer.h \
    ../inc/jigsaw/composite/CompositeFlow.h \
    ../inc/jigsaw/composite/CompositeFollowBar.h \
    ../inc/jigsaw/composite/CompositeFrame.h \
    ../inc/jigsaw/composite/CompositeIconButton.h \
    ../inc/jigsaw/composite/CompositeIconRadio.h \
    ../inc/jigsaw/composite/CompositeImageButton.h \
    ../inc/jigsaw/composite/CompositeLineInput.h \
    ../inc/jigsaw/composite/CompositeMask.h \
    ../inc/jigsaw/composite/CompositePage.h \
    ../inc/jigsaw/composite/CompositePalette.h \
    ../inc/jigsaw/composite/CompositePane.h \
    ../inc/jigsaw/composite/CompositePicture.h \
    ../inc/jigsaw/composite/CompositeScroll.h \
    ../inc/jigsaw/composite/CompositeStack.h \
    ../inc/jigsaw/composite/CompositeText.h \
    ../inc/jigsaw/composite/CompositeTextButton.h \
    ../inc/jigsaw/core/ColorPalette.h \
    ../inc/jigsaw/core/Composite.h \
    ../inc/jigsaw/core/Dispatcher.hxx \
    ../inc/jigsaw/core/MemDB.h \
    ../inc/jigsaw/core/NotifyCenter.h \
    ../inc/jigsaw/core/Translator.h

RESOURCES += \
    ../res/jigsaw.qrc

