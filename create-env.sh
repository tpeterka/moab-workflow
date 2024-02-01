#!/bin/bash

export SPACKENV=moab-workflow-env
export YAML=$PWD/env.yaml

# add custom spack repos
echo "adding custom spack repo for moab-workflow"
spack repo add . > /dev/null 2>&1
echo "adding spack repo for lowfive"
spack repo add lowfive > /dev/null 2>&1

# create spack environment
echo "creating spack environment $SPACKENV"
spack env deactivate > /dev/null 2>&1
spack env remove -y $SPACKENV > /dev/null 2>&1
spack env create $SPACKENV $YAML

# activate environment
echo "activating spack environment"
spack env activate $SPACKENV

# add moab in develop mode
spack develop moab@5.3.0~cgm~coupler~dagmc+debug~fbigeom~fortran+hdf5~irel~metis+mpi~netcdf~parmetis~pnetcdf+shared+zoltan build_system=autotools
spack add moab

# add lowfive in develop mode
# spack develop lowfive@master build_type=Debug
# spack add lowfive

# add moab-workflow in develop mode
spack develop moab-workflow@master build_type=Debug
spack add moab-workflow

# install everything in environment
echo "installing dependencies in environment"
spack install

# reset the environment (workaround for spack behavior)
spack env deactivate
spack env activate $SPACKENV

# set build flags
echo "setting flags for building moab-workflow"
export LOWFIVE_PATH=`spack location -i lowfive`
export MOAB_PATH=`spack location -i moab`
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

