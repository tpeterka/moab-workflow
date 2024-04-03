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

## Running the prod-con example

In src/prod-con/producer.cpp, you can control whether to create a synthetic mesh in memory or read an input file with
`#if 1` or `#if 0` at/around line 56. (NB, the source code is installed by spack in develop mode in the environment
location; make changes there, and don't forget to `spack install` if making changes.)

```
cd $MOAB_WORKFLOW_PATH/bin
cd prod-con
./wilkins-run.sh
```
-----

## Running the remap example

```
cd $MOAB_WORKFLOW_PATH/bin
cd remap
./wilkins-run.sh
```

