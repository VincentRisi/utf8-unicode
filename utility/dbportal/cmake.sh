mkdir -p ../build
cd ../build
CC=/opt/IBM/xlC/13.1.3/bin/xlC CXX=/opt/IBM/xlC/13.1.3/bin/xlC cmake -DCMAKE_BUILD_TYPE=Debug -DCREATE_INSTALL_PACKAGE=ON -DCMAKE_INSTALL_PREFIX=/main/nedcor/bbd/install -DCMAKE_CXX_FLAGS="-qlanglvl=noansifor -g -M -brtl -qstaticinline -qnolm -D_H_MACROS -qnolm" ../source
