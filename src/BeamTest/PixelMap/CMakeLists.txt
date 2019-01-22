add_library(PixelMap SHARED PixelMap.cxx H13700_180degree_v2.txt)

install (TARGETS PixelMap DESTINATION lib)
# target_link_libraries(PixelMap ${ROOT_LIBRARIES})
