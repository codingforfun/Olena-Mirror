#! /bin/sh

./cleanup.sh
( cd tools/utilities/morpho && ./generate_morpho.sh . )
autoreconf -v -f -i
( cd oln/conf && aclocal -I ../config && autoconf -f )
echo
echo "Reconfiguration done."
