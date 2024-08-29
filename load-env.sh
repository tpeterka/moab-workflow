#!/bin/bash

# activate the environment
export SPACKENV=moab-workflow-env
spack env deactivate > /dev/null 2>&1
spack env activate $SPACKENV
echo "activated spack environment $SPACKENV"

echo "setting flags for building moab-workflow"
export LOWFIVE_PATH=`spack location -i lowfive`
export MFA_PATH=`spack location -i mfa`
export MOAB_PATH=`spack location -i moab`
export MOAB_WORKFLOW_PATH=`spack location -i moab-workflow`
export HENSON_PATH=`spack location -i henson`
export WILKINS_PATH=`spack location -i wilkins`

echo "setting flags for building MPAS-Ocean"
export MPAS_EXTERNAL_LIBS=""
export MPAS_EXTERNAL_LIBS="${MPAS_EXTERNAL_LIBS} -lgomp"
export NETCDF=`spack location -i netcdf-c`
export NETCDFF=`spack location -i netcdf-fortran`
export PNETCDF=`spack location -i parallel-netcdf`
export PIO=`spack location -i mpas-o-scorpio`
export HDF5=`spack location -i hdf5`
# export LOWFIVE=`spack location -i lowfive`
# export HENSON=`spack location -i henson`
export USE_PIO2=true
export OPENMP=false
export HDF5_USE_FILE_LOCKING=FALSE
export MPAS_SHELL=/bin/bash
export CORE=ocean
export SHAREDLIB=true
export PROFILE_PRELIB="-L$HENSON/lib -lhenson-pmpi"

echo "setting flags for running moab-workflow"
export LD_LIBRARY_PATH=$NETCDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PNETCDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$NETCDFF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HDF5/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PIO/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LOWFIVE_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$MOAB_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HENSON_PATH/lib:$LD_LIBRARY_PATH

echo "setting environment variables for running LowFive"
unset HDF5_PLUGIN_PATH
unset HDF5_VOL_CONNECTOR
lowfive_module=`python3 -c "import lowfive; print(lowfive._lowfive.__file__)"`
lowfive_library=`ldd $lowfive_module | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep lowfive`
export HDF5_PLUGIN_PATH=`dirname $lowfive_library`
export HDF5_VOL_CONNECTOR="lowfive under_vol=0;under_info={};"

# give openMP 1 core for now to prevent using all cores for threading
# could set a more reasonable number to distribute cores between mpi + openMP
export OMP_NUM_THREADS=1


