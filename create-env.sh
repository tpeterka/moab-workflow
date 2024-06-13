#!/bin/bash

export SPACKENV=moab-workflow-env
export YAML=$PWD/env.yaml

# create spack environment
echo "creating spack environment $SPACKENV"
spack env deactivate > /dev/null 2>&1
spack env remove -y $SPACKENV > /dev/null 2>&1
spack env create $SPACKENV $YAML

# activate environment
echo "activating spack environment"
spack env activate $SPACKENV

# spack develop lowfive@master build_type=Debug
spack add lowfive

spack develop wilkins@master build_type=Debug
spack add wilkins

spack add henson+python+mpi-wrappers

spack add mfa~examples~tests

spack add moab@5.3.0~cgm~coupler~dagmc~debug~fbigeom~fortran+hdf5~irel~metis+mpi~netcdf~parmetis~pnetcdf+shared+zoltan build_system=autotools

spack develop mfa-remap@master build_type=Debug
spack add mfa-remap

spack develop moab-workflow@master build_type=Debug
spack add moab-workflow

# install
echo "installing dependencies in environment"
spack install mfa   # install separately so that MFA_PATH is set for later packages
export MFA_PATH=`spack location -i mfa`
spack install moab  # install separately so that MOAB_PATH is set for later packages
export MOAB_PATH=`spack location -i moab`
spack install       # install the rest

# reset the environment (workaround for spack behavior)
spack env deactivate
spack env activate $SPACKENV

# set build flags
echo "setting flags for building moab-workflow"
export LOWFIVE_PATH=`spack location -i lowfive`
export MOAB_WORKFLOW_PATH=`spack location -i moab-workflow`
export HENSON_PATH=`spack location -i henson`
export WILKINS_PATH=`spack location -i wilkins`

# set LD_LIBRARY_PATH
echo "setting flags for running moab-workflow"
export LD_LIBRARY_PATH=$LOWFIVE_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$MOAB_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HENSON_PATH/lib:$LD_LIBRARY_PATH

export HDF5_PLUGIN_PATH=$LOWFIVE_PATH/lib
export HDF5_VOL_CONNECTOR="lowfive under_vol=0;under_info={};"

