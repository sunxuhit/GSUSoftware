ROOT_GENERATE_DICTIONARY(G__PixelMap PixelMap.h LINKDEF
  ${CMAKE_CURRENT_SOURCE_DIR}/PixelMapLinkDef.h)
add_library(PixelMap SHARED PixelMap.cxx H13700_180degree_v2.txt
  G__PixelMap.cxx)
target_link_libraries(PixelMap ${ROOT_LIBRARIES})

install(TARGETS PixelMap DESTINATION lib)
