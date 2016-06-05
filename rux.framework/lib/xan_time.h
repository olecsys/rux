#ifndef XAN_TIME_H
#define XAN_TIME_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	extern dll_internal const ::rux::uint64 min_unix_time;
	begin_declare_rux_class( Time );
		void AddDays( size_t days );
		void SubtractDays( size_t days );
		void AddMonths( size_t months );
		void SubtractMonths( size_t months );
		void AddYears( size_t years );
		void SubtractYears( size_t years );
		Time& ConvertToUnixTime( void );
		Time& ConvertToWindowsTime( void );
		Time& ConvertToLocalUnixTime( void );
		Time& ConvertToLocalWindowsTime( void );
		::rux::uint32 get_DayOfWeek( void );
		::rux::uint32 get_DayOfMonth( void );
		::rux::uint32 DaysInMonth( void );
		::rux::uint32 get_Hour( void );
		::rux::uint32 get_Minute( void );
		::rux::uint32 get_Second( void );
		::rux::uint32 get_Millisecond( void );
		::rux::uint32 get_Year( void );
		::rux::uint32 get_Month( void );
		::rux::String& ToString( void );
		::rux::String& ToString( const char* format );
		::rux::uint64 get_Time( void );
		::rux::uint8 get_IsUnixTime( void );
		::rux::uint8 get_IsLocalTime( void );
	begin_declare_rux_class_members( Time );
		::rux::uint64 _time;
		::rux::uint8 _is_unix_time;
		::rux::uint8 _is_local_time;
	end_declare_rux_class_members( Time );
		XTime( ::rux::uint64 time , ::rux::uint8 is_unix_time , ::rux::uint8 is_local_time );
		XTime( ::rux::uint32 year , ::rux::uint32 month , ::rux::uint32 day , ::rux::uint32 hour , ::rux::uint32 minute , ::rux::uint32 second , ::rux::uint32 millisecond  );
		Time& ConvertToUnixTime( void );
		Time& ConvertToWindowsTime( void );
		Time& ConvertToLocalUnixTime( void );
		Time& ConvertToLocalWindowsTime( void );
		::rux::uint32 get_DayOfWeek( void );
		::rux::uint32 get_DayOfMonth( void );
		::rux::uint32 DaysInMonth( void );
		::rux::uint32 get_Hour( void );
		::rux::uint32 get_Minute( void );
		::rux::uint32 get_Second( void );
		::rux::uint32 get_Millisecond( void );
		::rux::uint32 get_Year( void );
		::rux::uint32 get_Month( void );
		::rux::String& ToString( void );
		::rux::String& ToString( const char* format );
		::rux::uint64 get_Time( void );
		::rux::uint8 get_IsUnixTime( void );
		::rux::uint8 get_IsLocalTime( void );
		void AddDays( size_t days );
		void SubtractDays( size_t days );
		void AddMonths( size_t months );
		void SubtractMonths( size_t months );
		void AddYears( size_t years );
		void SubtractYears( size_t years );
		static ::rux::uint8 TryParse( const ::rux::XString& local_time_string , const char* format , XTime& time );
		static ::rux::uint64 get_NowAsUnixTime( void );
		static ::rux::uint64 get_NowAsWindowsTime( void );
		static ::rux::uint64 get_LocalNowAsUnixTime( void );
		static ::rux::uint64 get_LocalNowAsWindowsTime( void );
		static ::rux::uint32 get_UnixTimeStamp( void );
		static ::rux::String& UnixTimeInMicroSecondsToString( ::rux::uint64 time , const char* format );
		static void UnixTimeInMicroSecondsToString( ::rux::uint64 time , const char* format , char* dst , size_t dst_size );
		static ::rux::String& WindowsTimeInNanoSecondsToString( ::rux::uint64 time , const char* format );
		static Time& Now( void );
		static ::rux::uint64 get_HighPerformanceTime( void );
		static ::rux::uint32 DaysInMonth( ::rux::uint32 month , ::rux::uint32 year );
		static ::rux::String& MonthToString( ::rux::uint32 month );
		static ::rux::uint32 GetTickCount( void );
		static bool SetSystemTime( XTime& time );
	end_declare_rux_class();
};
#endif
