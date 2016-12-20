#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rawdata.h>
#include <xan_asm.h>
#if defined( __ARM_ARCH_6__ ) || defined(__ELBRUS__)
#define NO_ASM 1
#elif defined( __WINDOWS__ ) || defined( __LINUX__ )
#define NO_ASM 0
#else
#define NO_ASM 1
#endif
#ifdef __x86__
#define raw_data_one_part_size sizeof( ::rux::uint32 )
typedef ::rux::uint32 raw_data_one_part_type;
typedef ::rux::int32 raw_data_one_part_signed_type;
#elif defined( __x64__ )
#define raw_data_one_part_size sizeof( ::rux::uint64 )
typedef ::rux::uint64 raw_data_one_part_type;
typedef ::rux::int64 raw_data_one_part_signed_type;
#endif
#define raw_data_check_size( number1 , number2 ) {\
	if( number1->_max_data_length > number2->_max_data_length )\
	{\
		if( number2->_raw_data_ptr )\
		{\
			::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , number1->_max_data_length );\
			::rux::engine::memcpy( raw_data_ptr , 0 , number2->_raw_data_ptr , number2->get_AlignedSize() );\
			if( number2->_raw_data_ptr )\
				::rux::engine::free_mem( number2->_raw_data_ptr );\
			number2->_raw_data_ptr = raw_data_ptr;\
		}\
		else\
			number2->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number1->_max_data_length );\
		number2->_max_data_length = number1->_max_data_length;\
	}\
	else if( number1->_max_data_length < number2->_max_data_length )\
	{\
		if( number1->_raw_data_ptr )\
		{\
			::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , number2->_max_data_length );\
			::rux::engine::memcpy( raw_data_ptr , 0 , number1->_raw_data_ptr , number1->get_AlignedSize() );\
			if( number1->_raw_data_ptr )\
				::rux::engine::free_mem( number1->_raw_data_ptr );\
			number1->_raw_data_ptr = raw_data_ptr;\
		}\
		else\
			number1->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number2->_max_data_length );\
		number1->_max_data_length = number2->_max_data_length;\
	}\
}
#define raw_data_fast_copy( number1 , number2 ) {\
		if( number1->_raw_data_ptr == NULL\
			|| number1->_max_data_length < number2->_max_data_length )\
		{\
			if( number1->_raw_data_ptr )\
			{\
				if( number1->_raw_data_ptr )\
					::rux::engine::free_mem( number1->_raw_data_ptr );\
				number1->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number2->_max_data_length );\
			}\
			else\
				number1->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number2->_max_data_length );\
			number1->_max_data_length = number2->_max_data_length;\
		}\
		else if( number1->_max_data_length > 0 )\
			memset( &number1->_raw_data_ptr[ number2->get_AlignedSize() ] , 0 , number1->_max_data_length - number2->get_AlignedSize() );\
		if( number2->get_AlignedSize() > 0 )\
			::rux::engine::memcpy( number1->_raw_data_ptr , 0 , number2->_raw_data_ptr , number2->get_AlignedSize() );\
		number1->_raw_data_ptr_size = number2->get_AlignedSize();\
		number1->_bits_count = number2->get_BitsCount();\
		number1->_is_carry = number2->_is_carry;\
		number1->_is_negative = number2->_is_negative;\
	}
#define raw_data_getbit( number1 , index , bit_index , byte_index , result ) {\
		byte_index = index / 8;\
		bit_index = index - byte_index * 8;\
		if( ( ( number1->_raw_data_ptr[ byte_index ] ) & ( 1 << ( bit_index ) ) ) )\
			result = 1;\
		else\
			result = 0;\
	}
#define raw_data_fast_minus_if_unsigned( number1 , number2 ) {\
	raw_data_minus( number1 , number2 );\
	number1->_is_negative = number1->get_IsZero() ? 0 : number1->_is_negative;\
	number1->_force_size = SIZE_MAX;\
	number1->_bits_count = 0xffffffff;\
	number1->_raw_data_ptr_size = 0xffffffff;\
}
#if NO_ASM
#define raw_data_compare( number1 , number2 , result ) {\
		result = 0;\
		::rux::uint32* uint32_buffer_ptr0 = (::rux::uint32*)number1->_raw_data_ptr;\
		::rux::uint32* uint32_buffer_ptr1 = (::rux::uint32*)number2->_raw_data_ptr;\
		for( size_t index0 = ( number1->_max_data_length / sizeof( ::rux::uint32 ) ) - 1 ; index0 >= 0 ; index0-- )\
		{\
			if( uint32_buffer_ptr0[ index0 ] > uint32_buffer_ptr1[ index0 ] )\
			{\
				result = 1;\
				break;\
			}\
			else if( uint32_buffer_ptr0[ index0 ] < uint32_buffer_ptr1[ index0 ] )\
			{\
				result = -1;\
				break;\
			}\
			if( index0 == 0 )\
				break;\
		};\
	}
#else
#define raw_data_compare( number1 , number2 , result ) {\
		result = 0;\
		AsmCompare( number1->_raw_data_ptr , number2->_raw_data_ptr , number1->_max_data_length / raw_data_one_part_size , &result );\
	}
#endif
#define raw_data_fast_greater_or_equal_if_unsigned( number1 , number2 , result ) {\
		raw_data_one_part_signed_type signed_type_result = 0;\
		raw_data_compare( number1 , number2 , signed_type_result );\
		result = signed_type_result >= 0;\
	}
#define raw_data_fast_plus_if_unsigned( number1 , number2 ) {\
		raw_data_check_size( number1 , number2 );\
		raw_data_plus( number1 , number2 );\
		number1->_force_size = SIZE_MAX;\
		number1->_bits_count = 0xffffffff;\
		number1->_raw_data_ptr_size = 0xffffffff;\
	}
#if NO_ASM
#define raw_data_fast_left_shift( number1 ) {\
		::rux::uint64 add = 0;\
		for( size_t index0 = 0 ; index0 < number1->_max_data_length ; index0 += sizeof( ::rux::uint32 ) )\
		{\
			add = ( ( (::rux::uint64)( *(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] ) ) << 1 ) + add;\
			*(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] = (::rux::uint32)add;\
			add >>= sizeof( ::rux::uint32 ) * 8;\
		}\
		number1->_force_size = SIZE_MAX;\
		number1->_bits_count = 0xffffffff;\
		number1->_raw_data_ptr_size = 0xffffffff;\
	}
#else
#define raw_data_fast_left_shift( number1 ) {\
		AsmLeftShift( number1->_raw_data_ptr , number1->_max_data_length );\
		number1->_force_size = SIZE_MAX;\
		number1->_bits_count = 0xffffffff;\
		number1->_raw_data_ptr_size = 0xffffffff;\
	}
#endif
#define raw_data_set_unsigned_uint32( number1 , uint32_number ) {\
		if( number1->_raw_data_ptr == NULL )\
		{\
			number1->_force_size = SIZE_MAX;\
			number1->_bits_count = 0xffffffff;\
			number1->_raw_data_ptr_size = ( ( sizeof( ::rux::uint32 ) + 7 ) >> 3 ) << 3;\
			number1->_max_data_length = ( number1->_raw_data_ptr_size << 1 ) + raw_data_one_part_size;\
			number1->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number1->_max_data_length );\
		}\
		else\
		{\
			number1->_force_size = SIZE_MAX;\
			number1->_bits_count = 0xffffffff;\
			number1->_raw_data_ptr_size = ( ( sizeof( ::rux::uint32 ) + 7 ) >> 3 ) << 3;\
		}\
		memset( &number1->_raw_data_ptr[ sizeof( ::rux::uint32 ) ] , 0 , number1->_max_data_length - sizeof( ::rux::uint32 ) );\
		*(::rux::uint32*)number1->_raw_data_ptr = uint32_number;\
		number1->_is_negative = 0;\
	}
#if NO_ASM
#define raw_data_minus( number1, number2 ) {\
		::rux::uint64 sub = 0;\
		::rux::int32 _is_carry = 0;\
		for( size_t index0 = 0 ; index0 < number1->_max_data_length ; index0 += sizeof( ::rux::uint32 ) )\
		{\
			sub = (::rux::uint64)( *(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] );\
			sub -= ( *(::rux::uint32*)&number2->_raw_data_ptr[ index0 ] );\
			sub -= (::rux::uint32)_is_carry;\
			_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;\
			*(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;\
		}\
	}
#else
#define raw_data_minus( number1, number2 ) {\
		AsmMinus( number1->_raw_data_ptr , number2->_raw_data_ptr , number1->_max_data_length / raw_data_one_part_size );\
	}
#endif
#if NO_ASM
#define raw_data_plus( number1 , number2 ) {\
		::rux::uint64 add = 0;\
		for( size_t index0 = 0 ; index0 < number1->_max_data_length ; index0 += sizeof( ::rux::uint32 ) )\
		{\
			add += (::rux::uint64)( *(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] );\
			add += ( *(::rux::uint32*)&number2->_raw_data_ptr[ index0 ] );\
			*(::rux::uint32*)&number1->_raw_data_ptr[ index0 ] = (::rux::uint32)add;\
			add >>= sizeof( ::rux::uint32 ) * 8;\
		}\
	}
#else
#define raw_data_plus( number1 , number2 ) {\
		AsmPlus( number1->_raw_data_ptr , number2->_raw_data_ptr , number1->_max_data_length / raw_data_one_part_size );\
	}
