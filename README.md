ensure protobuf is installed at a sufficient version (so as to include utils)

on the Cambridge HPC cluster, protobuf may be installed in `~/` without `sudo`. `$LD_LIBRARY_PATH` must be updated to include `~/lib`. Next, `-L~/lib` and `-I~/lib` flags are added to the `makefile`, before all system directories (to avoid conflict with an incompatible version of protobuf). `CXX` must support C++14 - `module load gcc-7.2.0-gcc-4.8.5-pqn7o2k` will enable appropriate compilers.

