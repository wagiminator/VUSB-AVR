/*

CDC Arduino Library by Ihsan Kehribar (kehribar.me)
and Digistump LLC (digistump.com)
- all changes made under the same license as V-USB


 */
#ifndef __VUSB_CDC_h__
#define __VUSB_CDC_h__
#include "usbdrv.h"

#include "Stream.h"


#define HW_CDC_BUF_SIZE     24
#define HW_CDC_BULK_OUT_SIZE     8
#define HW_CDC_BULK_IN_SIZE      8

#include "ringBuffer8.h"


/* library functions and variables start */
static uint8_t txtmp[HW_CDC_BULK_IN_SIZE];
static uint8_t tmpidx = 0;

static RingBuffer_t rxBuf;
static RingBuffer_t txBuf;

class VUSB_CDCDevice  : public Stream {
    public:
        VUSB_CDCDevice();
        void begin();
        void end();
        void refresh();
        void task();
        void delay(long milli);
        virtual int available(void);
        virtual int peek(void);
        virtual int read(void);
        virtual void flush(void);
        virtual size_t write(uint8_t);
        size_t write1(uint8_t);
        uint8_t WaitTxEmpty(); //return true if timeout
        using Print::write;
        operator bool();
    private:
        void usbBegin();
        void usbPollWrapper();
 };


extern VUSB_CDCDevice Serial_VUSB;


#endif // __VUSB_CDC_h__
