# producer callbacks
nafc = 0;       # number of times afc_cb was called
def prod_callback(vol, rank):
    print("prod_callback")

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

#         if rank == 0:
#             if nafc > 0:
#                 if !vol.get_passthru(name, "/*"):   # TODO: need API for get_passthru() for file/dataset
#                       vol.serve_all()
#                       vol.serve_all()
#         else:
#             if !vol.get_passthru(name, "/*"):       # TODO: need API for get_passthru() for file/dataset
#                   vol.serve_all()
#                   vol.serve_all()

        nafc += 1

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
        if name == "outfile.h5m":
            vol.broadcast_files()

    # set the callback
    vol.set_before_file_open(bfo_cb)

    vol.set_keep(True);
    vol.serve_on_close = False;

