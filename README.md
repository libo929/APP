# APP
The APRIL practice of shower reconstruction for calorimeter prototype

How to compile
1) git clone https://github.com/libo929/APP.git
2) cd APP; mkdir build; cd build
3) cmake .. -DCMAKE_PREFIX_PATH=${PandoraPFA_DIR} -DAPRILContent_DIR=${APRILContent_DIR} -DPANDORA_MONITORING=ON
4) make install
