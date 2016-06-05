#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_smb2client.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
//implement_rux_base_methods_with_namespace( IcmpClient , rux::network );
namespace rux
{
	namespace network
	{		
		smb2_packet_header::smb2_packet_header()
		{
			_ProtocolId[ 0 ] = 0xFE;
			_ProtocolId[ 1 ] = 'S';
			_ProtocolId[ 2 ] = 'M';
			_ProtocolId[ 3 ] = 'B';
			_StructureSize = 64;
			_CreditCharge = 0;
			memset( &_ChannelSequence_and_Reserved_or_Status , 0 , 4 );
			_CreditRequest_or_CreditResponse = 0;
			_Flags = 0;
			_NextCommand = 0;
			_MessageId = 0;
			_Reserved = 0;
			_TreeId = 0;
			_SessionId = 0;
			memset( &_Signature , 0 , 16 );
		};
		smb2_session_setup_request::smb2_session_setup_request()
		{
			_smb2_packet_header._Command = XEnum_SMB2_Command_SESSION_SETUP;
			_StructureSize = 25;
			_Flags = 0;
			_SecurityMode = SMB2_NEGOTIATE_SIGNING_ENABLED;
			_Capabilities = SMB2_GLOBAL_CAP_DFS;
			_Channel = 0;
			_SecurityBufferOffset = sizeof( _smb2_packet_header ) + 24;
			_SecurityBufferLength = 0;
			_PreviousSessionId = 0;
		};
		smb2_negotiate_request::smb2_negotiate_request()
		{
			_smb2_packet_header._Command = XEnum_SMB2_Command_NEGOTIATE;
			//_smb2_packet_header._CreditRequest_or_CreditResponse = ( max( SendPayloadSize , Expected ResponsePayloadSize ) – 1) / 65536 + 1
			_StructureSize = 36;
			_DialectCount = 1;
			_SecurityMode = SMB2_NEGOTIATE_SIGNING_ENABLED;
			_Reserved = 0;
			_Capabilities = 0;
			memset( &_ClientGuid , 0 , 16 );
			_ClientStartTime = 0;
			_Dialects = SMB_2_002_dialect;
		};
	};
};
