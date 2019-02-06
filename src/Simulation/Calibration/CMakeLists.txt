ROOT_GENERATE_DICTIONARY(G__gemcCalibration
  ${CMAKE_CURRENT_SOURCE_DIR}/gemcCalibration.h
  ${CMAKE_SOURCE_DIR}/src/Simulation/Material/Material.h
  ${CMAKE_SOURCE_DIR}/src/Simulation/Material/mRICH.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/gemcCalibrationLinkDef.h)

add_library(gemcCalibration SHARED gemcCalibration.cxx ${CMAKE_SOURCE_DIR}/src/Simulation/Material/Material.cxx G__gemcCalibration.cxx)
target_link_libraries(gemcCalibration ${ROOT_LIBRARIES})

install(TARGETS gemcCalibration DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libgemcCalibration_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libgemcCalibration.rootmap" DESTINATION lib)
