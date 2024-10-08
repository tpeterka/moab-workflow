# Instructions for Building and Running Moab Workflow with Wilkins

Installation is done through Spack. If you don't have Spack installed or if Spack is new to you, go [here](https://spack.readthedocs.io/en/latest/) first.

-----

## Clone this repository

```
git clone https://github.com/tpeterka/moab-workflow
```

-----

## First time: Add the following Spack repositories to your local Spack installation

LowFive
```
git clone https://github.com/diatomic/LowFive
spack repo add Lowfive
```

Wilkins
```
git clone https://github.com/orcunyildiz/wilkins
spack repo add wilkins
```

Mfa
```
git clone https://github.com/tpeterka/mfa
spack repo add mfa
```

Mfa-remap
```
git clone https://github.com/dclenz/climate-remap
spack repo add climate-remap
```

Mpas-o-scorpio
```
spack repo add /path_to/mpas-o-workflow/mpas-o-scorpio
```

Moab-workflow
```
cd /path/to/moab-workflow
spack repo add .
```

-----

## Setting up Spack environment

### First time: create and load the Spack environment

```
cd /path/to/moab-workflow
source ./create-env.sh     # requires being in the same directory to work properly
source ./load-env.sh
```

### Subsequent times: load the Spack environment

```
source /path/to/moab-workflow/load-env.sh
```

-----

## Building MPAS-Ocean

### Clone MPAS-Ocean

```
git clone https://github.com/E3SM-Project/E3SM
cd E3SM
git submodule update --init --recursive
```
On a new machine, if you are denied permission to execute the `git submodule update --init --recursive` command, you
need to copy your ssh public key to your github account:
```
cd ~/.ssh
ls
```
If a public key doesn't exist:
```
ssh-keygen -t ed25519 -C "<your email address>"
# press enter for all prompts
```
Copy the key to the clipboard, log into your account on github.com, edit your settings, and add the SSH key.

Also first time only for a new git configuration, you may want to do:
```
git config --global user.email "<your email address>"
git config --global user.name "<your name>"
```

### Build MPAS-Ocean

```
cd /path_to/E3SM/components/mpas-ocean
make clean              # if dirty
make -j gfortran
```
This will take ~ 5 minutes to compile.

-----

## Setting up a test case for MPAS-Ocean to execute

Compass is an E3SM system for generating and running test cases for MPAS-Ocean, and relies on conda environments.
If you don't have conda, install miniconda [here](https://docs.conda.io/en/latest/miniconda.html) as follows.

```
mkdir /path_to/miniconda3
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O /path_to/miniconda3/miniconda.sh
bash /path_to/miniconda3/miniconda.sh -b -u -p /path_to/miniconda3
rm -rf /path_to/miniconda3/miniconda.sh
/path_to/miniconda3/bin/conda init bash
```

The last step above will add a few lines to the end of your `.bashrc`. After
installing Compass and creating a Compass environment in the next section, you
may comment out those lines in `.bashrc`. Otherwise you will always be in a
conda environment when you log in.

### First time: install Compass and create Compass environment

Deactivate the spack environment.
```
spack env deactivate
```

```
git clone https://github.com/MPAS-Dev/compass.git compass-env-only
cd /path_to/compass-env-only
git submodule update --init --recursive
./conda/configure_compass_env.py --conda /path_to/miniconda3 --env_only
source load_dev_compass_1.4.0-alpha.2.sh        # assumes load_dev_compass_1.4.0-alpha.2.sh is the script created by the previous command
```

### First time: create a Compass configuration file for a new machine

Assumes the config file is named /path_to/compass-env-only/compass.cfg and has these contents, or similar (yours may vary)

```
# This file contains some common config options you might want to set

# The paths section describes paths to databases and shared compass environments
[paths]

# A root directory where MPAS standalone data can be found
# You can put any path you wish here
database_root = /path_to/mpas_standalonedata

# The parallel section describes options related to running tests in parallel
[parallel]

# parallel system of execution: slurm or single_node
system = single_node

# whether to use mpirun or srun to run the model
parallel_executable = mpiexec

# cores per node on the machine, detected automatically by default
# cores_per_node = 4
```

### Create a test case for MPAS-Ocean

