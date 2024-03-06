#include    <diy/master.hpp>
#include    <diy/decomposition.hpp>
#include    <diy/assigner.hpp>
#include    "../opts.h"

#include    "remap.hpp"

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
    std::string outfile     = "roms_outfile.h5m";
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
        fmt::print(stderr, "roms infile = {} local size = {}\n", infile, local_.size());

    // initialize moab
    Interface*                      mbi = new Core();                       // moab interface
    EntityHandle                    root;
    ErrorCode                       rval;

    rval = mbi->create_meshset(MESHSET_SET, root); ERR(rval);

    // read file
    fmt::print(stderr, "*** producer reading input file ***\n");
    rval = mbi->load_file(infile.c_str(), &root, read_opts.c_str() ); ERR(rval);
    fmt::print(stderr, "*** producer after reading file ***\n");

    // write file
    rval = mbi->write_file(outfile.c_str(), 0, write_opts.c_str(), &root, 1); ERR(rval);

    // debug
    fmt::print(stderr, "*** producer after writing file ***\n");

    return 0;
}
