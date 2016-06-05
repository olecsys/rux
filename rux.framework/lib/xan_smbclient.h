#ifndef XAN_SMBCLIENT_H
#define XAN_SMBCLIENT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tcpclient.h>
#include <xan_data.h>
namespace rux
{
	namespace network
	{
		class ntlm
		{
		public:
			static size_t generate_negotiate_message( rux::uint8* dst , size_t dst_size , rux::uint8& is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
			static size_t generate_authenticate_message( const rux::XString& username , const rux::XString& password , char challenge[ 8 ] , rux::uint8* dst , size_t dst_size , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0 , char client_challenge[ 8 ] = NULL );
			static rux::uint8 parse_challenge_message( rux::uint8* buffer , size_t size , size_t& processed_size );
			static void LmChallengeResponse( char challenge[ 8 ] , const rux::XString& password , char response[ 24 ] , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0 , char client_challenge[ 8 ] = NULL );
			static void LmPasswordHash( const rux::XString& password , char password_hash[ 16 ] );
			static void DesHash( char clear[ 7 ] , char cypher[ 8 ] );
			static void DesEncrypt( const char clear[ 8 ] , char key[ 7 ] , char cypher[ 8 ] );
			static void ChallengeResponse( char challenge[ 8 ] , char password_hash[ 16 ] , char response[ 24 ] );
			static void NtChallengeResponse( char challenge[ 8 ] , const rux::XString& password , char response[ 24 ] , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0 , char client_challenge[ 8 ] = NULL );
			static void NtPasswordHash( const rux::XString& password , char password_hash[ 16 ] );
			static void NTOWFv1( const rux::XString& password , rux::uint8 hash[ 16 ] );
			static void LMOWFv1( const rux::XString& password , rux::uint8 hash[ 16 ] );
			static void DESL( char challenge[ 8 ] , char password_hash[ 16 ] , char response[ 24 ] );
		};
		begin_struct_pack( 1 )
		struct ntlm_header
		{
            char _Signature[ 8 ];
            rux::uint32 _MessageType;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct ntlm_field_header
		{
            rux::uint16 _length;
			rux::uint16 _maximum_length;
			rux::uint32 _buffer_offset;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct ntlm_version
		{
            rux::uint8 _product_major_version;
			rux::uint8 _product_minor_version;
			rux::uint16 _product_build;
			rux::uint8 _reserved[ 3 ];
			rux::uint8 _ntlm_revision_current;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct ntlm_authenticate_message : public ntlm_header
		{
            ntlm_field_header _lm_challenge_response_header;
			ntlm_field_header _nt_challenge_response_header;
			ntlm_field_header _domain_name_header;
			ntlm_field_header _user_name_header;
			ntlm_field_header _workstation_header;
			ntlm_field_header _session_key_header;
			rux::uint32 _authenticate_flags;
			ntlm_version _version;
			rux::uint8 _data[ 1024 ];
			size_t _ntlm_authenticate_message_size;
			ntlm_authenticate_message( char challenge[ 8 ] , const rux::XString& domain_name , const rux::XString& user_name , const rux::XString& workstation , const rux::XString& password , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0 , char client_challenge[ 8 ] = NULL );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct ntlm_challenge_message : public ntlm_header
		{
            ntlm_field_header _target_name_header;
			rux::uint32 _challenge_flags;
			rux::uint8 _challenge[ 8 ];
			rux::uint8 _reserved[ 8 ];
			ntlm_field_header _target_info_fields_header;
			ntlm_version _version;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct ntlm_negotiate_message : public ntlm_header
		{            
			rux::uint32 _NegotiateFlags;
			uint64 _WorkstationDomainHeader;
			uint64 _WorkstationNameHeader;
			ntlm_version _version;
			ntlm_negotiate_message( rux::uint8& is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
		}
		end_struct_pack( 1 )
		const rux::uint32 NTLM_CHALLENGE_MESSAGE = 0x00000002;
		const rux::uint32 NTLM_AUTHENTICATE_MESSAGE = 0x00000003;
		begin_struct_pack( 1 )
		struct packet
		{
			virtual size_t size( void );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct netbios_session_service_packet_header
		{
			rux::uint8 _MessageType;
			rux::uint8 _Length[ 3 ];
			netbios_session_service_packet_header();
			netbios_session_service_packet_header( rux::uint32 size );
			void size( rux::uint32 size );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_packet_header
		{
			netbios_session_service_packet_header _netbios_session_service_packet_header;
			rux::uint8  _Protocol[ 4 ];
			rux::uint8  _Command;
			rux::uint32 _Status;
			rux::uint8  _Flags;
			rux::uint16 _Flags2;
			rux::uint16 _PIDHigh;
			rux::uint8  _SecurityFeatures[ 8 ];
			rux::uint16 _Reserved;
			rux::uint16 _TID;
			rux::uint16 _PIDLow;
			rux::uint16 _UID;
			rux::uint16 _MID;
			smb_packet_header();
			smb_packet_header( rux::uint32 size );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_dialect
		{
			rux::uint8 _BufferFormat;
			char _DialectString[ 11 ];//NT LM 0.12
			smb_dialect();
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_negotiate_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint16 _ByteCount;
			smb_dialect _smb_dialect;
			smb_com_negotiate_request();
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_negotiate_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint16 _DialectIndex;
			rux::uint8 _SecurityMode;
			rux::uint16 _MaxMpxCount;
			rux::uint16 _MaxNumberVcs;
			rux::uint32 _MaxBufferSize;
			rux::uint32 _MaxRawSize;
			rux::uint32 _SessionKey;
			rux::uint32 _Capabilities;
			uint64 _SystemTime;
			int16 _ServerTimeZone;
			rux::uint8 _ChallengeLength;
			rux::uint16 _ByteCount;
			//UCHAR      Challenge[];
			//SMB_STRING DomainName[];
			//SMB_STRING ServerName[];
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_negotiate_response_extended
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint16 _DialectIndex;
			rux::uint8 _SecurityMode;
			rux::uint16 _MaxMpxCount;
			rux::uint16 _MaxNumberVcs;
			rux::uint32 _MaxBufferSize;
			rux::uint32 _MaxRawSize;
			rux::uint32 _SessionKey;
			rux::uint32 _Capabilities;
			uint64 _SystemTime;
			int16 _ServerTimeZone;
			rux::uint8 _ChallengeLength;
			rux::uint16 _ByteCount;
			rux::uint8 _ServerGUID[ 16 ];
			//UCHAR  SecurityBlob[];
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_session_setup_andx_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _MaxBufferSize;
			rux::uint16 _MaxMpxCount;
			rux::uint16 _VcNumber;
			rux::uint32 _SessionKey;
			rux::uint16 _SecurityBlobLength;
			rux::uint32 _Reserved;
			rux::uint32 _Capabilities;
			rux::uint16 _ByteCount;
			rux::uint8 _Bytes[ 1024 ];
			rux::uint8 _Pad[ 1 ];
			char _NativeOS[ 66 ];
			char _NativeLanMan[ 34 ];
			char _PrimaryDomain[ 2 ];
			void* _current;
			size_t _Bytes_size;
			smb_com_session_setup_andx_request( rux::network::smb_com_negotiate_response_extended* smb_com_negotiate_response_ptr );
			void* next_to_send( size_t& send_size );
			void refresh( void );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_session_setup_andx_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _Action;
			rux::uint16 _SecurityBlobLength;
			rux::uint16 _ByteCount;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_logoff_andx_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _ByteCount;
			smb_com_logoff_andx_request( rux::uint16 UID );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_tree_connect_andx_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _Flags;
			rux::uint16 _PasswordLength;
			rux::uint16 _ByteCount;
			rux::uint8 _data[ 1024 ];
			size_t _smb_com_tree_connect_andx_request_size;
			//UCHAR      Password[PasswordLength];
			//UCHAR      Pad[];
			//SMB_STRING Path;
			//OEM_STRING Service;
			smb_com_tree_connect_andx_request( rux::uint16 UID , const rux::XString& path , const rux::XString& service );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_tree_connect_andx_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _OptionalSupport;
			rux::uint16 _ByteCount;
			//if SMB_SUPPORT_SEARCH_BITS 
			//then rux::uint16 MaximalShareAc
			//and rux::uint16 GuestMaximalSh
			//OEM_STRING Service;
			//SMB_STRING NativeFileSystem;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_write_andx_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _FID;
			rux::uint32 _Offset;
			rux::uint32 _Timeout;
			rux::uint16 _WriteMode;
			rux::uint16 _Remaining;
			rux::uint16 _Reserved;
			rux::uint16 _DataLength;
			rux::uint16 _DataOffset;
			rux::uint32 _OffsetHigh;
			rux::uint16 _ByteCount;
			rux::uint8 _Pad;
			size_t _size;
			smb_com_write_andx_request( rux::uint16 UID , rux::uint16 TID , rux::uint16 FID );
			void data( XData* dst , rux::uint8* data , uint64 offset , size_t size );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_write_andx_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint16 _Count;
			rux::uint16 _Available;
			rux::uint32 _Reserved;
			rux::uint16 _ByteCount;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_nt_create_andx_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint8 _Reserved;
			rux::uint16 _NameLength;
			rux::uint32 _Flags;
			rux::uint32 _RootDirectoryFID;
			rux::uint32 _DesiredAccess;
			uint64 _AllocationSize;
			rux::uint32 _ExtFileAttributes;
			rux::uint32 _ShareAccess;
			rux::uint32 _CreateDisposition;
			rux::uint32 _CreateOptions;
			rux::uint32 _ImpersonationLevel;
			rux::uint8 _SecurityFlags;
			rux::uint16 _ByteCount;
			char _FileName[ 1024 ];
			size_t _smb_com_nt_create_andx_request_size;
			smb_com_nt_create_andx_request( rux::uint16 UID , rux::uint16 TID , const rux::XString& path );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_nt_create_andx_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint8 _AndXCommand;
			rux::uint8 _AndXReserved;
			rux::uint16 _AndXOffset;
			rux::uint8 _OpLockLevel; 
			rux::uint16 _FID; 
			rux::uint32 _CreateDisposition; 
			uint64 _CreateTime; 
			uint64 _LastAccessTime; 
			uint64 _LastWriteTime; 
			uint64 _LastChangeTime; 
			rux::uint32 _ExtFileAttributes; 
			uint64 _AllocationSize; 
			uint64 _EndOfFile; 
			rux::uint16 _ResourceType; 
			rux::uint16 _NMPipeStatus; 
			rux::uint8 _Directory;
			rux::uint16 _ByteCount;
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_close_request
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint16 _FID;
			rux::uint32 _LastTimeModified;
			rux::uint16 _ByteCount;
			smb_com_close_request( rux::uint16 UID , rux::uint16 TID , rux::uint16 FID , rux::uint32 LastTimeModified );
		}
		end_struct_pack( 1 )
		begin_struct_pack( 1 )
		struct smb_com_close_response
		{
			smb_packet_header _smb_packet_header;
			rux::uint8 _WordCount;
			rux::uint16 _ByteCount;
		}
		end_struct_pack( 1 )
		const rux::uint16 SMB_SUPPORT_SEARCH_BITS = 0x0001;
		const rux::uint16 SMB_SHARE_IS_IN_DFS = 0x0002;

		const rux::uint16 WritethroughMode = 0x0001;//If set the server MUST NOT respond to the client before the data is written to disk (write-through).
		const rux::uint16 ReadBytesAvailable = 0x0002;//If set the server SHOULD set the Response.SMB_Parameters.Available field correctly for writes to named pipes or I/O devices.
		const rux::uint16 RAW_MODE = 0x0004;//Applicable to named pipes only. If set, the named pipe MUST be written to in raw mode (no translation).
		const rux::uint16 MSG_START = 0x0008;//Applicable to named pipes only. If set, this data is the start of a message.

		const rux::uint16 FileTypeDisk = 0x0000;//File or directory
		const rux::uint16 FileTypeByteModePipe = 0x0001;//Byte mode named pipe
		const rux::uint16 FileTypeMessageModePipe = 0x0002;//Message mode named pipe
		const rux::uint16 FileTypePrinter = 0x0003;//Printer device
		const rux::uint16 FileTypeCommDevice = 0x0004;//Character mode device. When an extended protocol has been negotiated, this value allows a device to be opened for driver-level I/O. This provides direct access to devices such as modems, scanners, and so forth.

		const rux::uint16 ICount = 0x000FF;//An 8-bit unsigned integer that gives the maximum number of instances the named pipe can have.
		const rux::uint16 ReadMode = 0x0300;//0
//This bit field indicates the client read mode for the named pipe. This bit field has no effect on writes to the named pipe. A value of zero indicates that the named pipe was opened in or set to byte mode by the client.
//1
//A value of 1 indicates that the client opened or set the named pipe to message mode.
//2,3
//Reserved. Bit 0x0200 MUST be ignored.
		const rux::uint16 NamedPipeType = 0x0C00;//
//0
//This bit field indicates the type of the named pipe when the named pipe was created by the server. A value of zero indicates that the named pipe was created as a byte mode pipe.
//1
//The named pipe was created by the server as a message mode pipe.
//2,3
//Reserved. Bit 0x0800 MUST be ignored.
//0x3000
//Reserved. MUST be ignored.
		const rux::uint16 Endpoint = 0x4000;//
//0
//Client-side end of the named pipe. The SMB server MUST clear the Endpoint bit (set it to zero) when responding to the client request because the CIFS client is a consumer requesting service from the named pipe. When this bit is clear, it indicates that the client is accessing the consumer endpoint.
//1
//Indicates the server end of the pipe.
		const rux::uint16 Nonblocking = 0x8000;//
//0
//A named pipe read or raw read request will wait (block) until sufficient data to satisfy the read request becomes available, or until the request is canceled.
//A named pipe write or raw write request blocks until its data is consumed, if the write request length is greater than zero.
//1
//A read or a raw read request returns all data available to be read from the named pipe, up to the maximum read size set in the request.
//Write operations return after writing data to named pipes without waiting for the data to be consumed.
//Named pipe non-blocking raw writes are not allowed. Raw writes MUST be performed in blocking mode.

		const rux::uint32 NT_CREATE_REQUEST_OPLOCK = 0x00000002;
		const rux::uint32 NT_CREATE_REQUEST_OPBATCH = 0x00000004;
		const rux::uint32 NT_CREATE_OPEN_TARGET_DIR = 0x00000008;

		const rux::uint32 SMB_FILE_READ_DATA = 0x00000001;//Indicates the right to read data from the file.
		const rux::uint32 SMB_FILE_WRITE_DATA = 0x00000002;//Indicates the right to write data into the file beyond the end of the file.
		const rux::uint32 SMB_FILE_APPEND_DATA = 0x00000004;//Indicates the right to append data to the file beyond the end of the file only.
		const rux::uint32 SMB_FILE_READ_EA = 0x00000008;//Indicates the right to read the extended attributes (EAs) of the file.
		const rux::uint32 SMB_FILE_WRITE_EA = 0x00000010;//Indicates the right to write or change the extended attributes (EAs) of the file.
		const rux::uint32 SMB_FILE_EXECUTE = 0x00000020;//Indicates the right to execute the file.
		const rux::uint32 SMB_FILE_READ_ATTRIBUTES = 0x00000080;//Indicates the right to read the attributes of the file.
		const rux::uint32 SMB_FILE_WRITE_ATTRIBUTES = 0x00000100;//Indicates the right to change the attributes of the file.
		const rux::uint32 SMB_DELETE = 0x00010000;//Indicates the right to delete or to rename the file.
		const rux::uint32 SMB_READ_CONTROL = 0x00020000;//Indicates the right to read the security descriptor of the file.
		const rux::uint32 SMB_WRITE_DAC = 0x00040000;//Indicates the right to change the discretionary access control list (DACL) in the security descriptor of the file.
		const rux::uint32 SMB_WRITE_OWNER = 0x00080000;//Indicates the right to change the owner in the security descriptor of the file.
		const rux::uint32 SMB_SYNCHRONIZE = 0x00100000;//SHOULD NOT be used by the sender and MUST be ignored by the receiver.
		const rux::uint32 SMB_ACCESS_SYSTEM_SECURITY = 0x01000000;//Indicates the right to read or change the system access control list (SACL) in the security descriptor for the file. If the SE_SECURITY_NAME privilege is not set in the access token, the server MUST fail the open request and return STATUS_PRIVILEGE_NOT_HELD.
		const rux::uint32 SMB_MAXIMUM_ALLOWED = 0x02000000;//Indicates that the client requests an open to the file with the highest level of access that the client has on this file. If no access is granted for the client on this file, the server MUST fail the open and return a STATUS_ACCESS_DENIED.
		const rux::uint32 SMB_GENERIC_ALL = 0x10000000;//Indicates a request for all of the access flags that are previously listed except MAXIMUM_ALLOWED and ACCESS_SYSTEM_SECURITY.
		const rux::uint32 SMB_GENERIC_EXECUTE = 0x20000000;//Indicates a request for the following combination of access flags listed previously in this table: FILE_READ_ATTRIBUTES, FILE_EXECUTE, SYNCHRONIZE, and READ_CONTROL.
		const rux::uint32 SMB_GENERIC_WRITE = 0x40000000;//Indicates a request for the following combination of access flags listed previously in this table: FILE_WRITE_DATA, FILE_APPEND_DATA, SYNCHRONIZE, FILE_WRITE_ATTRIBUTES, and FILE_WRITE_EA.
		const rux::uint32 SMB_GENERIC_READ = 0x80000000;

		const rux::uint32 SMB_ATTR_READONLY = 0x00000001;//The file is read only. Applications can read the file but cannot write to it or delete it.
		const rux::uint32 SMB_ATTR_HIDDEN = 0x00000002;//The file is hidden. It is not to be included in an ordinary directory listing.
		const rux::uint32 SMB_ATTR_SYSTEM = 0x00000004;//The file is part of or is used exclusively by the operating system.
		const rux::uint32 SMB_ATTR_DIRECTORY = 0x00000010;//The file is a directory.
		const rux::uint32 SMB_ATTR_ARCHIVE = 0x00000020;//The file has not been archived since it was last modified. Applications use this attribute to mark files for backup or removal.
		const rux::uint32 SMB_ATTR_NORMAL = 0x00000080;//The file has no other attributes set. This attribute is valid only if used alone.
		const rux::uint32 SMB_ATTR_TEMPORARY = 0x00000100;//The file is temporary. This is a hint to the cache manager that it does not need to flush the file to backing storage.
		const rux::uint32 SMB_ATTR_COMPRESSED = 0x00000800;//The file or directory is compressed. For a file, this means that all of the data in the file is compressed. For a directory, this means that compression is the default for newly created files and subdirectories.
		const rux::uint32 SMB_POSIX_SEMANTICS = 0x01000000;//Indicates that the file is to be accessed according to POSIX rules. This includes allowing multiple files with names differing only in case, for file systems that support such naming.<15>
		const rux::uint32 SMB_BACKUP_SEMANTICS = 0x02000000;//Indicates that the file is being opened or created for a backup or restore operation. The server SHOULD allow the client to override normal file security checks, provided it has the necessary permission to do so.
		const rux::uint32 SMB_DELETE_ON_CLOSE = 0x04000000;//Requests that the server delete the file immediately after all of its handles have been closed.
		const rux::uint32 SMB_SEQUENTIAL_SCAN = 0x08000000;//Indicates that the file is to be accessed sequentially from beginning to end.<16>
		const rux::uint32 SMB_RANDOM_ACCESS = 0x10000000;//Indicates that the application is designed to access the file randomly. The server can use this flag to optimize file caching.
		const rux::uint32 SMB_NO_BUFFERING = 0x20000000;//Requests that the server open the file with no intermediate buffering or caching; the server might not honor the request. The application MUST meet certain requirements when working with files opened with FILE_FLAG_NO_BUFFERING. File access MUST begin at offsets within the file that are integer multiples of the volume's sector size and MUST be for numbers of bytes that are integer multiples of the volume's sector size. For example, if the sector size is 512 bytes, an application can request reads and writes of 512, 1024, or 2048 bytes, but not of 335, 981, or 7171 bytes.
		const rux::uint32 SMB_WRITE_THROUGH = 0x80000000;//Instructs the operating system to write through any intermediate cache and go directly to the file. The operating system can still cache write operations, but cannot lazily flush them.

		const rux::uint32 SMB_FILE_SHARE_NONE = 0x00000000;//(No bits set.)Prevents the file from being shared.
		const rux::uint32 SMB_FILE_SHARE_READ = 0x00000001;//Other open operations can be performed on the file for read access.
		const rux::uint32 SMB_FILE_SHARE_WRITE = 0x00000002;//Other open operations can be performed on the file for write access.
		const rux::uint32 SMB_FILE_SHARE_DELETE = 0x00000004;//Other open operations can be performed on the file for delete access.

		const rux::uint32 FILE_SUPERSEDE = 0x00000000;//(No bits set.)If the file already exists, it SHOULD be superseded (overwritten). If it does not already exist, then it SHOULD be created.
		const rux::uint32 FILE_OPEN = 0x00000001;//If the file already exists, it SHOULD be opened rather than created. If the file does not already exist, the operation MUST fail.
		const rux::uint32 FILE_CREATE = 0x00000002;//If the file already exists, the operation MUST fail. If the file does not already exist, it SHOULD be created.
		const rux::uint32 FILE_OPEN_IF = 0x00000003;//If the file already exists, it SHOULD be opened. If the file does not already exist, then it SHOULD be created. This value is equivalent to (FILE_OPEN | FILE_CREATE).
		const rux::uint32 FILE_OVERWRITE = 0x00000004;//If the file already exists, it SHOULD be opened and truncated. If the file does not already exist, the operation MUST fail. The client MUST open the file with at least GENERIC_WRITE access for the command to succeed.
		const rux::uint32 FILE_OVERWRITE_IF = 0x00000005;//If the file already exists, it SHOULD be opened and truncated. If the file does not already exist, it SHOULD be created. The client MUST open the file with at least GENERIC_WRITE access.

		const rux::uint32 FILE_DIRECTORY_FILE = 0x00000001;//The file being created or opened is a directory file. With this option, the CreateDisposition field MUST be set to FILE_CREATE, FILE_OPEN, or FILE_OPEN_IF. When this bit field is set, other compatible CreateOptions include only the following: FILE_WRITE_THROUGH, FILE_OPEN_FOR_BACKUP_INTENT, and FILE_OPEN_BY_FILE_ID.
		const rux::uint32 FILE_WRITE_THROUGH = 0x00000002;//Applications that write data to the file MUST actually transfer the data into the file before any write request is considered complete. If FILE_NO_INTERMEDIATE_BUFFERING is set, the server MUST perform as if FILE_WRITE_THROUGH is set in the create request.
		const rux::uint32 FILE_SEQUENTIAL_ONLY = 0x00000004;//This option indicates that access to the file can be sequential. The server can use this information to influence its caching and read-ahead strategy for this file. The file MAY in fact be accessed randomly, but the server can optimize its caching and read-ahead policy for sequential access.
		const rux::uint32 FILE_NO_INTERMEDIATE_BUFFERING = 0x00000008;//The file SHOULD NOT be cached or buffered in an internal buffer by the server. This option is incompatible when the FILE_APPEND_DATA bit field is set in the DesiredAccess field.
		const rux::uint32 FILE_SYNCHRONOUS_IO_ALERT = 0x00000010;//This flag MUST be ignored by the server, and clients SHOULD set this to 0.
		const rux::uint32 FILE_SYNCHRONOUS_IO_NONALERT = 0x00000020;//This flag MUST be ignored by the server, and clients SHOULD set this to 0.
		const rux::uint32 FILE_NON_DIRECTORY_FILE = 0x00000040;//If the file being opened is a directory, the server MUST fail the request with STATUS_FILE_IS_A_DIRECTORY in the Status field of the SMB Header in the server response.
		const rux::uint32 FILE_CREATE_TREE_CONNECTION = 0x00000080;//This option SHOULD NOT be sent by the clients, and this option MUST be ignored by the server.
		const rux::uint32 FILE_COMPLETE_IF_OPLOCKED = 0x00000100;//This option SHOULD NOT be sent by the clients, and this option MUST be ignored by the server.
		const rux::uint32 FILE_NO_EA_KNOWLEDGE = 0x00000200;//The application that initiated the client's request does not support extended attributes (EAs). If the EAs on an existing file being opened indicate that the caller SHOULD support EAs to correctly interpret the file, the server SHOULD fail this request with STATUS_ACCESS_DENIED (ERRDOS/ERRnoaccess) in the Status field of the SMB Header in the server response.
		const rux::uint32 FILE_OPEN_FOR_RECOVERY = 0x00000400;//This option SHOULD NOT be sent by the clients, and this option MUST be ignored if received by the server.
		const rux::uint32 FILE_RANDOM_ACCESS = 0x00000800;//Indicates that access to the file can be random. The server MAY use this information to influence its caching and read-ahead strategy for this file. This is a hint to the server that sequential read-ahead operations might not be appropriate on the file.
		const rux::uint32 FILE_DELETE_ON_CLOSE = 0x00001000;//The file SHOULD be automatically deleted when the last open request on this file is closed. When this option is set, the DesiredAccess field MUST include the DELETE flag. This option is often used for temporary files.
		const rux::uint32 FILE_OPEN_BY_FILE_ID = 0x00002000;//Opens a file based on the FileId. If this option is set, the server MUST fail the request with STATUS_NOT_SUPPORTED in the Status field of the SMB Header in the server response.
		const rux::uint32 FILE_OPEN_FOR_BACKUP_INTENT = 0x00004000;//The file is being opened or created for the purposes of either a backup or a restore operation. Thus, the server can make appropriate checks to ensure that the caller is capable of overriding whatever security checks have been placed on the file to allow a backup or restore operation to occur. The server can check for certain access rights to the file before checking the DesiredAccess field.
		const rux::uint32 FILE_NO_COMPRESSION = 0x00008000;//When a new file is created, the file MUST NOT be compressed, even if it is on a compressed volume. The flag MUST be ignored when opening an existing file.
		const rux::uint32 FILE_RESERVE_OPFILTER = 0x00100000;//This option SHOULD NOT be sent by the clients, and this option MUST be ignored if received by the server.
		const rux::uint32 FILE_OPEN_NO_RECALL = 0x00400000;//In a hierarchical storage management environment, this option requests that the file SHOULD NOT be recalled from tertiary storage such as tape. A file recall can take up to several minutes in a hierarchical storage management environment. The clients can specify this option to avoid such delays.
		const rux::uint32 FILE_OPEN_FOR_FREE_SPACE_QUERY = 0x00800000;//This option SHOULD NOT be sent by the clients, and this option MUST be ignored if received by the server.

		const rux::uint32 SEC_ANONYMOUS = 0x00000000;//The application-requested impersonation level is Anonymous.
		const rux::uint32 SEC_IDENTIFY = 0x00000001;//The application-requested impersonation level is Identification.
		const rux::uint32 SEC_IMPERSONATE = 0x00000002;//The application-requested impersonation level is Impersonation.

		const rux::uint8 SMB_COM_CLOSE = 0x04;
		const rux::uint8 SMB_COM_NEGOTIATE = 0x72;
		const rux::uint8 SMB_COM_SESSION_SETUP_ANDX = 0x73;
		const rux::uint8 SMB_COM_LOGOFF_ANDX = 0x74;
		const rux::uint8 SMB_COM_TREE_CONNECT_ANDX = 0x75;
		const rux::uint8 SMB_COM_NT_CREATE_ANDX = 0xA2;
		const rux::uint8 SMB_COM_WRITE_ANDX = 0x2F;
		
		const rux::uint8 SMB_FLAGS_LOCK_AND_READ_OK = 0x01;
		const rux::uint8 SMB_FLAGS_BUF_AVAIL = 0x02;
		const rux::uint8 SMB_Reserved = 0x04;
		const rux::uint8 SMB_FLAGS_CASE_INSENSITIVE = 0x08;
		const rux::uint8 SMB_FLAGS_CANONICALIZED_PATHS = 0x10;
		const rux::uint8 SMB_FLAGS_OPLOCK = 0x20;
		const rux::uint8 SMB_FLAGS_OPBATCH = 0x40;
		const rux::uint8 SMB_FLAGS_REPLY = 0x80;

		const rux::uint16 SMB_FLAGS2_LONG_NAMES = 0x0001;
		const rux::uint16 SMB_FLAGS2_EAS = 0x0002;
		const rux::uint16 SMB_FLAGS2_SMB_SECURITY_SIGNATURE = 0x0004;
		const rux::uint16 SMB_FLAGS2_IS_LONG_NAME = 0x0040;
		const rux::uint16 SMB_FLAGS2_DFS = 0x1000;
		const rux::uint16 SMB_FLAGS2_PAGING_IO = 0x2000;
		const rux::uint16 SMB_FLAGS2_NT_STATUS = 0x4000;
		const rux::uint16 SMB_FLAGS2_UNICODE = 0x8000;

		const rux::uint16 SMB_FLAGS2_COMPRESSED = 0x0008;
		const rux::uint16 SMB_FLAGS2_SMB_SECURITY_SIGNATURE_REQUIRED = 0x0010;
		const rux::uint16 SMB_FLAGS2_REPARSE_PATH = 0x0400;
		const rux::uint16 SMB_FLAGS2_EXTENDED_SECURITY = 0x0800;

		const rux::uint8 NEGOTIATE_USER_SECURITY = 0x01;
		const rux::uint8 NEGOTIATE_ENCRYPT_PASSWORDS = 0x02;
		const rux::uint8 NEGOTIATE_SECURITY_SIGNATURES_ENABLED = 0x04;
		const rux::uint8 NEGOTIATE_SECURITY_SIGNATURES_REQUIRED = 0x08;
		const rux::uint8 NEGOTIATE_Reserved = 0xF0;

		const rux::uint32 CAP_RAW_MODE = 0x00000001;
		const rux::uint32 CAP_MPX_MODE = 0x00000002;
		const rux::uint32 CAP_UNICODE = 0x00000004;
		const rux::uint32 CAP_LARGE_FILES = 0x00000008;
		const rux::uint32 CAP_NT_SMBS = 0x00000010;
		const rux::uint32 CAP_RPC_REMOTE_APIS = 0x00000020;
		const rux::uint32 CAP_STATUS32 = 0x00000040;
		const rux::uint32 CAP_LEVEL_II_OPLOCKS = 0x00000080;
		const rux::uint32 CAP_LOCK_AND_READ = 0x00000100;
		const rux::uint32 CAP_NT_FIND = 0x00000200;
		const rux::uint32 CAP_BULK_TRANSFER = 0x00000400;
		const rux::uint32 CAP_COMPRESSED_DATA = 0x00000800;
		const rux::uint32 CAP_DFS = 0x00001000;
		const rux::uint32 CAP_QUADWORD_ALIGNED = 0x00002000;
		const rux::uint32 CAP_LARGE_READX = 0x00004000;
		const rux::uint32 CAP_INFOLEVEL_PASSTHRU = 0x00002000;
		const rux::uint32 CAP_LARGE_WRITEX = 0x00008000;
		const rux::uint32 CAP_LWIO = 0x00010000;
		const rux::uint32 CAP_UNIX = 0x00800000;
		const rux::uint32 CAP_DYNAMIC_REAUTH = 0x20000000;
		const rux::uint32 CAP_PERSISTENT_HANDLES = 0x40000000;
		const rux::uint32 CAP_EXTENDED_SECURITY = 0x80000000;
					  	
		const rux::uint16 SMB_SETUP_GUEST = 0x0001;
		const rux::uint16 SMB_SETUP_USE_LANMAN_KEY = 0x0002;

		const rux::uint8 ERRDOS = 0x01;
		const rux::uint16 ERRmoredata = 0x00EA;

		const rux::uint32 SMB_STATUS_SUCCESS = 0x00000000;//The client request is successful.
		const rux::uint32 SMB_STATUS_INVALID_SMB = 0x00010002;//An invalid SMB client request is received by the server.
		const rux::uint32 SMB_STATUS_SMB_BAD_TID = 0x00050002;//The client request received by the server contains an invalid TID value.
		const rux::uint32 SMB_STATUS_SMB_BAD_COMMAND = 0x00160002;//The client request received by the server contains an unknown SMB command code.
		const rux::uint32 SMB_STATUS_SMB_BAD_UID = 0x005B0002;//The client request to the server contains an invalid UID value.
		const rux::uint32 SMB_STATUS_SMB_USE_STANDARD = 0x00FB0002;//The client request received by the server is for a non-standard SMB operation (for example, an SMB_COM_READ_MPX request on a non-disk share ). The client SHOULD send another request with a different SMB command to perform this operation.
		const rux::uint32 SMB_STATUS_BUFFER_OVERFLOW = 0x80000005;//The data was too large to fit into the specified buffer.
		const rux::uint32 SMB_STATUS_NO_MORE_FILES = 0x80000006;//No more files were found that match the file specification.
		const rux::uint32 SMB_STATUS_STOPPED_ON_SYMLINK = 0x8000002D;//The create operation stopped after reaching a symbolic link.
		const rux::uint32 SMB_STATUS_NOT_IMPLEMENTED = 0xC0000002;//The requested operation is not implemented.
		const rux::uint32 SMB_STATUS_INVALID_PARAMETER = 0xC000000D;//The parameter specified in the request is not valid.
		const rux::uint32 SMB_STATUS_NO_SUCH_DEVICE = 0xC000000E;//A device that does not exist was specified.
		const rux::uint32 SMB_STATUS_INVALID_DEVICE_REQUEST = 0xC0000010;//The specified request is not a valid operation for the target device.
		const rux::uint32 SMB_STATUS_MORE_PROCESSING_REQUIRED = 0xC0000016;//If extended security has been negotiated, then this error code can be returned in the SMB_COM_SESSION_SETUP_ANDX response from the server to indicate that additional authentication information is to be exchanged. See section 2.2.4.6 for details.
		const rux::uint32 SMB_STATUS_ACCESS_DENIED = 0xC0000022;//The client did not have the required permission needed for the operation.
		const rux::uint32 SMB_STATUS_BUFFER_TOO_SMALL = 0xC0000023;//The buffer is too small to contain the entry. No information has been written to the buffer.
		const rux::uint32 SMB_STATUS_OBJECT_NAME_NOT_FOUND = 0xC0000034;//The object name is not found.
		const rux::uint32 SMB_STATUS_OBJECT_NAME_COLLISION = 0xC0000035;//The object name already exists.
		const rux::uint32 SMB_STATUS_OBJECT_PATH_NOT_FOUND = 0xC000003A;//The path to the directory specified was not found. This error is also returned on a create request if the operation requires the creation of more than one new directory level for the path specified.
		const rux::uint32 SMB_STATUS_BAD_IMPERSONATION_LEVEL = 0xC00000A5;//A specified impersonation level is invalid. This error is also used to indicate that a required impersonation level was not provided.
		const rux::uint32 SMB_STATUS_IO_TIMEOUT = 0xC00000B5;//The specified I/O operation was not completed before the time-out period expired.
		const rux::uint32 SMB_STATUS_FILE_IS_A_DIRECTORY = 0xC00000BA;//The file that was specified as a target is a directory and the caller specified that it could be anything but a directory.
		const rux::uint32 SMB_STATUS_NOT_SUPPORTED = 0xC00000BB;//The client request is not supported.
		const rux::uint32 SMB_STATUS_NETWORK_NAME_DELETED = 0xC00000C9;//The network name specified by the client has been deleted on the server. This error is returned if the client specifies an incorrect TID or the share on the server represented by the TID was deleted.
		const rux::uint32 SMB_STATUS_USER_SESSION_DELETED = 0xC0000203;//The user session specified by the client has been deleted on the server. This error is returned by the server if the client sends an incorrect UID.
		const rux::uint32 SMB_STATUS_NETWORK_SESSION_EXPIRED = 0xC000035C;//The client's session has expired; therefore, the client MUST re-authenticate to continue accessing remote resources.
		const rux::uint32 SMB_STATUS_SMB_TOO_MANY_UIDS = 0xC000205A;//The client has requested too many UID values from the server or the client already has an SMB session setup with this UID value.
		const rux::uint32 STATUS_PIPE_DISCONNECTED = (0xC00000B0);//Write to a named pipe with no reader.
		const rux::uint32 STATUS_PRINT_QUEUE_FULL = (0xC00000C6);//Print queue is full--too many queued items.
		const rux::uint32 STATUS_NO_SPOOL_SPACE = (0xC00000C7);//Print queue is full--too many queued items.
		const rux::uint32 STATUS_DISK_FULL= (0xC000007F);//The file system is full.
		const rux::uint32 STATUS_MORE_PROCESSING_REQUIRED = 0xC0000016;
		const rux::uint32 STATUS_OK = 0x00000000;
		const rux::uint32 STATUS_OBJECT_PATH_SYNTAX_BAD = (0xC000003B);//The file path syntax is invalid.
		const rux::uint32 STATUS_OS2_TOO_MANY_OPEN_FILES = (0x00040001);//
		const rux::uint32 STATUS_TOO_MANY_OPENED_FILES = (0xC000011F);//Too many open files; no more FIDs available.
		const rux::uint32 STATUS_ACCESS_DENIED = (0xC0000022);//Access denied.
		const rux::uint32 STATUS_FILE_IS_A_DIRECTORY = (0xC00000BA);//Named file is an existing directory and CreateOptions in the request contains FILE_NON_DIRECTORY_FILE.
		const rux::uint32 STATUS_SMB_BAD_FID = (0x00060001);//Invalid FID; RootDirectoryFID is not valid.
		const rux::uint32 STATUS_FILE_LOCK_CONFLICT = (0xC0000054);//A record lock has been taken on the file or the client has attempted to write to a portion of the file that the server detects has been locked.
		const rux::uint32 STATUS_PIPE_BUSY = (0xC00000AE);//Attempted to read from a busy pipe.
		const rux::uint32 STATUS_INSUFF_SERVER_RESOURCES = (0xC0000205);//The server is out of resources.
		const rux::uint32 STATUS_SHARING_VIOLATION = (0xC0000043);//Sharing violation.
		const rux::uint32 STATUS_NOT_SUPPORTED = (0xC00000BB);//This command is not supported by the server.
		const rux::uint32 STATUS_OBJECT_NAME_COLLISION = (0xC0000035);//The file already exists.
		const rux::uint32 STATUS_INVALID_SMB = (0x00010002);//Invalid SMB. Not enough parameter bytes were sent or the path extends beyond the end of the message.
		const rux::uint32 STATUS_SMB_BAD_TID = (0x00050002);//The TID is no longer valid.
		const rux::uint32 STATUS_BAD_DEVICE_TYPE = 0xC00000CB;//Invalid attempt to close an open spool file OR Invalid device - printer request made to a non-printer connection or non-printer request made to a printer connection
		const rux::uint32 STATUS_INVALID_DEVICE_TYPE = (0xC00000CB);//Device type and request are inconsistent.
		const rux::uint32 STATUS_SMB_BAD_UID = (0x005B0002);//The UID supplied is not defined to the session.
		const rux::uint32 STATUS_DATA_ERROR = (0xC000003E);//A problem has occurred in the physical I/O.

		const rux::uint32 NTLMSSP_NEGOTIATE_UNICODE = 0x00000001;
		const rux::uint32 NTLMSSP_NEGOTIATE_OEM = 0x00000002;
		const rux::uint32 NTLMSSP_REQUEST_TARGET = 0x00000004;
		const rux::uint32 NTLMSSP_NEGOTIATE_SIGN = 0x00000010;
		const rux::uint32 NTLMSSP_NEGOTIATE_SEAL = 0x00000020;
		const rux::uint32 NTLMSSP_NEGOTIATE_DATAGRAM = 0x00000040;
		const rux::uint32 NTLMSSP_NEGOTIATE_LM_KEY = 0x00000080;
		const rux::uint32 NTLMSSP_NEGOTIATE_NETWARE = 0x00000100;
		const rux::uint32 NTLMSSP_NEGOTIATE_NTLM = 0x00000200;
		const rux::uint32 NTLMSSP_NEGOTIATE_NT_ONLY = 0x00000400;
		const rux::uint32 NTLMSSP_ANONYMOUS = 0x00000800;
		const rux::uint32 NTLMSSP_NEGOTIATE_OEM_DOMAIN_SUPPLIED = 0x00001000;
		const rux::uint32 NTLMSSP_NEGOTIATE_OEM_WORKSTATION_SUPPLIED = 0x00002000;
		const rux::uint32 NTLMSSP_NEGOTIATE_THIS_IS_LOCAL_CALL = 0x00004000;
		const rux::uint32 NTLMSSP_NEGOTIATE_ALWAYS_SIGN	= 0x00008000;
		const rux::uint32 NTLMSSP_TARGET_TYPE_DOMAIN	= 0x00010000;
		const rux::uint32 NTLMSSP_TARGET_TYPE_SERVER = 0x00020000;
		const rux::uint32 NTLMSSP_TARGET_TYPE_SHARE = 0x00040000;
		const rux::uint32 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0x00080000;
		const rux::uint32 NTLMSSP_NEGOTIATE_IDENTIFY = 0x00100000;
		const rux::uint32 NTLMSSP_REQUEST_NON_NT_SESSION_KEY = 0x00400000;
		const rux::uint32 NTLMSSP_NEGOTIATE_TARGET_INFO = 0x00800000;
		const rux::uint32 NTLMSSP_NEGOTIATE_VERSION = 0x02000000;
		const rux::uint32 NTLMSSP_NEGOTIATE_128 = 0x20000000;
		const rux::uint32 NTLMSSP_NEGOTIATE_KEY_EXCH = 0x40000000;
		const rux::uint32 NTLMSSP_NEGOTIATE_56 = 0x80000000;
#ifndef STATUS_INVALID_HANDLE
		const rux::uint32 STATUS_INVALID_HANDLE = 0xC0000008L;
#endif
#ifndef STATUS_INVALID_PARAMETER
		const rux::uint32 STATUS_INVALID_PARAMETER = 0xC000000DL;
#endif
		begin_declare_rux_class( SmbClient );			
			~SmbClient();
			rux::uint8 Logon( const rux::XString& username , const rux::XString& password , const rux::XString& hostname_or_ip , XString& error );
			rux::uint8 Logoff( void );
			rux::uint8 TreeConnect( const rux::XString& path , rux::uint16& TID , rux::XString& error );
			rux::uint8 CreateFile( const rux::XString& filename , rux::uint16 TID , rux::uint16& FID , rux::XString& error );
			rux::uint8 CloseFile( rux::uint16 TID , rux::uint16 FID , rux::XString& error );
			rux::uint8 WriteFile( rux::uint16 TID , rux::uint16 FID , rux::uint8* data , size_t size , uint64 offset , rux::XString& error );
			static rux::uint8 CopyFile( const rux::XString& src_filename , const rux::XString& dst_path , const rux::XString& username , const rux::XString& password , rux::XString& error ); 
		begin_declare_rux_class_members( SmbClient );
			rux::uint32 _MaxBufferSize;
			rux::uint16 _UID;
			rux::uint8 _is_logon;
			rux::network::XTcpClient _tcp_client;
		end_declare_rux_class_members( SmbClient );
		end_declare_rux_class();
	};
};
#endif