Assumes that `load_dev_compass_1.4.0-alpha.2.sh` is the name of the conda environment load script created initially
Also assumes the test case being created is a 10km default baroclinic channel.
Other test cases would be named similarly.
See the [Compass docs](https://mpas-dev.github.io/compass/latest/users_guide/quick_start.html) for more information.
The `compass list` command shows all available test cases (after loading the compass environment).
Set `/path_to/compass-baroclinic-test` below where you want the test case to be created.

```
source /path_to/compass-env-only/load_dev_compass_1.4.0-alpha.2.sh
compass setup -t ocean/baroclinic_channel/10km/default -w /path_to/compass-baroclinic-test -p /path_to/E3SM/components/mpas-ocean -f /path_to/compass-env-only/compass.cfg
```

### Execute the initial state using Compass and partition the mesh using Gpmetis

Assumes that `load_dev_compass_1.4.0-alpha.2.sh` is the name of the conda environment load script created initially
Also requires loading the spack environment so that mpiexec executes the same mpi that was used to build mpas-ocean.
However, the LowFive VOL connector needs to be unset.

```
source /path_to/compass-env-only/load_dev_compass_1.4.0-alpha.2.sh
source /path_to/mpas-ocean-workflow/load-mpas.sh
unset HDF5_VOL_CONNECTOR
unset HDF5_PLUGIN_PATH
cd /path_to/compass-baroclinic-test/ocean/baroclinic_channel/10km/default/initial_state
compass run
cd ../forward
gpmetis graph.info 4
```

-----

## Customize the MPAS-Ocean test case configuration files

### First time: edit `namelist.ocean`

Edit `/path_to/compass-baroclinic-test/ocean/baroclinic_channel/10km/default/forward/namelist.ocean`.

Set `config_pio_num_iotasks = 4` and `config_pio_stride = 1` in the `&io` section of the file:

```
&io
    config_write_output_on_startup = .false.
    config_pio_num_iotasks = 4
    config_pio_stride = 1
/
```

### First time: edit `streams.ocean`

Set the output file type for the test case:

Edit `/path_to/compass-baroclinic-test/ocean/baroclinic_channel/10km/default/forward/streams.ocean`.

Add `io_type="netcdf4">` to the `<stream name="output"` section of the file:

```
<stream name="output"
        type="output"
        filename_template="output.nc"
        filename_interval="01-00-00_00:00:00"
        reference_time="0001-01-01_00:00:00"
        clobber_mode="truncate"
        precision="double"
        output_interval="0000_00:00:01"
        io_type="netcdf4">

    <var_struct name="tracers"/>
    <var name="xtime"/>
    <var name="normalVelocity"/>
    <var name="layerThickness"/>
</stream>
```

If you want to use the output for particle tracing, append additional stream `mesh` and additional variables `ssh`, `normalTransportVelocity`, `vertTransportVelocityTop`, `zTop`, `vertVelocityTop`, and `zMid`  to the `stream name="output"` section of
the `streams.ocean` file:

```
<stream name="output"
        type="output"
        filename_template="output.nc"
        filename_interval="01-00-00_00:00:00"
        reference_time="0001-01-01_00:00:00"
        clobber_mode="truncate"
        precision="double"
        output_interval="0000_00:00:01"
        io_type="netcdf4">

    <var_struct name="tracers"/>
    <var name="xtime"/>
    <var name="normalVelocity"/>
    <var name="layerThickness"/>
    <stream name="mesh"/>
    <var name="ssh"/>
    <var name="normalTransportVelocity"/>
    <var name="vertTransportVelocityTop"/>
    <var name="zTop"/>
    <var name="vertVelocityTop"/>
    <var name="zMid"/>
</stream>
```
-----

## Rebuild MPAS-Ocean and build the consumer application to run in a workflow

### First time: patch MPAS-Ocean

```
cd /path_to/E3SM
git apply /path_to/mpas-o-workflow/E3SM.patch
```

### First time: rebuild MPAS-Ocean as a shared object so that it can be loaded dynamicallly by the workflow

```
cd /path_to/E3SM/components/mpas-ocean
make clean              # if dirty
make -j gfortran
```
This will take ~ 5 minutes to compile.

-----

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

## Running the remap example with placeholders for the simulation codes

```
cd $MOAB_WORKFLOW_PATH/bin
cd remap
./wilkins-run.sh
```
-----

## Running the remapping workflow with MPAS-Ocean

```
cd $MOAB_WORKFLOW_PATH/bin
cd mpas_remap
./wilkins-run.sh
```

