# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = hello-coap

CONFIG += sailfishapp c++11
QT += multimedia

HEADERS += src/demomodel.h \
           src/hound_driver.h \
           src/recorder.h

SOURCES += src/hello-coap.cpp \
           src/demomodel.cpp \
           src/recorder.cpp \
           src/hound_driver.cpp

OTHER_FILES += qml/hello-coap.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/ThirdPage.qml \
    rpm/hello-coap.changes.in \
    rpm/hello-coap.spec \
    rpm/hello-coap.yaml \
    translations/*.ts \
    hello-coap.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
# CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
# TRANSLATIONS += translations/hello-coap-de.ts

include(src/qtcoap/coap/coap.pri)
