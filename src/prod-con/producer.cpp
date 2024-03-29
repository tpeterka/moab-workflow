#include    <diy/master.hpp>
#include    <diy/decomposition.hpp>
#include    <diy/assigner.hpp>
#include    "../opts.h"

#include    "prod-con.hpp"

herr_t fail_on_hdf5_error(hid_t stack_id, void*)
{
    H5Eprint(stack_id, stderr);
    fmt::print(stderr, "An HDF5 error was detected. Terminating.\n");
    exit(1);
}

int main(int argc, char**argv)
{
    // initialize MPI
    diy::mpi::environment  env(argc, argv);       // equivalent of MPI_Init(argc, argv)/MPI_Finalize()

    // for some reason, local has to be a duplicate of world, not world itself
    diy::mpi::communicator      world;
    communicator                local;
    MPI_Comm_dup(world, &local);
    diy::mpi::communicator local_(local);

    std::string infile;
    std::string outfile     = "outfile.h5m";
    std::string read_opts   = "PARALLEL=READ_PART;PARTITION=PARALLEL_PARTITION;PARALLEL_RESOLVE_SHARED_ENTS;DEBUG_IO=6;";
    std::string write_opts  = "PARALLEL=WRITE_PART;DEBUG_IO=6";
    bool        help;                           // show help

    // get command line arguments
    opts::Options ops;
    ops >> opts::Option('f', "infile",      infile,     " diy input file name");
    ops >> opts::Option('h', "help",        help,       " show help");

    if (!ops.parse(argc, argv) || help)
    {
        if (local_.rank() == 0)
            std::cerr << ops;
        return 1;
    }

    // echo command line args
    if (local_.rank() == 0)
        fmt::print(stderr, "producer infile = {} local size = {}\n", infile, local_.size());

    // initialize moab
    Interface*                      mbi = new Core();                       // moab interface
    ParallelComm*                   pc  = new ParallelComm(mbi, local);     // moab communicator
    EntityHandle                    root;
    ErrorCode                       rval;

    rval = mbi->create_meshset(MESHSET_SET, root); ERR(rval);

#if 0

    // create mesh in memory
    int                             mesh_type = 0;                          // source mesh type (0 = hex, 1 = tet)
    int                             mesh_size = 10;                         // source mesh size per side
    int                             mesh_slab = 0;                          // block shape (0 = cubes; 1 = slabs)
    double                          factor = 1.0;                           // scaling factor on field values
    fmt::print(stderr, "*** producer generating synthetic mesh in memory ***\n");
    PrepMesh(mesh_type, mesh_size, mesh_slab, mbi, pc, root, factor, false);
    fmt::print(stderr, "*** producer after creating mesh in memory ***\n");

#else

    // or

    // read file
    fmt::print(stderr, "*** producer reading input file ***\n");
    rval = mbi->load_file(infile.c_str(), &root, read_opts.c_str() ); ERR(rval);
    fmt::print(stderr, "*** producer after reading file ***\n");

#endif

    // write file
    rval = mbi->write_file(outfile.c_str(), 0, write_opts.c_str(), &root, 1); ERR(rval);

    // debug
    fmt::print(stderr, "*** producer after writing file ***\n");

    return 0;
}
