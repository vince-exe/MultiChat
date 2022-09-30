QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ban_list_dialog.cpp \
    black_word_dialog_helper.cpp \
    black_words_dialog.cpp \
    chat_utilities.cpp \
    client.cpp \
    client_side_dialog.cpp \
    client_stats_manager.cpp \
    info_user_dialog.cpp \
    ip_port_dialog.cpp \
    main.cpp \
    main_dialog.cpp \
    mute_list.cpp \
    nickname_dialog.cpp \
    options_server_dialog.cpp \
    server.cpp \
    server_side_dialog.cpp

HEADERS += \
    ban_list_dialog.h \
    black_word_dialog_helper.h \
    black_words_dialog.h \
    chat_utilities.h \
    client.h \
    client_side_dialog.h \
    client_stats_manager.h \
    info_user_dialog.h \
    ip_port_dialog.h \
    main_dialog.h \
    mute_list.h \
    nickname_dialog.h \
    options_server_dialog.h \
    server.h \
    server_side_dialog.h

FORMS += \
    ban_list_dialog.ui \
    black_word_dialog_helper.ui \
    black_words_dialog.ui \
    client_side_dialog.ui \
    info_user_dialog.ui \
    ip_port_dialog.ui \
    main_dialog.ui \
    mute_list.ui \
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
