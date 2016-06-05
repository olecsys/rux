#include <xan_sqliteprovider.h>
#include <xan_file.h>
#include <xan_asm.h>
#include <xan_jsonserializer.h>
#include <xan_console.h>
using namespace rux;
using namespace rux::io;
using namespace rux::data;
dll_internal const char* g_current_module_name = "rux.data.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
implement_global_variable( char , _rux_service_name , 1024 );
dll_internal uint8 _rux_is_service = 0;

begin_declare_rux_class( JSEvent1 );
	declare_register_rux_property();
begin_declare_rux_class_members( JSEvent1 );
	declare_rux_property( ::rux::XInt64 , name );
end_declare_rux_class_members( JSEvent1 );
end_declare_rux_class();

begin_implement_rux_class_with_properties( JSEvent1 )
end_implement_rux_class();

begin_register_rux_property( JSEvent1 )
	register_rux_property( ::rux::XInt64 , name );
end_register_rux_property();

implement_rux_property( JSEvent1 , ::rux::XInt64 , name );




begin_declare_rux_class( JSEvent );
	declare_register_rux_property();
begin_declare_rux_class_members( JSEvent );
	declare_rux_property( ::rux::XString , name );
	declare_rux_property( XJSEvent1 , obj );
	declare_rux_property( ::rux::XArray< XJSEvent1 > , rows );
end_declare_rux_class_members( JSEvent );
end_declare_rux_class();

begin_implement_rux_class_with_properties( JSEvent )
end_implement_rux_class();

begin_register_rux_property( JSEvent )
	register_rux_property( ::rux::XString , name );
	register_rux_property( XJSEvent1 , obj );	
	register_rux_property( ::rux::XArray< XJSEvent1 > , rows );
end_register_rux_property();

implement_rux_property( JSEvent , ::rux::XString , name );
implement_rux_property( JSEvent , XJSEvent1 , obj );
implement_rux_property( JSEvent , ::rux::XArray< XJSEvent1 > , rows );


