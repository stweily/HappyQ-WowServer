#ifndef _CkRsa_H
#define _CkRsa_H
#include "Chilkat_C.h"

HCkRsa CkRsa_Create(void);
void CkRsa_Dispose(HCkRsa handle);
void CkRsa_getCharset(HCkRsa handle, HCkString retval);
void CkRsa_putCharset(HCkRsa handle, const char *newVal);
void CkRsa_getEncodingMode(HCkRsa handle, HCkString retval);
void CkRsa_putEncodingMode(HCkRsa handle, const char *newVal);
void CkRsa_getLastErrorHtml(HCkRsa handle, HCkString retval);
void CkRsa_getLastErrorText(HCkRsa handle, HCkString retval);
void CkRsa_getLastErrorXml(HCkRsa handle, HCkString retval);
BOOL CkRsa_getLittleEndian(HCkRsa handle);
void CkRsa_putLittleEndian(HCkRsa handle, BOOL newVal);
long CkRsa_getNumBits(HCkRsa handle);
BOOL CkRsa_getOaepPadding(HCkRsa handle);
void CkRsa_putOaepPadding(HCkRsa handle, BOOL newVal);
BOOL CkRsa_getUtf8(HCkRsa handle);
void CkRsa_putUtf8(HCkRsa handle, BOOL newVal);
void CkRsa_getVersion(HCkRsa handle, HCkString retval);
BOOL CkRsa_DecryptBytes(HCkRsa handle, HCkByteData bData, BOOL bUsePrivateKey, HCkByteData out);
BOOL CkRsa_DecryptBytesENC(HCkRsa handle, const char *str, BOOL bUsePrivateKey, HCkByteData out);
BOOL CkRsa_DecryptString(HCkRsa handle, HCkByteData bData, BOOL bUsePrivateKey, HCkString out);
BOOL CkRsa_DecryptStringENC(HCkRsa handle, const char *str, BOOL bUsePrivateKey, HCkString out);
BOOL CkRsa_EncryptBytes(HCkRsa handle, HCkByteData bData, BOOL bUsePrivateKey, HCkByteData out);
BOOL CkRsa_EncryptBytesENC(HCkRsa handle, HCkByteData bData, BOOL bUsePrivateKey, HCkString out);
BOOL CkRsa_EncryptString(HCkRsa handle, const char *str, BOOL bUsePrivateKey, HCkByteData out);
BOOL CkRsa_EncryptStringENC(HCkRsa handle, const char *str, BOOL bUsePrivateKey, HCkString out);
BOOL CkRsa_ExportPrivateKey(HCkRsa handle, HCkString strXml);
BOOL CkRsa_ExportPublicKey(HCkRsa handle, HCkString strXml);
BOOL CkRsa_GenerateKey(HCkRsa handle, int numBits);
BOOL CkRsa_ImportPrivateKey(HCkRsa handle, const char *strXml);
BOOL CkRsa_ImportPublicKey(HCkRsa handle, const char *strXml);
BOOL CkRsa_SaveLastError(HCkRsa handle, const char *filename);
BOOL CkRsa_SignBytes(HCkRsa handle, HCkByteData bData, const char *hashAlg, HCkByteData out);
BOOL CkRsa_SignBytesENC(HCkRsa handle, HCkByteData bData, const char *hashAlg, HCkString out);
BOOL CkRsa_SignString(HCkRsa handle, const char *str, const char *hashAlg, HCkByteData out);
BOOL CkRsa_SignStringENC(HCkRsa handle, const char *str, const char *hashAlg, HCkString out);
BOOL CkRsa_SnkToXml(HCkRsa handle, const char *filename, HCkString strXml);
BOOL CkRsa_UnlockComponent(HCkRsa handle, const char *unlockCode);
BOOL CkRsa_VerifyBytes(HCkRsa handle, HCkByteData bData, const char *hashAlg, HCkByteData sigData);
BOOL CkRsa_VerifyBytesENC(HCkRsa handle, HCkByteData bData, const char *hashAlg, const char *encodedSig);
BOOL CkRsa_VerifyString(HCkRsa handle, const char *str, const char *hashAlg, HCkByteData sigData);
BOOL CkRsa_VerifyStringENC(HCkRsa handle, const char *str, const char *hashAlg, const char *sig);
const char *CkRsa_charset(HCkRsa handle);
const char *CkRsa_decryptString(HCkRsa handle, const char *bytes, int numBytes, BOOL bUsePrivateKey);
const char *CkRsa_decryptStringENC(HCkRsa handle, const char *str, BOOL bUsePrivateKey);
const char *CkRsa_encodingMode(HCkRsa handle);
const char *CkRsa_encryptBytesENC(HCkRsa handle, const char *bytes, int numBytes, BOOL bUsePrivateKey);
const char *CkRsa_encryptStringENC(HCkRsa handle, const char *str, BOOL bUsePrivateKey);
const char *CkRsa_exportPrivateKey(HCkRsa handle);
const char *CkRsa_exportPublicKey(HCkRsa handle);
const char *CkRsa_lastErrorHtml(HCkRsa handle);
const char *CkRsa_lastErrorText(HCkRsa handle);
const char *CkRsa_lastErrorXml(HCkRsa handle);
const char *CkRsa_signBytesENC(HCkRsa handle, HCkByteData bData, const char *hashAlg);
const char *CkRsa_signStringENC(HCkRsa handle, const char *str, const char *hashAlg);
const char *CkRsa_snkToXml(HCkRsa handle, const char *filename);
const char *CkRsa_version(HCkRsa handle);
#endif