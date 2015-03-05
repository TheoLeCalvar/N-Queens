#!/usr/bin/perl

use strict;

my $function = 'bt';

if ($#ARGV+1 > 0)  {
        $function = $ARGV[0];
}

my $outFile;

open $outFile, '>', 'results.txt' or die "Can't open resultats.txt\n";

print{$outFile}("$function\nsize;realtime;res\n");
print("$function\nsize;realtime;res\n");

for (my $size = 4; $size < 50; $size++) {
        my $output = `gtime -p ./n-dames -a $function $size 2>&1`;

        my ($res, $realTime, $userTime) = ($output =~ /(.*?)\nreal (.*?)\nuser (.*?)\nsys \d/);

        $res =~ tr/ //d;

        print{$outFile}("$size;$realTime;$res\n");
        print("$size;$realTime;$res\n");
}