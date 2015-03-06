#!/usr/bin/perl

use strict;

my $function = 'bt';
my $max = 50;


if ($#ARGV+1 > 1) {
        $function = $ARGV[0];
        $max = $ARGV[1];
}

my $outFile;

open $outFile, '>', 'results.txt' or die "Can't open resultats.txt\n";

print{$outFile}("$function\nsize;realtime;res\n");
print("$function\nsize;realtime;res\n");

for (my $size = 4; $size < $max; $size++) {
        my $output = `gtime -p ./n-dames -a $function $size 2>&1`;

        my ($res, $realTime, $userTime) = ($output =~ /(.*?)\nreal (.*?)\nuser (.*?)\nsys \d/);

        $res =~ tr/ //d;

        print{$outFile}("$size;$realTime;$res\n");
        print("$size;$realTime;$res\n");
}