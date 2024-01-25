#!/bin/bash

# activate the environment
export SPACKENV=moab-workflow-env
spack env deactivate > /dev/null 2>&1
spack env activate $SPACKENV
echo "activated spack environment $SPACKENV"

echo "setting flags for building moab-workflow"
export LOWFIVE_PATH=`spack location -i lowfive`
export MOAB_PATH=`spack location -i moab`
export MOAB_WORKFLOW_PATH=`spack location -i moab-workflow`
export HENSON_PATH=`spack location -i henson`
export WILKINS_PATH=`spack location -i wilkins`

echo "setting flags for running moab-workflow"
export LD_LIBRARY_PATH=$LOWFIVE_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$MOAB_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HENSON_PATH/lib:$LD_LIBRARY_PATH

export HDF5_PLUGIN_PATH=$LOWFIVE_PATH/lib
export HDF5_VOL_CONNECTOR="lowfive under_vol=0;under_info={};"


