#! /bin/sh

sh ./cleanup.sh
( cd tools/utilities/morpho && sh ./generate_morpho.sh . )
( cd doc/ref && sh ./gen_filelists.sh filelists.make ../../olena/oln )
autoreconf -v -f -i
( cd olena/conf && aclocal -I ../config && autoconf -f )
echo
echo "Reconfiguration done."
