#!/usr/bin/perl
    open(LOG_FILE,">>","looker.log");
    print LOG_FILE localtime(time).": Program Started\n";
    close(LOG_FILE);

    $prevres = "";

for(;;){
    $res = `../commandline/avrrftool.a --operation readpacket`;
    if($res ne $prevres) {
        $prevres = $res;
        print localtime(time).":\n".$res;
        open(LOG_FILE,">>","looker.log");
        print LOG_FILE localtime(time).":\n".$res;
        close(LOG_FILE);
    }
}
