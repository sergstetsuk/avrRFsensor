#!/usr/bin/perl

#typedef struct {
#    unsigned short DstID;
#    unsigned short SrcID;
#    unsigned char Cmd;
#    unsigned char Options;
#    unsigned short ErrID;
#    unsigned long ErrTickCounter;
#    unsigned short ExtraInfo;
#    unsigned short DebugInfo;
#    } PacketStruc;

#Reading configuration file
open(MYINPUTFILE, "<checker.conf");
while(<MYINPUTFILE>) {
    $line = $_;
	if ($line =~ m/^\s*check\s+(\d+)\s*$/) {
        $CHECKLIST{hex $1} = 1;
    } elsif ($line =~ m/^\s*link\s+(\d+)\s*$/) {
        $LINKSLIST{hex $1} = 1;
    } elsif ($line =~ m/^\s*myid\s+(\d+)\s*$/) {
        $myid = $1;
    }
}
close(MYINPUTFILE);

print "Введіть номер маячка:";
$id = <>;
chomp($id);

if($CHECKLIST{$id} == 1 || $LINKSLIST{$id} == 1) {
    $commandline = sprintf("../commandline/avrrftool.a --operation writepacket --buffer 0x00 0x%02X 0x%02X 0x%02X 0x%02X 0x04 0x00 0x%02X 0x%02X",$id&0xFF, ($id>>8)&0xFF, $myid&0xFF, ($myid>>8)&0xFF, $id&0xFF, ($id>>8)&0xFF);
    print $commandline."\n";
    print `$commandline`;
} else {
    foreach $linkid (keys(%LINKSLIST)) {
        $commandline = sprintf("../commandline/avrrftool.a --operation writepacket --buffer 0x00 0x%02X 0x%02X 0x%02X 0x%02X 0x04 0x00 0x%02X 0x%02X",$linkid&0xFF, ($linkid>>8)&0xFF, $myid&0xFF, ($myid>>8)&0xFF, $id&0xFF, ($id>>8)&0xFF);
        print $commandline."\n";
        print `$commandline`;
        sleep(3);
    }
}
