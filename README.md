# MPI-KOKKOS_SYCL-comm-test
This is a test for the communication problem of MPI+KOKKOS_SYCL

1. Access ThetaGPU compute node (Interactive).
2. cd /grand/catalyst/world-shared/spatel
3. source set_DPCPP_env.sh
4. module load openmpi
    export OMPI_CXX=clang++
5. Create a folder
	mkdir ~/test
	cd ~/test
6. Download tests
  
7. Compile
	make
8. Run
	mpirun -n {even number or ranks} ./waiall_ex
