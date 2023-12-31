#!/bin/sh
#
# eqn2graph -- compile EQN equation descriptions to bitmap images
#
# by Eric S. Raymond <esr@thyrsus.com>, July 2002
#
# In Unixland, the magic is in knowing what to string together...
#
# Take an eqn equation on stdin, emit cropped bitmap on stdout.
# The eqn markup should *not* be wrapped in .EQ/.EN, this script will do that.
# A -format FOO option changes the image output format to any format
# supported by convert(1).  All other options are passed to convert(1).
# The default format is PNG.
#
# This is separate from pic2graph because pic processing has some weird
# clipping effect on the output, mangling equations that are very wide 
# or deep.  Besides, this tool can supply its own delimiters.
#

# Requires the groff suite and the ImageMagick tools.  Both are open source.
# This code is released to the public domain.
#
# Here are the assumptions behind the option processing:
#
# 1. None of the options of eqn(1) are relevant.
#
# 2. Many options of convert(1) are potentially relevant, (especially
# -density, -interlace, -transparency, -border, and -comment).
#
# Thus, we pass everything except -format to convert(1).
#
convert_opts=""
convert_trim_arg="-trim"
format="png"

while [ "$1" ]
do
    case $1 in
    -format)
	format=$2
	shift;;
    -v | --version)
	echo "GNU eqn2graph (groff) version @VERSION@"
	exit 0;;
    --help)
	echo "usage: eqn2graph [ option ...] < in > out"
	exit 0;;
    *)
	convert_opts="$convert_opts $1";;
    esac
    shift
done

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

if ! tmp=`(umask 077 && mktemp -d -q "$d/eqn2graph-XXXXXX") 2> /dev/null`
then
    # mktemp failed--not installed or is a version that doesn't support those
    # flags?  Fall back to older method which uses more predictable naming.
    #
    # $RANDOM is a Bashism.  The fallback of $PPID is not good pseudorandomness,
    # but is supported by the stripped-down dash shell, for instance.
    tmp="$d/eqn2graph$$-${RANDOM:-$PPID}"
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
# 1. Add .EQ/.EN.
# 2. Process through eqn(1) to emit troff markup.
# 3. Process through groff(1) to emit Postscript.
# 4. Use convert(1) to crop the Postscript and turn it into a bitmap.
read equation
(echo ".EQ"; echo 'delim $$'; echo ".EN"; echo '$'"$equation"'$') | \
	groff -e -Tps -P-pletter > "$tmp"/eqn2graph.ps \
	&& convert $convert_trim_arg $convert_opts "$tmp"/eqn2graph.ps \
	   "$tmp"/eqn2graph.$format \
	&& cat "$tmp"/eqn2graph.$format

# End
