ROOT_GENERATE_DICTIONARY(G__BeamFinder
  ${CMAKE_CURRENT_SOURCE_DIR}/BeamFinder.h 
  ${CMAKE_SOURCE_DIR}/src/BeamTest/PixelMap/mRICH.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/BeamFinderLinkDef.h)

add_library(BeamFinder SHARED BeamFinder.cxx G__BeamFinder.cxx)
target_link_libraries(BeamFinder ${ROOT_LIBRARIES})

install(TARGETS BeamFinder DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libBeamFinder_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libBeamFinder.rootmap" DESTINATION lib)
