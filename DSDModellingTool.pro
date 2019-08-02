TEMPLATE = app
TARGET = DSDModellingTool

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
    /usr/local/include/eigen3 \
    libcmaes/src

HEADERS += \
    Headers/dashboardscene.h \
    Headers/domain.h \
    Headers/domainitem.h \
    Headers/identity.h \
    Headers/mainwindow.h \
    Headers/abstractnewdialog.h \
    Headers/newduplexdialog.h \
    Headers/newspeciesdialog.h \
    Headers/newstranddialog.h \
    Headers/species.h \
    Headers/strand.h \
    Headers/strandscene.h \
    Headers/strandsceneitem.h \
    Headers/strandsceneshape.h \
    Headers/springscene.h \
    Headers/dsdparser.h \
    libcmaes/src

SOURCES += \
    Sources/domainitem.cpp \
    Sources/dashboardscene.cpp \
    Sources/domain.cpp \
    Sources/dsdparser.cpp \
    Sources/identity.cpp \
    Sources/mainwindow.cpp \
    Sources/abstractnewdialog.cpp \
    Sources/newduplexdialog.cpp \
    Sources/newstranddialog.cpp \
    Sources/newspeciesdialog.cpp \
    Sources/species.cpp \
    Sources/springscene.cpp \
    Sources/strand.cpp \
    Sources/strandscene.cpp \
    Sources/strandsceneitem.cpp \
    Sources/strandsceneshape.cpp \
    main.cpp

RESOURCES += \
    files_to_parse.qrc \
    icons.qrc

INCLUDEPATH += $$PWD/libcmaes
DEPENDPATH += $$PWD/libcmaes

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libcmaes/lib/release/ -lcmaes.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libcmaes/lib/debug/ -lcmaes.0
else:unix: LIBS += -L$$PWD/libcmaes/lib/ -lcmaes.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libcmaes/lib/release/ -lgflags.2.2.2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libcmaes/lib/debug/ -lgflags.2.2.2
else:unix: LIBS += -L$$PWD/libcmaes/lib/ -lgflags.2.2.2

DISTFILES += \
    input.txt

win32:CONFIG(release, debug|release): QMAKE_CXXFLAGS += -std=c++11
