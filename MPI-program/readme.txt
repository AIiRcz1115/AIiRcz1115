AKS:

mpirun -np 4 -hosts localhost:1,localhost:2,localhost:3,localhost:4 ./prime aks 2851

Miller-Rabin:

mpirun -np 4 -hosts localhost:1,localhost:2,localhost:3,localhost:4 ./prime mr 2851 700000
