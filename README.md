# MPI-KOKKOS_SYCL-comm-test
This is a test for the communication problem of MPI+KOKKOS_SYCL

1. Access ThetaGPU compute node (Interactive).
2. cd /grand/catalyst/world-shared/spatel
3. source set_DPCPP_env.sh
4. Setup OMPI

   module load openmpi
   
   export OMPI_CXX=clang++
5. cd ~
6. Download tests

   git clone https://github.com/lucaso19891019/MPI-KOKKOS_SYCL-comm-test

   cd  MPI-KOKKOS_SYCL-comm-test
  
7. Compile

	make
8. Run

	mpirun -n {even number or ranks} ./waiall_ex
