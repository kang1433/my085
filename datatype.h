/**************************************************************/
/* XDATATYPE.H */
/**************************************************************/
#ifndef XDATATYPE_H
#define XDATATYPE_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "stdarg.h"	
#include <stdio.h>

//#define BIT8
#define BIT32


//#define NULL ((void*)0)
//typedef unsigned int	BOOL;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef float				FLOAT;
typedef double			DOUBLE;
typedef void				VOID;
typedef unsigned char		Boolean;
typedef unsigned char		boolean;
typedef unsigned char		bool;

typedef unsigned char       	BYTE;
typedef unsigned char		UINT8;
typedef unsigned char		uint8;
typedef unsigned char		u8;
typedef signed char		int8;
typedef signed char		INT8;

#ifdef	BIT32
typedef unsigned short		UINT16;
typedef unsigned short		uint16;
typedef unsigned short		u16;
typedef signed short		int16;
typedef signed short		INT16;

typedef unsigned int		UINT32;
typedef unsigned int		uint32;
typedef unsigned int		u32;
typedef signed int			int32;
typedef signed int			INT32;
	
typedef unsigned long long	UINT64;
typedef unsigned long long	u64;
typedef signed long long	int64;
typedef signed long long	INT64;

#elif defined(BIT8)
typedef unsigned int		UINT16;
typedef unsigned int		uint16;
typedef unsigned int		u16;
typedef signed int			int16;
typedef signed int			INT16;

typedef unsigned long int	UINT32;
typedef unsigned long int	uint32;
typedef unsigned long int	u32;
typedef signed long int		int32;
typedef signed long int		INT32;

#endif

typedef u16				X_NodeId;
typedef u8				X_Mac[8];

//--- Utility ----------------------------------------------------------------//
#define	DelBit(data, bit)   		((data) &= (0xff ^ ((uint32)0x01 << (bit))))
#define	SetBit(data, bit)   		((data) |= ((uint32)0x01 << (bit)))
#define	GetBit(data, bit)		(((data) >> (bit)) & (uint32)0x01)
#define	MAX(a, b)			(((a) > (b)) ? (a) : (b)) 
#define	MIN(a, b)			(((a) < (b)) ? (a) : (b)) 

/**
 * @brief Returns the low byte of the 16-bit value \c n as an \c int8u.
 */
#define LOW_BYTE(Data16)		((u8)((Data16) & 0xFF))
/**
 * @brief Returns the high byte of the 16-bit value \c n as an \c int8u.
 */
#define HIGH_BYTE(Data16)		((u8)(LOW_BYTE((Data16) >> 8)))

/**
 * @brief Returns the low byte of the 32-bit value \c n as an \c int8u.
 */
#define BYTE_0(Data32)			((u8)((Data32) & 0xFF))
/**
 * @brief Returns the second byte of the 32-bit value \c n as an \c int8u.
 */
#define BYTE_1(Data32)			((u8)(BYTE_0((Data32) >> 8)))
/**
 * @brief Returns the third byte of the 32-bit value \c n as an \c int8u.
 */
#define BYTE_2(Data32)			((u8)(BYTE_0((Data32) >> 16)))
/**
 * @brief Returns the high byte of the 32-bit value \c n as an \c int8u.
 */
#define BYTE_3(Data32)			((u8)(BYTE_0((Data32) >> 24)))

/**
 * @brief Returns the value built from the two \c int8u 
 * values \c high and \c low.
 */
#define HIGH_LOW_TO_INT16(high, low) ((( (u16) (high) ) << 8)+((u16) ( (low) & 0xFF)))     
                                   
#define Byte1_2_3_4_TO_INT32(byte1,byte2,byte3,byte4) (\
                                    (((u32) (byte1)) << 24) +  \
                                    (((u32) (byte2)) << 16)+  \
                                    (((u32) (byte3)) << 8) + \
                                    (((u32) (byte4)) & 0xFF))

#endif
