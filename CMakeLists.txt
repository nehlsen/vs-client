cmake_minimum_required(VERSION 3.5)
project(VenueShotClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Gui Network Multimedia MultimediaWidgets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui Network Multimedia MultimediaWidgets Quick)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVSC_CLI_LOG=1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DVSC_FULLSCREEN=1")

set(QSLOG
        QsLog/QsLog.cpp
        QsLog/QsLogDest.cpp
        QsLog/QsLogDestConsole.cpp
        QsLog/QsLogDestFile.cpp
        QsLog/QsLogDestFunctor.cpp
)

set(VSC_CORE
        Client/Client.cpp
        Client/KratosSessionToken.cpp
        Client/Settings.cpp
        Client/Api/BasicEndpoint.cpp
        Client/Api/Endpoint.cpp
        Client/Api/GetVenue.cpp
        Client/Api/GetVenuePictures.cpp
        Client/Api/PostPicture.cpp
        Client/Api/LinkPicture.cpp
        Client/Api/KratosStartFlow.cpp
        Client/Api/KratosAuthenticate.cpp
        Client/Model/BasicEntity.cpp
        Client/Model/Venue.cpp
        Client/Model/VenuePicture.cpp
        Client/Model/VenuePictures.cpp
        Client/Model/VenuePicturesModel.cpp
        Client/Model/SlideShow.cpp
)
set(VSC_GUI
        Client/Widget/MainWindow.cpp
        Client/Widget/LoginWidget.cpp
        Client/Widget/SelectVenueWidget.cpp
        Client/Widget/VenueWidget.cpp
        Client/Widget/CapturePublishWidget.cpp
        Client/Widget/SlideShowWidget.cpp
)
set(VSC_QML
        Client/VenueShot.cpp
)

set(VSC_SRC ${QSLOG} ${VSC_CORE} ${VSC_GUI} ${VSC_QML})

qt_add_library(vscLib STATIC ${VSC_SRC})
target_link_libraries(vscLib Qt6::Widgets Qt6::Gui Qt6::Network Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Quick)

################################################################################

add_executable(vsc vsc.cpp)
target_link_libraries(vsc vscLib)

add_executable(display display.cpp)
target_link_libraries(display vscLib)

#add_executable(capture capture.cpp)
#target_link_libraries(capture vscLib)
#
#add_executable(qmlDisplayModel qmlDisplayModel.cpp)
#target_link_libraries(qmlDisplayModel vscLib)
#
#add_executable(qmlDisplayBridge qmlDisplayBridge.cpp)
#target_link_libraries(qmlDisplayBridge vscLib)
#
#add_executable(qmlCapture qmlCapture.cpp)
#target_link_libraries(qmlCapture vscLib)

#if(ANDROID)
#    add_library(qmlCapture SHARED qmlCapture.cpp)
#    target_link_libraries(qmlCapture vscLib)
#
#    include(android/qt-android-cmake/AddQtAndroidApk.cmake)
#    add_qt_android_apk(qmlCapture_apk qmlCapture)
#else()
#    add_executable(qmlCapture qmlCapture.cpp)
#    target_link_libraries(qmlCapture vscLib)
#endif()
