if                          (NOT APPLE)
   set                      (linker_flags "-pie -Wl,--export-dynamic")
   set                      (linker_flags "${linker_flags} -Wl,-u,henson_set_contexts,-u,henson_set_namemap")
else                        ()
   set                      (linker_flags "-Wl,-u,_henson_set_contexts,-u,_henson_set_namemap")
endif                       (NOT APPLE)

add_library                 (producer SHARED producer.cpp)
target_link_libraries       (producer ${libraries})
set_target_properties       (producer PROPERTIES PREFIX "")
set_target_properties       (producer PROPERTIES SUFFIX ".so")
set_target_properties       (producer PROPERTIES LINK_FLAGS ${linker_flags})

add_library                 (consumer SHARED consumer.cpp)
target_link_libraries       (consumer ${libraries})
set_target_properties       (consumer PROPERTIES PREFIX "")
set_target_properties       (consumer PROPERTIES SUFFIX ".so")
set_target_properties       (consumer PROPERTIES LINK_FLAGS ${linker_flags})

install                     (TARGETS
                            producer
                            consumer
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)
