#!/bin/bash

# prepare the legacy ROOT version
cd /home/philipp/Software/root_v5.34.32
source bin/thisroot.sh
cd /home/philipp/Private/T2K/EM4T2K

# add libgem
export LD_LIBRARY_PATH=/home/philipp/Private/T2K/libgem/:${LD_LIBRARY_PATH} 
