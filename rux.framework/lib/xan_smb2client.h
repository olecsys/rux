#ifndef XAN_SMB2CLIENT_H
#define XAN_SMB2CLIENT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tcpclient.h>
namespace rux
{
	namespace network
	{
		//begin_struct_pack( 1 )
		struct smb2_packet_header
		{
			rux::uint8 _ProtocolId[ 4 ];
			rux::uint16 _StructureSize;
			rux::uint16 _CreditCharge;
			rux::uint8 _ChannelSequence_and_Reserved_or_Status[ 4 ];
			rux::uint16 _Command;
			rux::uint16 _CreditRequest_or_CreditResponse;
			rux::uint32 _Flags;
			rux::uint32 _NextCommand;
			uint64 _MessageId;
			rux::uint32 _Reserved;
			rux::uint32 _TreeId;
			uint64 _SessionId;
			rux::uint8 _Signature[ 16 ];
			smb2_packet_header();
		};
		//end_struct_pack( 1 )
		//begin_struct_pack( 1 )
		struct smb2_session_setup_request
		{
			smb2_packet_header _smb2_packet_header;
			rux::uint16 _StructureSize;
			rux::uint8 _Flags;
			rux::uint8 _SecurityMode;
			rux::uint32 _Capabilities;
			rux::uint32 _Channel;
			rux::uint16 _SecurityBufferOffset;
			rux::uint16 _SecurityBufferLength;
			uint64 _PreviousSessionId;
			smb2_session_setup_request();
		};
		//end_struct_pack( 1 )
		//begin_struct_pack( 1 )
		struct smb2_negotiate_request
		{
			smb2_packet_header _smb2_packet_header;
			rux::uint16 _StructureSize;
			rux::uint16 _DialectCount;
			rux::uint16 _SecurityMode;
			rux::uint16 _Reserved;
			rux::uint32 _Capabilities;
			rux::uint8 _ClientGuid[ 16 ];
			uint64 _ClientStartTime;
			rux::uint16 _Dialects;
			smb2_negotiate_request();
		};
		//end_struct_pack( 1 )
		const rux::uint16 XEnum_SMB2_Command_NEGOTIATE = 0x0000;
		const rux::uint16 XEnum_SMB2_Command_SESSION_SETUP = 0x0001;
		const rux::uint16 XEnum_SMB2_Command_LOGOFF = 0x0002;
		const rux::uint16 XEnum_SMB2_Command_TREE_CONNECT = 0x0003;
		const rux::uint16 XEnum_SMB2_Command_TREE_DISCONNECT = 0x0004;
		const rux::uint16 XEnum_SMB2_Command_CREATE = 0x0005;
		const rux::uint16 XEnum_SMB2_Command_CLOSE = 0x0006;
		const rux::uint16 XEnum_SMB2_Command_FLUSH = 0x0007;
		const rux::uint16 XEnum_SMB2_Command_READ = 0x0008;
		const rux::uint16 XEnum_SMB2_Command_WRITE = 0x0009;
		const rux::uint16 XEnum_SMB2_Command_LOCK = 0x000A;
		const rux::uint16 XEnum_SMB2_Command_IOCTL = 0x000B;
		const rux::uint16 XEnum_SMB2_Command_CANCEL = 0x000C;
		const rux::uint16 XEnum_SMB2_Command_ECHO = 0x000D;
		const rux::uint16 XEnum_SMB2_Command_QUERY_DIRECTORY = 0x000E;
		const rux::uint16 XEnum_SMB2_Command_CHANGE_NOTIFY = 0x000F;
		const rux::uint16 XEnum_SMB2_Command_QUERY_INFO = 0x0010;
		const rux::uint16 XEnum_SMB2_Command_SET_INFO = 0x0011;
		const rux::uint16 XEnum_SMB2_Command_OPLOCK_BREAK = 0x0012;

		const rux::uint32 SMB2_FLAGS_SERVER_TO_REDIR = 0x00000001;
		const rux::uint32 SMB2_FLAGS_ASYNC_COMMAND = 0x00000002;
		const rux::uint32 SMB2_FLAGS_RELATED_OPERATIONS = 0x00000004;
		const rux::uint32 SMB2_FLAGS_SIGNED = 0x00000008;
		const rux::uint32 SMB2_FLAGS_DFS_OPERATIONS = 0x10000000;
		const rux::uint32 SMB2_FLAGS_REPLAY_OPERATION = 0x20000000;

		const rux::uint8 SMB2_SESSION_FLAG_BINDING = 0x01;

		const rux::uint8 SMB2_NEGOTIATE_SIGNING_ENABLED = 0x01;
		const rux::uint8 SMB2_NEGOTIATE_SIGNING_REQUIRED = 0x02;

		const rux::uint32 SMB2_GLOBAL_CAP_DFS = 0x00000001;
		const rux::uint32 SMB2_GLOBAL_CAP_UNUSED1 = 0x00000002;
		const rux::uint32 SMB2_GLOBAL_CAP_UNUSED2 = 0x00000004;
		const rux::uint32 SMB2_GLOBAL_CAP_UNUSED3 = 0x00000008;
		const rux::uint32 SMB2_GLOBAL_CAP_LEASING = 0x00000002;
		const rux::uint32 SMB2_GLOBAL_CAP_LARGE_MTU = 0x00000004;
		const rux::uint32 SMB2_GLOBAL_CAP_MULTI_CHANNEL = 0x00000008;
		const rux::uint32 SMB2_GLOBAL_CAP_PERSISTENT_HANDLES = 0x00000010;
		const rux::uint32 SMB2_GLOBAL_CAP_DIRECTORY_LEASING = 0x00000020;
		const rux::uint32 SMB2_GLOBAL_CAP_ENCRYPTION = 0x00000040;

		const rux::uint16 SMB_2_002_dialect = 0x0202;
		const rux::uint16 SMB_2_1_dialect = 0x0210;
		const rux::uint16 SMB_3_0_dialect = 0x0300;
		const rux::uint16 SMB_3_02_dialect = 0x0302;
	};
};
#endif
