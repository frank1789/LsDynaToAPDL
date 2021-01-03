#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T14:53:49
#
#-------------------------------------------------

QT       += core \
            gui \
            concurrent

CONFIG += c++17

QMAKE_CXXFLAGS += \
    -O2 \
-c \
-Wall \
-Wextra \
-fPIC \
-Wredundant-decls \
-Wcast-align \
-Wmissing-declarations \
-Wmissing-include-dirs \
-Winvalid-pch \
-Wredundant-decls \
-Wmissing-format-attribute \
-Wformat-nonliteral \
-fstack-protector-strong \
-fexceptions


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LsDynaToAPDL
TEMPLATE = app


# GET VERSION FROM GIT
#PWD = /Users/francescoargentieri/LsDynaToAPDL
#GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --#tags)
#GIT_BUILD = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always)
##QMAKE_CXXFLAGS += -DVERSION=\\\"$$GIT_VERSION\\\"
#DEFINES +=  BUILD=\\\"$$GIT_BUILD\\\"
#message("VERSION:" $$GIT_VERSION", BUILD:" $$GIT_BUILD)


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    element.cc \
        main.cpp \
        mainwindow.cpp \
    reader.cpp \
    about.cpp \
    finitelement.cpp \
    node.cpp \
    shell.cpp \
    convertersintax.cpp \
    writeapdl.cpp \
    dialog.cpp \
    managefile.cpp

HEADERS += \
    element.h \
    finite_element_types.h \
        mainwindow.h \
    node_type_impl.h \
    reader.h \
    about.h \
    finitelement.h \
    node.h \
    shell.h \
    convertersintax.h \
    writeapdl.h \
    dialog.h \
    managefile.h

FORMS += \
        mainwindow.ui \
    about.ui \
    dialog.ui

RESOURCES += \ resources.qrc
