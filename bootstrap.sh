#! /bin/sh

sh ./cleanup.sh
( cd tools/utilities/morpho && sh ./generate_morpho.sh . )
( cd doc/ref && sh ./gen_filelists.sh filelists.make ../../oln/oln )
autoreconf -v -f -i
( cd oln/conf && aclocal -I ../config && autoconf -f )
echo
echo "Reconfiguration done."
