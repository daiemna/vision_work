cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(face_recognition)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(OPENCV_INCLUDE_DIRS "/opt/ros/hydro/include/opencv" "/opt/ros/hydro/include")
set(OPENCV_LIBRARY_DIRS "/opt/ros/hydro/lib")
set(OPENCV_LIBRARIES  "-lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab")

# set(CVPLOT_INCLUDE_DIRS "/home/dna/CV_workspace/cvplot_130223/src")
# set(CVPOLT_LIBRARY_DIRS "/home/dna/CV_workspace/cvplot_130223/lib")

include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})

#include_directories(${CVPLOT_INCLUDE_DIRS})
#link_directories(${CVPOLT_LIBRARY_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

set (INCLUDE_DIR "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIR ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIR)

include_directories(${INCLUDE_DIR})
add_executable (Main ${SOURCES})
target_link_libraries (Main ${OPENCV_LIBRARIES})
