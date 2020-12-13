/*

CDC Arduino Library by Ihsan Kehribar (kehribar.me)
and Digistump LLC (digistump.com)
- all changes made under the same license as V-USB
- removed ringbuffer bugs

*/

#include "VUSB_CDC.h"
#include <stdint.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uchar             sendEmptyFrame;
static uchar      sendintr3;    /* used to control interrupt endpoint transmissions */

VUSB_CDCDevice::VUSB_CDCDevice(void){}


void VUSB_CDCDevice::delay(long milli) {
  unsigned long now,last = millis();
  do {
    refresh();
    now = millis();
  } while (milli > (long)(now - last));
}

void VUSB_CDCDevice::flush()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      RingBuffer_InitBuffer(&rxBuf);
    }
    usbEnableAllRequests();
}

void VUSB_CDCDevice::begin()
{
    usbBegin();
    VUSB_CDCDevice::delay(500);//delay to allow enumeration and such
}

uint8_t VUSB_CDCDevice::WaitTxEmpty()  //return true if timeout
{
	word n=5*(RingBuffer_GetCount(&txBuf)+tmpidx);
	while ((!RingBuffer_IsEmpty(&txBuf) || tmpidx!=0) && n>0) {
		refresh();
		n--;
	};
	return n!=0;
}


size_t VUSB_CDCDevice::write(uint8_t c)
{
	for(uint8_t i=0;RingBuffer_IsFull(&txBuf) && i<30;i++)
		refresh();

    if(RingBuffer_IsFull(&txBuf))
    {
        return 0;
    }
    else
    {
        RingBuffer_Insert(&txBuf,c);
        refresh();
        return 1;
    }
}


size_t VUSB_CDCDevice::write1(uint8_t c)
{
    if(RingBuffer_IsFull(&txBuf))
    {
        refresh();
        return 0;
    }
    else
    {
        RingBuffer_Insert(&txBuf,c);
        VUSB_CDCDevice::delay(5); //gives 4.2-4.7ms per character for usb transfer at low speed
        return 1;
    }
}


int VUSB_CDCDevice::available()
{
    refresh();
    return RingBuffer_GetCount(&rxBuf);
}


int VUSB_CDCDevice::read()
{   uint8_t bl=RingBuffer_GetCount(&rxBuf);
    if(bl==0)
    {
        refresh();
        return 0;
    }
    else
    {
    	if(usbAllRequestsAreDisabled() &&  bl<=(HW_CDC_BUF_SIZE-HW_CDC_BULK_OUT_SIZE))
        {
          usbEnableAllRequests();
        }
        return RingBuffer_Remove(&rxBuf);
    }
}


int VUSB_CDCDevice::peek()
{
    if(RingBuffer_IsEmpty(&rxBuf))
    {
        refresh();
        return 0;
    }
    else
    {
        return RingBuffer_Peek(&rxBuf);
    }
}


void VUSB_CDCDevice::task(void)
{
  refresh();
}

void VUSB_CDCDevice::refresh(void)
{
  _delay_ms(1);
  usbPollWrapper();
}


void VUSB_CDCDevice::end(void)
{
    // drive both USB pins low to disconnect
    usbDeviceDisconnect();
    VUSB_CDCDevice::flush();
}


VUSB_CDCDevice::operator bool()
{
    refresh();
    return true;
}


void VUSB_CDCDevice::usbBegin()
{
    cli();

    USBOUT &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT));
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();
    usbInit();

    sendintr3 = 0;
    sendEmptyFrame =  0;

    RingBuffer_InitBuffer(&txBuf);
    VUSB_CDCDevice::flush();

    sei();
}

void VUSB_CDCDevice::usbPollWrapper()
{
    usbPoll();
    while((!(RingBuffer_IsEmpty(&txBuf)))&&(tmpidx<HW_CDC_BULK_IN_SIZE))
    {
        txtmp[tmpidx++] = RingBuffer_Remove(&txBuf);
    }

    if(usbInterruptIsReady() && (tmpidx>0 || sendEmptyFrame)) {
        usbSetInterrupt(txtmp,tmpidx);
        sendEmptyFrame = (tmpidx==HW_CDC_BULK_IN_SIZE);
        tmpidx = 0;
    }

    /* We need to report rx and tx carrier after open attempt */
    if(sendintr3 != 0 && usbInterruptIsReady3()){
        static const PROGMEM uchar serialStateNotification[10] = {0xa1, 0x20, 0, 0, 0, 0, 2, 0, 3, 0};

        if(sendintr3 == 2){
            usbSetInterrupt3((uchar *)serialStateNotification, 8);
        }else{
            usbSetInterrupt3(((uchar *)serialStateNotification)+8, 2);
        }
        sendintr3--;
    }

}







