QT       += core gui
QT       += charts
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ai_linking.cpp \
    creatnewfile.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    markdownhighlighter.cpp \
    showfile.cpp \
    showfileedit.cpp \
    showmyachievement.cpp \
    showreviewstatistics.cpp \
    thememanager.cpp

HEADERS += \
    ai_linking.h \
    creatnewfile.h \
    login.h \
    mainwindow.h \
    markdownhighlighter.h \
    showfile.h \
    showfileedit.h \
    showmyachievement.h \
    showreviewstatistics.h \
    thememanager.h

FORMS += \
    ai_linking.ui \
    creatnewfile.ui \
    login.ui \
    mainwindow.ui \
    showfile.ui \
    showfileedit.ui \
    showmyachievement.ui \
    showreviewstatistics.ui

CONFIG += resources_big

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
