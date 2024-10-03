cmake_minimum_required(VERSION 3.16)
project(StellwerkQt VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
## set for debug
#set(CMAKE_CXX_FLAGS "-g")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Svg Widgets SvgWidgets)

qt_standard_project_setup()

qt_add_executable(StellwerkQt MANUAL_FINALIZATION
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
  Configuration.cpp Configuration.h
  StellwerkQPushButton.cpp StellwerkQPushButton.h
  HSignalQPushButton.cpp HSignalQPushButton.h
  WSignalQPushButton.cpp WSignalQPushButton.h
  )


# for cmake version > 3.19 (s. https://doc.qt.io/qt-6//qt-set-finalizer-mode.html)
# otherwise warining like this:
#    CMake Warning at /home/daniel/raspi/qt-raspi/lib/cmake/Qt6Core/Qt6CoreMacros.cmake:2869 (message):
#    Deployment of plugins for target 'StellwerkQt' will not work.  Either,
#    upgrade CMake to version 3.19 or newer, or call
#    qt_finalize_target(StellwerkQt) after generating the deployment script. 
#
qt_set_finalizer_mode(StellwerkQt ENABLE MODES static_plugins)
qt_finalize_target(StellwerkQt)

# target_include_directories, but here only the wiringPi lib file:
target_sources(StellwerkQt PRIVATE
  /home/daniel/raspi/sysroot/usr/lib/libwiringPi.so.2.70
  )

target_compile_definitions(StellwerkQt PRIVATE
  QT_DEPRECATED_WARNINGS
  )

target_link_libraries(StellwerkQt PRIVATE
  :libwiringPi.so.2.70
  Qt::Core
  Qt::Gui
  Qt::Widgets
  Qt::Svg
  Qt::SvgWidgets
  )


# Resources:
set(resources_resource_files
  "resources/HS.svg"
  "resources/VS.svg"
  "resources/WS.svg"
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
  RUNTIME DESTINATION ${HOME}/Documents/StellwerkQt/
  )

qt_generate_deploy_app_script(
  TARGET StellwerkQt
  FILENAME_VARIABLE deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
  )
install(SCRIPT ${deploy_script})
