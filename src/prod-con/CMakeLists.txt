if                          (NOT APPLE)
   set                      (linker_flags "-pie -Wl,--export-dynamic")
   set                      (linker_flags "${linker_flags} -Wl,-u,henson_set_contexts,-u,henson_set_namemap")
else                        ()
   set                      (linker_flags "-Wl,-u,_henson_set_contexts,-u,_henson_set_namemap")
endif                       (NOT APPLE)

add_library                 (producer SHARED producer.cpp mesh_gen.cpp)
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
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/prod-con
                            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-config.yaml
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/prod-con
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/prod-con_actions.py
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-run.sh
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/prod-con
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

