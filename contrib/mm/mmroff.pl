#!@PERL@
# Copyright (C) 1989-2023 Free Software Foundation, Inc.
#
# This file is part of groff.
#
# groff is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# groff is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use strict;
use warnings;

use Config;

(my $progname = $0) =~s @.*/@@;

# runs groff in safe mode, that seems to be the default
# installation now. That means that I have to fix all nice
# features outside groff. Sigh.
# I do agree however that the previous way opened a whole bunch
# of security holes.

my $no_exec;

if (grep(/^--help$/, @ARGV)) {
	print "usage: mmroff [-x] [groff-option ...] [file ...]\n";
	print "usage: mmroff --version\n";
	print "usage: mmroff --help\n";
	exit;
}

if (grep(/^--version$/, @ARGV)) {
	print "mmroff (groff) @VERSION@\n";
	exit;
}

# check for -x and remove it
if (grep(/^-x$/, @ARGV)) {
	$no_exec++;
	@ARGV = grep(!/^-x$/, @ARGV);
}

# Locate groff executable.
my $path = $ENV{'GROFF_BIN_PATH'};
my $groff;

if ($path) {
	for my $dir (split($Config{path_sep}, $path)) {
		my $candidate = "$dir/groff";
		if (-x $candidate) {
			$groff = $candidate;
			last;
		}
	}
}

$groff = "groff" if (!$groff);

# mmroff should always have -mm, but not twice
@ARGV = grep(!/^-mm$/, @ARGV);
my $first_pass = "$groff -rRef=1 -z -mm @ARGV";
my $second_pass = "$groff -mm @ARGV";

my (%cur, $rfilename, $imacro, @out, @indi);
my $max_height = 0;
my $max_width = 0;
open(MACRO, "$first_pass 2>&1 |") || die "run $first_pass:$!";
while(<MACRO>) {
	if (m#^\.\\" Rfilename: (\S+)#) {
		# remove all directories just to be more secure
		($rfilename = $1) =~ s#.*/##;
		next;
	}
	if (m#^\.\\" Imacro: (\S+)#) {
		# remove all directories just to be more secure
		($imacro = $1) =~ s#.*/##;
		next;
	}
	if (m#^\.\\" Index: (\S+)#) {
		# remove all directories just to be more secure
		my $f;
		($f = $1) =~ s#.*/##;
		&print_index($f, \@indi, $imacro);
		@indi = ();
		$imacro = '';
		next;
	}
	my $x;
	if (($x) = m#^\.\\" IND (.+)#) {
		$x =~ s#\\##g;
		my @x = split(/\t/, $x);
		grep(s/\s+$//, @x);
		push(@indi, join("\t", @x));
		next;
	}
	if (m#^\.\\" PIC id (\d+)#) {
		%cur = ('id' => $1);
		next;
	}
	if (m#^\.\\" PIC file (\S+)#) {
		&psbb($1);
		&ps_calc($1);
		next;
	}
	if (m#^\.\\" PIC (\w+)\s+(\S+)#) {
		eval "\$cur{'$1'} = '$2'";
		next;
	}
	s#\\ \\ $##;
	push(@out, $_);
}
close(MACRO);

sub Die {
	print STDERR "$progname: fatal error: @_\n";
	exit 1;
}

if ($rfilename) {
	push(@out, ".nr pict*max-height $max_height\n") if defined $max_height;
	push(@out, ".nr pict*max-width $max_width\n") if defined $max_width;

	open(OUT, ">$rfilename")
		or &Die("unable to create $rfilename:$!");
	print OUT '.\" references', "\n";
	my $i;
	for $i (@out) {
		print OUT $i;
	}
	close(OUT);
}

exit 0 if $no_exec;
exit system($second_pass);

sub print_index {
	my ($f, $ind, $macro) = @_;

	open(OUT, ">$f") or &Die("unable to create $f:$!");
	my $i;
	for $i (sort @$ind) {
		if ($macro) {
			$i = '.'.$macro.' "'.join('" "', split(/\t/, $i)).'"';
		}
		print OUT "$i\n";
	}
	close(OUT);
}

sub ps_calc {
	return unless exists($cur{'llx'});
	my ($f) = @_;
	my $w = abs($cur{'llx'}-$cur{'urx'});
	my $h = abs($cur{'lly'}-$cur{'ury'});
	$max_width = $w if $w > $max_width;
	$max_height = $h if $h > $max_height;

	my $id = $cur{'id'};
	push(@out, ".ds pict*file!$id $f\n");
	push(@out, ".ds pict*id!$f $id\n");
	push(@out, ".nr pict*llx!$id $cur{'llx'}\n");
	push(@out, ".nr pict*lly!$id $cur{'lly'}\n");
	push(@out, ".nr pict*urx!$id $cur{'urx'}\n");
	push(@out, ".nr pict*ury!$id $cur{'ury'}\n");
	push(@out, ".nr pict*w!$id $w\n");
	push(@out, ".nr pict*h!$id $h\n");
}

sub psbb {
	my ($f) = @_;

	unless (open(IN, $f)) {
		print STDERR "$progname: warning: cannot open"
			. " PostScript file $f: $!\n";
		return;
	}
	while(<IN>) {
		if (/^%%BoundingBox:\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)/) {
			$cur{'llx'} = $1;
			$cur{'lly'} = $2;
			$cur{'urx'} = $3;
			$cur{'ury'} = $4;
		}
	}
	close(IN);
}

1;
# Local Variables:
# mode: CPerl
# End:
