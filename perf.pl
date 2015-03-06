#!/usr/bin/perl

use strict;

my $function = 'bt';
my $min = 4;
my $max = 50;
my $timeout = '10s';

if ($#ARGV+1 > 1) {
        $function = $ARGV[0];
        $min = $ARGV[1];
        $max = $ARGV[2];
        $timeout = $ARGV[3];
}
else {
        print("usage: perf.pl method minSize maxSize timeout\n");
        exit(0);
}

my $outFile;

open $outFile, '>', 'res/results.txt' or die "Can't open resultats.txt\n";

print{$outFile}("$function\nsize;realtime;res\n");
print("$function\nsize;realtime;res\n");

for (my $size = $min; $size < $max; $size++) {
        my $output = `timeout $timeout gtime -p ./n-dames -a $function $size 2>&1`;

        my ($res, $realTime, $userTime) = ($output =~ /(.*?)\nreal (.*?)\nuser (.*?)\nsys \d/);

        $res =~ tr/ //d;

        print{$outFile}("$size;$realTime;$res\n");
        print("$size;$realTime;$res\n");
}