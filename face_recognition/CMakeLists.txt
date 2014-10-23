cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(DisplayImage)

set(OPENCV_INCLUDE_DIRS "/opt/ros/hydro/include/opencv" "/opt/ros/hydro/include")
set(OPENCV_LIBRARY_DIRS "/opt/ros/hydro/lib")
set(OPENCV_LIBRARIES  "-lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab")

include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})
# add_definitions(${OPENCV_DEFINITIONS})

add_executable (DisplayImage src/DisplayImage.cpp)
target_link_libraries (DisplayImage ${OPENCV_LIBRARIES})

