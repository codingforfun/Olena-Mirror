#! /bin/sh

./cleanup.sh
( cd tools/utilities/morpho && ./generate_morpho.sh . )
autoreconf -v -f -i
echo
echo "Reconfiguration done."
