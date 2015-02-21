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

    if((err = usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 0)) != 0){
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
    fprintf(stderr, "  %s -operation <readpacket|writepacket|readconfig|writeconfig|readblock|writeblock>\n",myName);
    fprintf(stderr, "        readpacket - read last received packet\n");
    fprintf(stderr, "        writepacket - transmit packet\n");
    fprintf(stderr, "        readconfig - read configuration\n");
    fprintf(stderr, "        writeconfig - write configuration\n");
    fprintf(stderr, "        readblock - read data block (debug)\n");
    fprintf(stderr, "        writeblock - write data block (debug)\n");
    fprintf(stderr, "available options:\n");
    fprintf(stderr, "  -bufferinit <hexstring,...> - for debug puropses only\n");
    fprintf(stderr, "  -id <identificator>\n");
    fprintf(stderr, "  -mode <transmit|retransmit|monitor>\n");
    fprintf(stderr, "        transmit - transmit keep-alive and status\n");
    fprintf(stderr, "        retransmit - transmit keep-alive and status + listen neigbours\n");
    fprintf(stderr, "        monitor - listen ALL neigbours and display errors\n");
    fprintf(stderr, "  -key <crypt-key-hexstring>\n");
    fprintf(stderr, "  -list <id1>, <id2>,..., <idN>\n");
    fprintf(stderr, "examples:\n");
    fprintf(stderr, "%s -operation readpacket\n", myName);
    fprintf(stderr, "%s -operation readconfig\n", myName);
    fprintf(stderr, "%s -operation writeconfig -id 0x0001 -mode transmit\n", myName);
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
int     err;
int     SIZE[] = {0,17,512};
int     operation=OP_UNDEF, id=0, mode=MODE_UNDEF;
int     i, pos;
//READ COMMAND LINE PARAMETERS
    if(argc < 2){
        usage(argv[0]);
        exit(1);
    }
    
    for(i = 1; i < argc; i++) {
        if(!strcasecmp(argv[i], "-operation") ){
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
        if(!strcasecmp(argv[i], "-id") ){
            if(argc <= i+1) {
                usage(argv[0]);
                exit(1);
            }
            id = (int)strtol(argv[i+1], NULL, 0);
            i++;
        } else
        if(!strcasecmp(argv[i], "-mode") ){
            if(argc <= i+1) {
                usage(argv[0]);
                exit(1);
            }
            if(!strcasecmp(argv[i+1], "transmit") ){
                mode = MODE_TRANSMIT;
            } else
            if(!strcasecmp(argv[i+1], "retransmit") ){
                mode = MODE_RETRANSMIT;
            } else
            if(!strcasecmp(argv[i+1], "monitor") ){
                mode = MODE_MONITOR;
            } else {
                usage(argv[0]);
                exit(1);
            }
            i++;
        }
    }
//OPERATIONS
    if((dev = openDevice()) == NULL)
        exit(1);
    if(operation == OP_READBLOCK){
        memset(buffer, 0xff, sizeof(buffer));
        for(pos = 0, i = 2; i < argc && pos < sizeof(buffer); i++){
            pos += hexread(buffer + pos, argv[i], sizeof(buffer) - pos);
        }
        int len = SIZE[(int)buffer[0]];
            hexdump(buffer, 1);
            hexdump(buffer + 1, len);
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            printf("\n");
            hexdump(buffer, 1);
            hexdump(buffer + 1, len);
        } 
    } else
    if(operation == OP_WRITEBLOCK){
        memset(buffer, 0xff, sizeof(buffer));
        for(pos = 0, i = 2; i < argc && pos < sizeof(buffer); i++){
            pos += hexread(buffer + pos, argv[i], sizeof(buffer) - pos);
        }
            hexdump(buffer, 1);
            hexdump(buffer + 1, SIZE[(int)buffer[0]]);
        if((err = usbhidSetReport(dev, buffer, SIZE[(int)buffer[0]])) != 0) {  /* add a dummy report ID */
            fprintf(stderr, "error writing data: %s\n", usbErrorMessage(err));
        }else{
            printf("\n");
            hexdump(buffer, 1);
            hexdump(buffer + 1, SIZE[(int)buffer[0]]);
        }
    } else
    if(operation == OP_READPACKET){
        memset(buffer, 0xff, sizeof(buffer));
        buffer[0] = 0x01;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            hexdump(buffer + 1, len);
        }       
    } else
    if(operation == OP_WRITEPACKET){
        memset(buffer, 0xff, sizeof(buffer));
        buffer[0] = 0x01;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            hexdump(buffer + 1, len);
        }       
    } else
    if(operation == OP_READCONFIG){
        memset(buffer, 0xff, sizeof(buffer));
        buffer[0] = 0x02;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidGetReport(dev, buffer[0], buffer, &len)) != 0){
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            hexdump(buffer + 1, len);   //DEBUG todo: make human readable output
        }       
    } else
    if(operation == OP_WRITECONFIG){
        memset(buffer, 0xff, sizeof(buffer));
        buffer[0] = 0x02;
        buffer[1] = id & 0xFF;
        buffer[2] = id>>8 & 0xFF;
        buffer[3] = mode & 0xFF;
        int len = SIZE[(int)buffer[0]];
        if((err = usbhidSetReport(dev, buffer, len)) != 0) {  /* add a dummy report ID */
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
            //hexdump(buffer + 1, len);
        }       
    } else {
        usage(argv[0]);
        exit(1);
    }
    usbhidCloseDevice(dev);
    return 0;
}

/* ------------------------------------------------------------------------- */
