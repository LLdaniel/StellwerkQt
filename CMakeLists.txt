cmake_minimum_required(VERSION 3.16)
project(StellwerkQt VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Svg)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(StellwerkQt MACOSX_BUNDLE
    BU.cpp BU.h
    Block.cpp Block.h
    Connector.cpp Connector.h
    HSignal.cpp HSignal.h
    Spmemory.cpp Spmemory.h
    Stellwerkstechnik.cpp Stellwerkstechnik.h
    VSignal.h
    Vsignal.cpp
    WSignal.cpp WSignal.h
    Weiche.cpp Weiche.h
    clickmanager.cpp clickmanager.h
    filemanager.cpp filemanager.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    worker.cpp worker.h
)
target_include_directories(StellwerkQt PRIVATE
    /home/daniel/Documents/StellwerkQt/WiringPi/wiringPi
)

target_compile_definitions(StellwerkQt PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(StellwerkQt PRIVATE
    # Remove: L"/home/daniel/Documents/StellwerkQt/WiringPi/wiringPi"
    :libwiringPi.so.2.70
    Qt::Core
    Qt::Gui
    Qt::Svg
)


# Resources:
set(resources_resource_files
    "resources/HS.svg"
    "resources/HSVS.svg"
    "resources/HSVSfahrtfahrt.svg"
    "resources/HSVSfahrthalt.svg"
    "resources/HSVShalthalt.svg"
    "resources/HSfahrt.svg"
    "resources/HShalt.svg"
    "resources/HSrangier.svg"
    "resources/VS.svg"
    "resources/VSfahrt.svg"
    "resources/VShalt.svg"
    "resources/WS.svg"
    "resources/WSfahrt.svg"
    "resources/WShalt.svg"
)

qt_add_resources(StellwerkQt "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(StellwerkQt PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS StellwerkQt
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${HOME}/Documents/
)

qt_generate_deploy_app_script(
    TARGET StellwerkQt
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
qt6_finalize_target(StellwerkQt)
