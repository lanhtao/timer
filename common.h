#ifndef COMMON_H
#define COMMON_H
typedef signed char				schar;	///< sizeof(uchar) == sizeof(schar) == sizeof(char) == 1
typedef unsigned char			uchar;
typedef unsigned int			uint;	///< sizeof(uint) == sizeof(int) == 4
typedef unsigned short			ushort;	///< sizeof(ushort) == sizeof(short) == 2
typedef unsigned long			ulong;	///< sizeof(ulong) == sizeof(long) == 4
typedef signed char				int8;
typedef unsigned char			uint8;
typedef signed short			int16;
typedef unsigned short			uint16;
typedef signed long int			int32;
typedef unsigned long int		uint32;
#ifdef _MSC_VER
typedef __int64					int64;	///< sizeof(int64) == sizeof(uint64) == 8
typedef unsigned __int64		uint64;
#elif defined(__GNUC__)
typedef long long				int64;
typedef unsigned long long		uint64;
#elif defined(__TCS__)
typedef signed   long long int	int64;
typedef unsigned long long int	uint64;
#endif

#endif
