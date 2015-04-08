#!/usr/bin/perl

$prevres = "";

for(;;){
    $res = `../commandline/avrrftool.a --operation readpacket`;
    if($res ne $prevres) {
        $prevres = $res;
        print $res;
    }
}
