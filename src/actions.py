# set a callback for producer to broadcast/receive files before a file open
prod_nopen = 0;
def prod_callback(vol, rank):
    print("prod_callback")
    def bfo_cb(name):
        print("bfo_cb: name =", name)
        global prod_nopen
        if (name == "outfile.h5m"):
            if (prod_nopen == 0):
                vol.broadcast_files()
            prod_nopen += 1

    vol.set_before_file_open(bfo_cb)
    vol.set_keep(True);
    vol.serve_on_close = False;

con_nopen = 0;
def con_callback(vol, rank):
    vol.set_keep(True);
    vol.serve_on_close = False;