#ifdef __cplusplus
extern "C"{
#endif

enum {
    SEND_ENCAPSULATED_COMMAND = 0,
    GET_ENCAPSULATED_RESPONSE,
    SET_COMM_FEATURE,
    GET_COMM_FEATURE,
    CLEAR_COMM_FEATURE,
    SET_LINE_CODING = 0x20,
    GET_LINE_CODING,
    SET_CONTROL_LINE_STATE,
    SEND_BREAK
};

static const PROGMEM char configDescrCDC[] = {   /* USB configuration descriptor */
    9,          /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    67,
    0,          /* total length of data returned (including inlined descriptors) */
    2,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
#else
    (1 << 7),                           /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

    /* interface descriptor follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */

    /* CDC Class-Specific descriptor */
    5,           /* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    0,           /* header functional descriptor */
    0x10, 0x01,

    4,           /* sizeof(usbDescrCDC_AcmFn): length of descriptor in bytes    */
    0x24,        /* descriptor type */
    2,           /* abstract control management functional descriptor */
    0x02,        /* SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE    */

    5,           /* sizeof(usbDescrCDC_UnionFn): length of descriptor in bytes  */
    0x24,        /* descriptor type */
    6,           /* union functional descriptor */
    0,           /* CDC_COMM_INTF_ID */
    1,           /* CDC_DATA_INTF_ID */

    5,           /* sizeof(usbDescrCDC_CallMgtFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    1,           /* call management functional descriptor */
    3,           /* allow management on data interface, handles call management by itself */
    1,           /* CDC_DATA_INTF_ID */

    /* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x80|USB_CFG_EP3_NUMBER,        /* IN endpoint number 3 */
    0x03,        /* attrib: Interrupt endpoint */
    8, 0,        /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL,        /* in ms */

    /* Interface Descriptor  */
    9,           /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE,           /* descriptor type */
    1,           /* index of this interface */
    0,           /* alternate setting for this interface */
    2,           /* endpoints excl 0: number of endpoint descriptors to follow */
    0x0A,        /* Data Interface Class Codes */
    0,
    0,           /* Data Interface Class Protocol Codes */
    0,           /* string index for interface */

    /* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x01,        /* OUT endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    HW_CDC_BULK_OUT_SIZE, 0,        /* maximum packet size */
    0,           /* in ms */

    /* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,        /* IN endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    HW_CDC_BULK_IN_SIZE, 0,        /* maximum packet size */
    0,           /* in ms */
};

uchar usbFunctionDescriptor(usbRequest_t *rq)
{
    if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
        usbMsgPtr = (uchar *)usbDescriptorDevice;
        return usbDescriptorDevice[0];
    }else{  /* must be config descriptor */
        usbMsgPtr = (uchar *)configDescrCDC;
        return sizeof(configDescrCDC);
    }
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

uchar usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (usbRequest_t*)((void *)data);

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */

        if( rq->bRequest==GET_LINE_CODING || rq->bRequest==SET_LINE_CODING ){
            return 0xff;
        /*    GET_LINE_CODING -> usbFunctionRead()    */
        /*    SET_LINE_CODING -> usbFunctionWrite()    */
        }
        if(rq->bRequest == SET_CONTROL_LINE_STATE){
            /* Report serial state (carrier detect). On several Unix platforms,
             * tty devices can only be opened when carrier detect is set.
             */
            if( sendintr3==0 )
                sendintr3 = 2;
        }

        /*  Prepare bulk-in endpoint to respond to early termination   */
        if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
            sendEmptyFrame  = 1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                          */
/*---------------------------------------------------------------------------*/
uchar usbFunctionRead( uchar *data, uchar len )
{
    // data[0] = 0;
    // data[1] = 0;
    // data[2] = 0;
    // data[3] = 0;
    // data[4] = 0;
    // data[5] = 0;
    // data[6] = 8;

    return 7;
}

/*---------------------------------------------------------------------------*/
/* usbFunctionWrite                                                          */
/*---------------------------------------------------------------------------*/
uchar usbFunctionWrite( uchar *data, uchar len )
{
    // baud.bytes[0] = data[0];
    // baud.bytes[1] = data[1];

    return 1;
}

void usbFunctionWriteOut( uchar *data, uchar len )
{
    for(uint8_t qw=0;qw<len;qw++)
    {
        if(RingBuffer_GetCount(&rxBuf)<HW_CDC_BUF_SIZE)
        {
            RingBuffer_Insert(&rxBuf,data[qw]);
        }
    }

    /* postpone receiving next data */
    if(RingBuffer_GetCount(&rxBuf)>=HW_CDC_BUF_SIZE-HW_CDC_BULK_OUT_SIZE )
    {
        usbDisableAllRequests();
    }
}


#ifdef __cplusplus
} // extern "C"
#endif

VUSB_CDCDevice Serial_VUSB;
