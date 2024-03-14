TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -DDEBUG

SOURCES += \
        bytetools.cpp \
        chacha20.cpp \
        dhcpmessage.cpp \
        main.cpp \
        micromessage.cpp \
        udpsender.cpp

HEADERS += \
    bytetools.h \
    chacha20.h \
    dhcpmessage.h \
    micromessage.h \
    udpsender.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libgcrypt
