#!/usr/bin/perl

$prevres = "";
$sent = 0;
$recv = 0;

for(;;){
    #~ $res = `../commandline/avrrftool.a -o writepacket -b 0 2 0 1 0 1`;
    #~ print $res;
    #~ $sent++;
    #~ sleep(2);
    $res = `../commandline/avrrftool.a -o readpacket`;
    if($res =~ m/^0x0[^0] 0x0[^0] 0x0[^0]/) {
        print "$res";
        $recv++;
    }
    print "SENT: $sent RECV: $recv\n";
    sleep(1);
}
