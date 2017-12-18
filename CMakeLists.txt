cmake_minimum_required(VERSION 3.7)
project(VenueShotClient)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Network)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
#find_package(Qt5SerialPort)
#find_package(Qt5Sql)
#find_package(Qt5Qml)
#find_package(Qt5Quick)
#find_package(Qt5Charts)

set(QSLOG
        QsLog/QsLog.cpp
        QsLog/QsLogDest.cpp
        QsLog/QsLogDestConsole.cpp
        QsLog/QsLogDestFile.cpp
        QsLog/QsLogDestFunctor.cpp
)

set(VSC_CORE
        Client/Client.cpp
        Client/JwtToken.cpp
        Client/Settings.cpp
        Client/Api/BasicEndpoint.cpp
        Client/Api/Endpoint.cpp
        Client/Api/AcquireToken.cpp
        Client/Api/GetVenue.cpp
        Client/Api/GetVenuePictures.cpp
        Client/Api/PostPicture.cpp
        Client/Api/LinkPicture.cpp
        Client/Model/BasicEntity.cpp
        Client/Model/Venue.cpp
        Client/Model/VenuePicture.cpp
        Client/Model/VenuePictures.cpp
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

set(VSC vsc.cpp)
add_executable(vsc ${QSLOG} ${VSC_CORE} ${VSC_GUI} ${VSC})
target_link_libraries(vsc Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Multimedia Qt5::MultimediaWidgets)

set(VSC_DISPLAY display.cpp Client/Widget/SlideShowWidget.cpp)
add_executable(display ${QSLOG} ${VSC_CORE} ${VSC_DISPLAY})
target_link_libraries(display Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Multimedia Qt5::MultimediaWidgets)
