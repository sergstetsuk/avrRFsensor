#!/usr/bin/perl

$prevres = "";
$sent = 0;
$recv = 0;

for(;;){
    $res = `../commandline/avrrftool.a -o writepacket -b 0 7 0 1 0 1`;
    print $res;
    $sent++;
    sleep(5);
    $res = `../commandline/avrrftool.a -o readpacket`;
    print "$res";
    if($res =~ m/^0x01 0x00 0x07 0x00 0x02 0x00/) {
        $recv++;
    }
    print "SENT: $sent\n";
    print "RECV: $recv\n";
}
