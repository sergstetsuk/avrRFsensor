#!/usr/bin/perl

$prevint = -1;
$rcvcounter = 0;
$errcounter = 0;

for(;;){
    $res = `../commandline/hidtool.a -operation readpacket`;
    if(!($res =~ m/^0x00 0x00 0x00 0x00 0x00 0x00/) && ($res =~ m/^0x/)) {
        if($res =~ m/0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..)/) {
            $id = hex $2.$1;
            $int = hex $6.$5.$4.$3;
            
            $sec = $int%60;
            $min = $int/60%60;
            $hour = $int/3600%24;
            $day = $int/3600/24;
            
            if($prevint <0) {
                $prevint = $int - 4;
            }
            $rcvcounter++;
            $errcounter += ($int - $prevint)/4-1;
            $prevint = $int;
            print $res."\n";
            print $rcvcounter." ".$errcounter." ".($rcvcounter+$errcounter)."\n";
            $ber = $errcounter/($rcvcounter+$errcounter);
            $msg = sprintf("%04X %02d %02d:%02d:%02d: ALARM [%d, %d, %f]!!!\n",$id,$day,$hour,$min,$sec,$rcvcounter,$errcounter,$ber);
            #here we will playback specific audio file;
            ($lsec,$lmin,$lhour,$lmday,$lmon,$lyear,$lwday,$lyday,$lisdst) = localtime(time);
            $lmsg = sprintf("%04d.%02d.%02d %02d:%02d:%02d: ",$lyear+1900,$lmon,$lmday,$lhour,$lmin,$lsec);
            print $lmsg.$msg;
            $res = `play -q -n -c1 synth sin %-12 sin %-9 sin %-5 sin %-2 fade h 0.1 0.25 0.1`;
            open(LOG_FILE,">>","checker.log");
            print LOG_FILE $lmsg.$msg.$res;
            close(LOG_FILE);
        }
    }
    #~ sleep(1);
}