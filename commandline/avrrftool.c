/* Name: hidtool.c
 * Project: hid-data example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-11
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hiddata.h"
#include "../firmware/usbconfig.h"  /* for device VID, PID, vendor name and product name */

/* ------------------------------------------------------------------------- */
typedef struct __attribute__((packed)){
    unsigned short ID;
    unsigned char DebugMode;
    unsigned char CryptKey[16];
    unsigned short Monitor[30];
    } RunTimeConfigStruc;
/* ------------------------------------------------------------------------- */

static char *usbErrorMessage(int errCode)
{
static char buffer[80];

    switch(errCode){
        case USBOPEN_ERR_ACCESS:      return "Access to device denied";
        case USBOPEN_ERR_NOTFOUND:    return "The specified device was not found";
        case USBOPEN_ERR_IO:          return "Communication error with device";
        default:
            sprintf(buffer, "Unknown USB error %d", errCode);
            return buffer;
    }
    return NULL;    /* not reached */
}

static usbDevice_t  *openDevice(void)
{
usbDevice_t     *dev = NULL;
unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};
int             vid = rawVid[0] + 256 * rawVid[1];
int             pid = rawPid[0] + 256 * rawPid[1];
int             err;

    if((err = usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 1/*usesReportIDs*/)) != 0){
        fprintf(stderr, "error finding %s: %s\n", productName, usbErrorMessage(err));
        return NULL;
    }
    return dev;
}

/* ------------------------------------------------------------------------- */

static void hexdump(char *buffer, int len)
{
int     i;
FILE    *fp = stdout;

    for(i = 0; i < len; i++){
        if(i != 0){
            if(i % 16 == 0){
                fprintf(fp, "\n");
            }else{
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "0x%02x", buffer[i] & 0xff);
    }
    if(i != 0)
        fprintf(fp, "\n");
}

static int  hexread(char *buffer, char *string, int buflen)
{
char    *s;
int     pos = 0;

    while((s = strtok(string, ", ")) != NULL && pos < buflen){
        string = NULL;
        buffer[pos++] = (char)strtol(s, NULL, 0);
    }
    return pos;
}

/* ------------------------------------------------------------------------- */

static void usage(char *myName)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s -o <readpacket|writepacket|readconfig|writeconfig|readblock|writeblock>\n",myName);
    fprintf(stderr, "  %s --operation <readpacket|writepacket|readconfig|writeconfig|readblock|writeblock>\n",myName);
    fprintf(stderr, "        readpacket - read last received packet\n");
    fprintf(stderr, "        writepacket - transmit packet\n");
    fprintf(stderr, "        readconfig - read configuration\n");
    fprintf(stderr, "        writeconfig - write configuration\n");
    fprintf(stderr, "        readblock - read data block (debug)\n");
    fprintf(stderr, "        writeblock - write data block (debug)\n");
    fprintf(stderr, "available options:\n");
    fprintf(stderr, "  -b <hexstring,...> - for tx packet and debug puropses.\n");
    fprintf(stderr, "  --buffer <hexstring,...> - for tx packet and debug puropses.\n");
    fprintf(stderr, "                            buffer[0] is report ID\n");
    fprintf(stderr, "  -i <identificator>\n");
    fprintf(stderr, "  --id <identificator>\n");
    fprintf(stderr, "  -d <mode>\n");
    fprintf(stderr, "  --debugmode <mode>\n");
    fprintf(stderr, "  -a <id1> <id2> ... <idN>\n");
    fprintf(stderr, "  --alarmlist <id1> <id2> ... <idN>\n");
    fprintf(stderr, "  -c <id1> <id2> ... <idN>\n");
    fprintf(stderr, "  --checklist <id1> <id2> ... <idN>\n");
    fprintf(stderr, "  -k <crypt-key-hexstring>\n");
    fprintf(stderr, "  --key <crypt-key-hexstring>\n");
    fprintf(stderr, "examples:\n");
    fprintf(stderr, "%s --operation readpacket\n", myName);
    fprintf(stderr, "%s --operation readconfig\n", myName);
    fprintf(stderr, "%s --operation writeconfig --id 0x0001 --debugmode 1\n", myName);
}