rux::int32 test_empty( void )
{
	printf( "test_empty\n" );
	return 0;
};
rux::int32 test4( const char* param0 , const char* param1 , const char* param2 , void* param3 )
{
	printf( "test0 %p , %p , %p , %p\n" , param0 , param1 , param2 , param3 );
	printf( "test0 %s , %s , %s , %p\n" , param0 , param1 , param2 , param3 );
	return 0;
};
rux::int32 test0( const char* param0 , sqlite3* param1 , rux::int32 param2 , void* param3 )
{
	printf( "test0 %p , %p , %p , %p\n" , param0 , param1 , param2 , param3 );
	printf( "test0 %s , %p , %d , %p\n" , param0 , param1 , param2 , param3 );
	return 0;
};
int main(int argc , char** args )
{	
	{
		THREAD_REGISTER( "main" );
		if( argc == 2 && strcmp( args[ 1 ] , "--sqlite-test" ) == 0 )
		{
			rux::XString error;

			printf( "before CreateDatabase()\n" );
			XSqliteProvider sqlite_provider0( "./test.db" );
			sqlite_provider0.CreateDatabase( error );
			sqlite_provider0.ExecuteNonQuery( "--version(1001)\n"
				"begin transaction;\n"
				"---------------------------------------------------------------------\n"
				"-- таблицы связанные с guard\n"
				"---------------------------------------------------------------------\n"
				"-- таблица видеосерверов\n"
				"create table if not exists passwords(\n"
				"pswid         integer primary key,\n"
				"pswdeviceid   int not null,         -- guard device id\n"
				"pswpassword   text not null,        -- password\n"
				"pswnumber     integer not null      -- row\n"
				");\n"
				"---------------------------------------------------------------------\n"
				"-- таблицы связанные с устройствами\n"
				"---------------------------------------------------------------------\n"
				"-- таблица видеосерверов\n"
				"create table if not exists videoserver(\n"
				"vsrvid        integer primary key,\n"
				"vsrname       text not null collate nocase, -- название видео сервера 7\n"
				"vsrport       int not null,         		-- порт для StartListen\n"
				"vsrcomment    text not null collate nocase, -- примечание\n"
				"vsrsort       integer not null,     		-- сортирова для отображения\n"
				"vsrinfo       text not null collate nocase  -- служебная информация\n"
				");\n"
				"-- справочник типов настроек\n"
				"create table if not exists settingtype(\n"
				"sttid         integer not null primary key,\n"
				"sttname       text not null collate nocase, -- название типа устройства\n"
				"sttsort       integer not null,     		-- сортирова для отображения\n"
				"sttinfo       text not null collate nocase  -- служебная информация\n"
				");\n"
				"-- таблица профилей устройств\n"
				"create table if not exists setting(\n"
				"setid         integer primary key,\n"
				"settypeid     integer not null,     		-- тип настройки\n"
				"setname       text not null collate nocase, -- название устройства\n"
				"setvalue      text not null,        		-- значение\n"
				"setcomment    text not null collate nocase, -- значение\n"
				"setinfo       text not null collate nocase, -- служебная информация\n"
				"constraint fk_settypeid foreign key(settypeid) references settingtype(sttid) on delete cascade on update restrict\n"
				");\n"
				"create unique index uq_setting_typeid_name on setting(settypeid, setname);\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(1, 'profiles', 5, '');\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(2, 'servers', 10, '');\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(3, 'archive', 20, '');\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(4, 'events', 30, '');\n"
				"--insert into setting (settypeid, setname, setvalue, setcomment, setinfo) values(1, 'double', '', '', '')\n"
				"--insert into setting (settypeid, setname, setvalue, setcomment, setinfo) values(1, 'double', '', '', '')\n"
				"--insert into videoserver(vsrvid, vsrname, vsrcomment, vsrsort) values(1, '42', '', 10);\n"
				"--insert into videoserver(vsrvid, vsrname, vsrcomment, vsrsort) values(2, '84', '', 20);\n"
				"---------------------------------------------------------------------\n"
				"-- таблицы связанные с охраной стоянки\n"
				"---------------------------------------------------------------------\n"
				"-- таблица пользователей\n"
				"create table if not exists user(\n"
				"usrid             integer primary key,          -- pk\n"
				"usrcreateuser     integer not null default 1,   -- создатель, default 1 = admin\n"
				"usrcreatedate     text not null collate nocase, -- дата создания\n"
				"usrmodifyuser     integer not null default 1,   -- кто редактировал запись последним\n"
				"usrmodifydate     text not null collate nocase, -- дата последнего редактирования\n"
				"usrisdeleted      integer not null,             -- 1 - удалена, 0 - живой - строки не удаляются никогда\n"
				"usrisenabled      integer not null,             -- 0 - заблокирован, 1 - разрешен\n"
				"usrisadmin        integer not null,             -- администратор\n"
				"usrlogin          text not null collate nocase, -- логин\n"
				"usrpassword       text not null default \"1\",    -- хэш пароля\n"
				"usrname           text not null collate nocase, -- имя\n"
				"usrlastname       text not null collate nocase, -- фамилия\n"
				"usrsurname        text not null collate nocase, -- отчество\n"
				"usrmail           text not null collate nocase, -- email\n"
				"usrphone          text not null collate nocase, -- телефон\n"
				"usrissendmail     integer not null,             -- 1 - при событии необходимо отсылать email\n"
				"usrissendsms      integer not null,             -- 1 - при событии необходимо отсылать sms (Upgrade.sql 2502)\n"
				"usrissendmms      integer not null,             -- 1 - при событии необходимо отсылать mms\n"
				"usrmaxseczones    integer not null,             -- максимальное количество охраняемых зон\n"
				"usrpassport       text not null collate nocase, -- номер паспорта пользователя \n"
				"usrcomment        text not null collate nocase, -- примечание\n"
				"usrsysinfo        text not null collate nocase, -- системное поле\n"
				"constraint fk_usrcreateuser foreign key(usrcreateuser) references user(usrid) on delete set default on update restrict,\n"
				"constraint fk_usrmodifyuser foreign key(usrmodifyuser) references user(usrid) on delete set default on update restrict\n"
				");\n"
				"create unique index uq_user_login on user(usrlogin);\n"
				"PRAGMA foreign_keys = OFF;\n"
				"-- для пользователя \"admin\" пароль \"fulladmin\"\n"
				"insert into user(usrid, usrcreateuser, usrcreatedate, usrmodifyuser, usrmodifydate, usrisdeleted, usrisenabled, usrisadmin,\n"
				"usrlogin, usrpassword, usrname, usrlastname, usrsurname, usrmail, usrphone, usrissendmail, usrissendsms, usrissendmms, usrmaxseczones, usrpassport, usrcomment, usrsysinfo) \n"
				"values(1, 1, '20120419 12:00:00', 1, '20120419 12:00:00', 0, 1, 1,\n"
				"'admin', \"15676289173361244689\", 'Администратор', 'Фамилия', 'Отчество', 'mail@integra-s.com', '+78460001122', 0, 0, 0, 0, '', '', ',ADMIN,');\n"
				"PRAGMA foreign_keys = ON;\n"
				"pragma user_version=1001;\n"
				"commit;\n"
				"--version(1002)\n"
				"begin transaction;\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(5, 'logic', 40, '');\n"
				"pragma user_version=1002;\n"
				"commit;\n"
				"--version(1003)\n"
				"begin transaction;\n"
				"-- зоны для охраны( РЕНЕТ )\n"
				"create table if not exists parking_zone(\n"
				"pzid integer primary key not null,\n"
				"pzusrid integer not null,\n"
				"pzvalue      text not null,        		-- значение\n"
				"pzcomment    text not null collate nocase, -- значение\n"
				"pzinfo       text not null collate nocase, -- служебная информация\n"
				"constraint fk_pzuserid foreign key(pzusrid) references user(usrid) on delete restrict on update restrict\n"
				");\n"
				"insert into parking_zone( pzusrid , pzvalue , pzcomment , pzinfo )\n"
				"values( 1 , '{\"key1\":\"host\",\"key2\":\"205\",\"key3\":\"profile\",\"src\":\"4201.jpg\",\"conture\":{\"enable\":true , \"color\":\"magenta\",\"points\":[{\"x\":32768,\"y\":0},{\"x\":62535,\"y\":0},{\"x\":62535,\"y\":32768},{\"x\":32768,\"y\":32768}]}}' , 'TEST' , '' );\n"
				"pragma user_version=1003;\n"
				"commit;\n"
				"--version(1004)\n"
				"begin transaction;\n"
				"update user set usrname = 'Администратор', usrlastname = 'Фамилия', usrsurname = 'Отчество';\n"
				"pragma user_version=1004;\n"
				"commit;\n"
				"--version(1005)\n"
				"begin transaction;\n"
				"alter table user add usrlastreadedid integer not null default 0;\n"
				"create table if not exists user_param(\n"
				"uprid         integer primary key,\n"
				"upruserid     integer not null,             -- id пользователя\n"
				"uprname       text not null collate nocase, -- название параметра\n"
				"uprtext       text not null collate nocase, -- текстовое значение\n"
				"uprinteger    integer not null,             -- целочисленное значение\n"
				"uprfloat      real not null,                -- дробное значение\n"
				"constraint fk_upruserid foreign key(upruserid) references user(usrid) on delete cascade on update restrict\n"
				");\n"
				"create unique index uq_user_param_userid_name on user_param(upruserid, uprname);\n"
				"pragma user_version=1005;\n"
				"commit;\n"
				"--version(1006)\n"
				"begin transaction;\n"
				"create table if not exists server_param(\n"
				"sprid         integer primary key,\n"
				"sprcreateuser integer not null default 1,   -- создатель, default 1 = admin\n"
				"sprcreatedate text not null collate nocase, -- дата создания\n"
				"sprmodifyuser integer not null default 1,   -- кто редактировал запись последним\n"
				"sprmodifydate text not null collate nocase, -- дата последнего редактирования\n"
				"sprname       text not null collate nocase, -- название параметра\n"
				"sprvalue      text not null collate nocase, -- значение\n"
				"sprvaluetype  text not null collate nocase, -- тип значения: INT,FLOAT,TEXT,BOOL\n"
				"constraint fk_sprcreateuser foreign key(sprcreateuser) references user(usrid) on delete set default on update restrict,\n"
				"constraint fk_sprmodifyuser foreign key(sprmodifyuser) references user(usrid) on delete set default on update restrict\n"
				");\n"
				"create unique index uq_server_param_name on server_param(sprname);\n"
				"pragma user_version=1006;\n"
				"commit;\n"
				"--version(1007)\n"
				"begin transaction;\n"
				"ALTER TABLE 'user' ADD 'usrlastvisit' text not null default \"\";\n"
				"pragma user_version=1007;\n"
				"commit;\n"
				"--version(1008)\n"
				"begin transaction;\n"
				"ALTER TABLE 'user' ADD 'usrsetsjson' text not null default \"\";\n"
				"pragma user_version=1008;\n"
				"commit;\n"
				"--version(1009)\n"
				"begin transaction;\n"
				"insert into settingtype(sttid, sttname, sttsort, sttinfo) values(6, 'COMServers', 50, '');\n"
				"pragma user_version=1009;\n"
				"commit;\n"
				, error );
			printf( "after CreateDatabase()\n" );		
			printf( "before get_Schema()\n" );
			XDBSchema db_schema;		
			sqlite_provider0.get_Schema( db_schema , error );
			printf( "after get_Schema()\n" );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--reflection-test" ) == 0 )
		{			
		}
		else if( argc == 3 && strcmp( args[ 1 ] , "--get_schema" ) == 0 )
		{
			rux::XString error;		
			XSqliteProvider sqlite_provider0( args[ 2 ] );
			XDBSchema db_schema;		
			sqlite_provider0.get_Schema( db_schema , error );
		}
		else if( argc == 4 && strcmp( args[ 1 ] , "--create-db-by-script" ) == 0 )
		{
			rux::XString error;		
			XSqliteProvider sqlite_provider0( args[ 2 ] );
			XDBSchema db_schema;		
			sqlite_provider0.get_Schema( db_schema , error );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--json-big-string-test" ) == 0 )
		{		
			/*rux::XString utf8( "Привет!!!" );
			utf8.set_ByRef( utf8.ConvertToUTF8() );
			char* ptr = utf8()->_array_ptr;			
			
			uint8 is_first_byte = ( 0 & 0xc0 ) != 0x80;
			printf( "first byte = %u\n" , is_first_byte );

			size_t len = 0;
			while( *ptr ) len += ( *ptr++ & 0xc0 ) != 0x80;
			printf( "ptr = %s , size = %u\n" , ptr , len );*/

			rux::XString error;
			XFile file( XFile::Open( "../addons/json_big_string_test.txt" , error ) );
			if( file.Exists() == 1 )
			{
				rux::XString json;
				json.set_ByRef( file.ReadTextAll( error ) );		
				
				XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error ) );
				file.Close();

				if( error.Length() == 0 )
				{
					rux::data::XJsonSerializer::Serialize( json_object , 1 , error );
				}
			}
						
			file = XFile::Open( "../addons/DemuxerDevice.ftype" , error );
			if( file.Exists() == 1 )
			{
				rux::XString json;
				json.set_ByRef( file.ReadTextAll( error ) );		
				
				XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error ) );
				file.Close();
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--json-test" ) == 0 )
		{				
			const char* tt = "tr";
			printf( "%d\n" , strcmp( tt , "true"  ) );

			::rux::XString error;
			rux::data::XJsonSerializer::Deserialize( "-1988688" , error );
			rux::data::XJsonSerializer::Deserialize( "9223372036854775808" , error );
			rux::data::XJsonSerializer::Deserialize( "256757.768" , error );


			
			XFile file2( XFile::Open( "../addons/json_test2.txt" , error ) );
			if( file2.Exists() == 1 )
			{
				rux::XString json;
				json.set_ByRef( file2.ReadTextAll( error ) );		

				char* ptr = json()->_array_ptr;
				ptr++;
				char* ptr0 = strpbrk( ptr , "[]" );
				if( ptr0 )
					printf( "%s\n" , ptr0 );

				file2.Close();
				XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error ) );				
			}

			XFile file1( XFile::Open( "../addons/json_test1.txt" , error ) );
			if( file1.Exists() == 1 )
			{
				rux::XString json;
				json.set_ByRef( file1.ReadTextAll( error ) );	

				char* ptr = json()->_array_ptr;
				ptr++;
				char* ptr0 = strpbrk( ptr , "{}" );

				XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error ) );
				file1.Close();
			}
			


			/*rux::XString utf8( " { "
							"   \"common\": { "
							"     \"key1\": \"host\", "
							"     \"key2\": \"camera\", "
							"     \"key3\":\"profile\", "
							"     \"enable\": true, "
							"     \"_typeJS\":\"logic\" "
							"  }, "
							"  \"protection\": { "
							"    \"parking\": { "
							"				\"detector_timeout_in_sec\": { "
							"					\"_level\": 0, "
							"					\"_value\": 5, "		
							"        \"_minvalue\":1, "
							"        \"_maxvalue\":3600 "
							"				}, "
							"				\"detector_sms_message\": { "
							"					\"_level\": 1, "
							"					\"_value\": \"ВНИМАНИЕ!!!$name $lastname $surname,обнаружено движение в охраняемой зоне.\" "								
							"				}, "
							"				\"detector_mms_message\": { "
							"					\"_level\": 1, "
							"					\"_value\": \"ВНИМАНИЕ!!!$name $lastname $surname,обнаружено движение в охраняемой зоне.\" "								
							"				}, "
							"				\"detector_mms_subject\": { "
							"					\"_level\": 1, "
							"					\"_value\": \"Обнаружено движение в охраняемой зоне.\" "								
							"				} "
							"			} "
							"		} "
							"	}" , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ );*/
			
			//XJsonSerializer::Deserialize( utf8 , error );
			XJSEvent js_event;
			js_event.set_ByRef( ::rux::data::XJsonSerializer::Deserialize< XJSEvent >( "{ \"name\": \"\\П\\u041F\\u0440GONE\\u0438\\u0432\\u0435\\u0442\\\\nHello!!!\",\"obj\":{\"name\":1986},\"rows\":[{\"name\":1985}, {\"name\":1987}]}" , error ) );

			::rux::data::XJsonSerializer::Serialize( js_event , 1 , error );

			XObject json_object( rux::data::XJsonSerializer::Deserialize( "{ \"name\": \"\\П\\u041F\\u0440GONE\\u0438\\u0432\\u0435\\u0442\\\\nHello!!!\""
				",\"double\" : 1986.78 , \"int\" : 123456789 , \"NULL\" :null , \"boolean\" :true"
				", \"boolean1\" : false , \"array\" : [ {\"hash\" : 777 },1987.777,987654321,-8765"
				",\"Gamoskin\", true,false ,null],\"ip\":\"\\\\\",\"obj\":{\"name\":1986}}" , error ) );
			declare_variable_param( rux::data::XDataObject , obj , json_object );
			XObject name_obj( obj[ "obj->name" ] );
			obj.SetValue( "person->data->surname" , rux::XString( "Gamoskin" ) );
			obj.SetValue( "person->data->surname" , rux::XString( "Ivanov" ) );
			obj.AddValueToArray( "person->data->serials" , rux::XString( "Heroes" ) );
			obj.AddValueToArray( "person->data->serials" , rux::XString( "Dexter" ) );
			rux::XString json = rux::data::XJsonSerializer::Serialize( json_object , 1 , error );
			printf( "json = '%s'\n" , json()->_array_ptr );

			rux::data::XJsonSerializer::Deserialize( "{}"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "{ }"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "[]"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "[ ]"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "{\"name\"}"
				  , error );				
			rux::data::XJsonSerializer::Deserialize( "{\"name\":{\"name\":[]}"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "{\"instruction\": \"exec\", \"module\": \"search\", \"parameters\": }"
				  , error );				

			rux::data::XJsonSerializer::Deserialize( "{ \"hash\":   -2770454900348306927, "
				"\"hash0\":  -4905781796382055513, "
				"\"hash+1\": 5426059845394428163, "
				"\"hash-1\": 6608275161513788181, "
				"\"uhash\":  15676289173361244689, "
				"\"uhash0\": 13540962277327496103, "
				"\"uhash+1\":5426059845394428163, "
				"\"uhash-1\":6608275161513788181"
				"}" , error );				
			json_object = rux::data::XJsonSerializer::Deserialize( "{ \"name\":\"\\u041F\\u0440GONE\\u0438\\u0432\\u0435\\u0442\"}" , error );
			if( error.Length() == 0 
				&& rux_is_object( json_object , XDataObject ) )
			{
				declare_variable_param( rux::data::XDataObject , object , json_object );
				rux::XString name;
				uint8 found = 0;
				name.set_ByRef( object.GetValue< rux::XString >( "name" , found ) );
				name.set_ByRef( name.ConvertToAnsi() );
				XConsole::Print( stdout , XWhite , name );
			}
			XFile file( XFile::Open( "../addons/json_test.txt" , error ) );
			if( file.Exists() == 1 )
			{
				rux::XString json;
				json.set_ByRef( file.ReadTextAll( error ) );			
				XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error ) );
				file.Close();
			}
		}
	}
	rux::deinitialize();
	return 0;
};
