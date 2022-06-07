ROOT_GENERATE_DICTIONARY(G__gemcRingFinder
  ${CMAKE_CURRENT_SOURCE_DIR}/RingFinder.h 
  ${CMAKE_SOURCE_DIR}/src/Simulation/Material/mRICH.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/RingFinderLinkDef.h)

add_library(gemcRingFinder SHARED RingFinder.cxx G__gemcRingFinder.cxx)
target_link_libraries(gemcRingFinder ${ROOT_LIBRARIES})

install(TARGETS gemcRingFinder DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libgemcRingFinder_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libgemcRingFinder.rootmap" DESTINATION lib)
