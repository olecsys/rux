#ifndef XAN_SQLITEPROVIDER_H
#define XAN_SQLITEPROVIDER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <sqlite3.h>
#include <xan_dbschema.h>
#include <xan_loader_defines.h>
namespace rux
{	
	namespace data
	{	
		typedef int (*sqlite3_busy_handler_t)
		(
		 sqlite3*,
		 int(*)(void*,int),
		 void*
		);
		typedef const char* (*sqlite3_errmsg_t)(sqlite3*);
		typedef int (*sqlite3_callback_t)
		(
			void*,
			int,
			char**,
			char**
		);

		typedef int (*sqlite3_open_t)
		(
		 const char *filename,   /* Database filename (UTF-8) */
		 sqlite3 **ppDb          /* OUT: SQLite db handle */
		);

		typedef int (*sqlite3_exec_t)
		(
		 sqlite3*,                                  /* An open database */
		 const char *sql,                           /* SQL to be evaluated */
		 sqlite3_callback_t callback,            /* Callback function */
		 void *,                                    /* 1st argument to callback */
		 char **errmsg                              /* Error msg written here */
		);
		typedef int (*sqlite3_close_t)
		(
		 sqlite3*
		);
		typedef void (*sqlite3_free_t)
		(
		 void*
		);
		typedef int (*sqlite3_threadsafe_t)
		(
		 void
		);
		typedef int (*sqlite3_open_v2_t)
		(
		  const char *filename,   /* Database filename (UTF-8) */
		  sqlite3 **ppDb,         /* OUT: SQLite db handle */
		  int flags,              /* Flags */
		  const char *zVfs        /* Name of VFS module to use */
		);
		typedef int (*sqlite3_bind_blob_t)(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
		typedef int (*sqlite3_bind_double_t)(sqlite3_stmt*, int, double);
		typedef int (*sqlite3_bind_int_t)(sqlite3_stmt*, int, int);
		typedef int (*sqlite3_bind_int64_t)(sqlite3_stmt*, int, sqlite3_int64);
		typedef int (*sqlite3_bind_null_t)(sqlite3_stmt*, int);
		typedef int (*sqlite3_bind_text_t)(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
		typedef int (*sqlite3_bind_text16_t)(sqlite3_stmt*, int, const void*, int, void(*)(void*));
		typedef int (*sqlite3_bind_value_t)(sqlite3_stmt*, int, const sqlite3_value*);
		typedef int (*sqlite3_bind_zeroblob_t)(sqlite3_stmt*, int, int n);
		typedef int (*sqlite3_prepare_v2_t)(
			sqlite3 *db,            /* Database handle */
		  const char *zSql,       /* SQL statement, UTF-8 encoded */
		  int nByte,              /* Maximum length of zSql in bytes. */
		  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
		  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
		);
		typedef int (*sqlite3_step_t)(sqlite3_stmt*);
		typedef int (*sqlite3_finalize_t)(sqlite3_stmt *pStmt);
		typedef const char * (*sqlite3_column_name_t)(sqlite3_stmt*, int N);
		typedef int (*sqlite3_column_count_t)(sqlite3_stmt *pStmt);
		typedef const void * (*sqlite3_column_blob_t)(sqlite3_stmt*, int iCol);
		typedef int (*sqlite3_column_bytes_t)(sqlite3_stmt*, int iCol);
		typedef int (*sqlite3_column_bytes16_t)(sqlite3_stmt*, int iCol);
		typedef double (*sqlite3_column_double_t)(sqlite3_stmt*, int iCol);
		typedef int (*sqlite3_column_int_t)(sqlite3_stmt*, int iCol);
		typedef sqlite3_int64 (*sqlite3_column_int64_t)(sqlite3_stmt*, int iCol);
		typedef const unsigned char * (*sqlite3_column_text_t)(sqlite3_stmt*, int iCol);
		typedef const void * (*sqlite3_column_text16_t)(sqlite3_stmt*, int iCol);
		typedef int (*sqlite3_column_type_t)(sqlite3_stmt*, int iCol);

		begin_declare_rux_class( SqliteProvider );
			~SqliteProvider();
		begin_declare_rux_class_members( SqliteProvider );
			rux::XString _connection;
			rux::XModule _sqlite3_module;
			DBSchema& get_Schema( rux::XString& error );
			void CreateDatabase( rux::XString& error );
			void Execute( const rux::XString& sql , sqlite3_callback_t sqlite3_callback , void* user_data , rux::XString& error );
		end_declare_rux_class_members( SqliteProvider );
			XSqliteProvider( const rux::XString& connection );
			rux::data::DBSchema& get_Schema( rux::XString& error );
			void CreateDatabase( rux::XString& error );
			void ExecuteNonQuery( const rux::XString& sql , rux::XString& error );
		private:
			void Execute( const rux::XString& sql , sqlite3_callback_t sqlite3_callback , void* user_data , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif