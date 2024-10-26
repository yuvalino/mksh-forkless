#!/bin/sh
# $MirBSD: th.sh,v 1.2 2004/05/24 19:56:25 tg Stab $
# $OpenBSD: th.sh,v 1.4 2001/01/28 23:04:57 niklas Exp $
#
# Simple script to find perl and run it
#

# Avoid common problems with ENV (though perl shouldn't let it through)
# (can you believe some shells don't have an unset???)
unset ENV

x=x
[ -x /bin/sh ] 2> /dev/null || x=f

IFS=:$IFS
perl=
for i in $PATH; do
    [ X"$i" = X ] && i=.
    for j in perl perl4 perl5 ; do
	[ -$x "$i/$j" ] && perl=$i/$j && break 2
    done
done

[ X"$perl" = X ] && {
	echo "$0: can't find perl - bye\n" 1>&2
	exit 1
    }

exec $perl "$@"
