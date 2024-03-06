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

    std::string mpas_infile = "mpas_outfile.h5m";
    std::string roms_infile = "roms_outfile.h5m";
    std::string read_opts   = "PARALLEL=READ_PART;PARTITION=PARALLEL_PARTITION;PARALLEL_RESOLVE_SHARED_ENTS;DEBUG_IO=3;";
    std::string outfile     = "result.h5m";
    std::string write_opts  = "PARALLEL=WRITE_PART;DEBUG_IO=6";
    ErrorCode   rval;

    // initialize moab for mpas file
    Interface*              mpas_mbi = new Core();                              // moab interface
    EntityHandle            mpas_root;
    rval = mpas_mbi->create_meshset(MESHSET_SET, mpas_root); ERR(rval);

    // initialize moab for roms file
    Interface*              roms_mbi = new Core();                              // moab interface
    EntityHandle            roms_root;
    rval = roms_mbi->create_meshset(MESHSET_SET, roms_root); ERR(rval);

    // debug
    fmt::print(stderr, "*** consumer before reading files ***\n");

    // read files
    rval = mpas_mbi->load_file(mpas_infile.c_str(), &mpas_root, read_opts.c_str() ); ERR(rval);
    rval = roms_mbi->load_file(roms_infile.c_str(), &roms_root, read_opts.c_str() ); ERR(rval);

    // debug
    fmt::print(stderr, "*** consumer after reading files ***\n");

    // write result file (for now, the mpas file)
//     rval = mpas_mbi->write_file(outfile.c_str(), 0, write_opts.c_str(), &mpas_root, 1); ERR(rval);
//     fmt::print(stderr, "*** consumer wrote the result file ***\n");

    return 0;
}

