#! /bin/sh

ACLOCAL="aclocal -I `pwd`/oln" 
export ACLOCAL
autoreconf -v -f -i
echo
echo "Reconfiguration done. For now, set ACLOCAL = $ACLOCAL in your environment"