#endif
const ::rux::uint32 _prime_numbers[ 1240 ] = {
	2,    3,    5,    7,    11,   13,   17,   19,   23,   29,   31,   37,   41,   43,   47,   53,   59,   61,   67,   71,   73,   79,   83,   89,   97,
	101,  103,  107,  109,  113,  127,  131,  137,  139,  149,  151,  157,  163,  167,  173,  179,  181,  191,  193,  197,  199,
	211,  223,  227,  229,  233,  239,  241,  251,  257,  263,  269,  271,  277,  281,  283,  293,
	307,  311,  313,  317,  331,  337,  347,  349,  353,  359,  367,  373,  379,  383,  389,  397,
	401,  409,  419,  421,  431,  433,  439,  443,  449,  457,  461,  463,  467,  479,  487,  491,  499,
	503,  509,  521,  523,  541,  547,  557,  563,  569,  571,  577,  587,  593,  599,
	601,  607,  613,  617,  619,  631,  641,  643,  647,  653,  659,  661,  673,  677,  683,  691,
	701,  709,  719,  727,  733,  739,  743,  751,  757,  761,  769,  773,  787,  797,
	809,  811,  821,  823,  827,  829,  839,  853,  857,  859,  863,  877,  881,  883,  887,
	907,  911,  919,  929,  937,  941,  947,  953,  967,  971,  977,  983,  991,  997,
	1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,
	1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193,
	1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
	1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399,
	1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499,
	1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
	1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699,
	1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
	1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
	1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999,
	2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099,
	2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179,
	2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293, 2297,
	2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399,
	2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473, 2477,
	2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593,
	2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699,
	2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749, 2753, 2767, 2777, 2789, 2791, 2797,
	2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897,
	2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999,
	3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089,
	3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191,
	3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 3271,
	3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391,
	3407, 3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499,
	3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571, 3581, 3583, 3593,
	3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659, 3671, 3673, 3677, 3691, 3697,
	3701, 3709, 3719, 3727, 3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797,
	3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889,
	3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
	4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073, 4079, 4091, 4093, 4099,
	4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177,
	4201, 4211, 4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297,
	4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397,
	4409, 4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493,
	4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591, 4597,
	4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691,
	4703, 4721, 4723, 4729, 4733, 4751, 4759, 4783, 4787, 4789, 4793, 4799,
	4801, 4813, 4817, 4831, 4861, 4871, 4877, 4889,
	4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999,
	5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099,
	5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189, 5197,
	5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281, 5297,
	5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399,
	5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483,
	5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591,
	5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693,
	5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,
	5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897,
	5903, 5923, 5927, 5939, 5953, 5981, 5987,
	6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091,
	6101, 6113, 6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199,
	6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299,
	6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397,
	6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491,
	6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599,
	6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679, 6689, 6691,
	6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793,
	6803, 6823, 6827, 6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899,
	6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
	7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079,
	7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193,
	7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297,
	7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393,
	7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
	7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591,
	7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699,
	7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793,
	7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883,
	7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993,
	8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093,
	8101, 8111, 8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191,
	8209, 8219, 8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291, 8293, 8297,
	8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387, 8389,
	8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467,
	8501, 8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597, 8599,
	8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677, 8681, 8689, 8693, 8699,
	8707, 8713, 8719, 8731, 8737, 8741, 8747, 8753, 8761, 8779, 8783,
	8803, 8807, 8819, 8821, 8831, 8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893,
	8923, 8929, 8933, 8941, 8951, 8963, 8969, 8971, 8999,
	9001, 9007, 9011, 9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091,
	9103, 9109, 9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199,
	9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283, 9293,
	9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377, 9391, 9397,
	9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439, 9461, 9463, 9467, 9473, 9479, 9491, 9497,
	9511, 9521, 9533, 9539, 9547, 9551, 9587,
	9601, 9613, 9619, 9623, 9629, 9631, 9643, 9649, 9661, 9677, 9679, 9689, 9697,
	9719, 9721, 9733, 9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791,
	9803, 9811, 9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887,
	9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973,
	10007, 10009, 10037, 10039, 10061, 10067, 10069, 10079, 10091, 10093, 10099
};
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
implement_rux_base_methods_ns( RawData , rux );
implement_rux_set_operators_ns_( RawData , rux );
namespace rux
{	
	implement_ctor_and_register_type( RawData );
	void XRawData::x_initialize( void )
	{
	};
	RawData::RawData( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{
		_raw_data_ptr = NULL;
		_raw_data_ptr_size = 0xffffffff;
		_bits_count = 0xffffffff;
		_force_size = SIZE_MAX;
		_is_carry = 0;
		_is_negative = 0;
	};
	RawData::~RawData()
	{
		if( _raw_data_ptr )
			::rux::engine::free_mem( _raw_data_ptr );
	};
	::rux::uint32 RawData::get_SizeInUlong32Count( void )
	{
		::rux::uint32 size = 0;
		if( _bits_count == 0xffffffff )
		{
			_bits_count = 0;
			::rux::int32 index0 = _max_data_length - sizeof(::rux::uint32);
			for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
			{
				_bits_count = ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
				if( _bits_count )
					break;
			};
			if( index0 < 0 )
				_bits_count = 0;
			else
			{
				if( _bits_count >= 0x10000 ) 
					_bits_count >>= 16 , index0 +=2;
				if( _bits_count >= 0x100 ) 
					_bits_count >>= 8 , index0++;
				index0 <<= 3;
				if( _bits_count >= 0x80 ) 
					_bits_count = index0 + 7 + 1;
				else if( _bits_count >= 0x40 )
					_bits_count = index0 + 6 + 1;
				else if( _bits_count >= 0x20 ) 
					_bits_count = index0 + 5 + 1;
				else if( _bits_count >= 0x10 ) 
					_bits_count = index0 + 4 + 1;
				else if( _bits_count >= 0x08 ) 
					_bits_count = index0 + 3 + 1;
				else if( _bits_count >= 0x04 ) 
					_bits_count = index0 + 2 + 1;
				else if( _bits_count >= 0x02 ) 
					_bits_count = index0 + 1 + 1;
				else if( _bits_count >= 0x01 ) 
					_bits_count = index0 + 1;
				else
					_bits_count = index0 - 1 + 1;
			}
		}
		::rux::uint32 index1 = _bits_count;
		if( index1 != 0 )
		{		
			size = index1 / ( 8 * sizeof( ::rux::uint32 ) );
			if( index1 - size * ( 8 * sizeof( ::rux::uint32 ) ) > 0 )
				size += 1;		
		}
		return size;
	};
	::rux::uint32 RawData::get_BitsCount( void )
	{	
		if( _bits_count == 0xffffffff )
		{
			_bits_count = 0;
			::rux::int32 index0 = _max_data_length - sizeof(::rux::uint32);
			for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
			{
				_bits_count = ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
				if( _bits_count )
					break;
			};
			if( index0 < 0 )
				_bits_count = 0;
			else
			{
				if( _bits_count >= 0x10000 ) 
					_bits_count >>= 16 , index0 +=2;
				if( _bits_count >= 0x100 ) 
					_bits_count >>= 8 , index0++;
				index0 <<= 3;
				if( _bits_count >= 0x80 ) 
					_bits_count = index0 + 7 + 1;
				else if( _bits_count >= 0x40 )
					_bits_count = index0 + 6 + 1;
				else if( _bits_count >= 0x20 ) 
					_bits_count = index0 + 5 + 1;
				else if( _bits_count >= 0x10 ) 
					_bits_count = index0 + 4 + 1;
				else if( _bits_count >= 0x08 ) 
					_bits_count = index0 + 3 + 1;
				else if( _bits_count >= 0x04 ) 
					_bits_count = index0 + 2 + 1;
				else if( _bits_count >= 0x02 ) 
					_bits_count = index0 + 1 + 1;
				else if( _bits_count >= 0x01 ) 
					_bits_count = index0 + 1;
				else
					_bits_count = index0 - 1 + 1;
			}
		}
		return _bits_count;
	};
	::rux::uint32 RawData::get_AlignedSize( void )
	{	
		if( _raw_data_ptr_size == 0xffffffff )
		{
			if( _bits_count == 0xffffffff )
			{
				_bits_count = 0;
				::rux::int32 index0 = _max_data_length - sizeof(::rux::uint32);
				for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
				{
					_bits_count = ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
					if( _bits_count )
						break;
				};
				if( index0 < 0 )
					_bits_count = 0;
				else
				{
					if( _bits_count >= 0x10000 ) 
						_bits_count >>= 16 , index0 +=2;
					if( _bits_count >= 0x100 ) 
						_bits_count >>= 8 , index0++;
					index0 <<= 3;
					if( _bits_count >= 0x80 ) 
						_bits_count = index0 + 7 + 1;
					else if( _bits_count >= 0x40 )
						_bits_count = index0 + 6 + 1;
					else if( _bits_count >= 0x20 ) 
						_bits_count = index0 + 5 + 1;
					else if( _bits_count >= 0x10 ) 
						_bits_count = index0 + 4 + 1;
					else if( _bits_count >= 0x08 ) 
						_bits_count = index0 + 3 + 1;
					else if( _bits_count >= 0x04 ) 
						_bits_count = index0 + 2 + 1;
					else if( _bits_count >= 0x02 ) 
						_bits_count = index0 + 1 + 1;
					else if( _bits_count >= 0x01 ) 
						_bits_count = index0 + 1;
					else
						_bits_count = index0 - 1 + 1;
				}
			}
			_raw_data_ptr_size = _bits_count;
			if( _raw_data_ptr_size != 0 )
			{			
				::rux::uint32 index0 = _raw_data_ptr_size / 8;
				if( _raw_data_ptr_size - index0 * 8 > 0 )
					index0 += 1;
				_raw_data_ptr_size = ( ( index0 + 7 ) >> 3 ) << 3;						 
			}
		}
		return _raw_data_ptr_size;
	};
	XRawData::XRawData()
	{	
	};
	XRawData::XRawData( const RawData& raw_data )
	{
		raw_data.AddRef();
		(*this) = raw_data;
		raw_data.Release();
	};
	XRawData::XRawData( const XObject& object )
	{
	  *static_cast<XGCHandle<RawData>*>(this) = object;
	};
	XRawData::XRawData( const XRawData& raw_data )
	{
	  (*(static_cast<XGCHandle<RawData>*>( this ))) = raw_data;
	};
	XRawData& XRawData::operator =( const RawData& raw_data )
	{
		raw_data.AddRef();
		(*(static_cast<XGCHandle<RawData>*>( this ))) = raw_data;
		raw_data.Release();
		return *this;
	};
	XRawData& XRawData::operator =( const XRawData& raw_data )
	{
	  (*(static_cast<XGCHandle<RawData>*>( this ))) = raw_data;
		return *this;
	};
	XRawData::XRawData( const ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size )
	{
		if( raw_data_ptr )
		{
			if( raw_data_ptr_size > 0 )
			{			
				(*this)()->_raw_data_ptr_size = raw_data_ptr_size == 0 ? 8 : raw_data_ptr_size;
				(*this)()->_raw_data_ptr_size = ( ( (*this)()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;				
	#ifdef __x86__
				(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
				(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				if( raw_data_ptr_size > 0 )
					::rux::engine::memcpy( (*this)()->_raw_data_ptr , 0 , raw_data_ptr , raw_data_ptr_size );			
			}
		}
	};
	RawData& XRawData::FromReverseOrder( const ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size )
	{
		XRawData raw_data;
		if( raw_data_ptr )
		{
			if( raw_data_ptr_size > 0 )
			{			
				raw_data()->_raw_data_ptr_size = raw_data_ptr_size == 0 ? 8 : raw_data_ptr_size;
				raw_data()->_raw_data_ptr_size = ( ( raw_data()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__			
				raw_data()->_max_data_length = ( raw_data()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
				raw_data()->_max_data_length = ( raw_data()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				if( raw_data_ptr_size > 0 )
				{
					::rux::uint32 decrementer = raw_data_ptr_size - 1;
					for( ::rux::uint32 incrementer = 0 ; incrementer < raw_data_ptr_size ; incrementer++ )
					{
						raw_data()->_raw_data_ptr[ incrementer ] = raw_data_ptr[ decrementer];									
						decrementer--;
					}
				}
				raw_data()->_bits_count = 0xffffffff;
				raw_data()->_force_size = raw_data_ptr_size;
			}
		}
		return raw_data++;
	};
	void XRawData::set_RawData( ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size )
	{
		if( raw_data_ptr )
		{
			if( raw_data_ptr_size > 0 )
			{		
				(*this)()->_raw_data_ptr_size = raw_data_ptr_size == 0 ? 8 : raw_data_ptr_size;
				(*this)()->_raw_data_ptr_size = ( ( (*this)()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 			
				if( (*this)()->_raw_data_ptr )
				{
					memset( (*this)()->_raw_data_ptr , 0 , (*this)()->_max_data_length );
	#ifdef __x86__
					if( (*this)()->_max_data_length < ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 ) )
	#elif defined( __x64__ )
					if( (*this)()->_max_data_length < ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 ) )
	#endif
					{
	#ifdef __x86__
						(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
						(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
						::rux::engine::free_mem( (*this)()->_raw_data_ptr );
						(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
					}
				}			
				else
				{
	#ifdef __x86__
					(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
					(*this)()->_max_data_length = ( (*this)()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
					(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				}
				::rux::engine::memcpy( (*this)()->_raw_data_ptr , 0 , raw_data_ptr , raw_data_ptr_size );		
				(*this)()->_bits_count = 0xffffffff;
				(*this)()->_force_size = raw_data_ptr_size;
			}
			else
			{
				if( (*this)()->_raw_data_ptr 
					&& (*this)()->_max_data_length > 0 )
					memset( (*this)()->_raw_data_ptr , 0 , (*this)()->_max_data_length );
				(*this)()->_raw_data_ptr_size = 0;
				(*this)()->_bits_count = 0;
				(*this)()->_is_negative = 0;
			}
		}
		else
		{
			if( (*this)()->_raw_data_ptr 
				&& (*this)()->_max_data_length > 0 )
				memset( (*this)()->_raw_data_ptr , 0 , (*this)()->_max_data_length );
			(*this)()->_raw_data_ptr_size = 0;
			(*this)()->_bits_count = 0;
			(*this)()->_is_negative = 0;
		}
	};
	::rux::String& XRawData::ToString( void )
	{
		return (*this)()->ToString();
	}
	::rux::String& RawData::ToString( void )
	{
		::rux::XString string;	
		XRawData raw_data;
		raw_data.Copy( *this );
		::rux::uint8 letter = ' ';
		while( raw_data.get_IsZero() == false )
		{
			letter = (::rux::uint8)raw_data.private_Divide( 10 ) + 0x30;
			string.Insert( 0 , (char)letter );
		}
		if( string.Length() == 0 )
			string = "0";
		else if( _is_negative == 1 )
			string.Insert( 0 , '-' );
		return string++;
	};
	bool XRawData::operator == ( const XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );		
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );		
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		if( (*this)()->_is_negative != raw_data()->_is_negative )
			return false;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) != 0 )
			return false;
		return true;
	};
	bool operator == ( RawData& raw_data1 , ::rux::uint32 number )
	{
		raw_data1.AddRef();
		if( raw_data1._max_data_length >= sizeof( number ) )
		{
			if( raw_data1._is_negative == 1 || *(::rux::uint32*)&raw_data1._raw_data_ptr[ 0 ] != number )
			{
				raw_data1.Release();
				return false;	
			}
			for( ::rux::uint32 index0 = sizeof(::rux::uint32) ; index0 < raw_data1._max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&raw_data1._raw_data_ptr[ index0 ] )
				{
					raw_data1.Release();
					return false;
				}
			};
			raw_data1.Release();
			return true;
		}
		raw_data1.Release();
		return false;
	};
	bool XRawData::operator == ( ::rux::uint32 number )
	{
		if( (*this)()->_max_data_length >= sizeof( number ) )
		{
			if( (*this)()->_is_negative == 1 || *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ] != number )
				return false;	
			for( ::rux::uint32 index0 = sizeof(::rux::uint32) ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] )
					return false;
			};
			return true;
		}
		return false;
	};
	bool XRawData::operator != ( ::rux::uint32 number )
	{
		if( (*this)()->_max_data_length >= sizeof( number ) )
		{
			if( (*this)()->_is_negative == 1 || *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ] != number )
				return true;	
			for( ::rux::uint32 index0 = sizeof(::rux::uint32) ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] )
					return true;
			};
			return false;
		}
		return true;
	};
	bool XRawData::operator != ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );		
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		if( (*this)()->_is_negative != raw_data()->_is_negative )
			return true;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) != 0 )
			return true;
		return false;
	};
	bool operator > ( XRawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data1()->_max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
			raw_data2._max_data_length = raw_data1()->_max_data_length;
		}
		else if( raw_data1()->_max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1()->_raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data1()->_raw_data_ptr );
				raw_data1()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1()->_max_data_length = raw_data2._max_data_length;
		}		
		if( raw_data1()->_is_negative == 1 && raw_data2._is_negative == 0 )
		{
			raw_data2.Release();
			return false;
		}
		if( raw_data1()->_is_negative == 0 && raw_data2._is_negative == 1 )
		{
			raw_data2.Release();
			return true;
		}
		if( raw_data1.compare( raw_data1()->_raw_data_ptr , raw_data2._raw_data_ptr , raw_data1()->_max_data_length ) > 0 )
		{
			raw_data2.Release();
			return true;
		}
		raw_data2.Release();
		return false;
	};
	bool XRawData::operator > ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}		
		if( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 )
			return false;
		if( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 )
			return true;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) > 0 )
			return true;
		return false;
	};
	bool XRawData::operator > ( ::rux::uint32 number ) const
	{
		return (*this)()->operator>( number );
	};
	bool RawData::operator > ( ::rux::uint32 number )
	{
		if( _max_data_length >= sizeof( ::rux::uint32 ) )
		{
			if( _is_negative == 1 )
				return false;
			for( ::rux::uint32 index0 = _max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) 
					return true;
			};
			return *(::rux::uint32*)&_raw_data_ptr[ 0 ] > number;
		}	
		return false;
	};
	bool XRawData::operator < ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}		
		if( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 )
			return true;
		if( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 )
			return false;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) < 0 )
			return true;
		return false;
	};
	bool XRawData::operator < ( ::rux::uint32 number ) const
	{
		if( (*this)()->_max_data_length >= sizeof( ::rux::uint32 ) )
		{		
			if( (*this)()->_is_negative == 1 ) 
				return true;		
			for( ::rux::uint32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) 
					return false;
			};
			return *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ] < number;
		}
		return false;
	};
	bool RawData::operator < ( ::rux::uint32 number )
	{
		if( _max_data_length >= sizeof( ::rux::uint32 ) )
		{		
			if( _is_negative == 1 ) 
				return true;		
			for( ::rux::uint32 index0 = _max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) 
					return false;
			};
			return *(::rux::uint32*)&_raw_data_ptr[ 0 ] < number;
		}
		return false;
	};
	bool XRawData::operator >= ( RawData& raw_data )
	{	
		raw_data.AddRef();
		if( (*this)()->_max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data._max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data._max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			(*this)()->_max_data_length = raw_data._max_data_length;
		}		
		if( (*this)()->_is_negative == 1 && raw_data._is_negative == 0 )
		{
			raw_data.Release();
			return false;
		}
		if( (*this)()->_is_negative == 0 && raw_data._is_negative == 1 )
		{
			raw_data.Release();
			return true;
		}
		if( compare( (*this)()->_raw_data_ptr , raw_data._raw_data_ptr , (*this)()->_max_data_length ) < 0 )
		{
			raw_data.Release();
			return false;
		}
		raw_data.Release();
		return true;
	};
	bool RawData::operator >= ( RawData& raw_data )
	{		
		raw_data.AddRef();
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}		
		if( _is_negative == 1 && raw_data._is_negative == 0 )
		{
			raw_data.Release();
			return false;
		}
		if( _is_negative == 0 && raw_data._is_negative == 1 )
		{
			raw_data.Release();
			return true;
		}
		if( XRawData::compare( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length ) < 0 )
		{
			raw_data.Release();
			return false;
		}
		raw_data.Release();
		return true;
	};
	bool XRawData::operator >= ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}		
		if( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 )
			return false;
		if( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 )
			return true;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) < 0 )
			return false;
		return true;
	};
	bool XRawData::operator >= ( ::rux::uint32 number )
	{
		if( (*this)()->_max_data_length >= sizeof( ::rux::uint32 ) )
		{
			if( (*this)()->_is_negative == 1 )
				return false;		
			for( ::rux::uint32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) 
					return true;
			};
			return *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ] >= number;
		}	
		return false;
	};
	bool RawData::operator >= ( ::rux::uint32 number )
	{
		if( _max_data_length >= sizeof( ::rux::uint32 ) )
		{
			if( _is_negative == 1 )
				return false;		
			for( ::rux::uint32 index0 = _max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) 
					return true;
			};
			return *(::rux::uint32*)&_raw_data_ptr[ 0 ] >= number;
		}	
		return false;
	};
	bool XRawData::operator <= ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}		
		if( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 )
			return true;
		if( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 )
			return false;
		if( compare( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length ) > 0 )
			return false;
		return true;
	};
	bool XRawData::operator <= ( ::rux::uint32 number )
	{
		if( (*this)()->_max_data_length >= sizeof( ::rux::uint32 ) )
		{
			if( (*this)()->_is_negative == 1 ) 
				return true;
			for( ::rux::uint32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) 
					return false;
			};
			return *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ] <= number;
		}
		return false;
	};
	void XRawData::operator = ( const ::rux::uint32 number )
	{
		*(*this)() = number;
	};
	void RawData::operator = ( const ::rux::uint32 number )
	{
		if( _raw_data_ptr == NULL )
		{
			_force_size = SIZE_MAX;
			_bits_count = 0xffffffff;
			_raw_data_ptr_size = ( ( sizeof( ::rux::uint32 ) + 7 ) >> 3 ) << 3;			
	#ifdef __x86__
			_max_data_length = ( _raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
			_max_data_length = ( _raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
			_raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
		}
		else
		{
			_force_size = SIZE_MAX;
			_bits_count = 0xffffffff;
			_raw_data_ptr_size = ( ( sizeof( ::rux::uint32 ) + 7 ) >> 3 ) << 3;
		}
		memset( _raw_data_ptr , 0 , _max_data_length );
		*(::rux::uint32*)_raw_data_ptr = number;
		_is_negative = 0;
	};
	RawData& operator + ( XRawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data1()->_max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
			raw_data2._max_data_length = raw_data1()->_max_data_length;
		}
		else if( raw_data1()->_max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1()->_raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data1()->_raw_data_ptr );
				raw_data1()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1()->_max_data_length = raw_data2._max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
		result()->_max_data_length = raw_data1()->_max_data_length;
		::rux::uint32 index0 = 0;
		if( raw_data1()->_is_negative == raw_data2._is_negative )
		{
			::rux::uint64 add = 0;
			for( ; index0 < raw_data1()->_max_data_length; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
				add += ( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = add != 0 ? 1 : 0;
			result()->_is_negative = raw_data1()->_is_negative;
		}
		else
		{
			::rux::uint64 sub = 0;
			if( raw_data1.private_IsGreater( raw_data2 ) )
			{
				for( ; index0 < raw_data1()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data1()->_is_negative;
			}
			else
			{
				for( ; index0 < raw_data1()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data2._is_negative;
			}
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data2.Release();
		return result++;
	};
	void RawData::increase_memory( size_t size )
	{
		if( _max_data_length < size )
		{
			size = size == 0 ? 8 : size;
			size = ( ( size + 7 ) >> 3 ) << 3;
	#ifdef __x86__			
			size_t max_data_length = ( size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
			size_t max_data_length = ( size << 1 ) + sizeof( ::rux::uint64 );
	#endif
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , max_data_length );
			_max_data_length = max_data_length;
		}
	};
	RawData& XRawData::operator + ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint32 index0 = 0;
		if( (*this)()->_is_negative == raw_data()->_is_negative )
		{
			::rux::uint64 add = 0;
			for( ; index0 < (*this)()->_max_data_length; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				add += ( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = add != 0 ? 1 : 0;
			result()->_is_negative = (*this)()->_is_negative;
		}
		else
		{
			::rux::uint64 sub = 0;
			if( private_IsGreater( raw_data ) )
			{
				for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : (*this)()->_is_negative;
			}
			else
			{
				for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data()->_is_negative;
			}
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& RawData::operator += ( ::rux::uint32 number )
	{		
		::rux::uint32 index0 = 0;
		if( _is_negative == 1 )
		{
			if( private_IsLess( number ) )			
				*(::rux::uint32*)&_raw_data_ptr[ 0 ] = number - *(::rux::uint32*)&_raw_data_ptr[ 0 ];			
			else
			{
				::rux::uint64 sub = 0;
				_is_carry = (::rux::int32)number;
				for( ; index0 < _max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)_is_carry;
					_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				_is_negative = get_IsZero() ? 0 : 1;
			}
		}
		else
		{
			::rux::uint64 add = number;	
			for( ; index0 < _max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			_is_carry = add != 0 ? 1 : 0;
		}
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	RawData& XRawData::operator + ( ::rux::uint32 number )
	{
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint32 index0 = 0;
		if( (*this)()->_is_negative == 1 )
		{
			if( private_IsLess( number ) )			
				*(::rux::uint32*)&result()->_raw_data_ptr[ 0 ] = number - *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ];			
			else
			{
				::rux::uint64 sub = 0;
				result()->_is_carry = (::rux::int32)number;
				for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : 1;
			}
		}
		else
		{
			::rux::uint64 add = number;	
			for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = add != 0 ? 1 : 0;
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& operator - ( XRawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data1()->_max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
			raw_data2._max_data_length = raw_data1()->_max_data_length;
		}
		else if( raw_data1()->_max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1()->_raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data1()->_raw_data_ptr );
				raw_data1()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1()->_max_data_length = raw_data2._max_data_length;
		}
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
		result()->_max_data_length = raw_data1()->_max_data_length;
		::rux::uint64 sub = 0;
		if( raw_data1()->_is_negative != raw_data2._is_negative )
		{		
			for( ::rux::uint32 index0 = 0; index0 < raw_data1()->_max_data_length; index0 += sizeof(::rux::uint32) )
			{
				sub += (::rux::uint64)( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
				sub += ( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				sub >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = sub != 0 ? 1 : 0;
			result()->_is_negative = result.get_IsZero() ? 0 : raw_data1()->_is_negative;
		}
		else
		{
			if( raw_data1 > raw_data2 )
			{
				for( ::rux::uint32 index0 = 0 ; index0 < raw_data1()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data1()->_is_negative;
			}
			else
			{
				for( ::rux::uint32 index0 = 0 ; index0 < raw_data1()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data2._is_negative == 1 ? 0 : 1;
			}
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data2.Release();
		return result++;
	};
	RawData& XRawData::operator - ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint64 sub = 0;
		if( (*this)()->_is_negative != raw_data()->_is_negative )
		{		
			for( ::rux::uint32 index0 = 0; index0 < (*this)()->_max_data_length; index0 += sizeof(::rux::uint32) )
			{
				sub += (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				sub += ( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				sub >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = sub != 0 ? 1 : 0;
			result()->_is_negative = result.get_IsZero() ? 0 : (*this)()->_is_negative;
		}
		else
		{
			if( private_IsGreater( raw_data ) )
			{
				for( ::rux::uint32 index0 = 0 ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : (*this)()->_is_negative;
			}
			else
			{
				for( ::rux::uint32 index0 = 0 ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index0 ] );
					sub -= ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
				result()->_is_negative = result.get_IsZero() ? 0 : raw_data()->_is_negative == 1 ? 0 : 1;
			}
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	XRawData& XRawData::operator -= ( ::rux::uint32 number )
	{
		(*this)()->operator-=( number );
		return *this;
	};
	RawData& RawData::operator -= ( ::rux::uint32 number )
	{	
		::rux::uint32 index0 = 0;
		if( _is_negative == 1 )
		{
			::rux::uint64 add = number;	
			for( ; index0 < _max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			_is_carry = add != 0 ? 1 : 0;
			_is_negative = 1;
		}
		else
		{
			if( *this < number )	
			{
				*(::rux::uint32*)&_raw_data_ptr[ 0 ] = number - *(::rux::uint32*)&_raw_data_ptr[ 0 ];
				_is_negative = get_IsZero() ? 0 : 1;
			}
			else
			{
				::rux::uint64 sub = 0;
				_is_carry = (::rux::int32)number;
				for( ; index0 < _max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)_is_carry;
					_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
			}
		}
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;	
		return *this;
	};
	RawData& XRawData::operator - ( ::rux::uint32 number ) const
	{
		XRawData result;	
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint32 index0 = 0;
		if( (*this)()->_is_negative == 1 )
		{
			::rux::uint64 add = number;	
			for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				add += (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= sizeof(::rux::uint32) * 8;
			};
			result()->_is_carry = add != 0 ? 1 : 0;
			result()->_is_negative = 1;
		}
		else
		{
			if( *this < number )	
			{
				*(::rux::uint32*)&result()->_raw_data_ptr[ 0 ] = number - *(::rux::uint32*)&(*this)()->_raw_data_ptr[ 0 ];
				result()->_is_negative = result.get_IsZero() ? 0 : 1;
			}
			else
			{
				::rux::uint64 sub = 0;
				result()->_is_carry = (::rux::int32)number;
				for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
				{
					sub = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					sub -= (::rux::uint32)result()->_is_carry;
					result()->_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
					*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = (::rux::uint32)sub;
				};
			}
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	XRawData& XRawData::operator << ( const ::rux::int32 shift )
	{
		::rux::uint32 index0 = 0;
		if( ( shift > 0 ) && ( shift < 8 ) )
		{
			::rux::uint64 add = 0;
			for( ; index0 < (*this)()->_max_data_length ; index0 += sizeof(::rux::uint32) )
			{
				add = ( ( (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) ) << shift ) + ( add >> 0x20 );
				*(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] = (::rux::uint32)add;
				add >>= 0x20;
			};
			(*this)()->_is_carry = ::rux::int32( add >> 0x20 );
		};
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	void XRawData::FastLeftShift( void )
	{	
		(*this)()->FastLeftShift();
	};
	void XRawData::FastRightShift( void )
	{
		(*this)()->FastRightShift();
	};
	void RawData::FastLeftShift( void )
	{		
	#if NO_ASM
		::rux::uint64 add = 0;
		for( size_t index0 = 0 ; index0 < _max_data_length ; index0 += sizeof( ::rux::uint32 ) )
		{
			add = ( ( (::rux::uint64)( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) ) << 1 ) + add;
			*(::rux::uint32*)&_raw_data_ptr[ index0 ] = (::rux::uint32)add;
			add >>= sizeof( ::rux::uint32 ) * 8;
		};
	#else
		AsmLeftShift( _raw_data_ptr , _max_data_length );	
	#endif
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
	};
	void RawData::FastRightShift( void )
	{
	#if NO_ASM
		::rux::uint32 add0 = 0;
		::rux::uint32 add1 = 0;
		for( size_t index0 = _max_data_length - sizeof( ::rux::uint32 ) ; index0 >= 0 ; index0 -= sizeof( ::rux::uint32 ) )
		{
			add1 = add0;
			add0 = ( ( ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) & ( ( 1 << 1 ) - 1 ) ) << ( sizeof( ::rux::uint32 ) * 8 - 1 ) );
			*(::rux::uint32*)&_raw_data_ptr[ index0 ] = ( ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) >> 1 ) + add1;
			if( index0 == 0 )
				break;
		};
	#else
		AsmRightShift( _raw_data_ptr , _max_data_length );
	#endif
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
	};
	XRawData& XRawData::operator >> ( const ::rux::int32 shift )
	{
		::rux::uint32 index0 = 0;
		if( ( shift > 0 ) && ( shift < 8 ) )
		{
			::rux::uint32 add0 = 0;
			::rux::uint32 add1 = 0;
			for( index0 = (*this)()->_max_data_length - sizeof(::rux::uint32) ; index0 >= 0 ; index0 -= sizeof(::rux::uint32) )
			{
				add1 = add0;
				add0 = ( ( ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) & ( ( 1 << shift ) - 1 ) ) << ( sizeof(::rux::uint32) * 8 - shift ) );
				*(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] = ( ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) >> shift ) + add1;
				if( index0 == 0 )
					break;
			};
			(*this)()->_is_carry = (::rux::int32)add0;
		}
		else if( shift == 8 )
		{		
			(*this)()->_is_carry = (::rux::int32)( *(::rux::uint32*)(*this)()->_raw_data_ptr );
			for( index0 = 0 ; index0 < (*this)()->_max_data_length - sizeof(::rux::uint32) ; index0 += sizeof(::rux::uint32) )		
				*(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] = *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 + sizeof( ::rux::uint32 ) ];
			*(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] = 0;
		};
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	RawData& operator * ( RawData& raw_data1 , XRawData& raw_data2 )
	{
		raw_data1.AddRef();
		if( raw_data1._max_data_length > raw_data2()->_max_data_length )
		{
			if( raw_data2()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2()->_raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data2()->_raw_data_ptr );
				raw_data2()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
			raw_data2()->_max_data_length = raw_data1._max_data_length;
		}
		else if( raw_data1._max_data_length < raw_data2()->_max_data_length )
		{
			if( raw_data1._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1._raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1._raw_data_ptr )
					::rux::engine::free_mem( raw_data1._raw_data_ptr );
				raw_data1._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2()->_max_data_length );
			raw_data1._max_data_length = raw_data2()->_max_data_length;
		}
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
		result()->_max_data_length = raw_data1._max_data_length;
		::rux::uint32 ulong32_count0 = raw_data1.get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data2.get_SizeInUlong32Count();
		::rux::uint64 res = 0;
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0;
		for( index0 = 0 ; index0 < ulong32_count0 * sizeof( ::rux::uint32 ) ; index0 += sizeof( ::rux::uint32 ) )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 * sizeof( ::rux::uint32 ) ; index1 += sizeof( ::rux::uint32 ) )
			{
				res = (::rux::uint64)( *(::rux::uint32*)&raw_data1._raw_data_ptr[ index0 ] ) * (::rux::uint64)( *(::rux::uint32*)&raw_data2()->_raw_data_ptr[ index1 ] ) + *(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] + remainder;
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + ulong32_count1 * sizeof( ::rux::uint32 ) ] = remainder;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1._is_negative == 1 && raw_data2()->_is_negative == 0 ) 
			|| ( raw_data1._is_negative == 0 && raw_data2()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data1.Release();
		return result++;
	};
	RawData& operator * ( XRawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data1()->_max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
			raw_data2._max_data_length = raw_data1()->_max_data_length;
		}
		else if( raw_data1()->_max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1()->_raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data1()->_raw_data_ptr );
				raw_data1()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1()->_max_data_length = raw_data2._max_data_length;
		}
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
		result()->_max_data_length = raw_data1()->_max_data_length;
		::rux::uint32 ulong32_count0 = raw_data1.get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data2.get_SizeInUlong32Count();
		::rux::uint64 res = 0;
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0;
		for( index0 = 0 ; index0 < ulong32_count0 * sizeof( ::rux::uint32 ) ; index0 += sizeof( ::rux::uint32 ) )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 * sizeof( ::rux::uint32 ) ; index1 += sizeof( ::rux::uint32 ) )
			{
				res = (::rux::uint64)( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ index0 ] ) * (::rux::uint64)( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index1 ] ) + *(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] + remainder;
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + ulong32_count1 * sizeof( ::rux::uint32 ) ] = remainder;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1()->_is_negative == 1 && raw_data2._is_negative == 0 ) 
			|| ( raw_data1()->_is_negative == 0 && raw_data2._is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data2.Release();
		return result++;
	};
	RawData& operator * ( RawData& raw_data1 , RawData& raw_data2 )
	{	
		raw_data1.AddRef();
		raw_data2.AddRef();
		if( raw_data1._max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
			raw_data2._max_data_length = raw_data1._max_data_length;
		}
		else if( raw_data1._max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1._raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1._raw_data_ptr )
					::rux::engine::free_mem( raw_data1._raw_data_ptr );
				raw_data1._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1._max_data_length = raw_data2._max_data_length;
		}
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
		result()->_max_data_length = raw_data1._max_data_length;
		::rux::uint32 ulong32_count0 = raw_data1.get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data2.get_SizeInUlong32Count();
		::rux::uint64 res = 0;
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0;
		for( index0 = 0 ; index0 < ulong32_count0 * sizeof( ::rux::uint32 ) ; index0 += sizeof( ::rux::uint32 ) )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 * sizeof( ::rux::uint32 ) ; index1 += sizeof( ::rux::uint32 ) )
			{
				res = (::rux::uint64)( *(::rux::uint32*)&raw_data1._raw_data_ptr[ index0 ] ) * (::rux::uint64)( *(::rux::uint32*)&raw_data2._raw_data_ptr[ index1 ] ) + *(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] + remainder;
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + ulong32_count1 * sizeof( ::rux::uint32 ) ] = remainder;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1._is_negative == 1 && raw_data2._is_negative == 0 ) 
			|| ( raw_data1._is_negative == 0 && raw_data2._is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data1.Release();
		raw_data2.Release();
		return result++;
	};
	RawData& XRawData::operator * ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint32 ulong32_count0 = this->get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data.get_SizeInUlong32Count();
		::rux::uint64 res = 0;
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0;
		for( index0 = 0 ; index0 < ulong32_count0 * sizeof( ::rux::uint32 ) ; index0 += sizeof( ::rux::uint32 ) )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 * sizeof( ::rux::uint32 ) ; index1 += sizeof( ::rux::uint32 ) )
			{
				res = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) * (::rux::uint64)( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index1 ] ) + *(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] + remainder;
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + ulong32_count1 * sizeof( ::rux::uint32 ) ] = remainder;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 ) 
			|| ( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::operator * ( const XRawData& raw_data ) const
	{	
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		::rux::uint32 ulong32_count0 = this->get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data.get_SizeInUlong32Count();
		::rux::uint64 res = 0;
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0;
		for( index0 = 0 ; index0 < ulong32_count0 * sizeof( ::rux::uint32 ) ; index0 += sizeof( ::rux::uint32 ) )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 * sizeof( ::rux::uint32 ) ; index1 += sizeof( ::rux::uint32 ) )
			{
				res = (::rux::uint64)( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) * (::rux::uint64)( *(::rux::uint32*)&raw_data()->_raw_data_ptr[ index1 ] ) + *(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] + remainder;
				*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + index1 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 + ulong32_count1 * sizeof( ::rux::uint32 ) ] = remainder;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 ) 
			|| ( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::operator * ( ::rux::uint32 number )
	{
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp0;	
		temp0.Copy( *this );	
		::rux::uint64 temp_number = 0;
		::rux::uint64 multiply = 0;
		::rux::uint32 index0 = 0;
		for( ; index0 < this->get_AlignedSize() ; index0 += sizeof(::rux::uint32) )
		{
			temp_number = (::rux::uint64)( *(::rux::uint32*)&temp0()->_raw_data_ptr[ index0 ] ) * (::rux::uint64)number + multiply;
			multiply = temp_number >> ( sizeof(::rux::uint32) * 8 );
			*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = ::rux::uint32( temp_number );
		};
		*(::rux::uint32*)&result()->_raw_data_ptr[ index0 ] = ::rux::uint32( multiply );	
		result()->_is_negative = result.get_IsZero() ? 0 : (*this)()->_is_negative;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::GCD( const XRawData& raw_data1 , const XRawData& raw_data2 )
	{
		XRawData temp0 , temp1 , temp2;
		temp0.Copy( raw_data1 );
		temp1.Copy( raw_data2 );	
		while( temp0 != 0 )
		{
			temp1 = temp1 % temp0;
			temp2.set_ByRef( temp1 );
			temp1.set_ByRef( temp0 );
			temp0.set_ByRef( temp2 );
		}
		return temp1++;
	};
	RawData& operator % ( XRawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data1()->_max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
			raw_data2._max_data_length = raw_data1()->_max_data_length;
		}
		else if( raw_data1()->_max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1()->_raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data1()->_raw_data_ptr );
				raw_data1()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1()->_max_data_length = raw_data2._max_data_length;
		}
		XRawData result;
	 result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1()->_max_data_length );
	 result()->_max_data_length = raw_data1()->_max_data_length;
	 ::rux::int32 j = raw_data1.get_BitsCount() - 1;
	 ::rux::uint32 index0 = j + 1;
	 ::rux::uint32 i = index0 / 8;
	 if( index0 - i * 8 > 0 )
		 i += 1; 
	 ::rux::uint8 k = 1 << ( j % 8 );	
	 i -= 1; 
	 while( j-- >= 0 )
	 {	 
		 result.FastLeftShift();
		 result()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&raw_data1()->_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
		 if( result >= raw_data2 )
			 result -= raw_data2;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
	 }
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1()->_is_negative == 1 && raw_data2._is_negative == 0 ) 
			|| ( raw_data1()->_is_negative == 0 && raw_data2._is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data2.Release();
		return result++;
	};
	RawData& operator % ( RawData& raw_data1 , RawData& raw_data2 )
	{
		raw_data1.AddRef();
		raw_data2.AddRef();
		if( raw_data1._max_data_length > raw_data2._max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
			raw_data2._max_data_length = raw_data1._max_data_length;
		}
		else if( raw_data1._max_data_length < raw_data2._max_data_length )
		{
			if( raw_data1._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1._raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1._raw_data_ptr )
					::rux::engine::free_mem( raw_data1._raw_data_ptr );
				raw_data1._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			raw_data1._max_data_length = raw_data2._max_data_length;
		}
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
		result()->_max_data_length = raw_data1._max_data_length;
		::rux::int32 j = raw_data1.get_BitsCount() - 1;
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1; 
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1; 
		while( j-- >= 0 )
		{	 
			result.FastLeftShift();
			result()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&raw_data1._raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( result >= raw_data2 )
				result -= raw_data2;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1._is_negative == 1 && raw_data2._is_negative == 0 ) 
			|| ( raw_data1._is_negative == 0 && raw_data2._is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data2.Release();
		raw_data1.Release();
		return result++;
	};
	RawData& operator % ( RawData& raw_data1 , XRawData& raw_data2 )
	{
		raw_data1.AddRef();
		if( raw_data1._max_data_length > raw_data2()->_max_data_length )
		{
			if( raw_data2()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2()->_raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data2()->_raw_data_ptr );
				raw_data2()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
			raw_data2()->_max_data_length = raw_data1._max_data_length;
		}
		else if( raw_data1._max_data_length < raw_data2()->_max_data_length )
		{
			if( raw_data1._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data1._raw_data_ptr , raw_data1.get_AlignedSize() );
				if( raw_data1._raw_data_ptr )
					::rux::engine::free_mem( raw_data1._raw_data_ptr );
				raw_data1._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data1._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2()->_max_data_length );
			raw_data1._max_data_length = raw_data2()->_max_data_length;
		}
		XRawData result;
	 result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data1._max_data_length );
	 result()->_max_data_length = raw_data1._max_data_length;
	 ::rux::int32 j = raw_data1.get_BitsCount() - 1;
	 ::rux::uint32 index0 = j + 1;
	 ::rux::uint32 i = index0 / 8;
	 if( index0 - i * 8 > 0 )
		 i += 1; 
	 ::rux::uint8 k = 1 << ( j % 8 );	
	 i -= 1; 
	 while( j-- >= 0 )
	 {	 
		 result.FastLeftShift();
		 result()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&raw_data1._raw_data_ptr[ i ] ) & k ) ? 1 : 0;
		 if( result >= raw_data2 )
			 result -= raw_data2;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
	 }
		result()->_is_negative = result.get_IsZero() == false && ( ( raw_data1._is_negative == 1 && raw_data2()->_is_negative == 0 ) 
			|| ( raw_data1._is_negative == 0 && raw_data2()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data1.Release();
		return result++;
	};
	RawData& XRawData::operator % ( ::rux::uint32 number )
	{
		XRawData result;
		::rux::uint32 index0, index1 = 0;	
		if( (*this)()->_max_data_length > 0 )
		{
			result.Copy( *this );			
			while( result >= number  )
			{
				if( number == 0 )
					break;
				index1++;
				if( ((::rux::uint8*)&number)[ 3 ] & ( ( ::rux::uint8(-1) >> 1 ) + 1 ) ) 
					break;		
				number <<= 1;
			};
			for( index0 = 0; index0 < index1 ; index0++ )
			{
				if( result >= number )		
					result = result - number;
				number >>= 1;
			};
			if( result >= number )
				result = result - number;
			result()->_is_negative = result.get_IsZero() == false && (*this)()->_is_negative == 1 ? 1 : 0;
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	void RawData::modular( RawData& raw_data , ::rux::uint32 number )
	{
		Copy( raw_data );
		if( *this >= number )
		{		
			::rux::uint32 temp = number , prev_temp = 0;
			do
			{
				if( *this > temp )
				{
					temp = number;
					for( ; ; )
					{
						prev_temp = temp;
						temp <<= 1;
						if( *this < temp )
						{
							temp = prev_temp;
							break;
						}
						if( temp <= prev_temp )
						{
							temp = prev_temp;
							break;
						}
					}
				}
				*this -= temp;
			}
			while( *this >= number );
		}
		_is_negative = get_IsZero() == false && _is_negative == 1 ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
	};
	void RawData::modular( RawData& raw_data )
	{	
		raw_data.AddRef();
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}	 
		XRawData number_raw_data;
		RawData* number = number_raw_data();
		number->Copy( *this );
		memset( _raw_data_ptr , 0 , _max_data_length );
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		::rux::int32 j = number->get_BitsCount() - 1;
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1; 
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1; 
		while( j-- >= 0 )
		{	 
			FastLeftShift();
			_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&number->_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( *this >= raw_data )
				*this -= raw_data;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		_is_negative = get_IsZero() == false && ( ( number->_is_negative == 1 && raw_data._is_negative == 0 ) 
			|| ( number->_is_negative == 0 && raw_data._is_negative == 1 ) ) ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;	
		raw_data.Release();
	};
	void RawData::modular( RawData& number , RawData& raw_data )
	{	
		number.AddRef();
		raw_data.AddRef();
		if( number._max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			raw_data._max_data_length = number._max_data_length;
		}
		else if( number._max_data_length < raw_data._max_data_length )
		{
			if( number._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , number._raw_data_ptr , number.get_AlignedSize() );
				if( number._raw_data_ptr )
					::rux::engine::free_mem( number._raw_data_ptr );
				number._raw_data_ptr = raw_data_ptr;		
			}
			else
				number._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			number._max_data_length = raw_data._max_data_length;
		}	 
		 if( _max_data_length < number._max_data_length )
		 {
			 if( _raw_data_ptr )
				 ::rux::engine::free_mem( _raw_data_ptr );
			 _raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			 _max_data_length = number._max_data_length;
		 }
		 memset( _raw_data_ptr , 0 , _max_data_length );
		 _force_size = SIZE_MAX;
		 _bits_count = 0xffffffff;
		 _raw_data_ptr_size = 0xffffffff;
		 ::rux::int32 j = number.get_BitsCount() - 1;
		 ::rux::uint32 index0 = j + 1;
		 ::rux::uint32 i = index0 / 8;
		 if( index0 - i * 8 > 0 )
			 i += 1; 
		 ::rux::uint8 k = 1 << ( j % 8 );	
		 i -= 1; 
		 while( j-- >= 0 )
		 {	 
			 FastLeftShift();
			 _raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&number._raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			 if( *this >= raw_data )
				 *this -= raw_data;	 
				k >>= 1;
				if( !k ) k = 1 << 7, i --;
		 }
		_is_negative = get_IsZero() == false && ( ( number._is_negative == 1 && raw_data._is_negative == 0 ) 
			|| ( number._is_negative == 0 && raw_data._is_negative == 1 ) ) ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		number.Release();
		raw_data.Release();
	};
	RawData& RawData::operator % ( ::rux::uint32 number )
	{	
		XRawData result;
		result.Copy( *this );
		if( result >= number )
		{
			::rux::XString t;
			do
			{
				result -= number;
			}
			while( result >= number );
		}
		result()->_is_negative = result.get_IsZero() == false && _is_negative == 1 ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::operator % ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
	 XRawData result;
	 result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
	 result()->_max_data_length = (*this)()->_max_data_length;
	 ::rux::int32 j = this->get_BitsCount() - 1;
	 ::rux::uint32 index0 = j + 1;
	 ::rux::uint32 i = index0 / 8;
	 if( index0 - i * 8 > 0 )
		 i += 1; 
	 ::rux::uint8 k = 1 << ( j % 8 );	
	 i -= 1; 
	 while( j-- >= 0 )
	 {	 
		 result.FastLeftShift();
		 result()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
		 if( result >= raw_data )
			 result -= raw_data;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
	 }
		result()->_is_negative = result.get_IsZero() == false && ( ( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 ) 
			|| ( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::operator / ( ::rux::uint32 number )
	{	
		XRawData result;
		::rux::uint32 index0 , index1 = 0;	
		if( (*this)()->_max_data_length > 0 )
		{
			result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			result()->_max_data_length = (*this)()->_max_data_length;
			XRawData temp1;
			temp1.Copy( *this );			
			while( temp1 >= number )
			{
				if( number == 0 )
					break;
				index1++;
				if( ( (::rux::uint8*)&number )[ 3 ] & ( ( ::rux::uint8(-1) >> 1 ) + 1 ) )
					break;		
				number <<= 1;
			};
			for( index0 = 0; index0 < index1; index0++ )
			{		
				result.FastLeftShift();
				if( temp1 >= number )
				{
					result()->_raw_data_ptr[ 0 ] |= 1;
					temp1 = temp1 - number;
				};		
				number >>= 1;
			};	
			result.FastLeftShift();
			if( temp1 >= number )	
				result()->_raw_data_ptr[ 0 ] |= 1;		
			result()->_is_negative = result.get_IsZero() == false && (*this)()->_is_negative == 1 ? 1 : 0;
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& RawData::operator / ( ::rux::uint32 number )
	{	
		XRawData result;
		::rux::uint32 index0 , index1 = 0;	
		if( _max_data_length > 0 )
		{
			result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			result()->_max_data_length = _max_data_length;
			XRawData temp1;
			temp1.Copy( *this );			
			while( temp1 >= number )
			{
				if( number == 0 )
					break;
				index1++;
				if( ( (::rux::uint8*)&number )[ 3 ] & ( ( ::rux::uint8(-1) >> 1 ) + 1 ) )
					break;		
				number <<= 1;
			};
			for( index0 = 0; index0 < index1; index0++ )
			{		
				result.FastLeftShift();
				if( temp1 >= number )
				{
					result()->_raw_data_ptr[ 0 ] |= 1;
					temp1 = temp1 - number;
				};		
				number >>= 1;
			};	
			result.FastLeftShift();
			if( temp1 >= number )	
				result()->_raw_data_ptr[ 0 ] |= 1;		
			result()->_is_negative = result.get_IsZero() == false && _is_negative == 1 ? 1 : 0;
		}
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	void RawData::multiplication( RawData& number , RawData& raw_data )
	{
		number.AddRef();
		raw_data.AddRef();	
		if( number._max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			raw_data._max_data_length = number._max_data_length;
		}
		else if( number._max_data_length < raw_data._max_data_length )
		{
			if( number._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , number._raw_data_ptr , number.get_AlignedSize() );
				if( number._raw_data_ptr )
					::rux::engine::free_mem( number._raw_data_ptr );
				number._raw_data_ptr = raw_data_ptr;		
			}
			else
				number._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			number._max_data_length = raw_data._max_data_length;
		}	
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		if( _max_data_length < number._max_data_length )
		{
			if( _raw_data_ptr )
				::rux::engine::free_mem( _raw_data_ptr );
			_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			_max_data_length = number._max_data_length;
		}
		memset( _raw_data_ptr , 0 , _max_data_length );
		::rux::uint32 ulong32_count0 = number.get_SizeInUlong32Count();
		::rux::uint32 ulong32_count1 = raw_data.get_SizeInUlong32Count();
		::rux::uint64 res = 0;	
		::rux::uint32 remainder = 0;
		::rux::uint32 index0 = 0 , index1 = 0 , index2 = 0;
		::rux::uint32* number_raw_data_ptr = (::rux::uint32*)number._raw_data_ptr;
		::rux::uint32* raw_data_raw_data_ptr = (::rux::uint32*)raw_data._raw_data_ptr;
		::rux::uint32* this_raw_data_ptr = (::rux::uint32*)_raw_data_ptr;
		for( index0 = 0 ; index0 < ulong32_count0 ; index0++ )
		{
			remainder = 0;
			for( index1 = 0 ; index1 < ulong32_count1 ; index1++ )
			{
				::rux::uint64 first = (::rux::uint64)( number_raw_data_ptr[ index0 ] );
				::rux::uint64 second = (::rux::uint64)( raw_data_raw_data_ptr[ index1 ] );
				index2 = index0 + index1;
				res = first * second + this_raw_data_ptr[ index2 ] + remainder;
				this_raw_data_ptr[ index2 ] = ( (::rux::uint32*)&res )[ 0 ];
				remainder = ( (::rux::uint32*)&res )[ 1 ];
			}
			this_raw_data_ptr[ index0 + ulong32_count1 ] = remainder;
		}
		_is_negative = get_IsZero() == false && ( ( number._is_negative == 1 && raw_data._is_negative == 0 ) 
			|| ( number._is_negative == 0 && raw_data._is_negative == 1 ) ) ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		number.Release();
		raw_data.Release();
	};
	void RawData::divide( RawData& number , RawData& raw_data )
	{
		number.AddRef();
		raw_data.AddRef();
		if( number._max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			raw_data._max_data_length = number._max_data_length;
		}
		else if( number._max_data_length < raw_data._max_data_length )
		{
			if( number._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , number._raw_data_ptr , number.get_AlignedSize() );
				if( number._raw_data_ptr )
					::rux::engine::free_mem( number._raw_data_ptr );
				number._raw_data_ptr = raw_data_ptr;		
			}
			else
				number._raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			number._max_data_length = raw_data._max_data_length;
		}	
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		if( _max_data_length < number._max_data_length )
		{
			if( _raw_data_ptr )
				::rux::engine::free_mem( _raw_data_ptr );
			_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
			_max_data_length = number._max_data_length;
		}
		memset( _raw_data_ptr , 0 , _max_data_length );
		XRawData temp0;
		temp0()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , number._max_data_length );
		temp0()->_max_data_length = number._max_data_length;
		::rux::int32 j = number.get_BitsCount() - 1;  
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1;
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1;	
		while( j-- >= 0 )
		{	 
			FastLeftShift();
			temp0.FastLeftShift();
			temp0()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&number._raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( temp0 >= raw_data )
			{
				temp0 -= raw_data;
				_raw_data_ptr[ 0 ] |= 1;
			}
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		_is_negative = get_IsZero() == false && ( ( number._is_negative == 1 && raw_data._is_negative == 0 ) 
			|| ( number._is_negative == 0 && raw_data._is_negative == 1 ) ) ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		number.Release();
		raw_data.Release();
	};
	RawData& XRawData::operator / ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp0;
		temp0()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp0()->_max_data_length = (*this)()->_max_data_length;
		::rux::int32 j = this->get_BitsCount() - 1;  
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1;
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1;	
		while( j-- >= 0 )
		{	 
			result.FastLeftShift();
			temp0.FastLeftShift();
			temp0()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( temp0 >= raw_data )
			{
				temp0 -= raw_data;
				result()->_raw_data_ptr[ 0 ] |= 1;
			}
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( (*this)()->_is_negative == 1 && raw_data()->_is_negative == 0 ) 
			|| ( (*this)()->_is_negative == 0 && raw_data()->_is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& RawData::operator / ( RawData& raw_data )
	{	
		raw_data.AddRef();
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
		result()->_max_data_length = _max_data_length;
		XRawData temp0;
		temp0()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
		temp0()->_max_data_length = _max_data_length;
		::rux::int32 j = get_BitsCount() - 1;  
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1;
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1;	
		while( j-- >= 0 )
		{	 
			result.FastLeftShift();
			temp0.FastLeftShift();
			temp0()->_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( temp0 >= raw_data )
			{
				temp0 -= raw_data;
				result()->_raw_data_ptr[ 0 ] |= 1;
			}
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		result()->_is_negative = result.get_IsZero() == false && ( ( _is_negative == 1 && raw_data._is_negative == 0 ) 
			|| ( _is_negative == 0 && raw_data._is_negative == 1 ) ) ? 1 : 0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		raw_data.Release();
		return result++;
	};
	RawData& XRawData::operator ^ ( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData temp0;
		temp0.Copy( raw_data );
		XRawData temp1;
		temp1.Copy( *this );
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp3;
		temp3()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp3()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp4;
		temp4()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp4()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp5;
		temp5()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp5()->_max_data_length = (*this)()->_max_data_length;
		temp3 = 1;
		while( temp1.get_IsZero() == false )
		{
			temp4 = temp0 % temp1;
			temp5 = result - ( temp0 / temp1 ) * temp3;
			temp0.Copy( temp1 );
			temp1.Copy( temp4 );
			result.Copy( temp3 );
			temp3.Copy( temp5 );
		};	
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	bool XRawData::get_IsZero( void )
	{
		return (*this)()->get_IsZero();
	};
	bool RawData::get_IsZero( void )
	{
		size_t aligned_size = get_AlignedSize();
		if( aligned_size <= 1024 )
		{
			::rux::uint8 zero[ 1024 ] = {0};
			memset( zero , 0 , aligned_size );
			return memcmp( _raw_data_ptr , zero , aligned_size ) == 0 ? true : false;
		}
		else
		{
			for( size_t index0 = 0; index0 < aligned_size ; index0 += sizeof( ::rux::uint32 ) )
			{
				if( ( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) != 0 ) 
					return false;
			}
		}
		return true;
	};
	RawData& XRawData::Sqrt( void )
	{
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp0;
		temp0.Copy( *this );
		XRawData temp1;
		temp1()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp1()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp2;
		temp2()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp2()->_max_data_length = (*this)()->_max_data_length;
		temp2()->_raw_data_ptr[ this->get_AlignedSize() - 1 ] = ( ( ::rux::uint8(-1) >> 1 ) + 1 ) >> 1;
		result = (::rux::uint32)0;		
		while( temp2.get_IsZero() == false )
		{
			temp1 = result + temp2;
			result.FastRightShift();
			if( temp0 >= temp1 )
			{
				temp0 -= temp1;
				result += temp2;
			};
			temp2 >> 2;
		};
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	::rux::uint32 XRawData::get_BitsCount( void ) const
	{	
		if( (*this)()->_bits_count == 0xffffffff )
		{
			(*this)()->_bits_count = 0;
			::rux::int32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32);
			for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
			{
				(*this)()->_bits_count = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				if( (*this)()->_bits_count )
					break;
			};
			if( index0 < 0 )
				(*this)()->_bits_count = 0;
			else
			{
				if( (*this)()->_bits_count >= 0x10000 ) 
					(*this)()->_bits_count >>= 16 , index0 +=2;
				if( (*this)()->_bits_count >= 0x100 ) 
					(*this)()->_bits_count >>= 8 , index0++;
				index0 <<= 3;
				if( (*this)()->_bits_count >= 0x80 ) 
					(*this)()->_bits_count = index0 + 7 + 1;
				else if( (*this)()->_bits_count >= 0x40 )
					(*this)()->_bits_count = index0 + 6 + 1;
				else if( (*this)()->_bits_count >= 0x20 ) 
					(*this)()->_bits_count = index0 + 5 + 1;
				else if( (*this)()->_bits_count >= 0x10 ) 
					(*this)()->_bits_count = index0 + 4 + 1;
				else if( (*this)()->_bits_count >= 0x08 ) 
					(*this)()->_bits_count = index0 + 3 + 1;
				else if( (*this)()->_bits_count >= 0x04 ) 
					(*this)()->_bits_count = index0 + 2 + 1;
				else if( (*this)()->_bits_count >= 0x02 ) 
					(*this)()->_bits_count = index0 + 1 + 1;
				else if( (*this)()->_bits_count >= 0x01 ) 
					(*this)()->_bits_count = index0 + 1;
				else
					(*this)()->_bits_count = index0 - 1 + 1;
			}
		}
		return (*this)()->_bits_count;
	};
	RawData& XRawData::Sqr( XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp0;	
		XRawData temp1;
		temp1.Copy( *this );	
		temp0.Copy( temp1 );	
		::rux::uint32 index0 = 0;
		::rux::int32 index1 = temp1.get_BitsCount() - 1;
		::rux::uint32 index2 = 1;
		while( index1-- >= 0 )	
		{		
			if( ( *(::rux::uint32*)&temp1()->_raw_data_ptr[ index0 ] ) & index2 )		
				result += temp0;
			index2 <<= 1;
			if( index2 == 0 )
				index2 = 1, index0 += 4;		
			temp0.FastLeftShift();
		};
		temp0.Copy( raw_data );
		index0 = 0;
		while( temp0 <= result )
		{
			if( temp0.get_IsZero() )
				break;
			index0++;
			if( temp0()->_raw_data_ptr[ (*this)()->_max_data_length - 1 ] & ( ( ::rux::uint8(-1) >> 1 ) + 1 ) ) 
				break;		
			temp0.FastLeftShift();
		};
		for( index2 = 0; index2 < index0; index2++ )
		{
			if( temp0 <= result )		
				result -= temp0;
			temp0.FastRightShift();
		};
		if( temp0 <= result )	
			result -= temp0;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	RawData& XRawData::ModularInverse( const XRawData& raw_data ) const
	{	
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		XRawData result;
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		result()->_max_data_length = (*this)()->_max_data_length;
		result = (::rux::uint32)0;
		XRawData temp0;
		temp0.Copy( *this );
		XRawData temp1;
		temp1.Copy( raw_data );
		XRawData temp2;
		temp2()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp2()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp3;
		temp3()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp3()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp4;
		temp4()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp4()->_max_data_length = (*this)()->_max_data_length;
		temp4 = 1;	
		while( temp0.get_IsZero() == false )
		{
			temp2 = temp1 / temp0;			
			temp3 = temp1 % temp0;		
			temp1.Copy( temp0 );		
			temp0.Copy( temp3 );		
			temp3.Copy( temp4 );								
			temp4 = result - temp2 * temp4;
			result.Copy( temp3 );		
		}
		if( result()->_is_negative == 1 ) 
			result += raw_data;
		result()->_force_size = SIZE_MAX;
		result()->_bits_count = 0xffffffff;
		result()->_raw_data_ptr_size = 0xffffffff;
		return result++;
	};
	::rux::uint32 XRawData::private_Divide( ::rux::uint32 number )
	{	
		if( number < 2 )
			return 1;
		::rux::uint32 reminder = 0;
		::rux::uint64 temp_number = 0;
		::rux::uint32 index0 = this->get_AlignedSize() - sizeof(::rux::uint32);
		for( ; index0 >= 0 ; index0 -= sizeof(::rux::uint32) )
		{
			temp_number = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) + ::rux::uint64( ::rux::uint64( reminder ) << ( sizeof(::rux::uint32) * 8 ) );		
			*(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] = ::rux::uint32( temp_number / number );
			reminder = ::rux::uint32( temp_number % number );
			if( index0 == 0 )
				break;
		};
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
		return reminder;
	};
	::rux::uint32 XRawData::private_VirtualDivide( ::rux::uint32 number )
	{	
		if( (*this)()->_is_negative == 1 )
			return 1;
		if( number < 2 )
			return 1;
		::rux::uint32 reminder = 0;
		::rux::uint64 temp_number = 0;
		for( ::rux::uint32 index0 = this->get_AlignedSize() - sizeof(::rux::uint32) ; index0 >= 0 ; index0 -= sizeof(::rux::uint32) )
		{
			temp_number = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] ) + ::rux::uint64( ::rux::uint64( reminder ) << ( sizeof(::rux::uint32) * 8 ) );		
			reminder = ::rux::uint32( temp_number % number );
			if( index0 == 0 )
				break;
		};
		return reminder;
	};
	bool XRawData::get_IsPrimeNumber( void ) const
	{	
		XRawData temp0;	
		temp0()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp0()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp1;
		temp1.Copy( *this );
		XRawData temp2;
		temp2 = temp1.Sqrt();
		XRawData temp3;
		temp3()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp3()->_max_data_length = (*this)()->_max_data_length;
		XRawData temp4;
		temp4()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
		temp4()->_max_data_length = (*this)()->_max_data_length;
		::rux::int32 index0 = 0;
		for( index0 ; index0 < sizeof( _prime_numbers ) / sizeof( _prime_numbers[ 0 ] ) ; index0++ )
		{
			if( temp1 > _prime_numbers[ index0 ] )
			{
				if( temp1.private_VirtualDivide( _prime_numbers[ index0 ] ) == 0 )
					return false;
			}
		};
		temp1.Copy( *this );
		temp1 = temp1 - 1;
		temp4.Copy( temp1 );
		if( temp1.get_IsZero() )
			return false;
		::rux::int32 index1 = 0;
		while( temp1.get_IsOddNumber() == false )
		{		
			temp1.FastRightShift();
			index1++;
		};
		temp2.Copy( *this );
		::rux::int32 index2;
		for( index0 = 0 ; index0 < 10 ; index0++ )
		{
			if( *this > _prime_numbers[ index0 + 1 ] )
			{
				temp0 = _prime_numbers[ index0 + 1 ];
				temp3.Copy( temp0 );
				temp3()->modExp( *temp1() , *temp2() );
				temp0 = index1 - 1;
				if( ( temp3 != 1 ) && ( temp3 != temp4 ) )
				{
					index2 = 1;
					while( ( index2 > 0 ) && ( temp0 >= index2 ) && ( temp3 != temp4 ) )
					{				
						temp3 = temp3.Sqr( temp2 );
						if( temp3 == 1 )
							return false;
						index2++;
					};
					if( temp3 != temp4 )
						return false;
				};
			}
		};
		return true;
	};
	void XRawData::set_OddNumber( void )
	{
		if( (*this)()->_raw_data_ptr )
			(*this)()->_raw_data_ptr[ 0 ] |= 1;
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
	};
	void XRawData::set_EvenNumber( void )
	{
		if( (*this)()->_raw_data_ptr )
			(*this)()->_raw_data_ptr[ 0 ] &= ::rux::uint8(-1) - 1;
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
	};
	bool XRawData::get_IsOddNumber( void )
	{
		if( (*this)()->_raw_data_ptr )
			return (*this)()->_raw_data_ptr[ 0 ] & 0x01;
		else
			return false;
	};
	void XRawData::Copy( const XRawData& raw_data )
	{	
		(*this)()->Copy( *raw_data() );
	};
	void XRawData::Copy( RawData& raw_data )
	{
		(*this)()->Copy( raw_data );
	}
	void RawData::Copy( RawData& raw_data )
	{	
		raw_data.AddRef();
		if( _raw_data_ptr == NULL
			|| _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{			
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );			
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}
		else if( _max_data_length > 0 )
			memset( _raw_data_ptr , 0 , _max_data_length );
		if( raw_data.get_AlignedSize() > 0 )	
			::rux::engine::memcpy( _raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
		_raw_data_ptr_size = raw_data.get_AlignedSize();	
		_bits_count = raw_data.get_BitsCount();
		_is_carry = raw_data._is_carry;
		_is_negative = raw_data._is_negative;
		raw_data.Release();
	};
	::rux::uint8 XRawData::private_IsGreater( XRawData& raw_data )
	{
		return (*this)()->private_IsGreater( *raw_data() );
	};
	::rux::uint8 XRawData::private_IsGreater( const XRawData& raw_data )
	{
		return (*this)()->private_IsGreater( *raw_data() );
	};
	::rux::uint8 RawData::private_IsGreater( RawData& raw_data )
	{
		raw_data.AddRef();
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}
		if( XRawData::compare( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length ) > 0 )
		{
			raw_data.Release();
			return 1;
		}
		raw_data.Release();
		return 0;
	};
	::rux::uint8 XRawData::private_IsGreater( RawData& raw_data )
	{
		raw_data.AddRef();
		::rux::uint8 res = (*this)()->private_IsGreater( raw_data );
		raw_data.Release();
		return res;
	};
	::rux::uint8 XRawData::private_IsLess( ::rux::uint32 number )
	{
		return (*this)()->private_IsLess( number );
	};
	::rux::uint8 RawData::private_IsLess( ::rux::uint32 number )
	{
		if( _max_data_length >= sizeof( ::rux::uint32 ) )
		{		
			for( ::rux::uint32 index0 = _max_data_length - sizeof(::rux::uint32) ; index0 >= sizeof(::rux::uint32); index0 -= sizeof(::rux::uint32) )
			{
				if( *(::rux::uint32*)&_raw_data_ptr[ index0 ] ) 
					return 0;
			};
			return (*(::rux::uint32*)&_raw_data_ptr[ 0 ]) < number ? 1 : 0;
		}
		return 0;
	};
	::rux::uint8 XRawData::get_ByteAt( ::rux::uint32 index )
	{
		if( (*this)()->_max_data_length > index )
			return (*this)()->_raw_data_ptr[ index ];
		else
			return 0;
	};
	::rux::String& XRawData::ToOctetString( void )
	{
		::rux::XString hex_string;
		size_t size = get_Size();
		::rux::uint8* raw_data = get_RawData();
		for( size_t index0 = 0 ; index0 < size ; index0++ )		
		{
			if( index0 > 0 )
				hex_string += " ";
			hex_string += XString::Format( "%02lx" , (::rux::int32)raw_data[ index0 ] );
		}
		return hex_string++;
	};
	::rux::String& XRawData::ToReverseOctetString( void )
	{
		::rux::XString hex_string;
		size_t size = get_Size();
		if( size > 0 )
		{
			::rux::uint8* raw_data = get_RawData();
			for( size_t index0 = size - 1 ; ; index0-- )		
			{
				if( index0 != size - 1 )
					hex_string += " ";
				hex_string += XString::Format( "%02lx" , (::rux::int32)raw_data[ index0 ] );
				if( index0 == 0 )
					break;
			}
		}
		return hex_string++;
	};
	::rux::String& XRawData::ToOctetString( ::rux::uint8* data_ptr , size_t data_ptr_length )
	{
		::rux::XString hex_string;
		for( size_t index0 = 0 ; index0 < data_ptr_length ; index0++ )
		{
			if( index0 > 0 )
				hex_string += " ";
			hex_string += XString::Format( "%02lx" , (::rux::int32)data_ptr[ index0 ] );
		}
		return hex_string++;
	};
	::rux::String& XRawData::ToReverseOctetString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_length )
	{
		::rux::XString hex_string;
		if( data_ptr_length > 0 )
		{
			for( size_t index0 = data_ptr_length - 1 ; ; index0-- )
			{
				if( index0 != data_ptr_length - 1 )
					hex_string += " ";
				hex_string += XString::Format( "%02lx" , (::rux::int32)data_ptr[ index0 ] );
				if( index0 == 0 )
					break;
			}
		}
		return hex_string++;
	};
	::rux::String& XRawData::ToReverseHexString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_size )
	{
		::rux::XString hex_string;
		for( ::rux::uint32 index0 = 0 ; index0 < data_ptr_size ; index0++ )
			hex_string += XString::Format( "%02lx" , (::rux::int32)data_ptr[ index0 ] );
		return hex_string++;
	};
	::rux::String& XRawData::ToHexString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_length )
	{	
		::rux::XString hex_string;
		for( ::rux::uint32 index0 = 0 ; index0 < data_ptr_length ; index0++ )
		{
			if( index0 > 0 )
				hex_string += ", ";
			hex_string += XString::Format( "0x%02lx" , (::rux::int32)data_ptr[ index0 ] );		
		}
		return hex_string++;
	};
	RawData& XRawData::I2OSP( ::rux::uint32 octet_string_length , ::rux::XString& error )
	{
		error.Clear();
		XRawData result;
		if( octet_string_length > 0 )
		{
			result()->_raw_data_ptr_size = octet_string_length == 0 ? 8 : octet_string_length;
			result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
			result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
			result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif		
			result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );		
			::rux::uint64 length = 256;
			XRawData raw_data( (::rux::uint8*)&length , sizeof( ::rux::uint64 ) );
			length = 0;
			while( length < octet_string_length )
			{
				raw_data.FastLeftShift();
				length++;
			}
			//if( *this >= raw_data )
			//	error = "integer too large";
			//else
			{
				::rux::uint64 index0 = 0;
				for( length = octet_string_length - 1 ; length >= 0 ; length-- )
				{				
					if( length >= get_Size() )
						result()->_raw_data_ptr[ index0 ] = 0x00;
					else
						result()->_raw_data_ptr[ index0 ] = (*this)()->_raw_data_ptr[ length ];
					if( length == 0 )
						break;
					index0++;
				}
				result()->_force_size = octet_string_length;
			}
		}
		return result++;
	};
	::rux::String& XRawData::I2OSP_ToString( ::rux::uint32 octet_string_length , ::rux::XString& error )
	{
		::rux::XString octet_string;
		if( octet_string_length > 0 )
		{
			error.Clear();
			::rux::uint64 length = 256;
			XRawData raw_data( (::rux::uint8*)&length , sizeof( ::rux::uint64 ) );
			length = 0;
			while( length < octet_string_length )
			{
				raw_data.FastLeftShift();
				length++;
			}
			//if( *this >= raw_data )
			//	error = "integer too large";
			//else
			{
				for( length = octet_string_length - 1 ; length >= 0 ; length-- )
				{
					if( length < octet_string_length - 1 )
						octet_string += " ";
					if( length >= this->get_AlignedSize() )
						octet_string += "00";
					else
						octet_string += XString::Format( "%02lx" , (::rux::int32)(*this)()->_raw_data_ptr[ length ] );
					if( length == 0 )
						break;
				}
			}
		}
		return octet_string++;
	};
	Array<XString>& XRawData::SplitOctetStringByLength( const ::rux::XString& octet_string , ::rux::uint32 octet_string_length )
	{
		XArray<XString> bytes = octet_string.Split( ' ' );
		XArray<XString> octet_strings;
		::rux::uint32 index0 = 0;
		for( ; index0 < bytes.Count() ; index0 += octet_string_length )	
			octet_strings.Add( XString::Join( " " , bytes , index0 , octet_string_length ) );
		index0 = bytes.Count() / octet_string_length;
		if( bytes.Count() - index0 * octet_string_length > 0 )
			octet_strings.Add( XString::Join( " " , bytes , index0 , bytes.Count() - index0 * octet_string_length ) );
		return octet_strings++;
	};
	RawData& XRawData::FromReverseOctetString( const ::rux::XString& octet_string , ::rux::XString& error )
	{
		error.Clear();	
		XArray< XString > separators;
		separators.Add( ::rux::XString( " " ) );
		separators.Add( ::rux::XString( "\t" ) );
		separators.Add( ::rux::XString( "\n" ) );
		separators.Add( ::rux::XString( "\r" ) );
		XArray<XString> bytes;
		bytes.set_ByRef( octet_string.Split( separators ) );
		XRawData result;
		result()->_raw_data_ptr_size = bytes.Count() == 0 ? 8 : bytes.Count();
		result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );
		if( bytes.Count() > 0 )
		{		
			::rux::uint8 high = 0;
			::rux::uint8 low = 0;
			declare_stack_variable( char , letter , 2 );
			size_t index1 = bytes.Count() - 1;
			for( size_t index0 = 0 ; index0 < bytes.Count() ; index0++ )
			{
				if( bytes[ index0 ].Length() == 2 )
				{
					letter[ 0 ] = bytes[ index0 ].get_AnsiChar( 0 );
					if( letter[ 0 ] >= 0x30 && letter[ 0 ] < 0x3a )
						high = 16 * static_cast< ::rux::uint8>( atoi( letter ) );
					else if( letter[ 0 ] == 'a' || letter[ 0 ] == 'A' )
						high = 160;
					else if( letter[ 0 ] == 'b' || letter[ 0 ] == 'B' )
						high = 176;
					else if( letter[ 0 ] == 'c' || letter[ 0 ] == 'C' )
						high = 192;
					else if( letter[ 0 ] == 'd' || letter[ 0 ] == 'D' )
						high = 208;
					else if( letter[ 0 ] == 'e' || letter[ 0 ] == 'E' )
						high = 224;
					else if( letter[ 0 ] == 'f' || letter[ 0 ] == 'F' )
						high = 240;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter[ 0 ] );
						break;
					}					
					letter[ 0 ] = bytes[ index0 ].get_AnsiChar( 1 );
					if( letter[ 0 ] >= 0x30 && letter[ 0 ] < 0x3a )
						low = static_cast< ::rux::uint8>( atoi( letter ) );
					else if( letter[ 0 ] == 'a' || letter[ 0 ] == 'A' )
						low = 10;
					else if( letter[ 0 ] == 'b' || letter[ 0 ] == 'B' )
						low = 11;
					else if( letter[ 0 ] == 'c' || letter[ 0 ] == 'C' )
						low = 12;
					else if( letter[ 0 ] == 'd' || letter[ 0 ] == 'D' )
						low = 13;
					else if( letter[ 0 ] == 'e' || letter[ 0 ] == 'E' )
						low = 14;
					else if( letter[ 0 ] == 'f' || letter[ 0 ] == 'F' )
						low = 15;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter[ 0 ] );
						break;
					}
					result()->_raw_data_ptr[ index1 ] = high + low;			
					index1--;
				}
				else
				{
					error = "incorrect format of octet string";
					break;
				}
			}		
		}
		result()->_force_size = bytes.Count();
		return result++;
	};
	RawData& XRawData::FromOctetString( const ::rux::XString& octet_string , ::rux::XString& error )
	{
		error.Clear();	
		XArray< XString > separators;
		separators.Add( ::rux::XString( " " ) );
		separators.Add( ::rux::XString( "\t" ) );
		separators.Add( ::rux::XString( "\n" ) );
		separators.Add( ::rux::XString( "\r" ) );
		XArray<XString> bytes;
		bytes.set_ByRef( octet_string.Split( separators ) );
		XRawData result;
		result()->_raw_data_ptr_size = bytes.Count() == 0 ? 8 : bytes.Count();
		result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );
		if( bytes.Count() > 0 )
		{		
			::rux::uint8 high = 0;
			::rux::uint8 low = 0;
			declare_stack_variable( char , letter , 2 );
			for( ::rux::uint32 index0 = 0 ; index0 < bytes.Count() ; index0++ )
			{
				if( bytes[ index0 ].Length() == 2 )
				{
					letter[ 0 ] = bytes[ index0 ].get_AnsiChar( 0 );
					if( letter[ 0 ] >= 0x30 && letter[ 0 ] < 0x3a )
						high = 16 * static_cast< ::rux::uint8>( atoi( letter ) );
					else if( letter[ 0 ] == 'a' || letter[ 0 ] == 'A' )
						high = 160;
					else if( letter[ 0 ] == 'b' || letter[ 0 ] == 'B' )
						high = 176;
					else if( letter[ 0 ] == 'c' || letter[ 0 ] == 'C' )
						high = 192;
					else if( letter[ 0 ] == 'd' || letter[ 0 ] == 'D' )
						high = 208;
					else if( letter[ 0 ] == 'e' || letter[ 0 ] == 'E' )
						high = 224;
					else if( letter[ 0 ] == 'f' || letter[ 0 ] == 'F' )
						high = 240;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter[ 0 ] );
						break;
					}					
					letter[ 0 ] = bytes[ index0 ].get_AnsiChar( 1 );
					if( letter[ 0 ] >= 0x30 && letter[ 0 ] < 0x3a )
						low = static_cast< ::rux::uint8>( atoi( letter ) );
					else if( letter[ 0 ] == 'a' || letter[ 0 ] == 'A' )
						low = 10;
					else if( letter[ 0 ] == 'b' || letter[ 0 ] == 'B' )
						low = 11;
					else if( letter[ 0 ] == 'c' || letter[ 0 ] == 'C' )
						low = 12;
					else if( letter[ 0 ] == 'd' || letter[ 0 ] == 'D' )
						low = 13;
					else if( letter[ 0 ] == 'e' || letter[ 0 ] == 'E' )
						low = 14;
					else if( letter[ 0 ] == 'f' || letter[ 0 ] == 'F' )
						low = 15;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter[ 0 ] );
						break;
					}
					result()->_raw_data_ptr[ index0 ] = high + low;				
				}
				else
				{
					error = "incorrect format of octet string";
					break;
				}
			}		
		}
		result()->_force_size = bytes.Count();
		return result++;
	};
	RawData& XRawData::OS2IP( rux::io::file& i2osp_file , ::rux::XString& error )
	{
		error.Clear();	
		XRawData result;
		if( i2osp_file.opened() )
		{
			size_t i2osp_ptr_length = i2osp_file.size();
			if( i2osp_ptr_length > 0 )
			{
				result()->_raw_data_ptr_size = i2osp_ptr_length;
				result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
				result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
				result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
				result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );
				size_t index1 = 0 , readen = 0;
				for( size_t index0 = i2osp_ptr_length - 1 ; index0 >= 0 ; index0-- )
				{
					if( i2osp_file.seek( index0 ) == false )
					{
						error = "cannot seek in i2osp_file";
						break;
					}
					if( i2osp_file.read_bytes( (char*)&result()->_raw_data_ptr[ index1 ] , 1 , readen ) == false )
					{
						error = "cannot read from i2osp_file";
						break;
					}
					if( index0 == 0 )
						break;
					index1++;
				}
				result()->_force_size = i2osp_ptr_length;
			}
			else
				error = "i2osp pointer length is equal 0";
		}
		else
			error = "i2osp_file is not opened";
		return result++;
	};
	RawData& XRawData::OS2IP( const ::rux::uint8* i2osp_ptr , ::rux::uint32 i2osp_ptr_length , ::rux::XString& error )
	{
		error.Clear();	
		XRawData result;
		if( i2osp_ptr )
		{
			if( i2osp_ptr_length > 0 )
			{			
				result()->_raw_data_ptr_size = i2osp_ptr_length == 0 ? 8 : i2osp_ptr_length;
				result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
				result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
				result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
				result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );
				::rux::uint32 index1 = 0;			
				for( ::rux::uint32 index0 = i2osp_ptr_length - 1 ; index0 >= 0 ; index0-- )
				{
					result()->_raw_data_ptr[ index1 ] = i2osp_ptr[ index0 ];
					if( index0 == 0 )
						break;
					index1++;
				}
				result()->_force_size = i2osp_ptr_length;
			}
			else
				error = "i2osp pointer length is equal 0";
		}
		else
			error = "i2osp pointer is NULL";
		return result++;
	};
	RawData& XRawData::OS2IP( const ::rux::XString& i2osp , ::rux::XString& error )
	{
		error.Clear();	
		XArray< XString > separators;
		separators.Add( ::rux::XString( " " ) );
		separators.Add( ::rux::XString( "\t" ) );
		separators.Add( ::rux::XString( "\n" ) );
		separators.Add( ::rux::XString( "\r" ) );
		XArray<XString> bytes;
		bytes.set_ByRef( i2osp.Split( separators ) );
		XRawData result;
		result()->_raw_data_ptr_size = bytes.Count() == 0 ? 8 : bytes.Count();
		result()->_raw_data_ptr_size = ( ( result()->_raw_data_ptr_size + 7 ) >> 3 ) << 3;						 
	#ifdef __x86__
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		result()->_max_data_length = ( result()->_raw_data_ptr_size << 1 ) + sizeof( ::rux::uint64 );
	#endif
		result()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , result()->_max_data_length );
		if( bytes.Count() > 0 )
		{
			::rux::uint32 index1 = 0;
			::rux::uint8 high = 0;
			::rux::uint8 low = 0;
			char letter = ' ';
			for( ::rux::uint32 index0 = bytes.Count() - 1 ; index0 >= 0 ; index0-- )
			{
				if( bytes[ index0 ].Length() == 2 )
				{
					letter = bytes[ index0 ].get_AnsiChar( 0 );
					if( letter >= 0x30 && letter < 0x3a )
						high = 16 * static_cast< ::rux::uint8>( atoi( &letter ) );
					else if( letter == 'a' || letter == 'A' )
						high = 160;
					else if( letter == 'b' || letter == 'B' )
						high = 176;
					else if( letter == 'c' || letter == 'C' )
						high = 192;
					else if( letter == 'd' || letter == 'D' )
						high = 208;
					else if( letter == 'e' || letter == 'E' )
						high = 224;
					else if( letter == 'f' || letter == 'F' )
						high = 240;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter );
						break;
					}					
					letter = bytes[ index0 ].get_AnsiChar( 1 );
					if( letter >= 0x30 && letter < 0x3a )
						low = static_cast< ::rux::uint8>( atoi( &letter ) );
					else if( letter == 'a' || letter == 'A' )
						low = 10;
					else if( letter == 'b' || letter == 'B' )
						low = 11;
					else if( letter == 'c' || letter == 'C' )
						low = 12;
					else if( letter == 'd' || letter == 'D' )
						low = 13;
					else if( letter == 'e' || letter == 'E' )
						low = 14;
					else if( letter == 'f' || letter == 'F' )
						low = 15;
					else
					{
						error = XString::Format( "incorrect format of octet string , unknown symbol 0x%lx" , (::rux::uint8)letter );
						break;
					}
					result()->_raw_data_ptr[ index1 ] = high + low;
					if( index0 == 0 )
						break;
					index1++;
				}
				else
				{
					error = "incorrect format of octet string";
					break;
				}
			}		
		}
		result()->_force_size = bytes.Count();
		return result++;
	};
	::rux::uint32 XRawData::get_Size( void )
	{	
		if( (*this)()->_force_size == SIZE_MAX )
			{	
			if( (*this)()->_bits_count == 0xffffffff )
			{
				(*this)()->_bits_count = 0;
				::rux::int32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32);
				for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
				{
					(*this)()->_bits_count = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					if( (*this)()->_bits_count )
						break;
				};
				if( index0 < 0 )
					(*this)()->_bits_count = 0;
				else
				{
					if( (*this)()->_bits_count >= 0x10000 ) 
						(*this)()->_bits_count >>= 16 , index0 +=2;
					if( (*this)()->_bits_count >= 0x100 ) 
						(*this)()->_bits_count >>= 8 , index0++;
					index0 <<= 3;
					if( (*this)()->_bits_count >= 0x80 ) 
						(*this)()->_bits_count = index0 + 7 + 1;
					else if( (*this)()->_bits_count >= 0x40 )
						(*this)()->_bits_count = index0 + 6 + 1;
					else if( (*this)()->_bits_count >= 0x20 ) 
						(*this)()->_bits_count = index0 + 5 + 1;
					else if( (*this)()->_bits_count >= 0x10 ) 
						(*this)()->_bits_count = index0 + 4 + 1;
					else if( (*this)()->_bits_count >= 0x08 ) 
						(*this)()->_bits_count = index0 + 3 + 1;
					else if( (*this)()->_bits_count >= 0x04 ) 
						(*this)()->_bits_count = index0 + 2 + 1;
					else if( (*this)()->_bits_count >= 0x02 ) 
						(*this)()->_bits_count = index0 + 1 + 1;
					else if( (*this)()->_bits_count >= 0x01 ) 
						(*this)()->_bits_count = index0 + 1;
					else
						(*this)()->_bits_count = index0 - 1 + 1;
				}
			}
			::rux::uint32 size = 0;
			::rux::uint32 index0 = (*this)()->_bits_count;
			if( index0 != 0 )
			{			
				size = index0 / 8;
				if( index0 - size * 8 > 0 )
					size += 1;			
			}	
			return size;
		}
		else
			return (*this)()->_force_size;
	};
	::rux::uint32 XRawData::get_AlignedSize( void ) const
	{
		if( (*this)()->_raw_data_ptr_size == 0xffffffff )
		{
			if( (*this)()->_bits_count == 0xffffffff )
			{
				(*this)()->_bits_count = 0;
				::rux::int32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32);
				for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
				{
					(*this)()->_bits_count = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
					if( (*this)()->_bits_count )
						break;
				};
				if( index0 < 0 )
					(*this)()->_bits_count = 0;
				else
				{
					if( (*this)()->_bits_count >= 0x10000 ) 
						(*this)()->_bits_count >>= 16 , index0 +=2;
					if( (*this)()->_bits_count >= 0x100 ) 
						(*this)()->_bits_count >>= 8 , index0++;
					index0 <<= 3;
					if( (*this)()->_bits_count >= 0x80 ) 
						(*this)()->_bits_count = index0 + 7 + 1;
					else if( (*this)()->_bits_count >= 0x40 )
						(*this)()->_bits_count = index0 + 6 + 1;
					else if( (*this)()->_bits_count >= 0x20 ) 
						(*this)()->_bits_count = index0 + 5 + 1;
					else if( (*this)()->_bits_count >= 0x10 ) 
						(*this)()->_bits_count = index0 + 4 + 1;
					else if( (*this)()->_bits_count >= 0x08 ) 
						(*this)()->_bits_count = index0 + 3 + 1;
					else if( (*this)()->_bits_count >= 0x04 ) 
						(*this)()->_bits_count = index0 + 2 + 1;
					else if( (*this)()->_bits_count >= 0x02 ) 
						(*this)()->_bits_count = index0 + 1 + 1;
					else if( (*this)()->_bits_count >= 0x01 ) 
						(*this)()->_bits_count = index0 + 1;
					else
						(*this)()->_bits_count = index0 - 1 + 1;
				}
			}
			(*this)()->_raw_data_ptr_size = (*this)()->_bits_count;
			if( (*this)()->_raw_data_ptr_size != 0 )
			{			
				::rux::uint32 index0 = (*this)()->_raw_data_ptr_size / 8;
				if( (*this)()->_raw_data_ptr_size - index0 * 8 > 0 )
					index0 += 1;
				(*this)()->_raw_data_ptr_size = ( ( index0 + 7 ) >> 3 ) << 3;						 
			}
		}
		return (*this)()->_raw_data_ptr_size;
	};
	::rux::uint32 XRawData::get_SizeInUlong32Count( void ) const
	{
		::rux::uint32 size = 0;
		if( (*this)()->_bits_count == 0xffffffff )
		{
			(*this)()->_bits_count = 0;
			::rux::int32 index0 = (*this)()->_max_data_length - sizeof(::rux::uint32);
			for( ; index0 >= 0; index0 -= sizeof(::rux::uint32) )
			{
				(*this)()->_bits_count = ( *(::rux::uint32*)&(*this)()->_raw_data_ptr[ index0 ] );
				if( (*this)()->_bits_count )
					break;
			};
			if( index0 < 0 )
				(*this)()->_bits_count = 0;
			else
			{
				if( (*this)()->_bits_count >= 0x10000 ) 
					(*this)()->_bits_count >>= 16 , index0 +=2;
				if( (*this)()->_bits_count >= 0x100 ) 
					(*this)()->_bits_count >>= 8 , index0++;
				index0 <<= 3;
				if( (*this)()->_bits_count >= 0x80 ) 
					(*this)()->_bits_count = index0 + 7 + 1;
				else if( (*this)()->_bits_count >= 0x40 )
					(*this)()->_bits_count = index0 + 6 + 1;
				else if( (*this)()->_bits_count >= 0x20 ) 
					(*this)()->_bits_count = index0 + 5 + 1;
				else if( (*this)()->_bits_count >= 0x10 ) 
					(*this)()->_bits_count = index0 + 4 + 1;
				else if( (*this)()->_bits_count >= 0x08 ) 
					(*this)()->_bits_count = index0 + 3 + 1;
				else if( (*this)()->_bits_count >= 0x04 ) 
					(*this)()->_bits_count = index0 + 2 + 1;
				else if( (*this)()->_bits_count >= 0x02 ) 
					(*this)()->_bits_count = index0 + 1 + 1;
				else if( (*this)()->_bits_count >= 0x01 ) 
					(*this)()->_bits_count = index0 + 1;
				else
					(*this)()->_bits_count = index0 - 1 + 1;
			}
		}
		::rux::uint32 index1 = (*this)()->_bits_count;
		if( index1 != 0 )
		{		
			size = index1 / ( 8 * sizeof( ::rux::uint32 ) );
			if( index1 - size * ( 8 * sizeof( ::rux::uint32 ) ) > 0 )
				size += 1;		
		}
		return size;
	};
	size_t XRawData::get_AllocatedSize( void )
	{
		return (*this)()->_max_data_length;
	};
	XRawData& XRawData::operator -= (XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		if( (*this)()->_is_negative != raw_data()->_is_negative )
		{		
			plus( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );
			(*this)()->_is_negative = get_IsZero() ? 0 : (*this)()->_is_negative;
		}
		else
		{
			if( private_IsGreater( raw_data ) )
			{			
				minus( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );			
				(*this)()->_is_negative = get_IsZero() ? 0 : (*this)()->_is_negative;
			}
			else
			{
				::rux::uint8* temp_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( temp_ptr , 0 , (*this)()->_raw_data_ptr , (*this)()->_max_data_length );
				::rux::engine::memcpy( (*this)()->_raw_data_ptr , 0 , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );			
				minus( (*this)()->_raw_data_ptr , temp_ptr , (*this)()->_max_data_length );			
				(*this)()->_is_negative = get_IsZero() ? 0 : raw_data()->_is_negative == 1 ? 0 : 1;
				::rux::engine::free_mem( temp_ptr );
			}
		}	
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	XRawData& XRawData::operator -= ( RawData& raw_data )
	{
		raw_data.AddRef();
		*(*this)() -= raw_data;
		raw_data.Release();
		return *this;
	};
	RawData& RawData::operator -= ( RawData& raw_data )
	{	
		raw_data.AddRef();
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , this->get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}	
		if( _is_negative != raw_data._is_negative )
		{		
			XRawData::plus( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length );			
			_is_negative = get_IsZero() ? 0 : _is_negative;
		}
		else
		{
			if( private_IsGreater( raw_data ) )
			{			
				XRawData::minus( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length );
				_is_negative = get_IsZero() ? 0 : _is_negative;
			}
			else
			{
				::rux::uint8* temp_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( temp_ptr , 0 , _raw_data_ptr , _max_data_length );
				::rux::engine::memcpy( _raw_data_ptr , 0 , raw_data._raw_data_ptr , _max_data_length );			
				XRawData::minus( _raw_data_ptr , temp_ptr , _max_data_length );
				_is_negative = get_IsZero() ? 0 : raw_data._is_negative == 1 ? 0 : 1;
				::rux::engine::free_mem( temp_ptr );
			}
		}	
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		raw_data.Release();
		return *this;
	};
	RawData& RawData::operator %= (RawData& raw_data2 )
	{
		raw_data2.AddRef();
		if( raw_data2._max_data_length > _max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data2._max_data_length );
			_max_data_length = raw_data2._max_data_length;
		}
		else if( raw_data2._max_data_length < _max_data_length )
		{
			if( raw_data2._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data2._raw_data_ptr , raw_data2.get_AlignedSize() );
				if( raw_data2._raw_data_ptr )
					::rux::engine::free_mem( raw_data2._raw_data_ptr );
				raw_data2._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data2._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data2._max_data_length = _max_data_length;
		}
		XRawData raw_data1;
		raw_data1.Copy( *this );	
		RawData* raw_data1_ptr = raw_data1();
		memset( _raw_data_ptr , 0 , _max_data_length );
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		::rux::int32 j = raw_data1.get_BitsCount() - 1;
		::rux::uint32 index0 = j + 1;
		::rux::uint32 i = index0 / 8;
		if( index0 - i * 8 > 0 )
			i += 1; 
		::rux::uint8 k = 1 << ( j % 8 );	
		i -= 1; 
		while( j-- >= 0 )
		{	 
			FastLeftShift();
			_raw_data_ptr[ 0 ] |= ( ( *(::rux::uint32*)&raw_data1_ptr->_raw_data_ptr[ i ] ) & k ) ? 1 : 0;
			if( *this >= raw_data2 )
				*this -= raw_data2;	 
			k >>= 1;
			if( !k ) k = 1 << 7, i --;
		}
		_is_negative = get_IsZero() == false && ( ( raw_data1_ptr->_is_negative == 1 && raw_data2._is_negative == 0 ) 
			|| ( raw_data1_ptr->_is_negative == 0 && raw_data2._is_negative == 1 ) ) ? 1 : 0;
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;	
		raw_data2.Release();	
		return *this;
	}
	RawData& RawData::operator += (XRawData& raw_data )
	{	
		*this += *raw_data();
		return *this;
	};
	RawData& RawData::operator += (RawData& raw_data )
	{	
		if( _max_data_length > raw_data._max_data_length )
		{
			if( raw_data._raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data._raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data._raw_data_ptr )
					::rux::engine::free_mem( raw_data._raw_data_ptr );
				raw_data._raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data._raw_data_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
			raw_data._max_data_length = _max_data_length;
		}
		else if( _max_data_length < raw_data._max_data_length )
		{
			if( _raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , _raw_data_ptr , get_AlignedSize() );
				if( _raw_data_ptr )
					::rux::engine::free_mem( _raw_data_ptr );
				_raw_data_ptr = raw_data_ptr;		
			}
			else
				_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data._max_data_length );
			_max_data_length = raw_data._max_data_length;
		}	
		if( _is_negative == raw_data._is_negative )	
			XRawData::plus( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length );
		else
		{
			if( XRawData::compare( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length ) > 0 )
			{
				XRawData::minus( _raw_data_ptr , raw_data._raw_data_ptr , _max_data_length );
				_is_negative = get_IsZero() ? 0 : _is_negative;
			}
			else
			{
				::rux::uint8* temp_ptr = alloc_array_macros( ::rux::uint8 , _max_data_length );
				::rux::engine::memcpy( temp_ptr , 0 , _raw_data_ptr , _max_data_length );
				::rux::engine::memcpy( _raw_data_ptr , 0 , raw_data._raw_data_ptr , _max_data_length );	
				XRawData::minus( _raw_data_ptr , temp_ptr , _max_data_length );						
				_is_negative = get_IsZero() ? 0 : raw_data._is_negative;
				::rux::engine::free_mem( temp_ptr );
			}
		}
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	XRawData& XRawData::operator += (XRawData& raw_data )
	{
		*(*this)() += raw_data;
		return *this;
	};
	XRawData& XRawData::operator += (const XRawData& raw_data )
	{
		if( (*this)()->_max_data_length > raw_data()->_max_data_length )
		{
			if( raw_data()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , raw_data()->_raw_data_ptr , raw_data.get_AlignedSize() );
				if( raw_data()->_raw_data_ptr )
					::rux::engine::free_mem( raw_data()->_raw_data_ptr );
				raw_data()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				raw_data()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
			raw_data()->_max_data_length = (*this)()->_max_data_length;
		}
		else if( (*this)()->_max_data_length < raw_data()->_max_data_length )
		{
			if( (*this)()->_raw_data_ptr )
			{
				::rux::uint8* raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
				::rux::engine::memcpy( raw_data_ptr , 0 , (*this)()->_raw_data_ptr , this->get_AlignedSize() );
				if( (*this)()->_raw_data_ptr )
					::rux::engine::free_mem( (*this)()->_raw_data_ptr );
				(*this)()->_raw_data_ptr = raw_data_ptr;		
			}
			else
				(*this)()->_raw_data_ptr = alloc_array_macros( ::rux::uint8 , raw_data()->_max_data_length );
			(*this)()->_max_data_length = raw_data()->_max_data_length;
		}	
		if( (*this)()->_is_negative == raw_data()->_is_negative )	
			plus( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );
		else
		{
			if( private_IsGreater( raw_data ) )
			{
				minus( (*this)()->_raw_data_ptr , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );
				(*this)()->_is_negative = get_IsZero() ? 0 : (*this)()->_is_negative;
			}
			else
			{
				::rux::uint8* temp_ptr = alloc_array_macros( ::rux::uint8 , (*this)()->_max_data_length );
				::rux::engine::memcpy( temp_ptr , 0 , (*this)()->_raw_data_ptr , (*this)()->_max_data_length );
				::rux::engine::memcpy( (*this)()->_raw_data_ptr , 0 , raw_data()->_raw_data_ptr , (*this)()->_max_data_length );	
				minus( (*this)()->_raw_data_ptr , temp_ptr , (*this)()->_max_data_length );						
				(*this)()->_is_negative = get_IsZero() ? 0 : raw_data()->_is_negative;
				::rux::engine::free_mem( temp_ptr );
			}
		}
		(*this)()->_force_size = SIZE_MAX;
		(*this)()->_bits_count = 0xffffffff;
		(*this)()->_raw_data_ptr_size = 0xffffffff;
		return *this;
	};
	::rux::uint8* XRawData::get_RawData( void )
	{
		return (*this)()->_raw_data_ptr;
	};
	void XRawData::minus( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len )
	{
	#if NO_ASM
		::rux::uint64 sub = 0;
		::rux::int32 _is_carry = 0;
		for( size_t index0 = 0 ; index0 < len ; index0 += sizeof( ::rux::uint32 ) )
		{
			sub = (::rux::uint64)( *(::rux::uint32*)&buffer_ptr0[ index0 ] );
			sub -= ( *(::rux::uint32*)&buffer_ptr1[ index0 ] );
			sub -= (::rux::uint32)_is_carry;
			_is_carry = ( sub & 0x0000000100000000 ) ? 1 : 0;
			*(::rux::uint32*)&buffer_ptr0[ index0 ] = (::rux::uint32)sub;
		};
	#else
	#ifdef __x86__			
		len /= sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		len /= sizeof( ::rux::uint64 );
	#endif
		AsmMinus( buffer_ptr0 , buffer_ptr1 , len );	
	#endif
	};
	void XRawData::plus( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len )
	{
	#if NO_ASM
		::rux::uint64 add = 0;
		for( size_t index0 = 0 ; index0 < len ; index0 += sizeof( ::rux::uint32 ) )
		{
			add += (::rux::uint64)( *(::rux::uint32*)&buffer_ptr0[ index0 ] );
			add += ( *(::rux::uint32*)&buffer_ptr1[ index0 ] );
			*(::rux::uint32*)&buffer_ptr0[ index0 ] = (::rux::uint32)add;
			add >>= sizeof( ::rux::uint32 ) * 8;
		};
	#else
	#ifdef __x86__			
		len /= sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		len /= sizeof( ::rux::uint64 );
	#endif
		AsmPlus( buffer_ptr0 , buffer_ptr1 , len );
	#endif
	};
	::rux::int32 XRawData::compare( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len )
	{
	#if NO_ASM
		::rux::int32 result = 0;
		::rux::uint32* uint32_buffer_ptr0 = (::rux::uint32*)buffer_ptr0;
		::rux::uint32* uint32_buffer_ptr1 = (::rux::uint32*)buffer_ptr1;
		len /= sizeof( ::rux::uint32 );
		for( size_t index0 = len - 1 ; index0 >= 0 ; index0-- )
		{
			if( uint32_buffer_ptr0[ index0 ] > uint32_buffer_ptr1[ index0 ] )
			{
				result = 1;
				break;
			}
			else if( uint32_buffer_ptr0[ index0 ] < uint32_buffer_ptr1[ index0 ] )
			{
				result = -1;
				break;
			}		
			if( index0 == 0 )
				break;
		};
	#else
	#ifdef __x86__
		::rux::int32 result = 0;	
		len /= sizeof( ::rux::uint32 );
	#elif defined( __x64__ )
		::rux::int64 result = 0;	
		len /= sizeof( ::rux::uint64 );
	#endif
		AsmCompare( buffer_ptr0 , buffer_ptr1 , len , &result );	
	#endif
		return result;
	};
	::rux::String& XRawData::ToString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_size )
	{
		::rux::XString result;
		if( data_ptr )
		{
			for( ::rux::uint32 index0 = data_ptr_size - 1 ; index0 >= 0 ; index0-- )
			{
				result += XString::Format( "%02lx" , (::rux::int32)data_ptr[ index0 ] );		
				if( index0 == 0 )
					break;
			}
		}
		return result++;
	};
	void XRawData::FromReverseHexString( const ::rux::XString& data , ::rux::uint8*& data_ptr , ::rux::uint32& data_ptr_size )
	{
		::rux::uint32 local_data_ptr_size = 0;
		char* ptr = data.str();
		if( ptr[ 0 ] != 0 )
		{		
			size_t length = data.Length();
			if( strncmp( ptr , "0x" , 2 ) == 0 )
			{
				ptr += 2;
				length -= 2;
			}
			local_data_ptr_size = length / 2;		
			if( data_ptr && local_data_ptr_size != data_ptr_size )
			{
				::rux::engine::free_mem( data_ptr );
				data_ptr = NULL;
			}
			if( data_ptr == NULL )
			{
				data_ptr = alloc_array_macros( ::rux::uint8 , local_data_ptr_size );
				data_ptr_size = local_data_ptr_size;
			}
			char temp[ 5 ] = { '0' , 'x' , 0 , 0 , 0 };
			size_t index1 = 0;
			for( ; ; )
			{
				if( *ptr == 0 )
					break;
				memcpy( &temp[ 2 ] , ptr , 2 );
				if( temp[ 3 ] == 0 )
				{
					temp[ 3 ] = '0';
					data_ptr[ index1 ] = (::rux::uint8)strtoul( temp , NULL , 0 );
					break;
				}
				else
				{
					data_ptr[ index1 ] = (::rux::uint8)strtoul( temp , NULL , 0 );
					index1++;
				}
				ptr += 2;
			}		
		}
		else
			data_ptr_size = 0;
	};
	void XRawData::FromString( const ::rux::XString& data , ::rux::uint8*& data_ptr , ::rux::uint32& data_ptr_size )
	{
		::rux::uint32 local_data_ptr_size = 0;
		::rux::XString string = data;
		if( string.Length() > 0 )
		{
			if( string.IndexOf( "0x" ) == 0 )
				string.RemoveRange( 0 , 2 );		
			if( string.Length() % 2 != 0 )
				string.Insert( 0 , '0' );		
			local_data_ptr_size = string.Length() / 2;		
			if( data_ptr && local_data_ptr_size != data_ptr_size )
			{
				::rux::engine::free_mem( data_ptr );
				data_ptr = NULL;
			}
			if( data_ptr == NULL )
			{
				data_ptr = alloc_array_macros( ::rux::uint8 , local_data_ptr_size );
				data_ptr_size = local_data_ptr_size;
			}
			::rux::XString hex;		
			::rux::uint32 index1 = data_ptr_size - 1;
			for( ::rux::uint32 index0 = 0 ; index0 < string.Length() ; index0 += 2 )
			{
				hex.set_ByRef( "0x" + string.Substring( index0 , 2 ) );
				data_ptr[ index1 ] = (::rux::uint8)strtoul( hex.str() , NULL , 0 );
				index1--;
			}		
		}
		else
			data_ptr_size = 0;
	};
	::rux::uint8 RawData::getbit( size_t bit_index )
	{	
		size_t byte_index = bit_index / 8;
		bit_index -= byte_index * 8;
		if( ( ( _raw_data_ptr[ byte_index ] ) & ( 1 << ( bit_index ) ) ) )
			return 1;
		else
			return 0;
	};
	void XRawData::ResetForceSize( void )
	{
		(*this)()->ResetForceSize();
	};
	void RawData::ResetForceSize( void )
	{
		_force_size = SIZE_MAX;
		_bits_count = 0xffffffff;
		_raw_data_ptr_size = 0xffffffff;
	};
	void RawData::modmult( RawData& number , RawData& m , RawData& temp_a , RawData& temp_b )
	{		
		temp_a.Copy( *this );
		if( temp_a >= m )
			temp_a.modular( *this , m );
		if( number >= m )
			temp_b.modular( number , m );
		else
			temp_b.Copy( number );
		*this = 0;	 
		size_t maxbit = temp_b.get_BitsCount();
		size_t i = 0;	 
		for( ; ; )
		{
			if( temp_b.getbit( i ) == 1 )
			{			
				*this += temp_a;
				if( *this >= m )
					*this -= m;
			}
			i++;
			if( i >= maxbit )
				break;		
			temp_a.FastLeftShift();
			if( temp_a >= m )
				temp_a -= m;
		}
	};
	void RawData::modExp( RawData& e , RawData& m )
	{
		CODE_LABELS_INITIALIZE();
		CODE_LABEL( NULL , NULL , 100 );	
		XRawData result , temp_a , temp_b;
		RawData* result_ptr = result();	
		RawData* temp_a_ptr = temp_a();
		RawData* temp_b_ptr = temp_b();
		RawData* m_ptr = &m;
		raw_data_set_unsigned_uint32( result_ptr , 1 );	
		size_t maxbit = e.get_BitsCount() , maxbit0 = 0;
		size_t i = 0 , i0 = 0 , byte_index = 0 , bit_index = 0;
		::rux::uint8 bit = 0;
		bool bool_result = false;
		for( ; ; )
		{
			raw_data_getbit( ( &e ) , i , bit_index , byte_index , bit );
			if( bit == 1 )
			{			
				raw_data_fast_copy( temp_a_ptr , result_ptr );
				raw_data_check_size( temp_a_ptr , m_ptr );
				raw_data_fast_greater_or_equal_if_unsigned( temp_a_ptr , m_ptr , bool_result );
				if( bool_result )
					temp_a_ptr->modular( *result_ptr , m );
				raw_data_check_size( this , m_ptr );
				raw_data_fast_greater_or_equal_if_unsigned( this , m_ptr , bool_result );
				if( bool_result )
					temp_b_ptr->modular( *this , m );
				else
					raw_data_fast_copy( temp_b_ptr , this );
				raw_data_set_unsigned_uint32( result_ptr , 0 );	 
				maxbit0 = temp_b.get_BitsCount();
				i0 = 0;	 
				for( ; ; )
				{
					raw_data_getbit( temp_b_ptr , i0 , bit_index , byte_index , bit );
					if( bit == 1 )
					{			
						raw_data_fast_plus_if_unsigned( result_ptr , temp_a_ptr );
						raw_data_check_size( result_ptr , m_ptr );
						raw_data_fast_greater_or_equal_if_unsigned( result_ptr , m_ptr , bool_result );
						if( bool_result )
							raw_data_fast_minus_if_unsigned( result_ptr , m_ptr );
					}
					i0++;
					if( i0 >= maxbit0 )
						break;		
					raw_data_fast_left_shift( temp_a_ptr );
					raw_data_check_size( temp_a_ptr , m_ptr );
					raw_data_fast_greater_or_equal_if_unsigned( temp_a_ptr , m_ptr , bool_result );
					if( bool_result )
						raw_data_fast_minus_if_unsigned( temp_a_ptr , m_ptr );
				}
			}
			i++;
			if( i > maxbit )
				break;
			raw_data_fast_copy( temp_a_ptr , this );
			raw_data_check_size( temp_a_ptr , m_ptr );		
			raw_data_fast_greater_or_equal_if_unsigned( temp_a_ptr , m_ptr , bool_result );
			if( bool_result )
				temp_a_ptr->modular( *this , m );
			raw_data_check_size( this , m_ptr );
			raw_data_fast_greater_or_equal_if_unsigned( this , m_ptr , bool_result );
			if( bool_result )
				temp_b_ptr->modular( *this , m );
			else
				raw_data_fast_copy( temp_b_ptr , this );
			raw_data_set_unsigned_uint32( this , 0 );
			maxbit0 = temp_b.get_BitsCount();
			i0 = 0;	 
			for( ; ; )
			{
				raw_data_getbit( temp_b_ptr , i0 , bit_index , byte_index , bit );
				if( bit == 1 )
				{			
					raw_data_fast_plus_if_unsigned( this , temp_a_ptr );
					raw_data_check_size( this , m_ptr );
					raw_data_fast_greater_or_equal_if_unsigned( this , m_ptr , bool_result );
					if( bool_result )
						raw_data_fast_minus_if_unsigned( this , m_ptr );
				}
				i0++;
				if( i0 >= maxbit0 )
					break;		
				raw_data_fast_left_shift( temp_a_ptr );
				raw_data_check_size( temp_a_ptr , m_ptr );
				raw_data_fast_greater_or_equal_if_unsigned( temp_a_ptr , m_ptr , bool_result );
				if( bool_result )
					raw_data_fast_minus_if_unsigned( temp_a_ptr , m_ptr );
			}
		}
		raw_data_fast_copy( this , result_ptr );
	};

	void RawData::test()
	{
	};
};
