#!/bin/bash
source /storage/gpfs_data/cuore/users/vsharma/Miscalibration_sw/cuoresw/setup.sh
##printenv
##echo "LD LIBRARY PATH starts here"
##echo $LD_LIBRARY_PATH
cd /storage/gpfs_data/cuore/users/vsharma/Invisible_tri_nucleon_decay/macros/

##u238 chain
./getCorrelatedBkgs w /storage/gpfs_data/cuore/users/vsharma/Invisible_tri_nucleon_decay/cnaf/simulation_output/TeO2-ra226-pb210-Bk.root

##th232 chain
## ./getCorrelatedBkgs n /storage/gpfs_data/cuore/users/vsharma/Invisible_tri_nucleon_decay/cnaf/simulation_output/TeO2-ra228-pb208-Bk.root


