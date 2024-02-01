#!/usr/bin/env python3

import sys
import os
from glob import glob
from mpi4py import MPI
import time

from timeit import default_timer as timer

import pyhenson as h

#mpirun -n 2 -l python  henson_moab_runner.py ./mpas_2d_source_p128.h5m 1 1
print(f"Usage: {sys.argv[0]} producer_inputs producer_ranks consumer_ranks")

if not glob(os.path.join(os.environ["HDF5_PLUGIN_PATH"], "liblowfive.*")):
    raise RuntimeError("Bad HDF5_PLUGIN_PATH, lowfive library not found")

print(f"Using LowFive plugin built in {os.environ['HDF5_PLUGIN_PATH']}")

inputs_fname = sys.argv[1]
if not os.path.exists(inputs_fname):
    raise RuntimeError("producer inputs not found, cannot read {inputs_fname}")
else:
    print(f"Using {inputs_fname} as inputs for producer")

prod_ranks = int(sys.argv[2])
con_ranks = int(sys.argv[3])

world = MPI.COMM_WORLD.Dup()
size = world.Get_size()
rank = world.Get_rank()

if prod_ranks + con_ranks != size:
    raise RuntimeError(f"Size mismatch, {prod_ranks = } + {con_ranks = } != {size = }")

import lowfive

pm = h.ProcMap(world, [("producer", prod_ranks), ("consumer", con_ranks)])
nm = h.NameMap()

start = timer()

if pm.group() == "producer":
    tag = 0
    lowfive.create_logger("info")
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("consumer", tag))
    #vol.set_keep(True)
    #vol.set_memory("*", "*")
    vol.set_passthru("*", "*")
    vol.set_intercomm("*", "*", 0)
    prod = h.Puppet("./producer.so", ["--infile", inputs_fname], pm, nm)
    prod.proceed()
else:
    tag = 0
    lowfive.create_logger("info")
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("producer", tag))
    #vol.set_memory("*", "*")
    vol.set_passthru("*", "*")
    intercomm_index = 0
    vol.set_intercomm("*", "*", intercomm_index)
    cons = h.Puppet("./consumer.so", [], pm, nm)
    cons.proceed()

end = timer()

print(f"Henson script done time = {end - start} seconds")
