cmake_minimum_required(VERSION 3.7)
project(VenueShotClient)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
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
        Client/Api/Endpoint.cpp
        Client/Api/AcquireToken.cpp
        Client/Api/GetVenue.cpp
        Client/Model/Venue.cpp
)
set(VSC_GUI
        Client/Widget/MainWindow.cpp
        Client/Widget/LoginWidget.cpp
        Client/Widget/SelectVenueWidget.cpp
)

set(VSC vsc.cpp)
add_executable(vsc ${QSLOG} ${VSC_CORE} ${VSC_GUI} ${VSC})
target_link_libraries(vsc Qt5::Widgets Qt5::Network)
