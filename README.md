# Instructions for Building and Running Moab Workflow with Wilkins

Installation is done through Spack. If you don't have Spack installed or if Spack is new to you, go [here](https://spack.readthedocs.io/en/latest/) first.

## Setting up Spack environment

### First time: create and load the Spack environment

```
git clone https://github.com/tpeterka/moab-workflow
cd /path/to/moab-workflow
source ./create-env.sh     # requires being in the same directory to work properly
```

### Subsequent times: load the Spack environment

```
source /path/to/moab-workflow/load-env.sh
```

----

## Configuring the workflow

Modify wilkins-config.yaml and wilkins-run.sh for the desired number of processes and other configurations.

-----

## Running the example

```
cd $MOAB_WORKFLOW_PATH/bin
./wilkins-run.sh
```

