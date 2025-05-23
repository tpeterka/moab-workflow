if                          (NOT APPLE)
   set                      (linker_flags "-pie -Wl,--export-dynamic")
   set                      (linker_flags "${linker_flags} -Wl,-u,henson_set_contexts,-u,henson_set_namemap")
else                        ()
   set                      (linker_flags "-Wl,-u,_henson_set_contexts,-u,_henson_set_namemap")
endif                       (NOT APPLE)

add_library                 (roms-consumer SHARED consumer.cpp)
target_link_libraries       (roms-consumer ${libraries})
set_target_properties       (roms-consumer PROPERTIES PREFIX "")
set_target_properties       (roms-consumer PROPERTIES SUFFIX ".so")
set_target_properties       (roms-consumer PROPERTIES LINK_FLAGS ${linker_flags})

install                     (TARGETS
                            roms-consumer
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/mpas-roms
                            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)
file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-config.yaml
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/mpas-roms
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-run.sh
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/mpas-roms
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-srun.sh
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/mpas-roms
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)
