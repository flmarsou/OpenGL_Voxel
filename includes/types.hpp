#pragma once

// --- 8 bits / 1 byte Variables ---
# define I8MIN	-128
# define I8MAX	127
# define U8MIN	0
# define U8MAX	255

using	i8 = char;
using	u8 = unsigned char;

// --- 16 bits / 2 bytes Variables ---
# define I16MIN	-32768
# define I16MAX	32767
# define U16MIN	0
# define U16MAX	65535

using	i16 = short;
using	u16 = unsigned short;

// --- 32 bits / 4 bytes Variables ---
# define I32MIN	-2147483648
# define I32MAX	2147483647
# define U32MIN	0
# define U32MAX	4294967295

using	i32 = int;
using	u32 = unsigned int;

// --- 64 bits / 8 bytes Variables ---
# define I64MIN	-9223372036854775808
# define I64MAX	9223372036854775807
# define U64MIN	0
# define U64MAX	18446744073709551615

using	i64 = long long;
using	u64 = unsigned long long;
