#!/usr/bin/perl


for(;;){
    $res = `../commandline/hidtool.a -read 0x01`;
    if( $res =~ m/0x(90) 0x(aa) 0x(..) 0x(..) 0x(..) 0x(..)/) {
        $id = hex $2.$1;
        $int = hex $6.$5.$4.$3;
        
        $sec = $int%60;
        $min = $int/60%60;
        $hour = $int/3600%24;
        $day = $int/3600/24;
        $msg = sprintf("%04X %02d %02d:%02d:%02d: ALARM!!!\n",$id,$day,$hour,$min,$sec);
        #here we will playback specific audio file;
        ($lsec,$lmin,$lhour,$lmday,$lmon,$lyear,$lwday,$lyday,$lisdst) = localtime(time);
        $lmsg = sprintf("%04d.%02d.%02d %02d:%02d:%02d: ",$lyear+1900,$lmon,$lmday,$lhour,$lmin,$lsec);
        print $lmsg.$msg;
        $res = `play -q -n -c1 synth sin %-12 sin %-9 sin %-5 sin %-2 fade h 0.1 0.25 0.1`;
        open(LOG_FILE,">>","checker.log");
        print LOG_FILE $lmsg.$msg;
        close(LOG_FILE);
    }
    #~ sleep(1);
}