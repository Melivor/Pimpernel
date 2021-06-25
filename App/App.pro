QT += quick \
    widgets \

RC_ICONS = icon.ico
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        wallpapergenerator.cpp \
        parametricequation.cpp \
        parametricequationshapegenerator.cpp \
        #conformaldiskrepresentation.cpp \
        #curvelinegenerator.cpp \
        #networkpainter.cpp \
        #patternpainter.cpp \
        #polygongenerator.cpp \
        #polygonpainter2.cpp \
        #raingenerator.cpp \


RESOURCES += qml.qrc \
             $$PWD/../HorusStyle/horusStyle.qrc \
             $$PWD/../ItemView/itemView.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/../HorusStyle/import/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =


#INCLUDEPATH += ../StandardItemModel
#win32:CONFIG(release, debug|release): LIBS += -L../StandardItemModel/release -lStandardItemModel
#win32:CONFIG(debug, debug|release): LIBS += -L../StandardItemModel/debug -lStandardItemModel

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    wallpapergenerator.h\
    parametricequation.h \
    parametricequationshapegenerator.h \

    #ConformalDiskModel.h \
    #conformaldiskrepresentation.h \
    #curvelinegenerator.h \
    #networkpainter.h \
    #patternpainter.h \
    #polygongenerator.h \
    #polygonpainter2.h \
    #raingenerator.h \




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../muparser/release/ -lmuparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../muparser/debug/ -lmuparser

INCLUDEPATH += $$PWD/../muparser
DEPENDPATH += $$PWD/../muparser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../muparser/release/libmuparser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../muparser/debug/libmuparser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../muparser/release/muparser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../muparser/debug/muparser.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../StandardItemModel/release/ -lStandardItemModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../StandardItemModel/debug/ -lStandardItemModel
else:unix: LIBS += -L$$OUT_PWD/../StandardItemModel/ -lStandardItemModel

INCLUDEPATH += $$PWD/../StandardItemModel
DEPENDPATH += $$PWD/../StandardItemModel

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../StandardItemModel/release/libStandardItemModel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../StandardItemModel/debug/libStandardItemModel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../StandardItemModel/release/StandardItemModel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../StandardItemModel/debug/StandardItemModel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../StandardItemModel/libStandardItemModel.a
