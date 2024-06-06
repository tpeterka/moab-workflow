# producer callbacks
nmpas_afc = 0;       # number of times mpas_afc_cb was called
def mpas_callback(vol, rank):

    # define a callback to broadcast/receive files before a file open
    def mpas_bfo_cb(name):
        print("prod_callback mpas_bfo_cb: name =", name)
        if name == "mpas_outfile.h5m":
            vol.broadcast_files()

    # define a callback to serve files after a file close
    def mpas_afc_cb(name):
        print("prod_callback mpas_afc_cb: name =", name)
        global nmpas_afc
        if name != "mpas_outfile.h5m":
            return

        if rank == 0:
            if nmpas_afc > 0:
                if vol.is_passthru(name, "*") == False:
                    vol.serve_all()
                    vol.serve_all()
                else:
                    print("signaling consumer at mpas")
                    vol.serve_all(True, False) # signaling consumer that file is ready in passthru mode. TODO: Will be handled by Wilkins.
        else:
            if vol.is_passthru(name, "*") == False:
                vol.serve_all()
                vol.serve_all()
            else:
                print("signaling consumer at mpas")
                vol.serve_all(True, False) # signaling consumer that file is ready in passthru mode

        nmpas_afc += 1

    # set the callbacks
    vol.set_before_file_open(mpas_bfo_cb)
    vol.set_after_file_close(mpas_afc_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

nroms_afc = 0;       # number of times roms_afc_cb was called
def roms_callback(vol, rank):

    # define a callback to broadcast/receive files before a file open
    def roms_bfo_cb(name):
        print("prod_callback roms_bfo_cb: name =", name)
        if name == "roms_outfile.h5m":
            vol.broadcast_files()

    # define a callback to serve files after a file close
    def roms_afc_cb(name):
        print("prod_callback roms_afc_cb: name =", name)
        global nroms_afc
        if name != "roms_outfile.h5m":
            return

        if rank == 0:
            if nroms_afc > 0:
                if vol.is_passthru(name, "*") == False:
                    vol.serve_all()
                    vol.serve_all()
        else:
            if vol.is_passthru(name, "*") == False:
                vol.serve_all()
                vol.serve_all()

        nroms_afc += 1

    # set the callbacks
    vol.set_before_file_open(roms_bfo_cb)
    vol.set_after_file_close(roms_afc_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

# consumer callbacks
def mfa_remap_callback(vol, rank):

    # define a callback to broadcast/receive files before a file open
    def mfa_remap_bfo_cb(name):
        print("con_callback mfa_remap_bfo_cb: name =", name)
        if name == "result.h5m":
            vol.broadcast_files()

    # set the callback
    vol.set_before_file_open(mfa_remap_bfo_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

