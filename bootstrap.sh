#! /bin/sh

./cleanup.sh
( cd tools/utilities/morpho && ./generate_morpho.sh . )
autoreconf -v -f -i
( cd oln/conf && aclocal -I .. -I ../.. && autoconf )
echo
echo "Reconfiguration done."
