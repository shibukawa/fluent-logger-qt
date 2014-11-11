INCLUDEPATH += $$PWD/include

QT += network

HEADERS += \
    $$PWD/include/qfluent/qfluentsender.h \
    $$PWD/include/qfluent/qfluentsender_p.h \
    $$PWD/include/qfluent/qfluentlogger.h

SOURCES += \
    $$PWD/src/qfluent/qfluentsender.cpp \
    $$PWD/src/qfluent/qfluentsender_p.cpp \
    $$PWD/src/qfluent/qfluentlogger.cpp
