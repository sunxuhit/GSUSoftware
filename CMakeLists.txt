# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(BeamTest_mRICH)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# include Pixel Map
include_directories("${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap")
add_subdirectory(src/BeamTest/PixelMap)
# set(PixelMap_Libs PixelMap)

# add the executable
add_executable(BeamTest_mRICH
  ${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.cxx)
target_link_libraries(BeamTest_mRICH PixelMap ${ROOT_LIBRARIES})

# add the install targets
install (TARGETS BeamTest_mRICH DESTINATION bin)
