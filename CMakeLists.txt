# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(BeamTest_mRICH)
set(CMAKE_INSTALL_PREFIX $ENV{HOME}/WorkSpace/EICPID/BeamTest_mRICH)

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# include Pixel Map in BeamTest
include_directories("${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap")
add_subdirectory(src/BeamTest/PixelMap)

# include Beam Finder in BeamTest
include_directories("${PROJECT_SOURCE_DIR}/src/BeamTest/BeamFinder")
add_subdirectory(src/BeamTest/BeamFinder)

# include Ring Finder in BeamTest
include_directories("${PROJECT_SOURCE_DIR}/src/BeamTest/RingFinder")
add_subdirectory(src/BeamTest/RingFinder)

# include Calibration in BeamTest
include_directories("${PROJECT_SOURCE_DIR}/src/BeamTest/Calibration")
add_subdirectory(src/BeamTest/Calibration)

# include material in Simulation
include_directories("${PROJECT_SOURCE_DIR}/src/Simulation/Material")
add_subdirectory(src/Simulation/Material)

# include Ring Finder in Simulation
include_directories("${PROJECT_SOURCE_DIR}/src/Simulation/RingFinder")
add_subdirectory(src/Simulation/RingFinder)

# include gemc Calibration in Simulation
include_directories("${PROJECT_SOURCE_DIR}/src/Simulation/Calibration")
add_subdirectory(src/Simulation/Calibration)

# add the executable
# add_executable(BeamTest_mRICH ${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.cxx)
# target_link_libraries(BeamTest_mRICH PixelMap)

# add the install targets
# install (TARGETS BeamTest_mRICH DESTINATION bin)
