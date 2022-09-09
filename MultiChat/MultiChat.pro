QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_utilities.cpp \
    client.cpp \
    client_side_dialog.cpp \
    ip_port_dialog.cpp \
    main.cpp \
    main_dialog.cpp \
    nickname_dialog.cpp \
    options_server_dialog.cpp \
    server.cpp \
    server_side_dialog.cpp

HEADERS += \
    chat_utilities.h \
    client.h \
    client_side_dialog.h \
    ip_port_dialog.h \
    main_dialog.h \
    nickname_dialog.h \
    options_server_dialog.h \
    server.h \
    server_side_dialog.h

FORMS += \
    client_side_dialog.ui \
    ip_port_dialog.ui \
    main_dialog.ui \
    nickname_dialog.ui \
    options_server_dialog.ui \
    server_side_dialog.ui

INCLUDEPATH += D:/Programs/C++Libraries/boost_1_80_0
LIBS += -LD:/Programs/C++Libraries/boost_1_80_0/stage/lib
LIBS += -llibboost_filesystem-mgw6-mt-x32-1_80 -lws2_32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
