#include <gtest/gtest.h>

#include <xan_string.h>
#include <xan_io_defines.h>
#include <xan_code_performance_counter.h>
#include <xan_jsonserializer.h>
#include <xan_codegenerator.h>
#include <xan_time.h>
#include <xan_directory.h>
#include <xan_console.h>

#define IAM_EXPORT
#include <xan_core.h>
namespace rux
{
	//__declspec(dllexport) ::rux::int64 core;
};

#ifdef __WINDOWS__
#ifdef __x86__
#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>
#define USE_YAJL 1
static int reformat_null(void * ctx)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_null(g);  
}  
  
static int reformat_boolean(void * ctx, int boolean)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_bool(g, boolean);  
}  
  
static int reformat_number(void * ctx, const char * s, size_t l)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_number(g, s, l);  
}  
  
static int reformat_string(void * ctx, const unsigned char * stringVal,  
                           size_t stringLen)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_string(g, stringVal, stringLen);  
}  
  
static int reformat_map_key(void * ctx, const unsigned char * stringVal,  
                            size_t stringLen)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_string(g, stringVal, stringLen);  
}  
  
static int reformat_start_map(void * ctx)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_map_open(g);  
}  
  
  
static int reformat_end_map(void * ctx)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_map_close(g);  
}  
  
static int reformat_start_array(void * ctx)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_array_open(g);  
}  
  
static int reformat_end_array(void * ctx)  
{  
    yajl_gen g = (yajl_gen) ctx;  
    return yajl_gen_status_ok == yajl_gen_array_close(g);  
}  
  
static yajl_callbacks callbacks = {  
    reformat_null,  
    reformat_boolean,  
    NULL,  
    NULL,  
    reformat_number,  
    reformat_string,  
    reformat_start_map,  
    reformat_map_key,  
    reformat_end_map,  
    reformat_start_array,  
    reformat_end_array  
};  
#endif
#endif
namespace rux
{
	enum RefState
	{
		RefState_Own = 0 ,
		RefState_ByRef = 1
	};
	//const ::rux::byte RefState_Own = 0;
};
struct TestStringStructure
{
	char _test[ 32 ];
	size_t _length;
	void ctor( void );
	void dtor( void );
};
class TestString : public ::rux::RefHandle
{
private:
	::rux::RefState _ref_state;
	union
	{
		TestStringStructure _members;
		TestStringStructure members;
		TestString* _ref_handle;
	};
public:
	TestString( void );
	~TestString( void );
	TestStringStructure& operator->( void );
};
TestString::TestString( void )
{
	_ref_state = ::rux::RefState_Own;
	members.ctor();
};
TestString::~TestString( void )
{
	if( _ref_state == ::rux::RefState_Own )
		members.dtor();
	//memset( _test , 0 , sizeof( _test ) );
	
};



void TestStringStructure::ctor( void )
{
	
	strcpy( _test , "It`s test" );
	_length = 31;

};
void TestStringStructure::dtor( void )
{
	
	

};


TestStringStructure& TestString::operator->( void )
{
	if( _ref_state == ::rux::RefState_Own )
		return members;
	else
		return _ref_handle->members;
};


