#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_certificatex509v3.h>
#include <xan_asn_defines.h>
#include <xan_rsapublickey.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( CertificateX509v3 , rux::cryptography );
	_version = -1;	
	_tag0_length = 0;
end_implement_rux_class();
namespace rux
{
	namespace cryptography
	{
		implement_duplicate_internal_result_function( Object& , CertificateX509v3 , get_EncryptionPublicKey );
		CertificateX509v3& XCertificateX509v3::Parse( rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::XString& error )
		{
			error.Clear();
			XCertificateX509v3 certificate_x509_v3;
			if( data_ptr )
			{
				if( data_ptr_length >= 2 )
				{
					rux::XString oid;
					size_t position = 0;
					rux::uint8 decrementer = 0 , incrementer = 0 , is_set_8bit = 0 , is_set_7bit = 0 , length_of_length = 0;
					size_t total_asn_length = 0;			
					XEnum_AsnClass asn_class = XEnum_AsnClass_Universal;
					XEnum_AsnUniversalClassTags asn_universal_class_tags = XEnum_AsnUniversalClassTags_SEQUENCE;
					rux::uint8 is_constructed = 0;
					::rux::uint32 value = 0;
					size_t multiplicator = 0 , asn_length0 = 0 , index1 = 0 , asn_length1 = 0 , asn_length2 = 0 , index2 = 0;
					rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
						position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , total_asn_length , error );
					if( error.Length() == 0 )
					{
						if( total_asn_length + position == data_ptr_length )
						{
							if( asn_class == XEnum_AsnClass_Universal )
							{
								if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
								{
									if( is_constructed == 1 )
									{
										size_t tbs_certificate_asn_length = 0;
										rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
											position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , tbs_certificate_asn_length , error );
										if( error.Length() == 0 )
										{
											if( tbs_certificate_asn_length + position < data_ptr_length )
											{
												total_asn_length -= tbs_certificate_asn_length + length_of_length + 2;
												if( asn_class == XEnum_AsnClass_Universal )
												{
													if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
													{
														if( is_constructed == 1 )
														{												
															size_t asn_length = 0;
															while( tbs_certificate_asn_length > 0 )
															{
																rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																	position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length , error );
																if( error.Length() == 0 )
																{															
																	if( asn_length + length_of_length + 2 <= tbs_certificate_asn_length )
																	{
																		tbs_certificate_asn_length -= asn_length + length_of_length + 2;
																		if( asn_class == XEnum_AsnClass_Universal )
																		{
																			if( is_constructed == 1 )
																			{
																				if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																				{
																					if( certificate_x509_v3()->_ca_signature_algorithm.Length() == 0 )
																					{
																						rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																							position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																						if( error.Length() == 0 )
																						{
																							if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= asn_length )																				
																							{
																								if( asn_class == XEnum_AsnClass_Universal )
																								{
																									if( is_constructed == 0 )
																									{
																										if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
																										{
																											rux_convert_asn_objectidentifier_to_string( data_ptr , position , asn_length0 , index1 , 
																												value , multiplicator , certificate_x509_v3()->_ca_signature_algorithm , error );
																											position += index1;
																											if( error.Length() == 0 )
																											{
																												rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																													position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																												if( error.Length() == 0 )
																												{
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_NULL )
																															{
																															}
																															else
																															{
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																break;
																															}
																														}
																														else
																														{
																															error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																															break;
																														}
																													}
																													else
																													{
																														error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																														break;
																													}																					
																												}
																												else 
																													break;
																											}
																										}
																										else
																										{
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																											break;
																										}
																									}
																									else
																									{
																										error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																										break;
																									}
																								}
																								else
																								{
																									error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																									break;
																								}
																							}
																							else
																							{
																								error = "CA Signature Algorithm ASN length has incorrect value";
																								break;
																							}
																						}
																						else
																							break;
																					}
																			else if( certificate_x509_v3()->_issuer.Count() == 0 )
																					{
																						index1 = asn_length;
																						while( index1 > 0 )
																						{
																							rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																								position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																							if( error.Length() == 0 )
																							{																						
																								if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= index1 )																				
																								{
																									index1 -= length_of_length + 2;
																									if( asn_class == XEnum_AsnClass_Universal )
																									{
																										if( is_constructed == 1 )
																										{
																											if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SET )
																											{
																												rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																													position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length1 , error );
																												if( error.Length() == 0 )
																												{
																													if( asn_length1 > 0 && asn_length1 + length_of_length + 2 <= asn_length0 )																				
																													{																												
																														if( asn_class == XEnum_AsnClass_Universal )
																														{
																															if( is_constructed == 1 )
																															{
																																if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																																{
																																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																	if( error.Length() == 0 )
																																	{
																																		if( asn_length2 > 0 && asn_length2 + length_of_length + 2 <= asn_length1 )																				
																																		{																										
																																			asn_length1 -= asn_length2 + length_of_length + 2;
																																			if( asn_class == XEnum_AsnClass_Universal )
																																			{
																																				if( is_constructed == 0 )
																																				{
																																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
																																					{
																																						rux_convert_asn_objectidentifier_to_string( data_ptr , position , asn_length2 , index2 , value ,
																																							multiplicator , oid , error );
																																						if( error.Length() == 0 )
																																						{
																																							position += index2;
																																							rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																								position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																							if( error.Length() == 0 )
																																							{
																																								if( asn_length2 > 0 && asn_length2 + length_of_length + 2 == asn_length1 )																				
																																								{																																	
																																									if( asn_class == XEnum_AsnClass_Universal )
																																									{
																																										if( is_constructed == 0 )
																																										{
																																											if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_PrintableString )
																																											{
																																												oid += "=" + rux::XString( (char*)&data_ptr[ position ] , 0 , asn_length2 );
																																												certificate_x509_v3()->_issuer.Add( oid );
																																												position += asn_length2;
																																												index1 -= asn_length0;
																																											}
																																											else
																																											{
																																												error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																												break;
																																											}
																																										}
																																										else
																																										{
																																											error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																											break;
																																										}
																																									}
																																									else
																																									{
																																										error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																										break;
																																									}
																																								}
																																								else
																																								{
																																									error = "Issuer Object identifier ASN length has incorrect value";
																																									break;
																																								}
																																							}
																																							else
																																								break;
																																						}
																																						else
																																							break;
																																					}
																																					else
																																					{
																																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																						break;
																																					}
																																				}
																																				else
																																				{
																																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																					break;
																																				}
																																			}
																																			else
																																			{
																																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																				break;
																																			}
																																		}
																																		else
																																		{
																																			error = "Issuer Object identifier ASN length has incorrect value";
																																			break;
																																		}
																																	}
																																	else
																																		break;
																																}
																																else
																																{
																																	error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																	break;
																																}
																															}
																															else
																															{
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																break;
																															}
																														}
																														else
																														{
																															error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																															break;
																														}
																													}
																													else
																													{
																														error = "Issuer Object identifier ASN length has incorrect value";
																														break;
																													}
																												}
																												else
																													break;
																											}
																											else
																											{
																												error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																												break;
																											}
																										}
																										else
																										{
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																											break;
																										}
																									}
																									else
																									{
																										error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																										break;
																									}
																								}
																								else
																								{
																									error = "Issuer ASN length has incorrect value";
																									break;
																								}
																							}
																							else 
																								break;
																							if( error.Length() > 0 )
																								break;
																						}
																					}
																					else if( certificate_x509_v3()->_not_after.Length() == 0 
																						&& certificate_x509_v3()->_not_before.Length() == 0 )
																					{
																						rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																							position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																						if( error.Length() == 0 )
																						{																					
																							if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= asn_length )																				
																							{
																								asn_length -= length_of_length + 2;
																								if( asn_class == XEnum_AsnClass_Universal )
																								{
																									if( is_constructed == 0 )
																									{
																										if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_UTCTime )
																										{
																											certificate_x509_v3()->_not_before = rux::XString( (char*)&data_ptr[ position ] , 0 , asn_length0 );
																											position += asn_length0;
																											asn_length -= asn_length0;
																											rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																												position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																											if( error.Length() == 0 )
																											{																					
																												if( asn_length0 > 0 && asn_length0 + length_of_length + 2 == asn_length )																				
																												{																											
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_UTCTime )
																															{
																																certificate_x509_v3()->_not_after = rux::XString( (char*)&data_ptr[ position ] , 0 , asn_length0 );
																																position += asn_length0;																														
																															}
																															else
																															{
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																break;
																															}
																														}
																														else
																														{
																															error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																															break;
																														}
																													}
																													else
																													{
																														error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																														break;
																													}
																												}
																												else
																												{
																													error = "Validity ASN length has incorrect value";
																													break;
																												}
																											}
																											else 
																												break;				
																										}
																										else
																										{
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																											break;
																										}
																									}
																									else
																									{
																										error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																										break;
																									}
																								}
																								else
																								{
																									error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																									break;
																								}
																							}
																							else
																							{
																								error = "Validity ASN length has incorrect value";
																								break;
																							}
																						}
																						else 
																							break;																							
																					}
																					else if( certificate_x509_v3()->_subject.Count() == 0 )
																					{
																						index1 = asn_length;
																						while( index1 > 0 )
																						{
																							rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																								position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																							if( error.Length() == 0 )
																							{																						
																								if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= index1 )																				
																								{
																									index1 -= length_of_length + 2;
																									if( asn_class == XEnum_AsnClass_Universal )
																									{
																										if( is_constructed == 1 )
																										{
																											if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SET )
																											{
																												rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																													position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length1 , error );
																												if( error.Length() == 0 )
																												{
																													if( asn_length1 > 0 && asn_length1 + length_of_length + 2 <= asn_length0 )																				
																													{																												
																														if( asn_class == XEnum_AsnClass_Universal )
																														{
																															if( is_constructed == 1 )
																															{
																																if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																																{
																																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																	if( error.Length() == 0 )
																																	{
																																		if( asn_length2 > 0 && asn_length2 + length_of_length + 2 <= asn_length1 )																				
																																		{																										
																																			asn_length1 -= asn_length2 + length_of_length + 2;
																																			if( asn_class == XEnum_AsnClass_Universal )
																																			{
																																				if( is_constructed == 0 )
																																				{
																																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
																																					{
																																						rux_convert_asn_objectidentifier_to_string( data_ptr , position , asn_length2 , index2 , value ,
																																							multiplicator , oid , error );
																																						if( error.Length() == 0 )
																																						{
																																							position += index2;
																																							rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																								position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																							if( error.Length() == 0 )
																																							{
																																								if( asn_length2 > 0 && asn_length2 + length_of_length + 2 == asn_length1 )																				
																																								{																																	
																																									if( asn_class == XEnum_AsnClass_Universal )
																																									{
																																										if( is_constructed == 0 )
																																										{
																																											if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_PrintableString )
																																											{
																																												oid += "=" + rux::XString( (char*)&data_ptr[ position ] , 0 , asn_length2 );
																																												certificate_x509_v3()->_subject.Add( oid );
																																												position += asn_length2;
																																												index1 -= asn_length0;
																																											}
																																											else
																																											{
																																												error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																												break;
																																											}
																																										}
																																										else
																																										{
																																											error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																											break;
																																										}
																																									}
																																									else
																																									{
																																										error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																										break;
																																									}
																																								}
																																								else
																																								{
																																									error = "Subject ASN length has incorrect value";
																																									break;
																																								}
																																							}
																																							else
																																								break;
																																						}
																																						else
																																							break;
																																					}
																																					else
																																					{
																																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																						break;
																																					}
																																				}
																																				else
																																				{
																																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																					break;
																																				}
																																			}
																																			else
																																			{
																																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																				break;
																																			}
																																		}
																																		else
																																		{
																																			error = "Subject ASN length has incorrect value";
																																			break;
																																		}
																																	}
																																	else
																																		break;
																																}
																																else
																																{
																																	error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																	break;
																																}
																															}
																															else
																															{
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																break;
																															}
																														}
																														else
																														{
																															error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																															break;
																														}
																													}
																													else
																													{
																														error = "Subject ASN length has incorrect value";
																														break;
																													}
																												}
																												else
																													break;
																											}
																											else
																											{
																												error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																												break;
																											}
																										}
																										else
																										{
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																											break;
																										}
																									}
																									else
																									{
																										error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																										break;
																									}
																								}
																								else
																								{
																									error = "Subject ASN length has incorrect value";
																									break;
																								}
																							}
																							else 
																								break;
																							if( error.Length() > 0 )
																								break;
																						}
																					}
																					else if( certificate_x509_v3()->_encryption_algorithm.Length() == 0 )
																					{
																						rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																							position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																						if( error.Length() == 0 )
																						{
																							if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= asn_length )																				
																							{
																								asn_length -= asn_length0 + length_of_length + 2;
																								if( asn_class == XEnum_AsnClass_Universal )
																								{
																									if( is_constructed == 1 )
																									{
																										if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																										{
																											rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																												position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length1 , error );
																											if( error.Length() == 0 )
																											{
																												if( asn_length1 > 0 && asn_length1 + length_of_length + 2 <= asn_length0 )																				
																												{
																													asn_length0 -= asn_length1 + length_of_length + 2;
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
																															{
																																rux_convert_asn_objectidentifier_to_string( data_ptr , position , asn_length1 , index1 , 
																																	value , multiplicator , certificate_x509_v3()->_encryption_algorithm , error );
																																position += index1;
																																if( error.Length() == 0 )
																																{
																																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length1 , error );
																																	if( error.Length() == 0 )
																																	{
																																		if( asn_length1 + length_of_length + 2 == asn_length0 )
																																		{
																																			asn_length0 -= asn_length1 + length_of_length + 2;
																																			if( asn_class == XEnum_AsnClass_Universal )
																																			{
																																				if( is_constructed == 0 )
																																				{
																																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_NULL )
																																					{
																																					}
																																					else
																																					{
																																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																						break;
																																					}
																																				}
																																				else
																																				{
																																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																					break;
																																				}
																																			}
																																			else
																																			{
																																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																				break;
																																			}																				
																																		}
																																		else
																																		{
																																			error = "Encryption Algorithm ASN length has incorrect value";
																																			break;
																																		}
																																	}
																																	else 
																																		break;
																																}
																																else
																																	break;
																																if( error.Length() == 0 )
																																{
																																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																																	if( error.Length() == 0 )
																																	{
																																		if( asn_length0 > 0 && asn_length0 + length_of_length + 2 == asn_length )																				
																																		{																																	
																																			if( asn_class == XEnum_AsnClass_Universal )
																																			{
																																				if( is_constructed == 0 )
																																				{
																																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_BITSTRING )
																																					{
																																						if( certificate_x509_v3()->_encryption_algorithm == "1.2.840.113549.1.1.1" )
																																						{
																																							if( data_ptr[ position ]  == 0x00 )
																																							{
																																								position++;
																																								rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																									position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length1 , error );
																																								if( error.Length() == 0 )
																																								{
																																									if( asn_length1 > 0 && asn_length1 + length_of_length + 2 <= asn_length0 )																				
																																									{																																	
																																										asn_length0 -= asn_length1 + length_of_length + 3;
																																										if( asn_class == XEnum_AsnClass_Universal )
																																										{
																																											if( is_constructed == 1 )
																																											{
																																												if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																																												{
																																													rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																														position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																													if( error.Length() == 0 )
																																													{
																																														if( asn_length2 > 0 && asn_length2 + length_of_length + 2 <= asn_length1 )																				
																																														{																																	
																																															asn_length1 -= asn_length2 + length_of_length + 2;
																																															if( asn_class == XEnum_AsnClass_Universal )
																																															{
																																																if( is_constructed == 0 )
																																																{
																																																	if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_INTEGER )
																																																	{
																																																		XRsaPublicKey rsa_public_key;
																																																		rsa_public_key.set_ModulusFromReverseOrder( &data_ptr[ position ] , asn_length2 );
																																																		position += asn_length2;
																																																		rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																																																			position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length2 , error );
																																																		if( error.Length() == 0 )
																																																		{
																																																			if( asn_length2 > 0 && asn_length2 + length_of_length + 2 == asn_length1 )																				
																																																			{																																																		
																																																				if( asn_class == XEnum_AsnClass_Universal )
																																																				{
																																																					if( is_constructed == 0 )
																																																					{
																																																						if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_INTEGER )
																																																						{
																																																							rsa_public_key.set_PublicExponentFromReverseOrder( &data_ptr[ position ] , asn_length2 );
																																																							certificate_x509_v3()->_encryption_public_key = rsa_public_key;
																																																							position += asn_length2;
																																																						}
																																																						else
																																																						{
																																																							error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																																							break;
																																																						}
																																																					}
																																																					else
																																																					{
																																																						error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																																						break;
																																																					}
																																																				}
																																																				else
																																																				{
																																																					error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																																					break;
																																																				}																				
																																																			}
																																																			else
																																																			{
																																																				error = "Encryption Algorithm ASN length has incorrect value";
																																																				break;
																																																			}
																																																		}
																																																		else 
																																																			break;
																																																	}
																																																	else
																																																	{
																																																		error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																																		break;
																																																	}
																																																}
																																																else
																																																{
																																																	error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																																	break;
																																																}
																																															}
																																															else
																																															{
																																																error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																																break;
																																															}																				
																																														}
																																														else
																																														{
																																															error = "Encryption Algorithm ASN length has incorrect value";
																																															break;
																																														}
																																													}
																																													else 
																																														break;
																																												}
																																												else
																																												{
																																													error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																													break;
																																												}
																																											}
																																											else
																																											{
																																												error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																												break;
																																											}
																																										}
																																										else
																																										{
																																											error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																											break;
																																										}																				
																																									}
																																									else
																																									{
																																										error = "Encryption Algorithm ASN length has incorrect value";
																																										break;
																																									}
																																								}
																																								else 
																																									break;
																																							}
																																							else
																																							{
																																								error = "Encryption public key contains incorrect first byte";
																																								break;
																																							}
																																						}
																																						else
																																						{
																																							error = "Encryption algorithm '" + certificate_x509_v3()->_encryption_algorithm + "' is not implemented";
																																							break;
																																						}
																																					}
																																					else
																																					{
																																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																						break;
																																					}
																																				}
																																				else
																																				{
																																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																																					break;
																																				}
																																			}
																																			else
																																			{
																																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																																				break;
																																			}																				
																																		}
																																		else
																																		{
																																			error = "Encryption Algorithm ASN length has incorrect value";
																																			break;
																																		}
																																	}
																																	else 
																																		break;
																																}
																															}
																															else
																															{
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																																break;
																															}
																														}
																														else
																														{
																															error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																															break;
																														}
																													}
																													else
																													{
																														error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																														break;
																													}
																												}
																												else
																												{
																													error = "Encryption Algorithm ASN length has incorrect value";
																													break;
																												}
																											}
																											else
																												break;
																										}
																										else
																										{
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																											break;
																										}
																									}
																									else
																									{
																										error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																										break;
																									}																											
																								}
																								else
																								{
																									error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																									break;
																								}
																							}
																							else
																							{
																								error = "Encryption Algorithm ASN length has incorrect value";
																								break;
																							}
																						}
																						else
																							break;
																					}
																					else
																					{
																						error = "Incorrect x.509 certificate buffer,incorrect Tbs Certificate buffer";
																						break;
																					}
																				}
																				else 
																				{
																					error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																					break;
																				}
																			}
																			else
																			{
																				if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_INTEGER )
																				{
																					if( certificate_x509_v3()->_tag0_length > 0 
																						&& certificate_x509_v3()->_version == -1 )
																					{
																						if( asn_length == 1 )
																						{
																							if( data_ptr[ position ] == 0
																								|| data_ptr[ position ] == 1
																								|| data_ptr[ position ] == 2 )																					
																								certificate_x509_v3()->_version = data_ptr[ position ] + 1;
																							else
																							{
																								error = "Incorrect x.509 certificate buffer,incorrect Tbs Certificate Version";
																								break;
																							}
																						}
																						position += asn_length;																				
																					}
																					else if( certificate_x509_v3()->_serial_number.get_Size() == 0 )
																					{
																						if( asn_length > 0 )																				
																							certificate_x509_v3()->_serial_number = rux::XRawData::FromReverseOrder( &data_ptr[ position ] , asn_length );
																						position += asn_length;																				
																					}
																					else
																					{
																						error = "Incorrect x.509 certificate buffer,incorrect Tbs Certificate buffer";
																						break;
																					}
																				}															
																				else 
																				{
																					error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																					break;
																				}
																			}			
																		}
																		else if( asn_class == XEnum_AsnClass_ContextSpecific )
																		{
																			if( is_constructed == 1 )
																			{
																				if( asn_universal_class_tags == 0 )																																	
																					certificate_x509_v3()->_tag0_length = static_cast<rux::uint8>( asn_length );
																				else
																				{
																					error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																					break;
																				}
																			}
																			else
																			{
																				error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																				break;
																			}
																		}
																		else
																		{
																			error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																			break;
																		}
																	}
																	else
																	{
																		error = "Tbs Certificate ASN length is greater or equal to data pointer length";
																		break;
																	}
																}
																else
																	break;
																if( error.Length() > 0 )
																	break;
															}
															if( error.Length() == 0 )
															{
																if( position < data_ptr_length )
																{
																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length , error );
																	if( error.Length() == 0 )
																	{
																		if( asn_length > 0 && asn_length + length_of_length + 2 <= total_asn_length )																				
																		{
																			total_asn_length -= asn_length + length_of_length + 2;
																			if( asn_class == XEnum_AsnClass_Universal )
																			{
																				if( is_constructed == 1 )
																				{
																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																					{
																						rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																							position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																						if( error.Length() == 0 )
																						{
																							if( asn_length0 > 0 && asn_length0 + length_of_length + 2 <= asn_length )																				
																							{
																								asn_length -= asn_length0 + length_of_length + 2;
																								if( asn_class == XEnum_AsnClass_Universal )
																								{
																									if( is_constructed == 0 )
																									{
																										if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
																										{
																											rux_convert_asn_objectidentifier_to_string( data_ptr , position , asn_length0 , index1 , 
																												value , multiplicator , certificate_x509_v3()->_signature_algorithm , error );
																											position += index1;
																											if( error.Length() == 0 )
																											{
																												rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																													position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length0 , error );
																												if( error.Length() == 0 )
																												{
																													if( asn_length0 + length_of_length + 2 == asn_length )
																													{
																														if( asn_class == XEnum_AsnClass_Universal )
																														{
																															if( is_constructed == 0 )
																															{
																																if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_NULL )
																																{
																																}
																																else																														
																																	error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																															}
																															else
																																error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																														}
																														else
																															error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																													}
																													else
																														error = "Signature Algorithm ASN length has incorrect value";
																												}		
																											}
																										}
																										else
																											error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																									}
																									else
																										error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																								}
																								else
																									error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																							}
																							else
																								error = "Signature Algorithm ASN length has incorrect value";
																						}
																					}
																					else
																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																				}
																				else
																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																			}
																			else
																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																		}
																		else
																			error = "Signature Algorithm ASN length has incorrect value";
																	}
																}
															}
															if( error.Length() == 0 )
															{
																if( position < data_ptr_length )
																{
																	rux_parse_asn_header( data_ptr , data_ptr_length , length_of_length , is_set_8bit , is_set_7bit , 
																		position , decrementer , incrementer , asn_class , asn_universal_class_tags , is_constructed , asn_length , error );
																	if( error.Length() == 0 )
																	{
																		if( asn_length > 0 && asn_length + length_of_length + 2 == total_asn_length )																				
																		{
																			total_asn_length -= asn_length + length_of_length + 2;
																			if( asn_class == XEnum_AsnClass_Universal )
																			{
																				if( is_constructed == 0 )
																				{
																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_BITSTRING )
																					{
																						certificate_x509_v3()->_signature = rux::XRawData( &data_ptr[ position ] , asn_length );
																					}
																					else
																						error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
																				}
																				else
																					error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
																			}
																			else
																				error = "Incorrect x.509 certificate buffer,incorrect ASN class";
																		}
																		else
																			error = "Signature ASN length has incorrect value";
																	}
																}
															}
															if( total_asn_length != 0 )
																error = error = "Incorrect x.509 certificate buffer";
														}
														else
															error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
													}
													else
														error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
												}
												else
													error = "Incorrect x.509 certificate buffer,incorrect ASN class";
											}
											else
												error = "Tbs Certificate ASN length is greater or equal to data pointer length";			
										}
										else
											error = "Incorrect x.509 certificate buffer,incorrect ASN Primitive/Constructed";
									}
								}
								else
									error = "Incorrect x.509 certificate buffer,incorrect ASN Universal Class Tags class";
							}
							else
								error = "Incorrect x.509 certificate buffer,incorrect ASN class";
						}
						else
							error = "Total ASN length is not equal to data pointer length";			
					}
				}
				else
					error = "Data pointer length is less 2";
			}
			else 
				error = "Data pointer is NULL";
			return certificate_x509_v3++;
		};
		Object& CertificateX509v3::get_EncryptionPublicKey( void )
		{
			return _encryption_public_key++;
		};
	};
};