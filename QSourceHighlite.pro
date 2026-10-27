QT       += core gui

include(QSourceHighlite.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# ===== AddressSanitizer для Debug =====
# CONFIG(debug, debug|release) {
#     message("Building with AddressSanitizer")

#     # Для GCC/Clang
#     QMAKE_CXXFLAGS += -fsanitize=address
#     QMAKE_CXXFLAGS += -fno-omit-frame-pointer
#     QMAKE_CXXFLAGS += -g

#     # Флаги линковщика
#     QMAKE_LFLAGS += -fsanitize=address

#     # Также для UBSan (undefined behavior)
#     QMAKE_CXXFLAGS += -fsanitize=undefined
#     QMAKE_LFLAGS += -fsanitize=undefined
# }

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scriptrunner.cpp \
    searchdialog.cpp \
    themedialog.cpp

HEADERS += \
    mainwindow.h \
    scriptrunner.h \
    searchdialog.h \
    themedialog.h

FORMS += \
    mainwindow.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
