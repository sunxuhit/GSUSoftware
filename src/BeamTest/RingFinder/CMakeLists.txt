ROOT_GENERATE_DICTIONARY(G__RingFinder
  ${CMAKE_CURRENT_SOURCE_DIR}/RingFinder.h 
  ${CMAKE_SOURCE_DIR}/src/BeamTest/PixelMap/mRICH.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/RingFinderLinkDef.h)

add_library(RingFinder SHARED RingFinder.cxx G__RingFinder.cxx)
target_link_libraries(RingFinder ${ROOT_LIBRARIES})

install(TARGETS RingFinder DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libRingFinder_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libRingFinder.rootmap" DESTINATION lib)
