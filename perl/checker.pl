#!/usr/bin/perl
#Name: checker.pl
#Project: avrRFsensor
#Author: Serg Stetsuk <serg_stetsuk@ukr.net>
#Creation Date: 2015.03.18
#Tabsize: 4
#Copyright: (c) 2014-2015 Serg Stetsuk
#License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
#This Revision: $Id: checker.pl 1 2015.03.18 16:04 $
#

$prevtick = -1;
$rcvcounter = 0;
$errcounter = 0;
#~ %CONFIG = {};
#~ @CHEKCLIST=[];
@ALARMLIST = ("UNDEF","RESTART","NOLINK","NOANSWER");
$LOGLEVEL = 0;
    
$checklistcnt = 0;
$checkitem = 0;

##LOG information about checker restart
($lsec,$lmin,$lhour,$lmday,$lmon,$lyear,$lwday,$lyday,$lisdst) = localtime(time);
$lmsg = sprintf("%04d.%02d.%02d %02d:%02d:%02d: ",$lyear+1900,$lmon,$lmday,$lhour,$lmin,$lsec);
open(LOG_FILE,">>","checker.log");
print LOG_FILE $lmsg." checker.pl Started\n";
close(LOG_FILE);

#Reading configuration file
open(MYINPUTFILE, "<checker.conf");
while(<MYINPUTFILE>) {
    $line = $_;
	if ($line =~ m/^\s*alarm\s+(\d+)\s+"([^"]+)"\s+"([^"]+)"\s*$/) {
    $CONFIGAUDIO{hex $1} = $2;
    $CONFIGPIC{hex $1} = $3;
    } elsif ($line =~ m/^\s*check\s+(\d+)\s*$/) {
        $CHECKLIST[$checklistcnt++] = $1;
    } elsif ($line =~ m/^\s*myid\s+(\d+)\s*$/) {
        $MYID = $1;
    } elsif ($line =~ m/^\s*loglevel\s+(\d+)\s*$/) {
        $LOGLEVEL = $1;
    }
}
close(MYINPUTFILE);

#Initialization
    $Now = time;
    $Timer1 = $Now+10;
    $Timer2 = $Now+5;
    $CheckRetryCnt = 0;

if($LOGLEVEL > 0){
    print "START!\n"; #debug
}

#~ typedef struct {
    #~ unsigned short DstID;
    #~ unsigned short SrcID;
    #~ unsigned char Cmd;
    #~ unsigned char Options;
    #~ unsigned short ErrID;
    #~ unsigned long ErrTickCounter;
    #~ unsigned short ExtraInfo;
    #~ unsigned short DebugInfo;
    #~ } PacketStruc;
