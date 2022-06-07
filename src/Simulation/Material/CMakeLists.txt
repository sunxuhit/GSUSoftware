ROOT_GENERATE_DICTIONARY(G__Material Material.h LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/MaterialLinkDef.h)
add_library(Material SHARED Material.cxx G__Material.cxx)
target_link_libraries(Material ${ROOT_LIBRARIES})

install(TARGETS Material DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libMaterial_rdict.pcm" DESTINATION lib)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libMaterial.rootmap" DESTINATION lib)
