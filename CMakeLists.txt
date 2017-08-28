##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.3)
project(vss_sdk)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS 
	qt_build 
	roscpp
	rospy
 	std_msgs
 	message_generation
 	message_runtime
)

## System dependencies are found with CMake's conventions
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(sqlite3 REQUIRED)

## Generate messages in the 'msg' folder
add_message_files(
   FILES
   c_robot_command.msg
   d_path.msg
   d_pose.msg
   global_commands.msg
   global_debug.msg
   global_state.msg
   s_ball_state.msg
   s_pose.msg
   s_rgb.msg
   s_robot_state.msg
 )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

include_directories(
 include
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${SQLITE3_INCLUDE_DIRS}
)
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES vss_sdk
  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/vss_sdk/*.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(vss_sdk ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(vss_sdk ${QT_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS vss_sdk RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#interface
add_library(interface src/Interface/interface.cpp)
target_link_libraries(interface ${catkin_LIBRARIES})
add_dependencies(interface ${PROJECT_NAME}_generate_messages_cpp)

#vision
add_library(common src/Vision/common.cpp)
target_link_libraries(common ${catkin_LIBRARIES})
target_link_libraries(common ${QT_LIBRARIES})
target_link_libraries(common interface)
target_link_libraries(common ${OpenCV_LIBRARIES})

add_library(qtopencv src/Vision/QtOpenCV.cpp)
target_link_libraries(qtopencv ${catkin_LIBRARIES})
target_link_libraries(qtopencv ${QT_LIBRARIES})
target_link_libraries(qtopencv ${OpenCV_LIBRARIES})

add_library(calibration src/Vision/calibration.cpp)
target_link_libraries(calibration ${catkin_LIBRARIES})
target_link_libraries(calibration ${QT_LIBRARIES})
target_link_libraries(calibration ${OpenCV_LIBRARIES})
target_link_libraries(calibration qtopncv)
target_link_libraries(calibration common)


add_library(sqlite src/Vision/sqlite.cpp)
target_link_libraries(sqlite ${SQLITE3_LIBRARIES})
target_link_libraries(sqlite ${QT_LIBRARIES})
target_link_libraries(sqlite common)

add_library(qcustomlabel src/Vision/qcustomlabel.cpp)
target_link_libraries(qcustomlabel ${QT_LIBRARIES})
target_link_libraries(qcustomlabel ${catkin_LIBRARIES})

add_library(vision src/Vision/vision.cpp)
target_link_libraries(vision ${QT_LIBRARIES})
target_link_libraries(vision ${catkin_LIBRARIES})
target_link_libraries(vision common)
target_link_libraries(vision qtopencv)
target_link_libraries(vision qcustomlabel)

add_library(mainwindow src/Vision/mainwindow.cpp)
target_link_libraries(mainwindow ${QT_LIBRARIES})
target_link_libraries(mainwindow ${catkin_LIBRARIES})
target_link_libraries(mainwindow common)
target_link_libraries(mainwindow calibration)
target_link_libraries(mainwindow vision)
target_link_libraries(mainwindow sqlite)
target_link_libraries(mainwindow qtopencv)

add_executable(main src/Vision/main.cpp)
target_link_libraries(main ${QT_LIBRARIES})
target_link_libraries(main ${catkin_LIBRARIES})
target_link_libraries(main mainwindow)
target_link_libraries(main calibration)
target_link_libraries(main common)

