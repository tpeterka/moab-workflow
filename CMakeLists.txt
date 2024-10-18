project                     (moab-workflow)
cmake_minimum_required      (VERSION 3.12)

option                      (wrapped_mpi    "MPI compiler wrapper requires no further MPI libs"     OFF)

# C++11
if                          (NOT DEFINED CMAKE_CXX_STANDARD)
    set                     (CMAKE_CXX_STANDARD 11)
endif                       ()

set                         (CMAKE_POSITION_INDEPENDENT_CODE ON)

# ignore variadic args warning (no other good solution)
set                         (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-varargs")

# Default to Release
if                          (NOT CMAKE_BUILD_TYPE)
    set                     (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    set_property            (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif                       (NOT CMAKE_BUILD_TYPE)

# MPI
if                          (NOT wrapped_mpi)
    find_package            (MPI REQUIRED)
    set                     (libraries MPI::MPI_C)
endif                       ()

# Threads
find_package                (Threads)

# DIY
option                      (build_examples "" OFF)
option                      (build_tests "" OFF)
add_subdirectory            (ext/diy)
set                         (libraries ${libraries} diy)

# fmt
option                      (FMT_INSTALL "" OFF)
add_subdirectory            (ext/fmt)
set                         (libraries ${libraries} fmt::fmt)

# Moab
if                          (NOT DEFINED MOAB_PATH AND DEFINED ENV{MOAB_PATH})
    set                     (MOAB_PATH $ENV{MOAB_PATH})
endif                       ()
if                          (MOAB_PATH)
    message                 ("Found MOAB_PATH")
    set                     (MOAB_LIBRARY ${MOAB_PATH}/lib/libMOAB.so)
    set                     (MOAB_IMESH_LIBRARY ${MOAB_PATH}/lib/libiMesh.so)
    set                     (MOAB_INCLUDE_DIR ${MOAB_PATH}/include)
    add_definitions         (-DMOAB_PATH)
else                        ()
    message                 ("Looking for Moab")
    find_package            (moab REQUIRED)
endif                       ()
message                     ("MOAB_LIBRARY = ${MOAB_LIBRARY}")
message                     ("MOAB_IMESH_LIBRARY = ${MOAB_IMESH_LIBRARY}")
message                     ("MOAB_INCLUDE_DIR = ${MOAB_INCLUDE_DIR}")
set                         (libraries ${libraries} ${MOAB_LIBRARY} ${MOAB_IMESH_LIBRARY})
include_directories         (SYSTEM ${MOAB_INCLUDE_DIR})

# Henson
if                          (NOT DEFINED HENSON_PATH AND DEFINED ENV{HENSON_PATH})
    set                     (HENSON_PATH $ENV{HENSON_PATH})
endif                       ()
if                          (HENSON_PATH)
    set                     (HENSON_LIBRARY ${HENSON_PATH}/lib/libhenson.a)
    set                     (HENSON_PMPI_LIBRARY ${HENSON_PATH}/lib/libhenson-pmpi.so)
else                        ()
    message                 ("Looking for Henson")
    find_package            (henson REQUIRED)
endif                       ()
message                     ("HENSON_LIBRARY = ${HENSON_LIBRARY}")
message                     ("HENSON_PMPI_LIBRARY = ${HENSON_PMPI_LIBRARY}")
set                         (libraries ${libraries} ${HENSON_PMPI_LIBRARY} ${HENSON_LIBRARY})

# LowFive
if                          (NOT DEFINED LOWFIVE_PATH AND DEFINED ENV{LOWFIVE_PATH})
    set                     (LOWFIVE_PATH $ENV{LOWFIVE_PATH})
endif                       ()
if                          (LOWFIVE_PATH)
    message                 ("Found LOWFIVE_PATH")
    set                     (LOWFIVE_LIBRARY ${LOWFIVE_PATH}/lib/liblowfive.so)
    set                     (LOWFIVE_DIST_LIBRARY ${LOWFIVE_PATH}/lib/liblowfive-dist.a)
    set                     (LOWFIVE_INCLUDE_DIR ${LOWFIVE_PATH}/include)
    add_definitions         (-DLOWFIVE_PATH)
else                        ()
    message                 ("Looking for LowFive")
    find_package            (LowFive REQUIRED)
endif                       ()
message                     ("LOWFIVE_LIBRARY = ${LOWFIVE_LIBRARY}")
message                     ("LOWFIVE_DIST_LIBRARY = ${LOWFIVE_DIST_LIBRARY}")
message                     ("LOWFIVE_INCLUDE_DIR = ${LOWFIVE_INCLUDE_DIR}")
set                         (libraries ${libraries} ${LOWFIVE_LIBRARY} ${LOWFIVE_DIST_LIBRARY})
include_directories         (SYSTEM ${LOWFIVE_INCLUDE_DIR}
                            SYSTEM ${LOWFIVE_INCLUDE_DIR}/diy)

# HDF5
if                          (NOT DEFINED HDF5_PATH AND DEFINED ENV{HDF5_PATH})
    set                     (HDF5_PATH $ENV{HDF5_PATH})
endif                       ()
if                          (HDF5_PATH)
    set                     (HDF5_LIBRARY ${HDF5_PATH}/lib/libhdf5.so)
    set                     (HDF5_HL_LIBRARY ${HDF5_PATH}/lib/libhdf5_hl.so)
    set                     (HDF5_INCLUDE_DIR ${HDF5_PATH}/include)
  message                   ("HDF5_LIBRARY = " ${HDF5_LIBRARY})
  message                   ("HDF5_HL_LIBRARY = " ${HDF5_HL_LIBRARY})
  message                   ("HDF5_INCLUDE_DIR = " ${HDF5_INCLUDE_DIR})
  message                   ("Z_LIBRARY = " ${Z_LIBRARY})
  set                       (HDF5_LIBRARY ${HDF5_HL_LIBRARY} ${HDF5_LIBRARY} ${Z_LIBRARY})
else                        ()
  message                   ("Looking for HDF5")
  find_package              (HDF5 REQUIRED COMPONENTS C HL)
  message                   ("HDF5_C_LIBRARIES = " ${HDF5_C_LIBRARIES})
  message                   ("HDF5_HL_LIBRARIES = " ${HDF5_HL_LIBRARIES})
  message                   ("HDF5_C_LIBRARY_hdf5 = " ${HDF5_C_LIBRARY_hdf5})
  message                   ("HDF5_C_LIBRARY_z = " ${HDF5_C_LIBRARY_z})
  message                   ("HDF5_INCLUDE_DIR = " ${HDF5_INCLUDE_DIR})
  set                       (HDF5_LIBRARY ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES} ${HDF5_C_LIBRARY_hdf5} ${HDF5_C_LIBRARY_z})
endif                       ()
set                         (libraries ${libraries} ${HDF5_LIBRARY})
include_directories         (SYSTEM ${HDF5_INCLUDE_DIR})

# Scorpio
if                          (NOT DEFINED SCORPIO_PATH AND DEFINED ENV{SCORPIO_PATH})
    set                     (SCORPIO_PATH $ENV{SCORPIO_PATH})
endif                       ()
if                          (SCORPIO_PATH)
    set                     (SCORPIO_LIBRARY ${SCORPIO_PATH}/lib/libpioc.so)
    set                     (SCORPIO_INCLUDE_DIR ${SCORPIO_PATH}/include)
else                        ()
    message                 ("Looking for Scorpio")
    find_package            (Scorpio REQUIRED)
endif                       ()
    message                 ("SCORPIO_LIBRARY = ${SCORPIO_LIBRARY}")
    message                 ("SCORPIO_INCLUDE_DIR = ${SCORPIO_INCLUDE_DIR}")
set                         (libraries ${libraries} ${SCORPIO_LIBRARY})
include_directories         (SYSTEM ${SCORPIO_INCLUDE_DIR})

# NetCDF
if                          (NOT DEFINED NETCDF_PATH AND DEFINED ENV{NETCDF_PATH})
    set                     (NETCDF_PATH $ENV{NETCDF_PATH})
endif                       ()
if                          (NETCDF_PATH)
    if                      (EXISTS ${NETCDF_PATH}/lib/libnetcdf.so)
        set                 (NETCDF_LIBRARY ${NETCDF_PATH}/lib/libnetcdf.so)
    else                    ()
        if                  (EXISTS ${NETCDF_PATH}/lib64/libnetcdf.so)
            set             (NETCDF_LIBRARY ${NETCDF_PATH}/lib64/libnetcdf.so)
        else                ()
            message         (FATAL_ERROR "libnetcdf.so  not found in ${NETCDF_PATH}/lib or ${NETCDF_PATH}/lib64")
        endif               ()
    endif                   ()
    set                     (NETCDF_INCLUDE_DIR ${NETCDF_PATH}/include)
    message                 ("NETCDF_LIBRARY =  ${NETCDF_LIBRARY}")
    message                 ("NETCDF_INCLUDE_DIR =  ${NETCDF_INCLUDE_DIR}")
else                        ()
    message                 (FATAL_ERROR "NETCDF_PATH needs to be defined")
endif                       ()
set                         (libraries ${libraries} ${NETCDF_LIBRARY})
include_directories         (SYSTEM ${NETCDF_INCLUDE_DIR})

# PnetCDF
if                          (NOT DEFINED PNETCDF_PATH AND DEFINED ENV{PNETCDF_PATH})
    set                     (PNETCDF_PATH $ENV{PNETCDF_PATH})
endif                       ()
if                          (PNETCDF_PATH)
    set                     (PNETCDF_INCLUDE_DIR ${PNETCDF_PATH}/include)
    message                 ("PNETCDF_INCLUDE_DIR = ${PNETCDF_INCLUDE_DIR}")
else                        ()
    message                 (FATAL_ERROR "PNETCDF_PATH needs to be defined")
endif                       ()
include_directories         (SYSTEM ${PNETCDF_INCLUDE_DIR})

# Include dirs
set                         (CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem")
include_directories         (${CMAKE_CURRENT_SOURCE_DIR}/include
                            SYSTEM ${MPI_C_INCLUDE_PATH})

# Libraries
set                         (libraries
                             ${libraries}
                             ${CMAKE_THREAD_LIBS_INIT}
                             ${CMAKE_DL_LIBS})

file                        (COPY
                            ${CMAKE_CURRENT_SOURCE_DIR}/wilkins-master.py
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                            GROUP_READ GROUP_WRITE
                            WORLD_READ WORLD_WRITE)

file                        (COPY
                            ${MFA_REMAP_PATH}/bin/mfa_remap_shared.so
                            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/remap
                            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_WRITE GROUP_EXECUTE
                            WORLD_READ WORLD_WRITE WORLD_EXECUTE)

add_subdirectory            (src)

