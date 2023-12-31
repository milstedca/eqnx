#!/bin/sh
#
# pic2graph -- compile PIC image descriptions to bitmap images
#
# by Eric S. Raymond <esr@thyrsus.com>, July 2002

# In Unixland, the magic is in knowing what to string together...
#
# Take a pic/eqn diagram on stdin, emit cropped bitmap on stdout.
# The pic markup should *not* be wrapped in .PS/.PE, this script will do that.
# An -unsafe option on the command line enables gpic/groff "unsafe" mode.
# A -format FOO option changes the image output format to any format
# supported by convert(1).  An -eqn option changes the eqn delimiters.
# All other options are passed to convert(1).  The default format in PNG.
#
# Requires the groff suite and the ImageMagick tools.  Both are open source.
# This code is released to the public domain.
#
# Here are the assumptions behind the option processing:
#
# 1. Only the -U option of gpic(1) is relevant.  -C doesn't matter because
#    we're generating our own .PS/.PE, -[ntcz] are irrelevant because we're
#    generating Postscript.
#
# 2. Ditto for groff(1), though it's a longer and more tedious demonstration.
#
# 3. Many options of convert(1) are potentially relevant (especially 
#    -density, -interlace, -transparency, -border, and -comment).
#
# Thus, we pass -U to gpic and groff, and everything else to convert(1).
#
# We don't have complete option coverage on eqn because this is primarily
# intended as a pic translator; we can live with eqn defaults. 
#
groffpic_opts=""
convert_opts=""
convert_trim_arg="-trim"
format="png"
eqndelim='$$'

while [ "$1" ]
do
    case $1 in
    -unsafe)
	groffpic_opts="-U";;
    -format)
	format=$2
	shift;;
    -eqn)
	eqndelim=$2
	shift;;
    -v | --version)
	echo "GNU pic2graph (groff) version @VERSION@"
	exit 0;;
    --help)
	echo "usage: pic2graph [ option ...] < in > out"
	exit 0;;
    *)
	convert_opts="$convert_opts $1";;
    esac
    shift
done

if [ "$eqndelim" ]
then
    eqndelim="delim $eqndelim"
fi

# create temporary directory
tmp=
for d in "$GROFF_TMPDIR" "$TMPDIR" "$TMP" "$TEMP" /tmp
do
    test -n "$d" && break
done

if ! test -d "$d"
then
    echo "$0: error: temporary directory \"$d\" does not exist or is" \
        "not a directory" >&2
    exit 1
fi

if ! tmp=`(umask 077 && mktemp -d -q "$d/pic2graph-XXXXXX") 2> /dev/null`
then
    # mktemp failed--not installed or is a version that doesn't support those
    # flags?  Fall back to older method which uses more predictable naming.
    #
    # $RANDOM is a Bashism.  The fallback of $PPID is not good pseudorandomness,
    # but is supported by the stripped-down dash shell, for instance.
    tmp="$d/pic2graph$$-${RANDOM:-$PPID}"
    (umask 077 && mkdir "$tmp") 2> /dev/null
fi

if ! test -d "$tmp"
then
    echo "$0: error: cannot create temporary directory \"$tmp\"" >&2
    exit 1
fi

# See if the installed version of convert(1) is new enough to support the -trim
# option.  Versions that didn't were described as "old" as early as 2008.
is_convert_recent=`convert -help | grep -e -trim`
if test -z "$is_convert_recent"
then
    echo "$0: warning: falling back to old '-crop 0x0' trim method" >&2
    convert_trim_arg="-crop 0x0"
fi

trap 'exit_status=$?; rm -rf "$tmp" && exit $exit_status' EXIT INT TERM

# Here goes:
# 1. Wrap the input in dummy .PS/PE macros (and add possibly null .EQ/.EN)
# 2. Process through eqn and pic to emit troff markup.
# 3. Process through groff to emit Postscript.
# 4. Use convert(1) to crop the PostScript and turn it into a bitmap.
(echo ".EQ"; echo $eqndelim; echo ".EN"; echo ".PS"; cat; echo ".PE") | \
    groff -e -p $groffpic_opts -Tps -P-pletter > "$tmp"/pic2graph.ps \
    && convert $convert_trim_arg $convert_opts "$tmp"/pic2graph.ps \
       "$tmp"/pic2graph.$format \
    && cat "$tmp"/pic2graph.$format

# End
