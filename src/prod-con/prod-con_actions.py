# producer callbacks
nafc = 0;       # number of times afc_cb was called
def prod_callback(vol, rank):

    # define a callback to broadcast/receive files before a file open
    def bfo_cb(name):
        print("prod_callback bfo_cb: name =", name)
        if name == "outfile.h5m":
            vol.broadcast_files()

    # define a callback to serve files after a file close
    def afc_cb(name):
        print("prod_callback afc_cb: name =", name)
        global nafc
        if name != "outfile.h5m":
            return

        if rank == 0:
            if nafc > 0:
                if vol.is_passthru(name, "*") == False:
                    vol.serve_all()
                    vol.serve_all()
                else:
       	       	    vol.serve_all(True, False) # signaling consumer that file is ready in passthru mode. TODO: Will be handled by Wilkins.

        else:
            if vol.is_passthru(name, "*") == False:
                vol.serve_all()
                vol.serve_all()
            else:
                vol.serve_all(True, False) # signaling consumer that file is ready in passthru mode

        nafc += 1

    # set the input file to be read from disk
    # TODO: is this needed?
    vol.set_passthru("/home/tpeterka/software/moab-workflow/sample_data/mpas_2d_source_p128.h5m", "*")

    # set the callbacks
    vol.set_before_file_open(bfo_cb)
    vol.set_after_file_close(afc_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

# consumer callbacks
def con_callback(vol, rank):

    # define a callback to broadcast/receive files before a file open
    def bfo_cb(name):
        print("con_callback bfo_cb: name =", name)
        if name == "result.h5m":
            vol.broadcast_files()

    # set the result file to be written to disk
    # TODO: is this needed?
    vol.set_passthru("result.h5m", "*")

    # set the callback
    vol.set_before_file_open(bfo_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

