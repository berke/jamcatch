#!/usr/bin/perl

use strict;
require 5.003;

my $mode = 0;
my %font;
my $char;

while (<STDIN>) {
	chomp;
	next if (/^\;/ || /^\s*$/);
	my $line = $_;

	if (!$mode) {
		if ($line =~ /^\\([0-7]+)/) {
			$char = oct $1;
		} else {
			$char = ord $line;
		}
		print STDERR "--- line $. character $char\n";
		$mode = 1;
		$font{$char} = "\t\t{ /* character " . $char . " */\n";
	} else {
		$line =~ s/(\-|\#)/($& eq "-")?"0,":"1,"/eg;
		$font{$char} .= "\t\t\t{ " . $line . "},\n";
		$mode++;
		if ($mode > 8) {
			$font{$char} .= "\t\t},\n";
			$mode = 0;
		}
	}
}

print "#define FONT_NOCHAR {" . (("{" . ("0," x 6) . "},") x 8) . "}\n\n";
print "char font[256][8][6]={\n";

my $i;

for ($i = 0; $i<256; $i++) {
	if ($font{$i}) {
		print $font{$i};
	} else {
		print "\t\tFONT_NOCHAR,\n";
	}
}

print "};\n";
