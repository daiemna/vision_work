cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(face_recognition_doppelganger_list)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(OCV_DIR $ENV{OPENCV_DIR})
# message(STATUS "value in OCV : "${OCV_DIR})
if(${OCV_DIR})
	message(STATUS "unable to find opencv in"${OCV_DIR})
endif()

# set(OPENCV_INCLUDE_DIR $ENV{ROS_OPENCV_DIR})

#set(OPENCV_INCLUDE_DIRS "/opt/ros/hydro/include/opencv" "/opt/ros/hydro/include")
#set(OPENCV_LIBRARY_DIRS "/opt/ros/hydro/lib")
#set(OPENCV_LIBRARIES  "-lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab")

#include_directories(${OPENCV_INCLUDE_DIRS})
#link_directories(${OPENCV_LIBRARY_DIRS})

#file(GLOB_RECURSE SOURCES "src/*.cpp")
#file(GLOB_RECURSE HEADERS "src/*.h")

#set (INCLUDE_DIR "")
#foreach (_headerFile ${HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND INCLUDE_DIR ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES INCLUDE_DIR)

# include_directories(${INCLUDE_DIR})
# add_executable (Main ${SOURCES})
# target_link_libraries (Main ${OPENCV_LIBRARIES})
