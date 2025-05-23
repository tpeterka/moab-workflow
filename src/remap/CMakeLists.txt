if                          (NOT APPLE)
   set                      (linker_flags "-pie -Wl,--export-dynamic")
   set                      (linker_flags "${linker_flags} -Wl,-u,henson_set_contexts,-u,henson_set_namemap")
else                        ()
   set                      (linker_flags "-Wl,-u,_henson_set_contexts,-u,_henson_set_namemap")
endif                       (NOT APPLE)

add_library                 (mpas SHARED mpas.cpp)
target_link_libraries       (mpas ${libraries})
set_target_properties       (mpas PROPERTIES PREFIX "")
set_target_properties       (mpas PROPERTIES SUFFIX ".so")
set_target_properties       (mpas PROPERTIES LINK_FLAGS ${linker_flags})

add_library                 (roms SHARED roms.cpp)
target_link_libraries       (roms ${libraries})
set_target_properties       (roms PROPERTIES PREFIX "")
set_target_properties       (roms PROPERTIES SUFFIX ".so")
set_target_properties       (roms PROPERTIES LINK_FLAGS ${linker_flags})

install                     (TARGETS
                            mpas
                            roms
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/remap
                            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-config.yaml
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/remap
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/remap_actions.py
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-run.sh
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/remap
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-srun.sh
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/remap
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)
