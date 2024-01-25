echo "Running 1 producer process + 1 consumer process"
mpirun -n 2 -l python3 -u /home/tpeterka/software/moab-workflow/wilkins-master.py /home/tpeterka/software/moab-workflow/wilkins-config.yaml

