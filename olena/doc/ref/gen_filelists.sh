#! /bin/sh

if [ "x$1" = "x" ]; then
    echo "usage: $0 <output> <oln directory>"
    exit 1
fi

# Morpho lists

echo -n "morpho_files = " >"$1"
for i in "$2"/morpho/*.hh "$2"/morpho/*.inc; do
    if cat "$i" 2>/dev/null | grep '/\*=' >/dev/null 2>&1; then
       echo "\\" >>"$1"
       echo -n "  \$(olnsrc)/"`echo "$i" | sed -e 's,^'"$2"'/,,g'`" " >>"$1"
    fi
done
echo >>"$1"

# Level lists

echo -n "level_files = " >>"$1"
for i in "$2"/level/*.hh; do
    if cat "$i" 2>/dev/null | grep '/\*=' >/dev/null 2>&1; then
       echo "\\" >>"$1"
       echo -n "  \$(olnsrc)/"`echo "$i" | sed -e 's,^'"$2"'/,,g'`" " >>"$1"
    fi
done
echo >>"$1"



