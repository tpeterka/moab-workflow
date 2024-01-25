echo "Running 1 producer process + 1 consumer process"
mpirun -n 2 -l python3 -u ./wilkins-master.py ./wilkins-config.yaml

