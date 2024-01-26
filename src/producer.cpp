#include "prod-con.hpp"

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
    communicator local = MPI_COMM_WORLD;
    diy::mpi::communicator local_(local);

    std::string infile;
    std::string outfile     = "outfile.h5m";
    bool        help;                           // show help

    std::string read_opts   = "PARALLEL=READ_PART;PARTITION=PARALLEL_PARTITION;PARALLEL_RESOLVE_SHARED_ENTS;DEBUG_IO=6;";
    std::string write_opts  = "PARALLEL=WRITE_PART;DEBUG_IO=6";


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
        fmt::print(stderr, "producer infile = {}\n", infile);

    hid_t plist = H5Pcreate(H5P_FILE_ACCESS);

//         if (passthru)
            H5Pset_fapl_mpio(plist, local, MPI_INFO_NULL);

//         // set a callback to broadcast/receive files by other before a file open
//         static int nopen = 0;                   // needs to be static in order to be captured correctly by lambda, not sure why
//         vol_plugin.set_before_file_open([&](const std::string& name)
//         {
//             if (nopen == 0)
//                 vol_plugin.broadcast_files();
//             nopen++;
//         });

    // initialize moab
    Interface*                      mbi = new Core();                       // moab interface
    ParallelComm*                   pc  = new ParallelComm(mbi, local);     // moab communicator
    EntityHandle                    root;
    ErrorCode                       rval;

    rval = mbi->create_meshset(MESHSET_SET, root); ERR(rval);

    // debug
    fmt::print(stderr, "*** producer before reading file ***\n");

    // read file
    rval = mbi->load_file(infile.c_str(), &root, read_opts.c_str() ); ERR(rval);

    // debug
    fmt::print(stderr, "*** producer after reading file ***\n");

//     // write file
//     rval = mbi->write_file(outfile.c_str(), 0, write_opts.c_str(), &root, 1); ERR(rval);
// 
//     // debug
//     fmt::print(stderr, "*** producer after writing file ***\n");

    // clean up
    H5Pclose(plist);

    return 0;
}