#Main loop
for(;;){
    $Now = time;
    $res = `../commandline/avrrftool.a --operation readpacket`;
    if(!($res =~ m/^0x00 0x00 0x00 0x00 0x00 0x00/) && ($res =~ m/^0x/)) {
        if($res =~ m/^0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..) 0x(..)$/) {
            print "\n"; #after points
            if($LOGLEVEL >3) {
                print $res;
            }
            $dstid = hex $2.$1;
            $srcid = hex $4.$3;
            $cmd = hex $5;
            $options = hex $6;
            $errid = hex $8.$7;
            $errtick = hex $12.$11.$10.$9;
            $extrainfo = hex $14.$13;
            $debuginfo = hex $16.$15;
            $noansid = $extrainfo;

            if($cmd == 0x02) {   #command not CM_ANSW
                if($LOGLEVEL > 2){
                    $sec = int ($errtick/4)%60;
                    $min = int ($errtick/4)/60%60;
                    $hour = int ($errtick/4)/3600%24;
                    $day = int ($errtick/4)/3600/24;

                    $msg = sprintf("ANSWER %d Tick: %d (%dd %02d:%02d:%02d)",$srcid,$errtick,$day,$hour,$min,$sec); #debug
                    print "$msg\n";
                }
                if($srcid == $CHECKLIST[$checkitem] && $dstid == $MYID) {
                    $CheckRetryCnt = 0;
                    $checkitem = ($checkitem+1) % $checklistcnt;
                    $Timer2 = $Now; #fasten next check
                    if($checkitem == 0) {
                        $Timer2 = $Now+5*60;
                    }
                }
            }
            if($cmd == 0x03) {   #command not CM_ALRM
                #print $res; #debug
                PlayAlarm($errid,$noanswerid,$errtick,$options);
            }
        }
    } elsif($Timer1 <= $Now) {
        $filename = $CONFIGPIC{0};
        `xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorLVDS/workspace0/last-image -n -t string -s $filename`;
        $Timer1 = $Now+365*24*60*60;    #too far from now.
    } elsif($Timer2 <= $Now && 0) {
        if ($CheckRetryCnt++ < 5){
            $checkid = sprintf("0x%02X 0x%02X", $CHECKLIST[$checkitem] & 0xFF, ($CHECKLIST[$checkitem]>>8) & 0xFF);
            $myid = sprintf("0x%02X 0x%02X", $MYID & 0xFF, ($MYID>>8) & 0xFF);
            if ($LOGLEVEL > 2){
                print "CHECK REQUEST ".$CHECKLIST[$checkitem]." CNT: $CheckRetryCnt\n"; #debug
                #~ print "../commandline/avrrftool.a -operation writepacket -buffer 0x00 $checkid 0x01 0x00 0x01\n"; #debug
            }
            `../commandline/avrrftool.a --operation writepacket --buffer 0x00 $checkid $myid 0x01`;
            $Timer2 = $Now+5;
        } else {
            PlayAlarm($MYID,$CHECKLIST[$checkitem],0,3);
            $checkitem = ($checkitem+1) % $checklistcnt;
            $CheckRetryCnt = 0;
            $Timer2 = $Now; #fasten next check
            if($checkitem == 0) {
                $Timer2 = $Now+5*60;
            }
        }
    }
    print ".";
    sleep(1);
}


sub PlayAlarm(){
    ($errid, $noanswerid, $ticcounter, $options) = @_;

    $sec = ($ticcounter/4)%60;
    $min = ($ticcounter/4)/60%60;
    $hour = ($ticcounter/4)/3600%24;
    $day = ($ticcounter/4)/3600/24;

    if($options != 3) {
        $noanswerid = $errid; #for other alarms we have now noanswerID
    }
    #format msg
        $msg = sprintf("%04X [%04X] %02d %02d:%02d:%02d: ALARM [%s]!\n",$errid,$noanswerid,$day,$hour,$min,$sec,$ALARMLIST[$options]);
    ($lsec,$lmin,$lhour,$lmday,$lmon,$lyear,$lwday,$lyday,$lisdst) = localtime(time);
    $lmsg = sprintf("%04d.%02d.%02d %02d:%02d:%02d: ",$lyear+1900,$lmon,$lmday,$lhour,$lmin,$lsec);
    #Show picture
    $filename = $CONFIGPIC{hex $noanswerid};
    `xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorLVDS/workspace0/last-image -n -t string -s $filename`;
    #print to the screen
    print $lmsg.$msg;
    #print to the log file
    open(LOG_FILE,">>","checker.log");
    print LOG_FILE $lmsg.$msg;
    close(LOG_FILE);
    #play audio file
    $filename = $CONFIGAUDIO{hex $noanswerid};
    #~ `play -q -n -c1 synth sin %-12 sin %-9 sin %-5 sin %-2 fade h 0.1 0.25 0.1`;
    #~ `play -q -n -c1 synth 0.5 sine 1600-100 synth 0.6 sine fmod 200-1400`
    #~ `play -q "|sox -n -p synth 2 sin 300-2000" "|sox -n -p synth  2 sin 2000-300"`;
    `play -q audio/siren.ogg`;
    `play -q $filename`;
    $Timer1 = $Now+10;
}