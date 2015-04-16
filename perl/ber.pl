#!/usr/bin/perl

$prevres = "";
$sent = 0;
$recv = 0;

for(;;){
    $res = `../commandline/avrrftool.a -o writepacket -b 0 3 0 1 0 1`;
    print $res;
    $sent++;
    print "SENT: $sent\n";
    sleep(5);
}
