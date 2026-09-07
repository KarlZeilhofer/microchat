QT += core gui network widgets

lessThan(QT_MAJOR_VERSION, 6): error("Microchat requires Qt 6 or newer")

TARGET = microchat
TEMPLATE = app
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000


SOURCES += \
        main.cpp \
        microchat.cpp \
        qaesencryption.cpp

HEADERS += \
        microchat.h \
        qaesencryption.h

FORMS += \
        microchat.ui

DISTFILES += \
    README.md