enum OPERATIONS {
    OP_UNDEF,
    OP_READBLOCK,
    OP_READPACKET,
    OP_READCONFIG,
    OP_WRITEBLOCK,
    OP_WRITEPACKET,
    OP_WRITECONFIG
    };
enum MODES {
    MODE_UNDEF,
    MODE_USB,
    MODE_TRANSMIT,
    MODE_RETRANSMIT,
    MODE_MONITOR,
    };

int main(int argc, char **argv)
{
    usbDevice_t *dev;
    char    buffer[513];    /* room for dummy report ID */
    unsigned short     checklist[30];
    unsigned short     alarmlist[30];
    int     err, checkcnt=0, alarmcnt=0;
    int     SIZE[] = {0,17,513};
    //~ char*     MODES[]={"undefined","usb","transmit","retransmit","monitor"};
    int     operation=OP_UNDEF, id=0, debugmode=0;
    int     i, pos;
    RunTimeConfigStruc * RunTimeConfig = (RunTimeConfigStruc*) (buffer+1);

//READ COMMAND LINE PARAMETERS
    if(argc < 2){
        usage(argv[0]);
        exit(1);
    }

    memset(buffer, 0xff, sizeof(buffer));
    
    for(i = 1; i < argc; i++) {
        if(!strcasecmp(argv[i], "--operation") || !strcasecmp(argv[i], "-o") ){
            if(argc <= i+1) {
                usage(argv[0]);
                exit(1);
            }
            if(!strcasecmp(argv[i+1], "readblock") ){
                operation = OP_READBLOCK;
            } else
            if(!strcasecmp(argv[i+1], "writeblock") ){
                operation = OP_WRITEBLOCK;
            } else
            if(!strcasecmp(argv[i+1], "readpacket") ){
                operation = OP_READPACKET;
            } else
            if(!strcasecmp(argv[i+1], "writepacket") ){
                operation = OP_WRITEPACKET;
            } else
            if(!strcasecmp(argv[i+1], "readconfig") ){
                operation = OP_READCONFIG;
            } else
            if(!strcasecmp(argv[i+1], "writeconfig") ){
                operation = OP_WRITECONFIG;
            } else {
                usage(argv[0]);
                exit(1);
            }
            i++;
        } else
        if(!strcasecmp(argv[i], "--id") || !strcasecmp(argv[i], "-i")){
            if(argc <= i+1) {
                usage(argv[0]);
                exit(1);
            }
            id = (int)strtol(argv[i+1], NULL, 0);
            i++;
        } else
        if(!strcasecmp(argv[i], "--debugmode") || !strcasecmp(argv[i], "-d") ){
            if(argc <= i+1) {
                usage(argv[0]);
                exit(1);
            }
            debugmode = (int)strtol(argv[i+1], NULL, 0);
            i++;
        } else
        if(!strcasecmp(argv[i], "--buffer") || !strcasecmp(argv[i], "-b")){
            for(pos = 0, i++; i < argc && pos < sizeof(buffer) && (argv[i][0] != '-'); i++){
                pos += hexread(buffer + pos, argv[i], sizeof(buffer) - pos);
            }
            i--;
        } else
        if(!strcasecmp(argv[i], "--checklist") || !strcasecmp(argv[i], "-c")){
            for(checkcnt = 0, i++; i < argc && checkcnt+alarmcnt < sizeof(RunTimeConfig->Monitor)/sizeof(RunTimeConfig->Monitor[0]) && (argv[i][0] != '-'); i++, checkcnt++){
                checklist[checkcnt] = (int)strtol(argv[i], NULL, 0);
            }
            i--;
        } else
        if(!strcasecmp(argv[i], "--alarmlist") || !strcasecmp(argv[i], "-a")){
            for(alarmcnt = 0, i++; i < argc && checkcnt+alarmcnt < sizeof(RunTimeConfig->Monitor)/sizeof(RunTimeConfig->Monitor[0]) && (argv[i][0] != '-'); i++, alarmcnt++){
                alarmlist[alarmcnt] = (int)strtol(argv[i], NULL, 0);
            }
            i--;
        }
    }
//OPERATIONS
    if((dev = openDevice()) == NULL)
        exit(1);
    if(operation == OP_READBLOCK){
        int len = SIZE[(int)buffer[0]];
            hexdump(buffer, 1);
            hexdump(buffer + 1, len-1);
        printf("LEN1:%d\n",len);
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            printf("\n");
            printf("LEN2:%d\n",len);
            hexdump(buffer, 1);
            hexdump(buffer + 1, len-1);
        } 
    } else
    if(operation == OP_WRITEBLOCK){
        int len = SIZE[(int)buffer[0]];
            hexdump(buffer, 1);
            hexdump(buffer + 1, len-1);
        if((err = usbhidSetReport(dev, buffer, len)) != 0) {
            fprintf(stderr, "error writing data: %s\n", usbErrorMessage(err));
        }else{
            printf("\n");
            hexdump(buffer, 1);
            hexdump(buffer + 1, len-1);
        }
    } else
    if(operation == OP_READPACKET){
        buffer[0] = 0x01;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            hexdump(buffer + 1, len-1);
        }       
    } else
    if(operation == OP_WRITEPACKET){
        buffer[0] = 0x01;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidSetReport(dev, buffer, SIZE[(int)buffer[0]])) != 0) {
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            hexdump(buffer + 1, len-1);
        }       
    } else
    if(operation == OP_READCONFIG){
        buffer[0] = 0x02;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            //~ hexdump(buffer + 1, len);   //DEBUG todo: make human readable output
            printf("ID: %d\n",RunTimeConfig->ID);
            printf("DEBUGMODE: %d\n", RunTimeConfig->DebugMode);
            printf("ALARMLIST:");
            i = 0;
            while(RunTimeConfig->Monitor[i] & 0x8000 /*todo define*/ &&
                  RunTimeConfig->Monitor[i] != 0xFFFF) {
                //~ if(i>0) {
                    //~ printf(",");
                    //~ if(i%8 ==0) {
                        //~ printf("\n          ");
                    //~ }
                //~ }
                printf(" %d",RunTimeConfig->Monitor[i]& ~0x8000/*todo*/);
                i++;
            }
            printf("\n");
            printf("CHECKLIST:");
            pos = i;
            i = 0;
            while(RunTimeConfig->Monitor[pos+i] != 0xFFFF) {
                //~ if(i>0) {
                    //~ printf(",");
                    //~ if(i%8 ==0) {
                        //~ printf("\n          ");
                    //~ }
                //~ }
                printf(" %d",RunTimeConfig->Monitor[pos+i]);
                i++;
            }
            printf("\n");
        }       
    } else
    if(operation == OP_WRITECONFIG){
        buffer[0] = 0x02; //report ID
        RunTimeConfig->ID = id;
        RunTimeConfig->DebugMode = debugmode;
        for(i=0; i<alarmcnt; i++) {
            RunTimeConfig->Monitor[i] = alarmlist[i] | 0x8000; //todo: define 0x8000 
        }
        for(i=0; i<checkcnt; i++) {
            RunTimeConfig->Monitor[alarmcnt+i] = checklist[i] & ~0x8000; //todo: define 0x8000 
        }
        int len = SIZE[(int)buffer[0]];
        //~ hexdump(buffer, len);
        if((err = usbhidSetReport(dev, buffer, len)) != 0) {  /* add a dummy report ID */
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }       
    } else {
        usage(argv[0]);
        exit(1);
    }
    usbhidCloseDevice(dev);
    return 0;
}

/* ------------------------------------------------------------------------- */
