#pragma once

#include    <vector>
#include    <cassert>
#include    <thread>
#include    <mutex>

// diy
#include    <diy/mpi/communicator.hpp>
#include    <diy/decomposition.hpp>
#include    <diy/assigner.hpp>

#include    <fmt/format.h>

#include    <hdf5.h>

// #ifdef LOWFIVE_PATH
// 
// #include    <lowfive/vol-metadata.hpp>
// #include    <lowfive/vol-dist-metadata.hpp>
// #include    <lowfive/log.hpp>
// #include    <lowfive/H5VOLProperty.hpp>
// 
// namespace l5        = LowFive;
// 
// #endif

// moab
#include    "iMesh.h"
#include    "MBiMesh.hpp"
#include    "moab/Core.hpp"
#include    "moab/Range.hpp"
#include    "MBTagConventions.hpp"
#include    "moab/ParallelComm.hpp"
#include    "moab/HomXform.hpp"
#include    "moab/ReadUtilIface.hpp"

using communicator  = MPI_Comm;
using diy_comm      = diy::mpi::communicator;
using Bounds        = diy::DiscreteBounds;

#define ERR {if(rval!=MB_SUCCESS)printf("MOAB error at line %d in %s\n", __LINE__, __FILE__);}

using namespace moab;

