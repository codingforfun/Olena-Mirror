#! /usr/bin/perl -w

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.


use strict;
use IO::File;
use Text::Wrap;
use File::Basename;
use Getopt::Long qw(GetOptions);
use Pod::Usage qw(pod2usage);


sub conv($ $ $)
{
    my $file;
    my $d = $_[0];
    my $o = $_[1];
    my $preserve_name = 0;
    if ($_[2] =~ /^preserve$/)
    {
	$preserve_name = 1;
    }

    opendir DIR, "$d" or die "Unable to open $d.\n";
    foreach $file(readdir DIR)
    {
	if ($file =~ /(.*)\.(pgm|ppm|ppgm|pppgm|pbm)$/)
	{
	    if ($preserve_name == 1)
	    {
		print "convert $d/$file to $o/$1_$2.png\n";
		system "convert $d/$file $o/$1_$2.png";
	    }
	    else
	    {
		print "convert $d/$file to $o/$1.png\n";
		system "convert $d/$file $o/$1.png";
	    }
	}
    }
    closedir DIR or die "unable to close $d.";
}
my $argc=@ARGV;
if ($argc != 3)
{
   die "usage: img_conv.pl input-dir output-dir preserve|nopreserve\n";
}
conv $ARGV[0], $ARGV[1], $ARGV[2];
