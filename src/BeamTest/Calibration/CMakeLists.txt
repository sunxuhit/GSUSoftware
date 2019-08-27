ROOT_GENERATE_DICTIONARY(G__Calibration
  ${CMAKE_CURRENT_SOURCE_DIR}/Calibration.h 
  ${CMAKE_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.h
  ${CMAKE_SOURCE_DIR}/src/BeamTest/PixelMap/mRICH.h
  ${CMAKE_SOURCE_DIR}/src/BeamTest/BeamFinder/BeamFinder.h
  ${CMAKE_SOURCE_DIR}/src/BeamTest/RingFinder/RingFinder.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/CalibrationLinkDef.h)

add_library(Calibration SHARED Calibration.cxx ${CMAKE_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.cxx ${CMAKE_SOURCE_DIR}/src/BeamTest/BeamFinder/BeamFinder.cxx ${CMAKE_SOURCE_DIR}/src/BeamTest/RingFinder/RingFinder.cxx G__Calibration.cxx)
target_link_libraries(Calibration ${ROOT_LIBRARIES})

install(TARGETS Calibration DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libCalibration_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libCalibration.rootmap" DESTINATION lib)
