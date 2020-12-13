/*******************************************************************************************************************
*
* See the http://www.fourwalledcubicle.com/files/LightweightRingBuff.h for the license information.
*
*******************************************************************************************************************/
#include <stdint.h>
#include <util/atomic.h>
/*----------------------------------------------------------------------------------------------------------------*/
typedef struct
{
	uint8_t  Buf_Data[HW_CDC_BUF_SIZE]; /**< Internal ring buffer data, referenced by the buffer pointers. */
    uint8_t* In; /**< Current storage location in the circular buffer. */
    uint8_t* Out; /**< Current retrieval location in the circular buffer. */
volatile uint8_t  Count; /**< Number of bytes currently stored in the buffer. */
} RingBuffer_t;
/*----------------------------------------------------------------------------------------------------------------*/
static inline void RingBuffer_InitBuffer(RingBuffer_t* Buffer)
{
	Buffer->In    = Buffer->Buf_Data;
	Buffer->Out   = Buffer->Buf_Data;
    Buffer->Count  = 0;
}
/*----------------------------------------------------------------------------------------------------------------*/
static inline volatile uint8_t RingBuffer_GetCount(RingBuffer_t* const Buffer)
{
    return (Buffer->Count);
}
/*----------------------------------------------------------------------------------------------------------------*/
static inline uint8_t RingBuffer_GetFreeCount(RingBuffer_t* const Buffer)
{
    return (HW_CDC_BUF_SIZE - RingBuffer_GetCount(Buffer));
}
/*----------------------------------------------------------------------------------------------------------------*/
static inline uint8_t RingBuffer_IsEmpty(RingBuffer_t* const Buffer)
{
    return (RingBuffer_GetCount(Buffer) == 0);
}
/*----------------------------------------------------------------------------------------------------------------*/
static inline uint8_t RingBuffer_IsFull(RingBuffer_t* const Buffer)
{
    return (RingBuffer_GetCount(Buffer) == HW_CDC_BUF_SIZE);
}
/*----------------------------------------------------------------------------------------------------------------*/
static void RingBuffer_Insert(RingBuffer_t* Buffer, const uint8_t Data)
{
    *Buffer->In = Data;

    if (++Buffer->In == &Buffer->Buf_Data[HW_CDC_BUF_SIZE])
      Buffer->In = Buffer->Buf_Data;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        Buffer->Count++;
    }
}
/*----------------------------------------------------------------------------------------------------------------*/
static uint8_t RingBuffer_Remove(RingBuffer_t* Buffer)
{
    uint8_t Data = *Buffer->Out;

    if (++Buffer->Out == &Buffer->Buf_Data[HW_CDC_BUF_SIZE])
      Buffer->Out = Buffer->Buf_Data;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        Buffer->Count--;
    }

    return Data;
}
/*----------------------------------------------------------------------------------------------------------------*/
static inline uint8_t RingBuffer_Peek(RingBuffer_t* const Buffer)
{
    return *Buffer->Out;
}
/*----------------------------------------------------------------------------------------------------------------*/
