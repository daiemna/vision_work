CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2 FATAL_ERROR)
SET(FindPkg_DIR "/home/dna/cmake-3.2.3-Linux-x86_64/share/cmake-3.2/Modules/")
FIND_PACKAGE(FindPkg REQUIRED)

PROJECT(pedestrian_detector)

LIST( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -pthread -g -ftest-coverage -fprofile-arcs")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
# SET(FEATURES "/home/dna/eclipse_workspace/RnD-Pedestrian-Detection/Features/")
# SET(CLASSIFIER "/home/dna/eclipse_workspace/RnD-Pedestrian-Detection/Classifier/")

# INCLUDE_DIRECTORIES("include")
# INCLUDE_DIRECTORIES(${FEATURES}include/)
# SET(FEATURE_SOURCES ${FEATURES}src/)

# SET(OPENCV_PATH $ENV{OPENCV_DIR})
SET(OPENCV_LIBRARY  "-lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core -lopencv_hal")

# INCLUDE_DIRECTORIES(${CLASSIFIER}include/)
# SET(CLASSIFIER_SOURCES ${CLASSIFIER}src/)

pkg_check_modules(OPENCV opencv3)
IF(NOT OPENCV_FOUND)
	MESSAGE(FATAL_ERROR "OPENCV3 does not exsist")
	exit()
ELSE()
	MESSAGE("OPENCV_LIBRARIES : ${OPENCV_LIBRARIES}")	
	MESSAGE("OPENCV_LIBRARY_DIRS : ${OPENCV_LIBRARY_DIRS}")	
	MESSAGE("OPENCV_LDFLAGS : ${OPENCV_LDFLAGS}")	
	MESSAGE("OPENCV_LDFLAGS_OTHER : ${OPENCV_LDFLAGS_OTHER}")	
	MESSAGE("OPENCV_INCLUDE_DIRS : ${OPENCV_INCLUDE_DIRS}")	
	MESSAGE("OPENCV_CFLAGS : ${OPENCV_CFLAGS}")	
	MESSAGE("OPENCV_CFLAGS_OTHER : ${OPENCV_CFLAGS_OTHER}")	
ENDIF()

SET(SOURCES 
	${SOURCES} )
	# ${CLASSIFIER_SOURCES}AbstractClassifier.cpp 
	# ${CLASSIFIER_SOURCES}CascadedClassifier.cpp
	# ${FEATURE_SOURCES}FeatureEvaluator.cpp
	# ${FEATURE_SOURCES}hog_features.cpp
	# ${FEATURE_SOURCES}hog_lite_features.cpp)

INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIR})

ADD_LIBRARY(_${PROJECT_NAME} SHARED ${SOURCES})
ADD_EXECUTABLE(Main ${SOURCES})
TARGET_LINK_LIBRARIES(Main _${PROJECT_NAME} ${OPENCV_LIBRARIES})