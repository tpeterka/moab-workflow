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

spack add mpich@4

spack add hdf5@1.14+hl+mpi

# spack develop lowfive@master build_type=Debug
spack add lowfive

# spack develop wilkins@master build_type=Debug
spack add wilkins

spack add henson+python+mpi-wrappers

spack add netcdf-c@4.9+mpi

spack add netcdf-fortran@4.5.3

spack add mpas-o-scorpio+hdf5

spack add mfa~examples~tests

spack add moab@5.3.0~cgm~coupler~dagmc~debug~fbigeom~fortran+hdf5~irel~metis+mpi~netcdf~parmetis~pnetcdf+shared+zoltan build_system=autotools

# spack develop mfa-remap@master build_type=Debug
spack add mfa-remap

# spack develop moab-workflow@master build_type=Debug
spack add moab-workflow

# install
echo "installing dependencies in environment"
spack install mfa   # install separately so that MFA_PATH is set for later packages
export MFA_PATH=`spack location -i mfa`
spack install moab  # install separately so that MOAB_PATH is set for later packages
export MOAB_PATH=`spack location -i moab`
spack install       # install the rest

spack env deactivate

