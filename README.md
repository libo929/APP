# APP
The APRIL practice of shower reconstruction for calorimeter prototype

How to compile
1) git clone https://github.com/libo929/APP.git
2) cd APP; mkdir build; cd build
3) cmake .. -DCMAKE_PREFIX_PATH=${PandoraPFA_DIR} -Dmlpack_DIR=${mlpack_DIR} -DPANDORA_MONITORING=ON -DBoost_INCLUDE_DIR=${boost_include} \
-DARMADILLO_LIBRARY=${armadillo_lib} -DARMADILLO_INCLUDE_DIR=${armadillo_include}
4) make install
