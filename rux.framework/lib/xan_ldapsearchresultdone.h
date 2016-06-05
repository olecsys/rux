#ifndef XAN_LDAPRESULTDONEPACKET_H
#define XAN_LDAPRESULTDONEPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_asn_defines.h>
#include <xan_string.h>
enum XEnum_LdapResultCode
{
	XEnum_LdapResultCode_success = 0 ,
	XEnum_LdapResultCode_operationsError = 1 ,
	XEnum_LdapResultCode_protocolError = 2 ,
	XEnum_LdapResultCode_timeLimitExceeded = 3 ,
	XEnum_LdapResultCode_sizeLimitExceeded = 4 ,
	XEnum_LdapResultCode_compareFalse = 5 ,
	XEnum_LdapResultCode_compareTrue = 6 ,
	XEnum_LdapResultCode_authMethodNotSupported = 7 ,
	XEnum_LdapResultCode_strongerAuthRequired = 8 ,
	XEnum_LdapResultCode_referral = 10 ,
	XEnum_LdapResultCode_adminLimitExceeded = 11 ,
	XEnum_LdapResultCode_unavailableCriticalExtension = 12 ,
	XEnum_LdapResultCode_confidentialityRequired = 13 ,
	XEnum_LdapResultCode_saslBindInProgress = 14 ,
	XEnum_LdapResultCode_noSuchAttribute = 16 ,
	XEnum_LdapResultCode_undefinedAttributeType = 17 ,
	XEnum_LdapResultCode_inappropriateMatching = 18 ,
	XEnum_LdapResultCode_constraintViolation = 19 ,
	XEnum_LdapResultCode_attributeOrValueExists = 20 ,
	XEnum_LdapResultCode_invalidAttributeSyntax = 21 ,
	XEnum_LdapResultCode_noSuchObject = 32 ,
	XEnum_LdapResultCode_aliasProblem = 33 ,
	XEnum_LdapResultCode_invalidDNSyntax = 34 ,
	XEnum_LdapResultCode_aliasDereferencingProblem = 36 ,
	XEnum_LdapResultCode_inappropriateAuthentication = 48 ,
	XEnum_LdapResultCode_invalidCredentials = 49 ,
	XEnum_LdapResultCode_insufficientAccessRights = 50 ,
	XEnum_LdapResultCode_busy = 51 ,
	XEnum_LdapResultCode_unavailable = 52 ,
	XEnum_LdapResultCode_unwillingToPerform = 53 ,
	XEnum_LdapResultCode_loopDetect = 54 ,
	XEnum_LdapResultCode_namingViolation = 64 ,
	XEnum_LdapResultCode_objectClassViolation = 65 ,
	XEnum_LdapResultCode_nonAllowedOnNonLeaf = 66 ,
	XEnum_LdapResultCode_nonAllowedOnRDN = 67 ,
	XEnum_LdapResultCode_entryAlreadyExists = 68 ,
	XEnum_LdapResultCode_objectClassModsProhibited = 69 ,
	XEnum_LdapResultCode_affectsMultipleDSAs = 71 ,
	XEnum_LdapResultCode_other = 80
};
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapSearchResultDone );
			friend class XLdapPacket;
		begin_declare_rux_class_members( LdapSearchResultDone );
			rux::uint32 _message_id;
			XEnum_LdapResultCode _result_code;
			rux::XString _matched_dn;
			rux::XString _error_message;
		end_declare_rux_class_members( LdapSearchResultDone );
			XEnum_LdapResultCode get_ResultCode( void );	
		end_declare_rux_class();
	};
};
#endif
