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

#ifdef LOWFIVE_PATH

#include    <lowfive/vol-metadata.hpp>
#include    <lowfive/vol-dist-metadata.hpp>
#include    <lowfive/log.hpp>
#include    <lowfive/H5VOLProperty.hpp>

namespace l5        = LowFive;

#endif

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


enum {producer_task, producer1_task, producer2_task, consumer_task, consumer1_task, consumer2_task};

#define ERR {if(rval!=MB_SUCCESS)printf("MOAB error at line %d in %s\n", __LINE__, __FILE__);}

using namespace moab;

void hex_mesh_gen(int *mesh_size, Interface *mbint, EntityHandle *mesh_set,
        ParallelComm *mbpc, diy::RegularDecomposer<Bounds>& decomp, diy::RoundRobinAssigner& assign);
void tet_mesh_gen(int *mesh_size, Interface *mbint, EntityHandle *mesh_set,
        ParallelComm *mbpc, diy::RegularDecomposer<Bounds>& decomp, diy::RoundRobinAssigner& assign);
void create_hexes_and_verts(int *mesh_size, Interface *mbint, EntityHandle *mesh_set,
        diy::RegularDecomposer<Bounds>& decomp, diy::RoundRobinAssigner& assign, ParallelComm* mbpc);
void create_tets_and_verts(int *mesh_size, Interface *mbint, EntityHandle *mesh_set,
        diy::RegularDecomposer<Bounds>& decomp, diy::RoundRobinAssigner& assign, ParallelComm* mbpc);
void resolve_and_exchange(Interface *mbint, EntityHandle *mesh_set, ParallelComm *mbpc);

double PhysField(double x, double y, double z, double factor);

void PutElementField(Interface *mbi, EntityHandle eh, const char *tagname, double factor);

void GetElementField(Interface *mbi, EntityHandle eh, const char *tagname, double factor, MPI_Comm comm, bool debug);

void PutVertexField(Interface *mbi, EntityHandle eh, const char *tagname, double factor);

void GetVertexField(Interface *mbi, EntityHandle eh, const char *tagname, double factor, MPI_Comm comm, bool debug);

void PrintMeshStats(Interface *mbint, EntityHandle *mesh_set, ParallelComm *mbpc);

void PrepMesh(int src_type, int src_size, int slab, Interface* mbi, ParallelComm* pc, EntityHandle root, double factor,
        bool debug);
