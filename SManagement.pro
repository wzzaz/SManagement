QT += quick sql widgets quickcontrols2
CONFIG += c++11

RC_ICONS = $$PWD/res/manager.ico
RC_FILE = icon.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    data/DBHelper.cpp \
    data/ConnectionPool.cpp \
    model/ScheduleModel.cpp \
    model/SubScheduleModel.cpp \
    model/StageModel.cpp \
    ScheduleManager.cpp \
    Utils.cpp \
    CalendarListManager.cpp \
    CommonManager.cpp

RESOURCES += icon.qrc \
    font.qrc \
    js.qrc \
    management.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    data/DBHelper.h \
    data/ConnectionPool.h \
    ScheduleManager.h \
    model/ScheduleModel.h \
    model/SubScheduleModel.h \
    model/StageModel.h \
    Utils.h \
    CalendarListManager.h \
    CommonManager.h

# mutex DEFINE
DEFINES += MYSQL_DATA       # [-]
#DEFINES += SQLITE_DATA      # [-]

DISTFILES += \
    icon.rc
