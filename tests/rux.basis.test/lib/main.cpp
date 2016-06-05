#include <string.h>
#include <jni.h>

#include <android_native_app_glue.h>

#include <android/log.h>
#include <xan_string.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "test_native", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "test_native", __VA_ARGS__))
const char* g_current_module_name = "rux.basis.test";
/**
 * Process the next input event.
 */
static int32_t engine_handle_input( struct android_app* app , AInputEvent* event ) 
{
	rux::int32 input_event_type = AInputEvent_getType( event );
	if( input_event_type == AINPUT_EVENT_TYPE_MOTION ) 
	{
		void* glesLib = (void*)dlopen( "libGLESv1_CM.so" , RTLD_LAZY );
		if( glesLib )
			dlclose( glesLib );
		glesLib = (void*)dlopen( "libGLESv2.so" , RTLD_LAZY );
		if( glesLib )
			dlclose( glesLib );
	}
	else if( input_event_type == AINPUT_EVENT_TYPE_KEY )
	{
		

		LOGI("key code = %d"  , AKeyEvent_getKeyCode(event));
		LOGI("scan code = %d"  , AKeyEvent_getScanCode(event));
		LOGI("key event action = %d"  , AKeyEvent_getAction(event));
		LOGI("key flag = %d"  , AKeyEvent_getFlags(event));
		LOGI("key meta state = %d"  , AKeyEvent_getMetaState(event));
		return 1;
   }
	return 0;
};
/**
 * Process the next main command.
 */
static void engine_handle_cmd( struct android_app* app , int32_t cmd ) 
{
	switch( cmd ) 
	{
	case APP_CMD_SAVE_STATE:
		{
			// The system has asked us to save our current state.
			break;
		}
	case APP_CMD_INIT_WINDOW:
		{
			// The window is being shown.
			break;
		}
	case APP_CMD_TERM_WINDOW:
		{
			// The window is being hidden or closed.
			break;
		}
	case APP_CMD_GAINED_FOCUS:
		{
			// Our app gains focus.
			break;
		}
	case APP_CMD_LOST_FOCUS:
		{
			// Our app looses focus.
			break;
		}
	}
};
void android_main( struct android_app* state ) 
{
	app_dummy();
	rux::engine::initialize( NULL , state->activity );

	
	rux::XString test_str( "Hello,my dear friends!!!" );
	//test_str.set_ByRef( test_str.ConvertToUTF16() );
	
	LOGW( "test_native entered main" );
	
	state->userData = NULL;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;

	// loop waiting for stuff to do.
	rux::uint8 quit = 0;
	for( ; ; )
	{		
		// Read all pending events.
		int ident = 0;
		int events = 0;
		struct android_poll_source* source = NULL;
		// We will block forever waiting for events.
		while( ( ident = ALooper_pollAll( -1 , NULL , &events , (void**)&source ) ) >= 0 )
		{
			// Process this event.
			if( source != NULL )
			{	
				source->process( state , source );
			}
			// Check if we are exiting.
			if( state->destroyRequested != 0 )
			{
				LOGW( "test_native exited main" );
				quit = 1;
				break;
			}
		}
		if( quit )
			break;
	}
	rux::engine::deinitialize();
};