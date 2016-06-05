#include "gtest/gtest.h"
#include <xan_codegenerator.h>
const char* g_current_module_name = "rux.generator.test";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
class CodeGeneratorTest : public ::testing::Test 
{
};
TEST_F( CodeGeneratorTest , generate ) 
{
	rux::engine::initialize();
	{
		char filename[ 1024 ] = {0} , modulename[ 1024 ] = {0};
		rux_get_executable_directory( filename );
		rux_concatenation( filename , "/../../rux.basis/basis.ruxprj" );
		rux::string::replace_all( filename , '\\' , '/' );
		rux::XString error;
		rux::generator::XRUXProject project( rux::generator::XRUXProject::load( filename , error ) );
	}
	rux::engine::deinitialize();
};
int main( int argc , char **argv )
{
    ::testing::InitGoogleTest( &argc , argv );
    return RUN_ALL_TESTS();
}
