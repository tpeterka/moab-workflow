# set a callback for producer to broadcast/receive files before a file open
prod_nopen = 0;
def prod_callback(vol, rank):
    def bfo_cb(name):
        if (prod_nopen == 0):
            vol.broadcast_files()
        prod_nopen++
    vol.set_before_file_open(bfo_cb)

# set a callback for consumer to broadcast/receive files before a file open
con_nopen = 0;
def con_callback(vol, rank):
    def bfo_cb(name)
        if (name != outfile):
            return
        if (con_nopen == 0):
            vol.broadcast_files();
        con_nopen++ # only increment nopen when the file name matches
    vol.set_before_file_open(bfo_cb)
    vol.set_keep = True;
    vol.serve_on_close = False;

