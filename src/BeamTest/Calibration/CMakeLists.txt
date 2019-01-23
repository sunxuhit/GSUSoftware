ROOT_GENERATE_DICTIONARY(G__Calibration
  ${CMAKE_CURRENT_SOURCE_DIR}/Calibration.h
  ${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.h LINKDEF
  ${CMAKE_CURRENT_SOURCE_DIR}/CalibrationLinkDef.h)

add_library(Calibration SHARED Calibration.cxx G__Calibration.cxx
  ${PROJECT_SOURCE_DIR}/src/BeamTest/PixelMap/PixelMap.cxx)
target_link_libraries(Calibration ${ROOT_LIBRARIES})

install(TARGETS Calibration DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libCalibration_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libCalibration.rootmap" DESTINATION lib)
