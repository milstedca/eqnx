#!/usr/bin/env perl
#
#
# hyphenex.pl
#
# This small filter converts a hyphenation exception log article for
# TUGBoat to a real \hyphenation block.
#
# Written by Werner Lemberg <wl@gnu.org>.
#
# Version 1.2 (2007/11/16)
#
# Public domain.
#
#
# Usage:
#
#   [perl] hyphenex.pl < tugboat-article > hyphenation-exceptions

# print header
print "% Hyphenation exceptions for US English,\n";
print "% based on hyphenation exception log articles in TUGboat.\n";
print "%\n";
print "% Copyright 2007 TeX Users Group.\n";
print "% You may freely use, modify and/or distribute this file.\n";
print "%\n";
print "% This is an automatically generated file.  Do not edit!\n";
print "%\n";
print "% Please contact the TUGboat editorial staff <tugboat\@tug.org>\n";
print "% for corrections and omissions.\n";
print "\n";
print "\\hyphenation{\n";

unshift @ARGV, '-' unless @ARGV;
foreach my $filename (@ARGV) {
  my $input;
  if ($filename eq '-') {
    $input = \*STDIN;
  } elsif (not open $input, '<', $filename) {
    warn $!;
    next;
  }
  while (<$input>) {
    # retain only lines starting with \1 ... \6 or \tabalign
    next if not (m/^\\[123456]/ || m/^\\tabalign/);
    # remove final newline
    chop;
    # remove all TeX commands except \1 ... \6
    s/\\[^123456\s{]+//g;
    # remove all paired { ... }
    1 while s/{(.*?)}/\1/g;
    # skip lines which now have only whitespace before '&'
    next if m/^\s*&/;
    # remove comments
    s/%.*//;
    # remove trailing whitespace
    s/\s*$//;
    # remove trailing '*' (used as a marker in the document)
    s/\*$//;
    # split at whitespace
    @field = split(' ');
    if ($field[0] eq "\\1" || $field[0] eq "\\4") {
      print "  $field[2]\n";
    }
    elsif ($field[0] eq "\\2" || $field[0] eq "\\5") {
      print "  $field[2]\n";
      # handle multiple suffixes separated by commata
      @suffix_list = split(/,/, "$field[3]");
      foreach $suffix (@suffix_list) {
        print "  $field[2]$suffix\n";
      }
    }
    elsif ($field[0] eq "\\3" || $field[0] eq "\\6") {
      # handle multiple suffixes separated by commata
      @suffix_list = split(/,/, "$field[3],$field[4]");
      foreach $suffix (@suffix_list) {
        print "  $field[2]$suffix\n";
      }
    }
    else {
      # for '&', split at '&' with trailing whitespace
      @field = split(/&\s*/);
      print "  $field[1]\n";
    }
  }
}

# print trailer
print "}\n";
print "\n";
print "% EOF\n";
