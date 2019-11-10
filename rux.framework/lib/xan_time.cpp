#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_time.h"
#include "xan_uint32.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_ns( Time , rux )
	_time = 0;
	_is_unix_time = 0;
	_is_local_time = 0;
end_implement_rux_class();
namespace rux 
{	
	const ::rux::uint64 min_unix_time = 18446744073708551616ULL;
	#ifdef __UNIX__
	typedef struct _FILETIME 
	{
		::rux::uint32 dwLowDateTime;
		::rux::uint32 dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;
	#define _FILETIME_
	#endif
	XTime::XTime( ::rux::uint64 time , ::rux::uint8 is_unix_time , ::rux::uint8 is_local_time )
	{
		(*this)()->_time = time;
		(*this)()->_is_unix_time = is_unix_time;
		(*this)()->_is_local_time = is_local_time;
	};
	XTime::XTime( ::rux::uint32 year , ::rux::uint32 month , ::rux::uint32 day , ::rux::uint32 hour , ::rux::uint32 minute , ::rux::uint32 second , ::rux::uint32 millisecond )
	{
		declare_stack_variable( char , format , 64 );
		::rux::uint32 days_in_month = ::rux::XTime::DaysInMonth( month , year );
		if( days_in_month < day )
			day = days_in_month;
		::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , hour , minute , second , millisecond , day , month , year );
		XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , *this );
	};
	::rux::uint64 XTime::get_NowAsUnixTime( void )
	{	
	#ifdef __WINDOWS__
		::rux::uint64 time = 0;
		FILETIME now;
		GetSystemTimeAsFileTime( (LPFILETIME)&now );
		time = now.dwHighDateTime;
		time <<= 32;
		time |= now.dwLowDateTime;
		time /= 10;
		time -= DELTA_EPOCH_IN_USEC;
		return time;
	#endif
	#ifdef __UNIX__
		rux_tzset();
		struct timeval tv;  
		gettimeofday( &tv , NULL );
		return tv.tv_sec * 1000000LL + tv.tv_usec;
	#endif
	};
	::rux::uint64 XTime::get_LocalNowAsUnixTime( void )
	{
	#ifdef __WINDOWS__
		::rux::uint64 time = 0;	
	  FILETIME local_file_time;
	  GetSystemTimeAsFileTime((LPFILETIME)&time);
	  FileTimeToLocalFileTime((LPFILETIME)&time, &local_file_time);	
		time = local_file_time.dwHighDateTime;
		time <<= 32;
		time |= local_file_time.dwLowDateTime;
		time /= 10;
		time -= DELTA_EPOCH_IN_USEC;
		return time;
	#endif
	#ifdef __UNIX__
		rux_tzset();
		struct timeval tv;	
		gettimeofday( &tv , NULL );
		return ( tv.tv_sec - timezone ) * 1000000LL + tv.tv_usec;
	#endif
	};
	::rux::uint64 XTime::get_HighPerformanceTime( void )
	{
		::rux::uint64 time0 = 0;
	#ifdef __WINDOWS__		
		GetSystemTimeAsFileTime( (LPFILETIME)&time0 );
	#endif
	#ifdef __UNIX__
		struct timespec ts = { 0 };
		clock_gettime( CLOCK_MONOTONIC , &ts );
		time0 = ts.tv_sec * 1000ULL * 10000ULL + ts.tv_nsec;
	#endif
		return time0;
	};
	::rux::uint64 XTime::get_NowAsWindowsTime( void )
	{
		::rux::uint64 time0 = 0;
	#ifdef __WINDOWS__		
		GetSystemTimeAsFileTime( (LPFILETIME)&time0 );
	#endif
	#ifdef __UNIX__
		rux_tzset();
		struct timeval tv;
		gettimeofday( &tv , NULL );
		::rux::uint64 unix_time = tv.tv_sec * 1000000LL + tv.tv_usec;
		unix_time += DELTA_EPOCH_IN_USEC;
		time0 = unix_time * 10;	
	#endif
		return time0;
	};
	::rux::uint64 XTime::get_LocalNowAsWindowsTime( void )
	{
		::rux::uint64 time0 = 0;
	#ifdef __WINDOWS__	
		::rux::uint64 file_time = 0;  
	  GetSystemTimeAsFileTime((LPFILETIME)&file_time);
	  FileTimeToLocalFileTime((LPFILETIME)&file_time, (LPFILETIME)&time0);			
	#endif
	#ifdef __UNIX__
		rux_tzset();
		struct timeval tv;
		gettimeofday( &tv , NULL );
		::rux::uint64 unix_time = ( tv.tv_sec - timezone ) * 1000000LL + tv.tv_usec;
		unix_time += DELTA_EPOCH_IN_USEC;
		time0 = unix_time * 10;	
	#endif
		return time0;
	};
	::rux::uint32 XTime::get_UnixTimeStamp( void )
	{
		::rux::uint32 time = 0;
	#ifdef __WINDOWS__	
		SYSTEMTIME unix_epoch;
		memset( &unix_epoch , 0 , sizeof( SYSTEMTIME ) );
		unix_epoch.wDay = 1;
		unix_epoch.wMonth = 1;
		unix_epoch.wYear = 1970;
		::rux::uint64 unix_epoch_uint64 = 0;
		SystemTimeToFileTime( &unix_epoch , (LPFILETIME)&unix_epoch_uint64 );
		::rux::uint64 file_time = 0;  
	  GetSystemTimeAsFileTime((LPFILETIME)&file_time);
	  time = (::rux::uint32)( ( file_time - unix_epoch_uint64 ) / ( 10000LL * 1000LL ) );
	#else
		struct timeval tv;
		gettimeofday( &tv , NULL );
		time = tv.tv_sec;
	#endif
		return time;
	};
	void XTime::UnixTimeInMicroSecondsToString( ::rux::uint64 time , const char* format , char* dst , size_t dst_size )
	{
		rux_tzset();
		time_t time_struct;
		time_struct = time / 1000000;
		tm tm_value;
		::rux::gmtime_r( &time_struct , &tm_value );
		if( format == NULL )
			format = "%H:%M:%S %d.%m.%Y";
		char number[ 4 ] = {0} , temp_format[ 1024 ] = {0};
		snprintf( temp_format , 1024 , "%s" , format );
		snprintf( number , 4 , "%u" , (::rux::uint32)( ( time % 1000000ULL ) / 1000ULL ) );
		size_t length = strlen( number );
		if( length == 1 )
		{
			number[ 2 ] = number[ 0 ];
			number[ 0 ] = '0';
			number[ 1 ] = '0';
		}
		else if( length == 2 )
		{
			number[ 2 ] = number[ 1 ];
			number[ 1 ] = number[ 0 ];
			number[ 0 ] = '0';
		}
		char* ptr = strstr( temp_format , "%MS" );
		if( ptr )
		{
			do
			{
				memcpy( ptr , number , 3 );
				ptr += 3;
				ptr = strstr( temp_format , "%MS" );
			}
			while( ptr );
		}
		strftime( dst , dst_size , temp_format , &tm_value );
	};
	String& XTime::UnixTimeInMicroSecondsToString( ::rux::uint64 time , const char* format )
	{
		rux_tzset();
		time_t time_struct;
		time_struct = time / 1000000;
		tm tm_value;
		::rux::gmtime_r( &time_struct , &tm_value );
		::rux::XString format_string;
		if( format == NULL )
			format_string = "%H:%M:%S %d.%m.%Y";
		else
			format_string = format;
		format_string.set_ByRef( format_string.ReplaceAll( "%MS" , XUInt32( ( time % 1000000ULL ) / 1000ULL ).ToString() ) );
		declare_stack_variable( char , time_string , 32 );
		strftime( time_string , 32 , format_string.str() , &tm_value );
		::rux::XString result( time_string );	
		return result++;	
	};
	String& XTime::WindowsTimeInNanoSecondsToString( ::rux::uint64 time , const char* format )
	{
		rux_tzset();
		time_t time_struct;
		time_struct = time / ( 10000ULL * 1000ULL );
		tm tm_value;
		::rux::gmtime_r( &time_struct , &tm_value );
		::rux::XString format_string;
		if( format == NULL )
			format_string = "%H:%M:%S %d.%m.%Y";
		else
			format_string = format;
		format_string.set_ByRef( format_string.ReplaceAll( "%MS" , XUInt32( ( time % 1000000ULL ) / 1000ULL ).ToString() ) );
		declare_stack_variable( char , time_string , 32 );
		strftime( time_string , 32 , format_string.str() , &tm_value );
		::rux::XString result( time_string );	
		return result++;	
	};
	Time& XTime::Now( void )
	{
		XTime time;
		::rux::uint64 time_uint64 = 0;
	#ifdef __WINDOWS__
		GetSystemTimeAsFileTime( (LPFILETIME)&time_uint64 );
	#elif defined( __UNIX__ )
		rux_tzset();
		struct timeval tv;  
		gettimeofday( &tv , NULL );
		time_uint64 = tv.tv_sec * 1000000LL + tv.tv_usec;
		time()->_is_unix_time = 1;
	#endif
		time()->_time = time_uint64;
		return time++;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , DaysInMonth );
	::rux::uint32 Time::DaysInMonth( void )
	{
		return XTime::DaysInMonth( get_DayOfMonth() , get_Year() );
	};
	implement_duplicate_internal_function_1( Time , SubtractMonths , size_t );
	void Time::SubtractMonths( size_t months )
	{
		if( months > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
			if( months >= month )
			{
				months -= month;
				month = 12;
				year--;
			}
			year -= months / 12;
			month -= months % 12;
			size_t days_in_month = (size_t)XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
			if( day_of_month > days_in_month )
				day_of_month = days_in_month;
			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_function_1( Time , AddYears , size_t );
	void Time::AddYears( size_t years )
	{
		if( years > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
		
			year += years;

			size_t days_in_month = XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
			if( day_of_month > days_in_month )
				day_of_month = days_in_month;

			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_function_1( Time , SubtractYears , size_t );
	void Time::SubtractYears( size_t years )
	{
		if( years > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
		
			year -= years;
			if( year < 1970 )
				year = 1970;

			size_t days_in_month = (size_t)XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
			if( day_of_month > days_in_month )
				day_of_month = days_in_month;

			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_function_1( Time , AddMonths , size_t );
	void Time::AddMonths( size_t months )
	{
		if( months > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
		
			month += months;
			if( month != 12 )
			{
				year += month / 12;
				month %= 12;
			}
			size_t days_in_month = XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
			if( day_of_month > days_in_month )
				day_of_month = days_in_month;

			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_function_1( Time , SubtractDays , size_t );
	void Time::SubtractDays( size_t days )
	{
		if( days > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
			while( days )
			{
				if( days >= day_of_month )
				{
					days -= day_of_month;
					month--;
					if( month == 0 )
					{
						month = 12;
						year--;
					}
					day_of_month = XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
				}
				else
				{
					day_of_month -= days;
					break;
				}
			}
			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_function_1( Time , AddDays , size_t );
	void Time::AddDays( size_t days )
	{
		if( days > 0 )
		{
			declare_variable_param( rux::XTime , local_time , ConvertToLocalUnixTime() );
			size_t year = local_time.get_Year();
			size_t month = local_time.get_Month();
			size_t day_of_month = local_time.get_DayOfMonth();
			while( days )
			{
				size_t days_in_month = XTime::DaysInMonth( (::rux::uint32)month , (::rux::uint32)year );
				if( day_of_month + days > days_in_month )
				{
					days -= 1 + ( days_in_month - day_of_month );
					day_of_month = 1;
					month++;
					if( month > 12 )
					{
						month = 1;
						year++;
					}
				}
				else
				{
					day_of_month += days;
					break;
				}
			}
			declare_stack_variable( char , format , 64 );
			::rux::safe_sprintf( format , 64 , "%u:%u:%u.%u %u.%u.%u" , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() , (::rux::uint32)day_of_month , (::rux::uint32)month , (::rux::uint32)year );
			XTime::TryParse( format , "%H:%M:%S.%MS %d.%m.%Y" , local_time );
			if( _is_local_time )
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToLocalUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToLocalWindowsTime() );
			}
			else
			{
				if( _is_unix_time )
					local_time.set_ByRef( local_time.ConvertToUnixTime() );
				else
					local_time.set_ByRef( local_time.ConvertToWindowsTime() );
			}
			_time = local_time.get_Time();
		}
	};
	implement_duplicate_internal_result_function( Time& , Time , ConvertToUnixTime );
	Time& Time::ConvertToUnixTime( void )
	{
		XTime unix_time;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				::rux::uint64 time_uint64 = 0;
				LocalFileTimeToFileTime( &local_file_time , (FILETIME*)&time_uint64 );
				time = ((FILETIME*)&time_uint64)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&time_uint64)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				unix_time()->_time = time;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;
	#elif defined( __UNIX__ )
				rux_tzset();
				unix_time()->_time = _time + timezone * 1000000ULL;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;			
	#endif
			}
			else
			{
				unix_time()->_time = _time;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				::rux::uint64 time = 0;
				FILETIME file_time;
				LocalFileTimeToFileTime( (FILETIME*)&_time , &file_time );
				time = file_time.dwHighDateTime;
				time <<= 32;
				time |= file_time.dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				unix_time()->_time = time;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;			
				unix_time()->_time = time + timezone * 1000000ULL;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;
	#endif
			}
			else
			{
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				unix_time()->_time = time;
				unix_time()->_is_unix_time = 1;
				unix_time()->_is_local_time = 0;
			}
		}
		return unix_time++;
	};
	implement_duplicate_internal_result_function( Time& , Time , ConvertToWindowsTime );
	Time& Time::ConvertToWindowsTime( void )
	{
		XTime windows_time;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				::rux::uint64 time_uint64 = 0;
				LocalFileTimeToFileTime( &local_file_time , (FILETIME*)&time_uint64 );			
				windows_time()->_time = time_uint64;
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;
	#elif defined( __UNIX__ )
				rux_tzset();			
				::rux::uint64 time = _time + timezone * 1000000ULL;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;	
				::rux::uint64 file_time = 0;
				((FILETIME*)&file_time)->dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				((FILETIME*)&file_time)->dwHighDateTime = time & 0xffffffff;
				windows_time()->_time = file_time;
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;			
	#endif
			}
			else
			{	
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;	
				::rux::uint64 file_time = 0;
				((FILETIME*)&file_time)->dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				((FILETIME*)&file_time)->dwHighDateTime = time & 0xffffffff;
				windows_time()->_time = file_time;
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__			
				::rux::uint64 file_time = 0;
				LocalFileTimeToFileTime( (FILETIME*)&_time , (FILETIME*)&file_time );			
				windows_time()->_time = file_time;
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;
	#elif defined( __UNIX__ )	
				rux_tzset();			
				windows_time()->_time = _time + timezone * 10000ULL * 1000ULL;			
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;
	#endif
			}
			else
			{
				windows_time()->_time = _time;
				windows_time()->_is_unix_time = 0;
				windows_time()->_is_local_time = 0;
			}
		}
		return windows_time++;
	};
	implement_duplicate_internal_result_function( Time& , Time , ConvertToLocalUnixTime );
	Time& Time::ConvertToLocalUnixTime( void )
	{
		XTime local_unix_time;	
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
				local_unix_time()->_time = _time;
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;
				::rux::uint64 time_uint64 = 0;
				FileTimeToLocalFileTime( &file_time , (FILETIME*)&time_uint64 );
				time = ((FILETIME*)&time_uint64)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&time_uint64)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				local_unix_time()->_time = time;
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;
	#elif defined( __UNIX__ )
				rux_tzset();			
				local_unix_time()->_time = _time - ( timezone* 1000000ULL );
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;			
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{			
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;			
				local_unix_time()->_time = time;
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;
				FileTimeToLocalFileTime( (FILETIME*)&_time , &local_file_time );
				::rux::uint64 time = 0;			
				time = local_file_time.dwHighDateTime;
				time <<= 32;
				time |= local_file_time.dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				local_unix_time()->_time = time;
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;			
				local_unix_time()->_time = time - timezone * 1000000ULL;
				local_unix_time()->_is_unix_time = 1;
				local_unix_time()->_is_local_time = 1;
	#endif
			}
		}	
		return local_unix_time++;
	};
	implement_duplicate_internal_result_function( Time& , Time , ConvertToLocalWindowsTime );
	Time& Time::ConvertToLocalWindowsTime( void )
	{
		XTime local_windows_time;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;	
				::rux::uint64 file_time = 0;
				((FILETIME*)&file_time)->dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				((FILETIME*)&file_time)->dwHighDateTime = time & 0xffffffff;			
				local_windows_time()->_time = file_time;			
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;
				::rux::uint64 time_uint64 = 0;
				FileTimeToLocalFileTime( &file_time , (FILETIME*)&time_uint64 );			
				local_windows_time()->_time = time_uint64;
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;
	#elif defined( __UNIX__ )
				rux_tzset();			
				::rux::uint64 time = _time - timezone * 1000000ULL;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;	
				::rux::uint64 file_time = 0;
				((FILETIME*)&file_time)->dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				((FILETIME*)&file_time)->dwHighDateTime = time & 0xffffffff;			
				local_windows_time()->_time = file_time;
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;			
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
				local_windows_time()->_time = _time;
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;
			}
			else
			{
	#ifdef __WINDOWS__
				::rux::uint64 time;
				FileTimeToLocalFileTime( (FILETIME*)&_time , (FILETIME*)&time );			
				local_windows_time()->_time = time;
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;
	#elif defined( __UNIX__ )
				rux_tzset();			
				local_windows_time()->_time = _time - timezone * 10000ULL * 1000ULL;
				local_windows_time()->_is_unix_time = 0;
				local_windows_time()->_is_local_time = 1;
	#endif
			}
		}	
		return local_windows_time++;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_DayOfWeek );
	::rux::uint32 Time::get_DayOfWeek( void )
	{
		::rux::uint32 day_of_week = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				day_of_week = st.wDayOfWeek;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_week = tm_value.tm_wday;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				day_of_week = st.wDayOfWeek;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_week = tm_value.tm_wday;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				day_of_week = st.wDayOfWeek;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_week = tm_value.tm_wday;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				day_of_week = st.wDayOfWeek;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_week = tm_value.tm_wday;
	#endif
			}
		}
		return day_of_week;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_DayOfMonth );
	::rux::uint32 Time::get_DayOfMonth( void )
	{
		::rux::uint32 day_of_month = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				day_of_month = st.wDay;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_month = tm_value.tm_mday;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				day_of_month = st.wDay;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_month = tm_value.tm_mday;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				day_of_month = st.wDay;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_month = tm_value.tm_mday;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				day_of_month = st.wDay;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				day_of_month = tm_value.tm_mday;
	#endif
			}
		}
		return day_of_month;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Hour );
	::rux::uint32 Time::get_Hour( void )
	{	
		::rux::uint32 hour = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				hour = st.wHour;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				hour = tm_value.tm_hour;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				hour = st.wHour;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				hour = tm_value.tm_hour;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				hour = st.wHour;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				hour = tm_value.tm_hour;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				hour = st.wHour;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				hour = tm_value.tm_hour;
	#endif
			}
		}
		return hour;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Minute );
	::rux::uint32 Time::get_Minute( void )
	{	
		::rux::uint32 minute = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				minute = st.wMinute;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				minute = tm_value.tm_min;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				minute = st.wMinute;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				minute = tm_value.tm_min;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				minute = st.wMinute;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				minute = tm_value.tm_min;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				minute = st.wMinute;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				minute = tm_value.tm_min;
	#endif
			}
		}
		return minute;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Millisecond );
	::rux::uint32 Time::get_Millisecond( void )
	{
		if( _is_unix_time == 1 )
			return (::rux::uint32)( ( _time - ( ( _time / 1000000ULL ) * 1000000ULL ) ) / 1000ULL );
		else
			return (::rux::uint32)( ( _time - ( ( _time / 10000000ULL ) * 10000000ULL ) ) / 10000ULL );
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Second );
	::rux::uint32 Time::get_Second( void )
	{	
		::rux::uint32 second = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				second = st.wSecond;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				second = tm_value.tm_sec;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				second = st.wSecond;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				second = tm_value.tm_sec;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				second = st.wSecond;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				second = tm_value.tm_sec;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				second = st.wSecond;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				second = tm_value.tm_sec;
	#endif
			}
		}
		return second;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Month );
	::rux::uint32 Time::get_Month( void )
	{
		::rux::uint32 month = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				month = st.wMonth;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				month = tm_value.tm_mon + 1;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				month = st.wMonth;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				month = tm_value.tm_mon + 1;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				month = st.wMonth;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				month = tm_value.tm_mon + 1;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				month = st.wMonth;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				month = tm_value.tm_mon + 1;
	#endif
			}
		}
		return month;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Time , get_Year );
	::rux::uint32 Time::get_Year( void )
	{	
		::rux::uint32 year = 0;
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				FILETIME local_file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				local_file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				local_file_time.dwHighDateTime = time & 0xffffffff;
				SYSTEMTIME st;
				FileTimeToSystemTime( &local_file_time , &st );
				year = st.wYear;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				year = tm_value.tm_year + 1900;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				FILETIME file_time;			
				::rux::uint64 time = _time;
				time += DELTA_EPOCH_IN_USEC;
				time *= 10;
				file_time.dwLowDateTime = time & 0xffffffff;
				time >>= 32;
				file_time.dwHighDateTime = time & 0xffffffff;			
				SYSTEMTIME st;
				FileTimeToSystemTime( &file_time , &st );			
				year = st.wYear;
	#elif defined( __UNIX__ )
				rux_tzset();
				time_t time_struct;
				time_struct = _time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				year = tm_value.tm_year + 1900;
	#endif
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				year = st.wYear;
	#elif defined( __UNIX__ )	
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				year = tm_value.tm_year + 1900;
	#endif
			}
			else
			{
	#ifdef __WINDOWS__
				SYSTEMTIME st;			
				FileTimeToSystemTime( (FILETIME*)&_time , &st );
				year = st.wYear;
	#elif defined( __UNIX__ )
				rux_tzset();
				::rux::uint64 time = ((FILETIME*)&_time)->dwHighDateTime;
				time <<= 32;
				time |= ((FILETIME*)&_time)->dwLowDateTime;
				time /= 10;
				time -= DELTA_EPOCH_IN_USEC;
				time_t time_struct;
				time_struct = time / 1000000;
				tm tm_value;
				::rux::gmtime_r( &time_struct , &tm_value );
				mktime( &tm_value );
				year = tm_value.tm_year + 1900;
	#endif
			}
		}
		return year;
	};
	::rux::uint8 XTime::TryParse( const ::rux::XString& time_string , const char* format , XTime& time )
	{
		::rux::uint8 is_parseble = 1;
		rux_tzset();
		::rux::int32 ms = 0;
		tm tm_value;
		memset( &tm_value , 0 , sizeof( tm_value ) );	
		::rux::XString format_string;
		if( format == NULL )
			format_string = "%H:%M:%S %d.%m.%Y";
		else
			format_string = format;
		XArray< XString > time_replaces;
		time_replaces.Add( ::rux::XString( "%MS" ) );
		time_replaces.Add( ::rux::XString( "%H" ) );
		time_replaces.Add( ::rux::XString( "%M" ) );
		time_replaces.Add( ::rux::XString( "%S" ) );
		time_replaces.Add( ::rux::XString( "%d" ) );
		time_replaces.Add( ::rux::XString( "%m" ) );
		time_replaces.Add( ::rux::XString( "%Y" ) );	
		::rux::int32 index0 = 0 , index1 = 0;
		size_t start_index0 = 0 , array_index = 0 , start_index1 = 0;	
		while( ( index0 = format_string.IndexOfAny( time_replaces , start_index0 , array_index ) ) > -1 )
		{
			if( static_cast< ::rux::uint32 >( index0 ) > start_index0 )
			{
				index1 = time_string.IndexOf( format_string.Substring( start_index0 , index0 - start_index0 ) , start_index1 );
				if( index1 != static_cast< ::rux::int32 >( start_index1 ) )
				{
					is_parseble = 0;
					break;
				}
				start_index1 += index0 - start_index0;
			}
			if( array_index == 0 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{					
						start_index1++;
						if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
						{					
							start_index1++;
							ms = XString( time_string.Substring( start_index1 - 3 , 3 ) ).ToInt32();
						}
						else
						{
							ms = XString( time_string.Substring( start_index1 - 2 , 2 ) ).ToInt32();
						}
					}
					else
					{
						ms = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32();
					}
					if( ms < 0 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 3;
				}
				else
				{
					is_parseble = 0;
					break;
				}
			}
			else if( array_index == 1 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{
						tm_value.tm_hour = XString( time_string.Substring( start_index1 - 1 , 2 ) ).ToInt32();
						start_index1++;
					}
					else
					{
						tm_value.tm_hour = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32();
					}
					if( tm_value.tm_hour < 0 || tm_value.tm_hour > 23 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}			
			}
			else if( array_index == 2 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{
						tm_value.tm_min = XString( time_string.Substring( start_index1 - 1 , 2 ) ).ToInt32();
						start_index1++;
					}
					else
					{
						tm_value.tm_min = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32();
					}
					if( tm_value.tm_min < 0 || tm_value.tm_min > 59 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}		
			}
			else if( array_index == 3 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{
						tm_value.tm_sec = XString( time_string.Substring( start_index1 - 1 , 2 ) ).ToInt32();
						start_index1++;
					}
					else
					{
						tm_value.tm_sec = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32();
					}
					if( tm_value.tm_sec < 0 || tm_value.tm_sec > 59 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}		
			}
			else if( array_index == 4 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{
						tm_value.tm_mday = XString( time_string.Substring( start_index1 - 1 , 2 ) ).ToInt32();
						start_index1++;
					}
					else
					{
						tm_value.tm_mday = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32();
					}
					if( tm_value.tm_mday < 1 || tm_value.tm_mday > 31 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}		
			}
			else if( array_index == 5 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{
						tm_value.tm_mon = XString( time_string.Substring( start_index1 - 1 , 2 ) ).ToInt32() - 1;
						start_index1++;
					}
					else
					{
						tm_value.tm_mon = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32() - 1;
					}
					if( tm_value.tm_mon < 0 || tm_value.tm_mon > 11 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}		
			}
			else if( array_index == 6 )
			{
				if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
				{
					start_index1++;
					if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
					{					
						start_index1++;
						if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
						{					
							start_index1++;
							if( XString::IsInt32( time_string.get_UTF8Char( start_index1 ) ) == 1 )
							{					
								start_index1++;
								tm_value.tm_year = XString( time_string.Substring( start_index1 - 4 , 4 ) ).ToInt32() - 1900;
							}
							else
							{
								tm_value.tm_year = XString( time_string.Substring( start_index1 - 3 , 3 ) ).ToInt32() - 1900;
							}
						}
						else
						{
							tm_value.tm_year = XString( time_string.Substring( start_index1 - 2 , 2 ) ).ToInt32() - 1900;
						}
					}
					else
					{
						tm_value.tm_year = XString( time_string.Substring( start_index1 - 1 , 1 ) ).ToInt32() - 1900;
					}
					if( tm_value.tm_year < 0 )
					{
						is_parseble = 0;
						break;
					}
					start_index0 = index0 + 2;
				}
				else
				{
					is_parseble = 0;
					break;
				}
			}
		}
		if( is_parseble == 1 )
		{		
			time_t time_struct = mktime( &tm_value ) - tm_value.tm_isdst * 3600;		
			time()->_is_local_time = 0;
			time()->_is_unix_time = 1;
			time()->_time = time_struct * 1000000ULL + ms * 1000ULL;
		}
		return is_parseble;
	};
	implement_duplicate_internal_result_function_1( String& , Time , ToString , const char* );
	String& Time::ToString( const char* format )
	{
		declare_variable( XString , result );
		rux_tzset();
		if( _is_unix_time == 1 )
		{
			::rux::XString format_string( format );
			declare_variable( XString , ms_string );
			::rux::uint32 ms = get_Millisecond();
			if( ms < 10 )
				ms_string.set_ByRef( "00" + XUInt32( ms ).ToString() );
			else if( ms < 100 )
				ms_string.set_ByRef( "0" + XUInt32( ms ).ToString() );
			else
				ms_string.set_ByRef( XUInt32( ms ).ToString() );
			format_string.set_ByRef( format_string.ReplaceAll( "%MS" , ms_string ) );
			format_string.set_ByRef( format_string.ConvertToAnsi() );
			if( _is_local_time == 1 )
			{
				time_t time_struct;
				time_struct = _time / 1000000ULL;
				tm tm_value;
				if( ::rux::gmtime_r( &time_struct , &tm_value ) )
				{
					declare_stack_variable( char , time_string , 4096 );
					strftime( time_string , 4096 , format_string.str() , &tm_value );
					result = time_string;
				}
			}
			else
			{
				time_t time_struct;
				time_struct = _time / 1000000ULL;
				tm tm_value;
				if( ::rux::gmtime_r( &time_struct , &tm_value ) )
				{
					declare_stack_variable( char , time_string , 4096 );
					strftime( time_string , 4096 , format_string.str() , &tm_value );
					result = time_string;
				}
			}
		}
		else
		{		
			if( _is_local_time == 1 )
			{
				XTime unix_time( ConvertToLocalUnixTime() );
				result = unix_time.ToString( format );
			}
			else
			{
				XTime unix_time( ConvertToUnixTime() );
				result = unix_time.ToString( format );
			}
		}
		return result++;
	};
	implement_duplicate_internal_result_function( String& , Time , ToString );
	String& Time::ToString( void )
	{
		::rux::XString result;
		rux_tzset();
		if( _is_unix_time == 1 )
		{
			if( _is_local_time == 1 )
			{
				time_t time_struct;
				time_struct = _time / 1000000ULL;
				tm tm_value;
				if( ::rux::gmtime_r( &time_struct , &tm_value ) )
				{
					declare_stack_variable( char , time_string , 32 );
					strftime( time_string , 32 , "%H:%M:%S %d.%m.%Y" , &tm_value );
					result = time_string;
				}
				else
					result = "00:00:00 01.01.1970";			
			}
			else
			{
				time_t time_struct;
				time_struct = _time / 1000000ULL;
				tm tm_value;
				if( ::rux::gmtime_r( &time_struct , &tm_value ) )
				{
					declare_stack_variable( char , time_string , 32 );
					strftime( time_string , 32 , "%H:%M:%S %d.%m.%Y" , &tm_value );
					result = time_string;
				}
				else
					result = "00:00:00 01.01.1970";			
			}
		}
		else
		{
			if( _is_local_time == 1 )
			{
				XTime unix_time( ConvertToLocalUnixTime() );
				result = unix_time.ToString();
			}
			else
			{
				XTime unix_time( ConvertToUnixTime() );
				result = unix_time.ToString();
			}
		}
		return result++;
	};
	implement_duplicate_internal_result_function( ::rux::uint64 , Time , get_Time );
	::rux::uint64 Time::get_Time( void )
	{
		return _time;
	};
	implement_duplicate_internal_result_function( ::rux::uint8 , Time , get_IsUnixTime );
	::rux::uint8 Time::get_IsUnixTime( void )
	{
		return _is_unix_time;
	};
	implement_duplicate_internal_result_function( ::rux::uint8 , Time , get_IsLocalTime );
	::rux::uint8 Time::get_IsLocalTime( void )
	{
		return _is_local_time;
	};
	::rux::uint32 XTime::DaysInMonth( ::rux::uint32 month , ::rux::uint32 year )
	{	    
		return ( 30 + ( ( ( month & 9 ) == 8 ) || ( ( month & 9 ) == 1 ) ) - ( month == 2 ) - ( !( ( ( year % 4 ) == 0 ) && ( ( ( year % 100 ) != 0 ) || ( ( year % 400 ) == 0 ) ) ) && ( month == 2 ) ) );
	};
	String& XTime::MonthToString( ::rux::uint32 month )
	{
		::rux::XString month_string;
		switch( month )
		{
		case 1:
			{
				month_string = "Январь";
				break;
			}
		case 2:
			{
				month_string = "Февраль";
				break;
			}
		case 3:
			{
				month_string = "Март";
				break;
			}
		case 4:
			{
				month_string = "Апрель";
				break;
			}
		case 5:
			{
				month_string = "Май";
				break;
			}
		case 6:
			{
				month_string = "Июнь";
				break;
			}
		case 7:
			{
				month_string = "Июль";
				break;
			}
		case 8:
			{
				month_string = "Август";
				break;
			}
		case 9:
			{
				month_string = "Сентябрь";
				break;
			}
		case 10:
			{
				month_string = "Октябрь";
				break;
			}
		case 11:
			{
				month_string = "Ноябрь";
				break;
			}
		case 12:
			{
				month_string = "Декабрь";
				break;
			}
		}
		return month_string++;
	};
	::rux::uint32 XTime::GetTickCount( void )
	{
	#ifdef __WINDOWS__
		return ::rux::engine::_globals->_winmm_module.timeGetTime();
	#else
		struct timespec ts = { 0 };
		clock_gettime( CLOCK_MONOTONIC , &ts );
		return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
	#endif
	};
	bool XTime::SetSystemTime( XTime& time )
	{
		XTime st;
	#ifdef __WINDOWS__
		st.set_ByRef( time.ConvertToWindowsTime() );
		::rux::uint64 time_uint64 = st.get_Time();
		SYSTEMTIME system_time;
		FileTimeToSystemTime( (LPFILETIME)&time_uint64 , &system_time );
		if( ::SetSystemTime( &system_time ) == 0 )
			return false;
		else
			return true;
	#else
		st.set_ByRef( time.ConvertToUnixTime() );
		::rux::uint64 time_uint64 = st.get_Time();
		timeval tv;
		tv.tv_sec = time_uint64 / 1000000;
		tv.tv_usec = time_uint64 % 1000000;
		if( settimeofday( &tv , NULL ) == 0 )
			return true;
		else
			return false;
	#endif
	};
};