const char* g_current_module_name = "rux.basis.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_with_result_t _post_deinitialization = NULL;
};
#define test_rpr_cp1251 -16 , -17 , -16
#define test_Apr_cp1251 -64 , -17 , -16
#define test_0Apr_cp1251 48 , -64 , -17 , -16
#define test_rpr_utf8 -47 , -128 , -48 , -65 , -47 , -128
#define test_Apr_utf8 -48 , -112 , -48 , -65 , -47 , -128
#define test_0Apr_utf8 48 , -48 , -112 , -48 , -65 , -47 , -128
#define test_rpr_utf16 64 , 4 , 63 , 4 , 64 , 4
#define test_Apr_utf16 16 , 4 , 63 , 4 , 64 , 4
#define test_0Apr_utf16 48 , 0 , 16 , 4 , 63 , 4 , 64 , 4
#define test_il_cp1251 -27 , -21 , -4
#define test_0il_cp1251 48 , -27 , -21 , -4
#define test_il0_cp1251 -27 , -21 , -4 , 48
#define test_il_utf8 -48 , -75 , -48 , -69 , -47 , -116
#define test_0il_utf8 48 , -48 , -75 , -48 , -69 , -47 , -116
#define test_il0_utf8 -48 , -75 , -48 , -69 , -47 , -116 , 48
#define test_il_utf16 53 , 4 , 59 , 4 , 76 , 4
#define test_0il_utf16 48 , 0 , 53 , 4 , 59 , 4 , 76 , 4
#define test_il0_utf16 53 , 4 , 59 , 4 , 76 , 4 , 48 , 0
char cp1251_test0[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test0[] = 
{
	test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test0[] = 
{
	test_Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test1[] = 
{
	test_Apr_cp1251 , 0 
};
char utf8_test1[] = 
{
	test_Apr_utf8 , 0
};
char utf16_test1[] = 
{
	test_Apr_utf16 , 0 , 0
};

char cp1251_test2[] = 
{
	test_il_cp1251 , 0 
};
char utf8_test2[] = 
{
	test_il_utf8 , 0
};
char utf16_test2[] = 
{
	test_il_utf16 , 0 , 0
};

char cp1251_test3[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , 0 
};
char utf8_test3[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , 0
};
char utf16_test3[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test4[] = 
{
	test_il_cp1251 , test_il_cp1251 , 0 
};
char utf8_test4[] = 
{
	test_il_utf8 , test_il_utf8 , 0
};
char utf16_test4[] = 
{
	test_il_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test5[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_il_cp1251 , 0 
};
char utf8_test5[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_il_utf8 , 0
};
char utf16_test5[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test6[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test6[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test6[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test7[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test7[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0 
};
char utf16_test7[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 
};

char cp1251_test8[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test8[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test8[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test9[] = 
{
	test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test9[] = 
{
	test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test9[] = 
{
	test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test10[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test10[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test10[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test11[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test11[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test11[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test12[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test12[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test12[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test13[] = 
{
	test_rpr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , 0 
};
char utf8_test13[] = 
{
	test_rpr_utf8 , test_il_utf8 , test_rpr_utf8 , test_il_utf8 , test_rpr_utf8 , 0
};
char utf16_test13[] = 
{
	test_rpr_utf16 , test_il_utf16 , test_rpr_utf16 , test_il_utf16 , test_rpr_utf16 , 0 , 0
};

char cp1251_test14[] = 
{
	test_rpr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test14[] = 
{
	test_rpr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test14[] = 
{
	test_rpr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test15[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test15[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_rpr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test15[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_rpr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test16[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , 0 
};
char utf8_test16[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_rpr_utf8 , test_il_utf8 , test_rpr_utf8 , 0
};
char utf16_test16[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_rpr_utf16 , test_il_utf16 , test_rpr_utf16 , 0 , 0
};

char cp1251_test17[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_rpr_cp1251 , 0 
};
char utf8_test17[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_rpr_utf8 , 0
};
char utf16_test17[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_rpr_utf16 , 0 , 0
};

char cp1251_test18[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test18[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test18[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test19[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test19[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test19[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test20[] = 
{
	test_rpr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test20[] = 
{
	test_rpr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test20[] = 
{
	test_rpr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test21[] = 
{
	test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test21[] = 
{
	test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test21[] = 
{
	test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test22[] = 
{
	test_Apr_cp1251 , test_il_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , 0 
};
char utf8_test22[] = 
{
	test_Apr_utf8 , test_il_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , 0
};
char utf16_test22[] = 
{
	test_Apr_utf16 , test_il_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , 0 , 0
};

char cp1251_test23[] = 
{
	test_0Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test23[] = 
{
	test_0Apr_utf8 , test_il_utf8 , 0
};
char utf16_test23[] = 
{
	test_0Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test24[] = 
{
	test_Apr_cp1251 , test_0il_cp1251 , 0 
};
char utf8_test24[] = 
{
	test_Apr_utf8 , test_0il_utf8 , 0
};
char utf16_test24[] = 
{
	test_Apr_utf16 , test_0il_utf16 , 0 , 0
};

char cp1251_test25[] = 
{
	test_Apr_cp1251 , test_il0_cp1251 , 0 
};
char utf8_test25[] = 
{
	test_Apr_utf8 , test_il0_utf8 , 0
};
char utf16_test25[] = 
{
	test_Apr_utf16 , test_il0_utf16 , 0 , 0
};

char cp1251_test26[] = 
{
	test_0Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test26[] = 
{
	test_0Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test26[] = 
{
	test_0Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 , 0
};

char cp1251_test27[] = 
{
	test_0Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , test_Apr_cp1251 , test_il_cp1251 , 0 
};
char utf8_test27[] = 
{
	test_0Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , test_Apr_utf8 , test_il_utf8 , 0
};
char utf16_test27[] = 
{
	test_0Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , test_Apr_utf16 , test_il_utf16 , 0 , 0
};

class StringTest : public ::testing::Test 
{
};
TEST_F( StringTest , Convert ) 
{
	{
		{
			::rux::XString res_str;
			{
				::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
			
				res_str.set_ByRef( str0.ConvertToAnsi() );
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}				
			
				res_str.set_ByRef( str0.ConvertToUTF8() );

				ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );

				res_str.set_ByRef( str0.ConvertToUTF16() );

				ASSERT_TRUE( memcmp( res_str.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );
			}

			{
				::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
			
				res_str.set_ByRef( str0.ConvertToAnsi() );
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}				
			
				res_str.set_ByRef( str0.ConvertToUTF8() );

				ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );

				res_str.set_ByRef( str0.ConvertToUTF16() );

				ASSERT_TRUE( memcmp( res_str.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );
			}

			{
				::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
			
				res_str.set_ByRef( str0.ConvertToAnsi() );
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

					ASSERT_TRUE( res_str.Length() == 6 );
				}				
			
				res_str.set_ByRef( str0.ConvertToUTF8() );

				ASSERT_TRUE( memcmp( res_str.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );

				res_str.set_ByRef( str0.ConvertToUTF16() );

				ASSERT_TRUE( memcmp( res_str.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );

				ASSERT_TRUE( res_str.Length() == 6 );
			}
		}
	}
};

TEST_F( StringTest , Substring ) 
{
	{
		{
			::rux::XString res_str;
			{
				::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
			
				res_str.set_ByRef( str0.Substring( 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , cp1251_test2 , sizeof( cp1251_test2 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );

				res_str.set_ByRef( str0.Substring( 0 , 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );			
			}

			{
				::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
			
				res_str.set_ByRef( str0.Substring( 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , utf8_test2 , sizeof( utf8_test2 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );

				res_str.set_ByRef( str0.Substring( 0 , 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );
			}

			{
				::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
			
				res_str.set_ByRef( str0.Substring( 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , utf16_test2 , sizeof( utf16_test2 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );

				res_str.set_ByRef( str0.Substring( 0 , 3 ) );
				
				ASSERT_TRUE( memcmp( res_str.str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
				
				ASSERT_TRUE( res_str.Length() == 3 );
			}
		}
	}
};

TEST_F( StringTest , ReplaceAll ) 
{
	{
		{
			::rux::XString res_str;
			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );
				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test13 , sizeof( cp1251_test13 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test4 , sizeof( cp1251_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test7 , sizeof( cp1251_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test4 , sizeof( utf8_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test4 , sizeof( utf16_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test13 , sizeof( utf16_test13 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test4 , sizeof( utf16_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test4 , sizeof( utf8_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test4 , sizeof( utf16_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test7 , sizeof( utf16_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test13 , sizeof( utf8_test13 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test4 , sizeof( utf8_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test4 , sizeof( utf8_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test4 , sizeof( utf8_test4 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 6 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test7 , sizeof( utf8_test7 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 24 );
					}
				}
			}	



			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test16 , sizeof( cp1251_test16 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test3 , sizeof( cp1251_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test8 , sizeof( cp1251_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test16 , sizeof( utf16_test16 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test8 , sizeof( utf16_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceAll( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test16 , sizeof( utf8_test16 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceAll( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 9 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceAll( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test8 , sizeof( utf8_test8 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 21 );
					}
				}
			}	
		}
	}
};

TEST_F( StringTest , ReplaceFirst ) 
{
	{
		{
			::rux::XString res_str;
			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test14 , sizeof( cp1251_test14 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test9 , sizeof( cp1251_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test10 , sizeof( cp1251_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test9 , sizeof( utf8_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test9 , sizeof( utf16_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test14 , sizeof( utf16_test14 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test9 , sizeof( utf16_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test9 , sizeof( utf8_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test9 , sizeof( utf16_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test10 , sizeof( utf16_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test14 , sizeof( utf8_test14 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test9 , sizeof( utf8_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test9 , sizeof( utf8_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test9 , sizeof( utf8_test9 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test10 , sizeof( utf8_test10 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}	



			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test15 , sizeof( cp1251_test15 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test11 , sizeof( cp1251_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test12 , sizeof( cp1251_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test11 , sizeof( utf8_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test11 , sizeof( utf16_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test15 , sizeof( utf16_test15 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test11 , sizeof( utf16_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test11 , sizeof( utf8_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test11 , sizeof( utf16_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test12 , sizeof( utf16_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceFirst( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test15 , sizeof( utf8_test15 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test11 , sizeof( utf8_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test11 , sizeof( utf8_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceFirst( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test11 , sizeof( utf8_test11 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceFirst( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test12 , sizeof( utf8_test12 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}	
		}
	}
};

TEST_F( StringTest , ReplaceLast ) 
{
	{
		{
			::rux::XString res_str;
			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test17 , sizeof( cp1251_test17 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test18 , sizeof( cp1251_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test19 , sizeof( cp1251_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test18 , sizeof( utf8_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test18 , sizeof( utf16_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test17 , sizeof( utf16_test17 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test18 , sizeof( utf16_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test18 , sizeof( utf8_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test18 , sizeof( utf16_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test19 , sizeof( utf16_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test17 , sizeof( utf8_test17 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test18 , sizeof( utf8_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test18 , sizeof( utf8_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test18 , sizeof( utf8_test18 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test19 , sizeof( utf8_test19 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}	



			{
				::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , cp1251_test20 , sizeof( cp1251_test20 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test21 , sizeof( cp1251_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , cp1251_test22 , sizeof( cp1251_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test21 , sizeof( utf8_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test21 , sizeof( utf16_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf16_test20 , sizeof( utf16_test20 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test21 , sizeof( utf16_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test21 , sizeof( utf8_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test21 , sizeof( utf16_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf16_test22 , sizeof( utf16_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}

			{
				::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

				{
#ifdef __WINDOWS__
					res_str.set_ByRef( str0.ReplaceLast( (char)-64 , (char)-16 , 4 ) );
				
					ASSERT_TRUE( memcmp( res_str.str() , utf8_test20 , sizeof( utf8_test20 ) ) == 0 );
				
					ASSERT_TRUE( res_str.Length() == 15 );
#endif
				}

				{
					::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test21 , sizeof( utf8_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test21 , sizeof( utf8_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}

				{
					::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );
					
					{
						res_str.set_ByRef( str0.ReplaceLast( str1 , "" , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test21 , sizeof( utf8_test21 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 12 );
					}

					{
						::rux::XString str2( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf8_test0 , 0 , XEnumCodePage_UTF8 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}

					{
						::rux::XString str2( utf16_test0 , 0 , XEnumCodePage_UTF16 );

						res_str.set_ByRef( str0.ReplaceLast( str1 , str2 , 4 ) );
				
						ASSERT_TRUE( memcmp( res_str.str() , utf8_test22 , sizeof( utf8_test22 ) ) == 0 );
				
						ASSERT_TRUE( res_str.Length() == 18 );
					}
				}
			}	
		}
	}
};

TEST_F( StringTest , IndexOf ) 
{
	{
		{
			::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );

			{
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
					ASSERT_TRUE( str0.IndexOf( cp1251_test2 ) == 3 );
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
					ASSERT_TRUE( str0.IndexOf( utf8_test2 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}
		}

		{
			::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );

			{
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
					ASSERT_TRUE( str0.IndexOf( cp1251_test2 ) == 3 );
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
					ASSERT_TRUE( str0.IndexOf( utf8_test2 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}
		}

		{
			::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );

			{
				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
					ASSERT_TRUE( str0.IndexOf( cp1251_test2 ) == 3 );
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
					ASSERT_TRUE( str0.IndexOf( utf8_test2 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 ) == 3 );
			}

			{
				::rux::XString str1( cp1251_test1 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf16_test1 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}

			{
				::rux::XString str1( utf8_test1 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.IndexOf( str1 , 2 ) == 6 );
			}
		}
	}
};

TEST_F( StringTest , LastIndexOf ) 
{
	{
		{
			::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}
		}

		{
			::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}
		}

		{
			::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 ) == 9 );
			}

			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0.LastIndexOf( str1 , 7 ) == 3 );
			}
		}
	}
};

TEST_F( StringTest , Equals ) 
{
	{
		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 == str1 );
			}
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 == str1 );
			}
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 == str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 == str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 == str1 );
			}
		}
	}

	{
		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 != str1 );
			}
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 != str1 );
			}
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				ASSERT_TRUE( str0 != str1 );
			}

			{
				::rux::XString str1( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf16_test0 , 0 , XEnumCodePage_UTF16 );

				ASSERT_FALSE( str0 != str1 );
			}

			{
				::rux::XString str1( utf8_test0 , 0 , XEnumCodePage_UTF8 );

				ASSERT_FALSE( str0 != str1 );
			}
		}
	}
}


TEST_F( StringTest , InsertRange ) 
{
	{
		{
			::rux::XString str0;
					
			::rux::XString str1;
				
			str0.InsertRange( 0 , str1 );

			ASSERT_TRUE( str0.Length() == 0 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test2 , sizeof( cp1251_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test2 , sizeof( utf8_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test2 , sizeof( utf16_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( cp1251_test27 , 0 , XEnumCodePage_CP1251 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test27 , sizeof( cp1251_test27 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 43 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( utf8_test27 , 0 , XEnumCodePage_UTF8 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test27 , sizeof( utf8_test27 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 43 );
		}

		{
			::rux::XString str0;
					
			::rux::XString str1( utf16_test27 , 0 , XEnumCodePage_UTF16 );
				
			str0.InsertRange( SIZE_MAX , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test27 , sizeof( utf16_test27 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 43 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
					
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test3 , sizeof( cp1251_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
					
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
					
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );
				
			str0.InsertRange( 3 , str1 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}
				
		{
			::rux::XString str0;

			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			{
				str0.InsertRange( 3 , cp1251_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , cp1251_test2 , sizeof( cp1251_test2 ) ) == 0 );
			}
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			{
				str0.InsertRange( 3 , utf8_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test2 , sizeof( utf8_test2 ) ) == 0 );
			}
			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			{
				str0.InsertRange( 3 , cp1251_test4 , SIZE_MAX );

				ASSERT_TRUE( memcmp( str0.str() , cp1251_test5 , sizeof( cp1251_test5 ) ) == 0 );
			}
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			{
				str0.InsertRange( 3 , utf8_test4 , SIZE_MAX );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test5 , sizeof( utf8_test5 ) ) == 0 );
			}

			ASSERT_TRUE( str0.Length() == 12 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
				str0.InsertRange( 3 , cp1251_test4 , SIZE_MAX );
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				str0.InsertRange( 3 , utf8_test4 , SIZE_MAX );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test5 , sizeof( utf8_test5 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 12 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			{
				str0.InsertRange( 3 , cp1251_test4 , SIZE_MAX );

				ASSERT_TRUE( memcmp( str0.str() , utf16_test5 , sizeof( utf16_test5 ) ) == 0 );
			}
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			{
				str0.InsertRange( 3 , utf8_test4 , SIZE_MAX );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test5 , sizeof( utf8_test5 ) ) == 0 );
			}

			ASSERT_TRUE( str0.Length() == 12 );
		}


		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			{
				str0.InsertRange( 3 , cp1251_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , cp1251_test3 , sizeof( cp1251_test3 ) ) == 0 );
			}
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			{
				str0.InsertRange( 3 , utf8_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
			}

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
				str0.InsertRange( 3 , cp1251_test4 , 3 );
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				str0.InsertRange( 3 , utf8_test4 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 9 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			{
				str0.InsertRange( 3 , cp1251_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );
			}
			else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			{
				str0.InsertRange( 3 , utf8_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );
			}

			ASSERT_TRUE( str0.Length() == 9 );
		}
		if( sizeof( wchar_t ) == 2 )
		{
			{
				::rux::XString str0;
				
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , 3 );

				if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
					ASSERT_TRUE( memcmp( str0.str() , utf16_test2 , sizeof( utf16_test2 ) ) == 0 );
				else if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
					ASSERT_TRUE( memcmp( str0.str() , utf8_test2 , sizeof( utf8_test2 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 3 );
			}

			{
				::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
				
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 9 );
			}

			{
				::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test3 , sizeof( utf8_test3 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 9 );
			}

			{
				::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , 3 );

				ASSERT_TRUE( memcmp( str0.str() , utf16_test3 , sizeof( utf16_test3 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 9 );
			}

			{
				::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , SIZE_MAX );
				
				ASSERT_TRUE( memcmp( str0.str() , utf16_test5 , sizeof( utf16_test5 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 12 );
			}

			{
				::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
					
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , SIZE_MAX );

				ASSERT_TRUE( memcmp( str0.str() , utf8_test5 , sizeof( utf8_test5 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 12 );
			}

			{
				::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
					
				str0.InsertRange( 3 , (wchar_t*)utf16_test4 , SIZE_MAX );
				
				ASSERT_TRUE( memcmp( str0.str() , utf16_test5 , sizeof( utf16_test5 ) ) == 0 );

				ASSERT_TRUE( str0.Length() == 12 );
			}
		}
	}
};

TEST_F( StringTest , RemoveRange ) 
{
	{
		{
			::rux::XString str0;
				
			str0.RemoveRange( 0 , 3 );

			ASSERT_TRUE( str0.Length() == 0 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
				
			str0.RemoveRange( 3 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
				
			str0.RemoveRange( 3 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
				
			str0.RemoveRange( 3 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
				
			str0.RemoveRange( 3 , SIZE_MAX );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
				
			str0.RemoveRange( 3 , SIZE_MAX );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
				
			str0.RemoveRange( 3 , SIZE_MAX );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );
				
			str0.RemoveRange( 0 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , cp1251_test2 , sizeof( cp1251_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );
				
			str0.RemoveRange( 0 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , utf8_test2 , sizeof( utf8_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}

		{
			::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );
				
			str0.RemoveRange( 0 , 3 );

			ASSERT_TRUE( memcmp( str0.str() , utf16_test2 , sizeof( utf16_test2 ) ) == 0 );

			ASSERT_TRUE( str0.Length() == 3 );
		}
	}
};

TEST_F( StringTest , Plus ) 
{
	::rux::XString res_string;
	{
		::rux::XString str0( cp1251_test1 , 0 , XEnumCodePage_CP1251 );

		{
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				res_string.set_ByRef( str0 + utf8_test2 );
			else
				res_string.set_ByRef( str0 + cp1251_test2 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
			else
				ASSERT_TRUE( memcmp( res_string.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );
		}
	}

	{
		::rux::XString str0( utf16_test1 , 0 , XEnumCodePage_UTF16 );

		{
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				res_string.set_ByRef( str0 + utf8_test2 );
			else
				res_string.set_ByRef( str0 + cp1251_test2 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
			else
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );
		}
	}

	{
		::rux::XString str0( utf8_test1 , 0 , XEnumCodePage_UTF8 );

		{
			if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
				res_string.set_ByRef( str0 + utf8_test2 );
			else
				res_string.set_ByRef( str0 + cp1251_test2 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}

		{
			::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

			res_string.set_ByRef( str0 + str1 );
			
			ASSERT_TRUE( res_string.Length() == 6 );
			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		}
	}

	{
		::rux::XString str1( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			res_string.set_ByRef( utf8_test1 + str1 );
		else
			res_string.set_ByRef( cp1251_test1 + str1 );
			
		ASSERT_TRUE( res_string.Length() == 6 );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( res_string.str() , cp1251_test0 , sizeof( cp1251_test0 ) ) == 0 );
	}

	{
		::rux::XString str1( utf16_test2 , 0 , XEnumCodePage_UTF16 );

		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			res_string.set_ByRef( utf8_test1 + str1 );
		else
			res_string.set_ByRef( cp1251_test1 + str1 );
			
		ASSERT_TRUE( res_string.Length() == 6 );
			
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )			
			ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( res_string.str() , utf16_test0 , sizeof( utf16_test0 ) ) == 0 );
	}

	{
		::rux::XString str1( utf8_test2 , 0 , XEnumCodePage_UTF8 );

		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			res_string.set_ByRef( utf8_test1 + str1 );
		else
			res_string.set_ByRef( cp1251_test1 + str1 );
			
		ASSERT_TRUE( res_string.Length() == 6 );
			
		ASSERT_TRUE( memcmp( res_string.str() , utf8_test0 , sizeof( utf8_test0 ) ) == 0 );
	}
};

TEST_F( StringTest , Split ) 
{
	{
		::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );
		{
			::rux::XString str1( cp1251_test23 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , cp1251_test1 , sizeof( cp1251_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}
		}

		{
			::rux::XString str1( utf8_test23 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}
		}

		{
			::rux::XString str1( utf16_test23 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf8_test1 , sizeof( utf8_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				splits.set_ByRef( str0.Split( splits ) );

				ASSERT_TRUE( splits.Count() == 3 );

				ASSERT_TRUE( memcmp( splits[ 0 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 0 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 1 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 1 ].Length() == 3 );

				ASSERT_TRUE( memcmp( splits[ 2 ].str() , utf16_test1 , sizeof( utf16_test1 ) ) == 0 );
		
				ASSERT_TRUE( splits[ 2 ].Length() == 3 );
			}
		}
	}
};

TEST_F( StringTest , Join ) 
{
	::rux::XString res_string;
	{
		::rux::XString str0( cp1251_test6 , 0 , XEnumCodePage_CP1251 );
		{
			::rux::XString str1( cp1251_test23 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , cp1251_test26 , sizeof( cp1251_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf16_test23 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf8_test23 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}		
	}

	{
		::rux::XString str0( utf16_test6 , 0 , XEnumCodePage_UTF16 );
		{
			::rux::XString str1( cp1251_test23 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf16_test23 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );
				
				ASSERT_TRUE( memcmp( res_string.str() , utf16_test26 , sizeof( utf16_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf8_test23 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}		
	}

	{
		::rux::XString str0( utf8_test6 , 0 , XEnumCodePage_UTF8 );
		{
			::rux::XString str1( cp1251_test23 , 0 , XEnumCodePage_CP1251 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf16_test23 , 0 , XEnumCodePage_UTF16 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}

		{
			::rux::XString str1( utf8_test23 , 0 , XEnumCodePage_UTF8 );
			{
				::rux::XString str2( cp1251_test2 , 0 , XEnumCodePage_CP1251 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}

			{
				::rux::XString str2( utf16_test2 , 0 , XEnumCodePage_UTF16 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	

			{
				::rux::XString str2( utf8_test2 , 0 , XEnumCodePage_UTF8 );

				::rux::XArray< ::rux::XString > splits;
				splits.AddByRef( str1 );
				splits.AddByRef( str2 );		

				res_string.set_ByRef( ::rux::XString::Join( str0 , splits ) );

				ASSERT_TRUE( memcmp( res_string.str() , utf8_test26 , sizeof( utf8_test26 ) ) == 0 );
		
				ASSERT_TRUE( res_string.Length() == 25 );
			}	
		}		
	}
};


TEST_F( StringTest , Insert ) 
{
	{
		::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

		str0.Insert( 0 , '0' );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test23 , sizeof( utf8_test23 ) ) == 0 );
		else if( ::rux::XString::get_global_code_page() == XEnumCodePage_CP1251 )
			ASSERT_TRUE( memcmp( str0.str() , cp1251_test23 , sizeof( cp1251_test23 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );

		str0.Insert( 0 , '0' );

		ASSERT_TRUE( memcmp( str0.str() , utf8_test23 , sizeof( utf8_test23 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );

		str0.Insert( 0 , '0' );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test23 , sizeof( utf8_test23 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( str0.str() , utf16_test23 , sizeof( utf16_test23 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

		str0.Insert( 3 , '0' );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test24 , sizeof( utf8_test24 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( str0.str() , cp1251_test24 , sizeof( cp1251_test24 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );

		str0.Insert( 3 , '0' );

		ASSERT_TRUE( memcmp( str0.str() , utf8_test24 , sizeof( utf8_test24 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );

		str0.Insert( 3 , '0' );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test24 , sizeof( utf8_test24 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( str0.str() , utf16_test24 , sizeof( utf16_test24 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( cp1251_test0 , 0 , XEnumCodePage_CP1251 );

		str0.Insert( SIZE_MAX , '0' );

		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test25 , sizeof( utf8_test25 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( str0.str() , cp1251_test25 , sizeof( cp1251_test25 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf8_test0 , 0 , XEnumCodePage_UTF8 );

		str0.Insert( SIZE_MAX , '0' );

		ASSERT_TRUE( memcmp( str0.str() , utf8_test25 , sizeof( utf8_test25 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}

	{
		::rux::XString str0( utf16_test0 , 0 , XEnumCodePage_UTF16 );

		str0.Insert( SIZE_MAX , '0' );
		if( ::rux::XString::get_global_code_page() == XEnumCodePage_UTF8 )
			ASSERT_TRUE( memcmp( str0.str() , utf8_test25 , sizeof( utf8_test25 ) ) == 0 );
		else
			ASSERT_TRUE( memcmp( str0.str() , utf16_test25 , sizeof( utf16_test25 ) ) == 0 );
		
		ASSERT_TRUE( str0.Length() == 7 );
	}
};

class JsonTest : public ::testing::Test 
{
};

TEST_F( JsonTest , DesrializeSpeed ) 
{
	::rux::XConsole::Print( stdout , XColors::XWhite , "%s" , "Enter to start json test:" );
	//::rux::XConsole::WaitForEnter();

	declare_variable( ::rux::XString , error );
	declare_variable( ::rux::XString , js );
	::rux::XString dir( ::rux::engine::_globals->_executable_directory );
	dir += "/json_test_files";
	::rux::XArray< ::rux::io::XFile > files;
	::rux::io::XDirectory::GetFiles( dir , XAllDirectories , files );
	for( size_t index0 = 0 ; index0 < files.Count() ; index0++ )
	{
		js.set_ByRef( files[ index0 ].ReadTextAll( error ) );
		if( error.Length() == 0 )
		{
			if( js.Length() > 0 )
			{
				::rux::uint32 hash = 0;
				::rux::XConsole::Print( stdout , XColors::XWhite , files[ index0 ].get_FileName() + "\n" );

				::rux::XConsole::Print( stdout , XColors::XWhite , "%s" , "======================\n" );
				for( size_t index2 = 0 ; index2 < 10 ; index2++ )
				{
					size_t number_of_tests = 1000;
					::rux::XConsole::Print( stdout , XColors::XWhite , "%s" , "-------------------\n" );
					::rux::uint64 test_begin = ::rux::XTime::get_NowAsUnixTime();
					for( size_t index1 = 0 ; index1 < number_of_tests ; index1++ )
					{
#if USE_YAJL
						yajl_handle hand;
						yajl_gen g;
						yajl_status stat;
						size_t rd;
						int retval = 0;
						int a = 1;  
  
						g = yajl_gen_alloc( NULL );  
						yajl_gen_config( g , yajl_gen_beautify , 1 );  
						yajl_gen_config( g , yajl_gen_validate_utf8 , 1 );  
  
						/* ok.  open file.  let's read and parse */  
						hand = yajl_alloc(&callbacks, NULL, (void *) g);  
						/* and let's allow comments by default */  
						yajl_config( hand , yajl_allow_comments , 1 );  
						stat = yajl_parse( hand , (::rux::byte*)js.str() , js.Size() - 1 );  
  
						stat = yajl_complete_parse(hand);
  
						yajl_gen_free(g);  
						yajl_free(hand);  
#else
						//XObject obj( ::rux::data::XJsonSerializer::Deserialize( js , error , __FILE__ , __LINE__ , 0 ) );

						//::rux::data::XJsonSerializer::IsValidJSON( js.str() , 1 , error , __FILE__ , __LINE__ , 0/*&hash*/ );

						//size_t length = strlen( js.str() );
						/*char* ptr = js.str();
						while( *ptr++ != 0 )
							length++;*/
						
#endif
					}
					::rux::uint64 test_end = ::rux::XTime::get_NowAsUnixTime();
					::rux::uint64 delta = test_end - test_begin;
					::rux::XConsole::Print( stdout , XColors::XWhite , ::rux::XUInt64( delta ).ToString() + " microseconds, avg " + ::rux::XUInt64( delta / number_of_tests ).ToString() + " microseconds\n" );
					::rux::XConsole::Print( stdout , XColors::XWhite , "%s" , "-------------------\n" );
				}
				::rux::XConsole::Print( stdout , XColors::XWhite , "%s" , "======================\n" );
			}
		}
	}
};

TEST_F( JsonTest , Desrialize ) 
{


	TestString string0;
	TestString string1;



	declare_stack_variable( char , filename , 1024 );
	::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
	rux_concatenation( filename , "/../../rux.framework/test/test.ruxprj" );

	::rux::XString error;
	/*rux::generator::XRUXProject::load( filename , error );*/

	::rux::io::file file( filename );
	if( file.opened() )
	{
		size_t size = file.size() + 1 , readen = 0;
		char* json = alloc_array_macros( char , size );
		if( file.read_text( json , size , readen ) )
		{
			char* ptr = json;
			if( memcmp( json , _rux_utf8_header , 3 ) == 0 )
				ptr = &json[ 3 ];
			
			{
				LAZY_PERFORMANCE;
				XObject obj( ::rux::data::XJsonSerializer::Deserialize( ptr , 1 , error , __FILE__ , __LINE__ ) );

				ASSERT_TRUE( error.Length() == 0 );

				ASSERT_TRUE( rux_is_object( obj , ::rux::data::XDataObject ) );
			}
		}
		::rux::engine::free_mem( json );
	}

	
	{
		XObject obj;
		obj.set_ByRef( ::rux::data::XJsonSerializer::Deserialize( "[{\"name\":\"Joe\"},{\"name\":\"Matthew\"},{\"name\":\"Bill\"}]" , 1 , error , __FILE__ , __LINE__ ) );

		ASSERT_TRUE( error.Length() == 0 );

		ASSERT_TRUE( rux_is_object( obj , ::rux::XArray< ::rux::data::XDataObject > ) );
	}

	{
		::rux::XTime time( 1970 , 1 , 1 , 0 , 0 , 0 , 0 );
	}
};

TEST_F( JsonTest , Merge ) 
{
	declare_stack_variable( char , filename , 1024 );
	::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
	::rux::safe_strncat( filename , "/../../addons/tests/json/config.txt" , 1024 );

	declare_variable( ::rux::data::XDataObject , config );

	::rux::XString error;
	::rux::io::file file( filename );
	if( file.opened() )
	{
		size_t size = file.size() + 1 , readen = 0;
		char* json = alloc_array_macros( char , size );
		if( file.read_text( json , size , readen ) )
		{
			char* ptr = json;
			if( memcmp( json , _rux_utf8_header , 3 ) == 0 )
				ptr = &json[ 3 ];
			
			{
				LAZY_PERFORMANCE;
				XObject object( ::rux::data::XJsonSerializer::Deserialize( ptr , 1 , error , __FILE__ , __LINE__ ) );

				ASSERT_TRUE( error.Length() == 0 );

				ASSERT_TRUE( rux_is_object( object , ::rux::data::XDataObject ) );

				config.set_ByRef( object );
			}
		}
		::rux::engine::free_mem( json );
	}
	::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
	::rux::safe_strncat( filename , "/../../addons/tests/json/obj.txt" , 1024 );

	declare_variable( ::rux::data::XDataObject , obj );

	file.open( filename );
	if( file.opened() )
	{
		size_t size = file.size() + 1 , readen = 0;
		char* json = alloc_array_macros( char , size );
		if( file.read_text( json , size , readen ) )
		{
			char* ptr = json;
			if( memcmp( json , _rux_utf8_header , 3 ) == 0 )
				ptr = &json[ 3 ];
			
			{
				LAZY_PERFORMANCE;
				XObject object( ::rux::data::XJsonSerializer::Deserialize( ptr , 1 , error , __FILE__ , __LINE__ ) );

				ASSERT_TRUE( error.Length() == 0 );

				ASSERT_TRUE( rux_is_object( object , ::rux::data::XDataObject ) );

				obj.set_ByRef( object );
			}
		}
		::rux::engine::free_mem( json );
	}

	::rux::byte changed = 0;
	config.Merge( obj , changed , 1 );
};
int main( int argc , char **argv )
{
	//::rux::core = 1900;

	rux::engine::initialize();

	//printf( I64d , ::rux::core );

    ::testing::InitGoogleTest( &argc , argv );

    return RUN_ALL_TESTS();
};