#!/bin/sh
# $MirBSD: emacs-gen.sh,v 1.2 2004/05/24 19:56:05 tg Stab $
# $OpenBSD: emacs-gen.sh,v 1.1.1.1 1996/08/14 06:19:10 downsj Exp $

case $# in
1)	file=$1;;
*)
	echo "$0: Usage: $0 path-to-emacs.c" 1>&2
	exit 1
esac;

if [ ! -r "$file" ] ;then
	echo "$0: can't read $file" 1>&2
	exit 1
fi

cat << E_O_F || exit 1
/*
 * NOTE: THIS FILE WAS GENERATED AUTOMATICALLY FROM $file
 *
 * DO NOT BOTHER EDITING THIS FILE
 */
E_O_F

# Pass 1: print out lines before @START-FUNC-TAB@
#	  and generate defines and function declarations,
sed -e '1,/@START-FUNC-TAB@/d' -e '/@END-FUNC-TAB@/,$d' < $file |
	awk 'BEGIN { nfunc = 0; }
	    /^[	 ]*#/ {
			    print $0;
			    next;
		    }
	    {
		fname = $2;
		c = substr(fname, length(fname), 1);
		if (c == ",")
			fname = substr(fname, 1, length(fname) - 1);
		if (fname != "0") {
			printf "#define XFUNC_%s %d\n", substr(fname, 3, length(fname) - 2), nfunc;
			printf "static int %s ARGS((int c));\n", fname;
			nfunc++;
		}
	    }' || exit 1

exit 0
