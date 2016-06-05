#include <gtest/gtest.h>

#include <xan_javascript.h>

#include "v8/include/v8.h"
#include "v8/include/libplatform/libplatform.h"

const char* g_current_module_name = "rux.elk.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_with_result_t _post_deinitialization = NULL;
};
class JavaScriptTest : public ::testing::Test 
{
};
TEST_F( JavaScriptTest , First ) 
{
	// Initialize V8.
	::v8::V8::InitializeICU();
  	::v8::Platform* platform = ::v8::platform::CreateDefaultPlatform();
	::v8::V8::InitializePlatform(platform);
	::v8::V8::Initialize();
  // Create a new Isolate and make it the current one.
	::v8::Isolate* isolate = ::v8::Isolate::New();
  	{
    		::v8::Isolate::Scope isolate_scope( isolate );
// Create a stack-allocated handle scope.
		::v8::HandleScope handle_scope(isolate);
// Create a new context.
		::v8::Local<::v8::Context> context = ::v8::Context::New(isolate);
// Enter the context for compiling and running the hello world script.
		::v8::Context::Scope context_scope(context);
// Create a string containing the JavaScript source code.
		::v8::Local<::v8::String> source = ::v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'");
// Compile the source code.
		::v8::Local<::v8::Script> script = ::v8::Script::Compile(source);
// Run the script to get the result.
		::v8::Local<::v8::Value> result = script->Run();
// Convert the result to an UTF8 string and print it.
		::v8::String::Utf8Value utf8(result);
		printf("%s\n", *utf8);
	}  
// Dispose the isolate and tear down V8.
	isolate->Dispose();
	::v8::V8::Dispose();
	::v8::V8::ShutdownPlatform();
	delete platform;
};
int main( int argc , char **argv )
{
	::rux::engine::initialize();
	::testing::InitGoogleTest( &argc , argv );
	return RUN_ALL_TESTS();
};