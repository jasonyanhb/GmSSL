/* ====================================================================
 * Copyright (c) 2016 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */
/*
 * Boneh-Boyen Identity-Based Encryption (BB1-IBE)
 * see [RFC 5091](https://tools.ietf.org/html/rfc5091)
 * Identity-Based Cryptography Standard (IBCS) #1:
 * Supersingular Curve Implementations of the BF and BB1 Cryptosystems
 */

#ifndef HEADER_BB1IBE_H
#define HEADER_BB1IBE_H

#include <string.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>
#include <openssl/fppoint.h>

#define BB1IBE_VERSION	2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BB1PublicParameters_st {
	long version;
	ASN1_OBJECT *curve;
	BIGNUM *p;
	BIGNUM *q;
	FpPoint *pointP;
	FpPoint *pointP1;
	FpPoint *pointP2;
	FpPoint *pointP3;
	FpPoint *v;
	ASN1_OBJECT *hashfcn;
} BB1PublicParameters;
DECLARE_ASN1_FUNCTIONS(BB1PublicParameters)

typedef struct BB1MasterSecret_st {
	long version;
	BIGNUM *alpha;
	BIGNUM *beta;
	BIGNUM *gamma;
} BB1MasterSecret;
DECLARE_ASN1_FUNCTIONS(BB1MasterSecret)

typedef struct BB1PrivateKeyBlock_st {
	long version;
	FpPoint *pointD0;
	FpPoint *pointD1;
} BB1PrivateKeyBlock;
DECLARE_ASN1_FUNCTIONS(BB1PrivateKeyBlock)

typedef struct BB1CiphertextBlock_st {
	long version;
	FpPoint *pointChi0;
	FpPoint *pointChi1;
	BIGNUM *nu;
	ASN1_OCTET_STRING *y;
} BB1CiphertextBlock;
DECLARE_ASN1_FUNCTIONS(BB1CiphertextBlock)


int BB1IBE_setup(const EC_GROUP *group, const EVP_MD *md,
	BB1PublicParameters **mpk, BB1MasterSecret **msk);
BB1PrivateKeyBlock *BB1IBE_extract_private_key(BB1PublicParameters *mpk,
	BB1MasterSecret *msk, const char *id, size_t idlen);
BB1CiphertextBlock *BB1IBE_do_encrypt(BB1PublicParameters *mpk,
	const unsigned char *in, size_t inlen,
	const char *id, size_t idlen);
int BB1IBE_do_decrypt(BB1PublicParameters *mpk,
	const BB1CiphertextBlock *in, unsigned char *out, size_t *outlen,
	BB1PrivateKeyBlock *sk);
int BB1IBE_encrypt(BB1PublicParameters *mpk,
	const unsigned char *in, size_t inlen,
	unsigned char *out, size_t *outlen,
	const char *id, size_t idlen);
int BB1IBE_decrypt(BB1PublicParameters *mpk,
	const unsigned char *in, size_t inlen,
	unsigned char *out, size_t *outlen,
	BB1PrivateKeyBlock *sk);


#ifdef __cplusplus
}
#endif
#endif
