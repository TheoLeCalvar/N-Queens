#!/usr/bin/perl

use strict;

my $outFile;

open $outFile, '>>', 'results.txt' or die "Can't open resultats.txt\n";

for (my $size = 4; $size < 50; $size++) {
        my $output = `gtime -p ./n-dames -a bt $size 2>&1`;

        my ($res, $realTime, $userTime) = ($output =~ /(.*?)\nreal (.*?)\nuser (.*?)\nsys \d/);

        $res =~ tr/ //d;

        print{$outFile}("$size;$realTime;$userTime;$res\n");
        print("$size;$realTime;$userTime;$res\n");
}