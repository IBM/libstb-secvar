/*
 * SPDX-License-Identifier:  BSD-2-Clause
 * Copyright 2023 IBM Corp.
 */
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/objects.h>
#include <openssl/ossl_typ.h>
#include <openssl/asn1.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include "secvar/crypto.h"

/* X509 */

bool crypto_x509_is_CA (crypto_x509_t *x509)
{
  return !!X509_check_ca (x509);
}

int crypto_x509_get_der_len (crypto_x509_t *x509)
{
  return i2d_X509 (x509, NULL);
}

int crypto_x509_get_tbs_der_len (crypto_x509_t *x509)
{
  return i2d_re_X509_tbs (x509, NULL);
}

int crypto_x509_get_version (crypto_x509_t *x509)
{
  /*
   * add one because function return one less than actual certificate version,
   * see https://www.openssl.org/docs/man1.1.0/man3/X509_get_version.html
   */
  return X509_get_version (x509) + 1;
}

bool crypto_x509_is_RSA (crypto_x509_t *x509)
{
  int pk_type;
  EVP_PKEY *pub = NULL;

  pub = X509_get_pubkey (x509);
  if (!pub)
    {
      prlog (PR_ERR, "ERROR: Failed to extract public key from x509\n");
      return false;
    }

  pk_type = EVP_PKEY_base_id (pub);
  if (pk_type == NID_undef)
    prlog (PR_ERR, "ERROR: Failed to extract key type from x509\n");

  EVP_PKEY_free (pub);
  return pk_type == EVP_PKEY_RSA;
}

int crypto_x509_get_pk_bit_len (crypto_x509_t *x509)
{
  EVP_PKEY *pub = NULL;
  int length, rc;

  pub = X509_get_pubkey (x509);
  if (!pub)
    {
      prlog (PR_ERR, "ERROR: Failed to extract public key from x509\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_PACK(ERR_LIB_X509, 0, X509_R_UNABLE_TO_GET_CERTS_PUBLIC_KEY) : rc;
      // make sure negative
      return rc > 0 ? rc * -1 : rc;
    }

#if !defined(OPENSSL_VERSION_MAJOR) || OPENSSL_VERSION_MAJOR < 3
  RSA *rsa = NULL;
  rsa = EVP_PKEY_get1_RSA (pub);
  if (!rsa)
    {
      prlog (PR_ERR, "ERROR: Failed to extract RSA information from public key "
                     "of x509\n");
      EVP_PKEY_free(pub);
      goto err_out;
    }
  length = RSA_bits (rsa);
  RSA_free (rsa);
#else
  if (EVP_PKEY_get_base_id (pub) != EVP_PKEY_RSA)
    {
      prlog (PR_ERR, "ERROR: Public key of x509 is not of type RSA\n");
      EVP_PKEY_free (pub);
      EVP_PKEY_free(pub);
      goto err_out;
    }

  length = EVP_PKEY_get_bits (pub);
#endif
  if (!length)
    {
      prlog (PR_ERR, "ERROR: Failed to extract key length from RSA public key "
                     "of x509\n");
      EVP_PKEY_free(pub);
      goto err_out;
    }

  EVP_PKEY_free (pub);
  return length;

err_out:
  rc = ERR_get_error();
  // if no error then make a reasonable one
  rc = !rc ? ERR_PACK(ERR_LIB_X509, 0, X509_R_UNABLE_TO_GET_CERTS_PUBLIC_KEY) : rc;
  // make sure negative
  return rc > 0 ? rc * -1 : rc;
}

int crypto_x509_get_sig_len (crypto_x509_t *x509)
{
  int rc;
  ASN1_BIT_STRING *sig;

  sig = X509_get0_pubkey_bitstr (x509);
  if (!sig)
    {
      prlog (PR_ERR, "ERROR: Could not extract signature length from x509\n");

      rc = ERR_get_error ();
      rc = !rc ? ERR_PACK (ERR_LIB_X509, 0, X509_R_INVALID_FIELD_NAME) : rc;
      if (rc >= 0)
        rc *= -1;

      return rc;
    }

  return sig->length;
}

int crypto_x509_oid_is_pkcs1_sha256 (crypto_x509_t *x509)
{
  int rc;
  const X509_ALGOR *alg = NULL;

  alg = X509_get0_tbs_sigalg (x509);
  if (!alg)
    {
      prlog (PR_ERR, "ERROR: Could not extract algorithm from X509\n");
      rc = ERR_get_error ();
      return !rc ? ERR_PACK (ERR_LIB_X509, 0, X509_R_UNSUPPORTED_ALGORITHM) : rc;
    }

  if (OBJ_obj2nid (alg->algorithm) != NID_sha256WithRSAEncryption)
    {
      rc = ERR_get_error ();
      return !rc ? ERR_PACK (ERR_LIB_X509, 0, X509_R_UNSUPPORTED_ALGORITHM) : rc;
    }

  return OPENSSL_SUCCESS;
}

void crypto_x509_free (crypto_x509_t *x509)
{
  X509_free (x509);
}

crypto_pkcs7_t *crypto_pkcs7_parse_der (const unsigned char *buf, const int buflen)
{
  PKCS7 *pkcs7;

  pkcs7 = d2i_PKCS7 (NULL, &buf, buflen);
  if (!pkcs7)
    {
      PKCS7_SIGNED *signed_data;
      /*
       * Something could be wrong, or it could be that we've been
       * given a signedData instead of a full message.
       */
      signed_data = d2i_PKCS7_SIGNED (NULL, &buf, buflen);
      if (!signed_data)
        {
          prlog (PR_ERR, "ERROR: parsing PKCS7 with OpenSSL failed\n");
          return NULL;
        }
      /* allocate PKCS7 for signed data to be stored in */
      pkcs7 = PKCS7_new();
      if (!pkcs7)
        {
          prlog (PR_ERR, "ERROR: PKCS7 allocation failed\n");
          PKCS7_SIGNED_free(signed_data);
          return NULL;
        }
      pkcs7->type = OBJ_nid2obj(NID_pkcs7_signed);
      pkcs7->d.sign = signed_data;
    }

  /* make sure it contains signed data, openssl supports other types */
  if (!PKCS7_type_is_signed (pkcs7))
    {
      prlog (PR_ERR, "ERROR: PKCS7 does not contain signed data\n");
      PKCS7_free (pkcs7);
      return NULL;
    }

  return pkcs7;
}

int crypto_pkcs7_md_is_sha256 (crypto_pkcs7_t *pkcs7)
{
  int rc;
  X509_ALGOR *alg;
  /*
   * extract signer algorithms from pkcs7
   * we successfully parsed the PKCS#7 message so we do not expect
   * this to fail
   */
  alg = sk_X509_ALGOR_value (pkcs7->d.sign->md_algs, 0);
  if (!alg)
    {
      prlog (PR_ERR, "ERROR: Could not extract message digest identifiers from "
                     "PKCS7\n");
      rc = ERR_get_error();
      return !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_UNKNOWN_DIGEST_TYPE) : rc;
    }

  /* extract nid from algorithms and ensure it is the same nid as SHA256 */
  if (OBJ_obj2nid (alg->algorithm) == NID_sha256)
    return OPENSSL_SUCCESS;
  
  rc = ERR_get_error();
  return !rc ? ERR_PACK(ERR_LIB_PKCS7, 0,PKCS7_R_DIGEST_FAILURE) : rc;
}

void crypto_pkcs7_free (crypto_pkcs7_t *pkcs7)
{
  PKCS7_free (pkcs7);
}

crypto_x509_t *crypto_pkcs7_get_signing_cert (crypto_pkcs7_t *pkcs7, int cert_num)
{
  X509 *pkcs7_cert = NULL;

  pkcs7_cert = sk_X509_value (pkcs7->d.sign->cert, cert_num);

  return pkcs7_cert;
}

/*
 * currently this function works and the mbedtls version currently perform the following steps
 *   1. the hash, md context and given x509 are used to generated a signature
 *   2. all of the signatures in the pkcs7 are compared to the signature generated by the x509
 *   3. if any of the signatures in the pkcs7 match the genrated signature then return SV_SUCCESS
 */
int crypto_pkcs7_signed_hash_verify (crypto_pkcs7_t *pkcs7, crypto_x509_t *x509,
                                     unsigned char *hash, int hash_len)
{
  int exp_size, md_nid, num_signers, rc = ERR_R_INTERNAL_ERROR;
  unsigned char *exp_sig;
  EVP_PKEY *pk;
  EVP_PKEY_CTX *pk_ctx;
  X509_ALGOR *alg;
  const EVP_MD *evp_md;
  PKCS7_SIGNER_INFO *signer_info;

  /* generate a signature with the x509 */
  pk = X509_get_pubkey (x509);
  pk_ctx = EVP_PKEY_CTX_new (pk, NULL);
  if (pk == NULL || pk_ctx == NULL)
    {
      prlog (PR_ERR, "ERROR: Failed to create public key context from x509\n");
      rc = ERR_get_error();
      return !rc ? ERR_R_INTERNAL_ERROR : rc;
    }
  if (EVP_PKEY_verify_init (pk_ctx) <= 0)
    {
      prlog (PR_ERR, "ERROR: Failed to initialize pk context for x509 pk \n");
      rc = ERR_get_error();
      goto out;
    }
  if (EVP_PKEY_CTX_set_rsa_padding (pk_ctx, RSA_PKCS1_PADDING) <= 0)
    {
      prlog (PR_ERR, "ERROR: Failed to setup pk context with RSA padding\n");
      rc = ERR_get_error();
      goto out;
    }
  /* extract signer algorithms from pkcs7 */
  alg = sk_X509_ALGOR_value (pkcs7->d.sign->md_algs, 0);
  if (!alg)
    {
      prlog (PR_ERR, "ERROR: Could not extract message digest identifiers from "
                     "PKCS7\n");
      rc = ERR_get_error();
      goto out;
    }
  /* extract nid from algorithms */
  md_nid = OBJ_obj2nid (alg->algorithm);
  /* set signature md depending on md in pkcs7 */
  evp_md = EVP_get_digestbynid (md_nid);
  if (!evp_md)
    {
      prlog (PR_ERR, "ERROR: Unknown NID (%d) for MD found in PKCS7\n", md_nid);
      rc = ERR_get_error();;
      goto out;
    }

  if (EVP_PKEY_CTX_set_signature_md (pk_ctx, evp_md) <= 0)
    {
      prlog (PR_ERR, "ERROR: Failed to set signature md for pk ctx\n");
      rc = ERR_get_error();
      goto out;
    }

  //assume hash length if none given
  if (hash_len == 0)
    hash_len = EVP_MD_size(evp_md);

  /* verify on all signatures in pkcs7 */
  num_signers = sk_PKCS7_SIGNER_INFO_num (PKCS7_get_signer_info (pkcs7));
  if (num_signers == 0)
    {
      prlog (PR_ERR, "ERROR: no signers to verify");
      goto out;
    }
  else if (num_signers < 0)
    {
      prlog(PR_ERR, "ERROR: pkcs7->signer_info was NULL");
      goto out;
    }

  for (int s = 0; s < num_signers; s++)
    {
      /* make sure we can get the signature data */
      signer_info = sk_PKCS7_SIGNER_INFO_value (PKCS7_get_signer_info (pkcs7), s);

      if (!signer_info)
        {
          prlog (PR_ERR, "ERROR: Could not get PKCS7 signer information\n");
          rc = ERR_get_error();
          goto out;
        }

      exp_size = signer_info->enc_digest->length;
      exp_sig = signer_info->enc_digest->data;

      if (exp_size <= 0 || !exp_sig)
        {
          prlog (PR_ERR, "ERROR: No data found in PKCS7\n");
          rc = ERR_get_error();
          goto out;
        }
      rc = EVP_PKEY_verify (pk_ctx, exp_sig, exp_size, hash, hash_len);
      /*
       * returns 1 on success
       * if successfull then exit
       */
      if (rc == 1)
        goto out;

      rc = ERR_get_error();
    }
out:
  EVP_PKEY_free (pk);
  EVP_PKEY_CTX_free (pk_ctx);

  if (rc == 1)
    return OPENSSL_SUCCESS;

  return !rc ? ERR_R_INTERNAL_ERROR : rc;
}

void crypto_strerror (int rc, char *out_str, size_t out_max_len)
{
  ERR_error_string_n (rc, out_str, out_max_len);
}

crypto_x509_t *crypto_x509_parse_der (const unsigned char *data, size_t data_len)
{
  return d2i_X509 (NULL, &data, data_len);
}

int crypto_x509_md_is_sha256 (crypto_x509_t *x509)
{
  int rc;
  const X509_ALGOR *alg = NULL;

  alg = X509_get0_tbs_sigalg (x509);
  if (!alg)
    {
      prlog (PR_ERR, "ERROR: Could not extract algorithm from X509\n");
      rc = ERR_get_error ();
      return !rc ? ERR_PACK (ERR_LIB_X509, 0, X509_R_INVALID_FIELD_NAME) : rc;
    }

  if (OBJ_obj2nid (alg->algorithm) == NID_sha256WithRSAEncryption)
    return OPENSSL_SUCCESS;
  else
    {
      prlog (PR_ERR, "ERROR: Certificate NID is not SHA256, expected %d found %d\n",
             NID_sha256, OBJ_obj2nid (alg->algorithm));
      rc = ERR_get_error ();
      return !rc ? ERR_PACK (ERR_LIB_X509, 0, X509_R_UNSUPPORTED_ALGORITHM) : rc;
    }
}

void crypto_x509_get_short_info (crypto_x509_t *x509, char *short_desc, size_t max_len)
{
  const X509_ALGOR *alg = NULL;
  alg = X509_get0_tbs_sigalg (x509);
  /* unlikely failure */
  if (!alg)
    {
      prlog (PR_ERR, "ERROR: Could not extract algorithm from X509\n");
      return;
    }
  /* last arg set as ZERO to get short description in string */
  OBJ_obj2txt (short_desc, max_len, alg->algorithm, 0);
}

int crypto_x509_get_long_desc (char *x509_info, size_t max_len, const char *delim, crypto_x509_t *x509)
{
  int rc;
  long actual_mem_len;
  BIO *bio = BIO_new (BIO_s_mem ());
  char *tmp = NULL;
  rc = X509_print_ex (bio, x509, XN_FLAG_MULTILINE,
                      X509_FLAG_COMPAT | X509_FLAG_NO_PUBKEY | X509_FLAG_NO_SIGDUMP);
  if (rc < 0)
    {
      prlog (PR_ERR, "ERROR: could not get BIO data on X509, openssl err#%d\n", rc);
      return rc;
    }
  /* returns total data avialable */
  actual_mem_len = BIO_get_mem_data (bio, &tmp);
  /* check to make sure we do not overflow the allocated mem */
  actual_mem_len = max_len > actual_mem_len ? actual_mem_len : max_len - 1;
  memcpy (x509_info, tmp, actual_mem_len);
  BIO_free (bio);
  return actual_mem_len;
}

int crypto_convert_pem_to_der (const unsigned char *input, size_t ilen, unsigned char **output, size_t *olen)
{
  int rc;
  /* these variables are not needed on return, just needed to properly call the function */
  char *header = NULL, *name = NULL;
  BIO *bio;

  bio = BIO_new_mem_buf (input, ilen);
  /* returns 1 on success so flip result*/
  rc = !PEM_read_bio (bio, &name, &header, output, (long int *) olen);
  if (header)
    OPENSSL_free (header);

  if (name)
    OPENSSL_free (name);

  BIO_free (bio);

  return rc;
}

int crypto_pkcs7_generate_w_signature (unsigned char **pkcs7, size_t *pkcs7_size,
                            const unsigned char *new_data, size_t new_data_size,
                            const char **crt_files, const char **key_files,
                            int key_pairs, int hash_funct)
{
  int rc;
  PKCS7 *gen_pkcs7_struct = NULL;
  BIO *bio = NULL, *out_bio = NULL;
  FILE *fp;
  EVP_PKEY *evp_pkey = NULL;
  const EVP_MD *evp_md = NULL;
  crypto_x509_t *x509 = NULL;
  long pkcs7_out_len;
  unsigned char *key = NULL, *key_tmp, *crt = NULL, *out_bio_der = NULL;
  char *unnecessary_hdr = NULL, *unnecessary_name = NULL;
  long int key_size, crt_size;

  if (key_pairs == 0)
    {
      prlog (PR_ERR, "ERROR: No signers given, cannot generate PKCS7\n");
      return ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR);
    }

  evp_md = EVP_get_digestbynid (hash_funct);
  if (!evp_md)
    {
      prlog (PR_ERR, "ERROR: Unknown NID (%d) for MD found in PKCS7\n", hash_funct);
      rc = ERR_get_error();
      return !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
    }

  bio = BIO_new_mem_buf (new_data, new_data_size);
  if (!bio)
    {
      prlog (PR_ERR, "ERROR: Failed to initialize new data BIO structure\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_R_MALLOC_FAILURE : rc;
      goto out;
    }

  gen_pkcs7_struct = PKCS7_sign (NULL, NULL, NULL, bio, PKCS7_PARTIAL | PKCS7_DETACHED);
  if (!gen_pkcs7_struct)
    {
      prlog (PR_ERR, "ERROR: Failed to initialize pkcs7 structure\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
      goto out;
    }
  /* for every key pair get the data and add the signer to the pkcs7 */
  for (int i = 0; i < key_pairs; i++)
    {
      /* get data of private keys */
      fp = fopen (key_files[i], "r");
      if (fp == NULL)
        {
          prlog (PR_ERR, "ERROR: failed to open file %s: %s\n", key_files[i],
                 strerror (errno));
          rc = ERR_PACK(ERR_LIB_PKCS7, 0 ,PKCS7_R_NO_CONTENT);
          goto out;
        }
      rc = PEM_read (fp, &unnecessary_name, &unnecessary_hdr, &key, &key_size);
      OPENSSL_free (unnecessary_name);
      OPENSSL_free (unnecessary_hdr);
      fclose (fp);
      /* returns 1 on success */
      if (rc != 1)
        {
          prlog (PR_ERR, "ERROR: failed to get data from priv key file %s\n", key_files[i]);
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      /* get data from crt */
      fp = fopen (crt_files[i], "r");
      if (fp == NULL)
        {
          prlog (PR_ERR, "ERROR: failed to open file %s: %s\n", crt_files[i],
                 strerror (errno));
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      rc = PEM_read (fp, &unnecessary_name, &unnecessary_hdr, &crt, &crt_size);
      OPENSSL_free (unnecessary_name);
      OPENSSL_free (unnecessary_hdr);
      fclose (fp);
      /* returns 1 on success */
      if (rc != 1)
        {
          prlog (PR_ERR, "ERROR: failed to get data from cert file %s\n", crt_files[i]);
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      /* get private key from private key DER buff */
      key_tmp = key;
      evp_pkey = d2i_AutoPrivateKey (NULL, (const unsigned char **) &key_tmp, key_size);
      if (!evp_pkey)
        {
          prlog (PR_ERR, "ERROR: Failed to parse private key into EVP_PKEY "
                         "openssl struct\n");
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      /* get x509 from cert DER buff */
      x509 = crypto_x509_parse_der (crt, crt_size);
      if (!x509)
        {
          prlog (PR_ERR, "ERROR: Failed to parse certificate into x509 openssl "
                         "struct\n");
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      /*
       * add the signature to the pkcs7
       * returns NULL is failure
       */
      if (!PKCS7_sign_add_signer (gen_pkcs7_struct, x509, evp_pkey, evp_md, PKCS7_NOATTR))
        {
          prlog (PR_ERR,
                 "ERROR: Failed to add signer to the pkcs7 structure\n");
          rc = ERR_get_error();
          rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
          goto out;
        }
      /* reset mem */
      OPENSSL_free (key);
      key = NULL;
      EVP_PKEY_free (evp_pkey);
      evp_pkey = NULL;
      OPENSSL_free (crt);
      crt = NULL;
      X509_free (x509);
      x509 = NULL;
    }

  /* finalize the struct, runs hashing and signatures */
  rc = PKCS7_final (gen_pkcs7_struct, bio, PKCS7_BINARY);
  if (rc != 1)
    {
      prlog (PR_ERR, "ERROR: Failed to finalize openssl pkcs7 struct\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
      goto out;
    }

  /* convert to DER */
  out_bio = BIO_new (BIO_s_mem ());
  if (!out_bio)
    {
      prlog (PR_ERR, "ERROR: Failed to initialize openssl BIO \n");
      rc = ERR_get_error();
      rc = !rc ? ERR_R_MALLOC_FAILURE : rc;
      goto out;
    }

  /* returns 1 for success */
  rc = i2d_PKCS7_bio (out_bio, gen_pkcs7_struct);
  if (!rc)
    {
      prlog (PR_ERR, "ERROR: Failed to convert PKCS7 Struct to DER\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
      goto out;
    }

  /* get data out of BIO and into return values */
  pkcs7_out_len = BIO_get_mem_data (out_bio, &out_bio_der);
  /* returns number of bytes decoded or error */
  if (pkcs7_out_len <= 0)
    {
      prlog (PR_ERR,
             "ERROR: Failed to extract PKCS7 DER data from openssl BIO\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_PKCS7_ADD_SIGNER_ERROR) : rc;
      goto out;
    }

  *pkcs7 = OPENSSL_malloc (pkcs7_out_len);
  if (!*pkcs7)
    {
      prlog (PR_ERR, "ERROR: Failed to allocate memory\n");
      rc = ERR_get_error();
      rc = !rc ? ERR_R_MALLOC_FAILURE : rc;
      goto out;
    }

  *pkcs7_size = pkcs7_out_len;
  /* copy memory over so it is persistent */
  memcpy (*pkcs7, out_bio_der, *pkcs7_size);
  /* if here then successfull generation */
  rc = OPENSSL_SUCCESS;

out:
  if (key)
    OPENSSL_free (key);

  if (crt)
    OPENSSL_free (crt);

  if (evp_pkey)
    EVP_PKEY_free (evp_pkey);

  if (x509)
    X509_free (x509);

  if (gen_pkcs7_struct)
    PKCS7_free (gen_pkcs7_struct);

  BIO_free (bio);
  BIO_free (out_bio);

  return rc;
}

int crypto_pkcs7_generate_w_already_signed_data (unsigned char **pkcs7, size_t *pkcs7_size,
                                      const unsigned char *new_data, size_t new_data_size,
                                      const char **crt_files, const char **sig_files,
                                      int key_pairs, int hash_funct)
{
  prlog (PR_ERR,
         "ERROR: Currently unable to support generation of PKCS7 with "
         "externally generated signatures when compiling with OpenSSL\n");
  return ERR_PACK(ERR_LIB_PKCS7, 0, PKCS7_R_UNKNOWN_OPERATION);
}

int crypto_md_ctx_init (crypto_md_ctx_t **ctx, int md_id)
{
  int rc;
  const EVP_MD *md;

  md = EVP_get_digestbynid (md_id);
  if (!md)
    {
      prlog (PR_ERR, "ERROR: Invalid MD NID\n");
      rc = ERR_get_error();
      return !rc ? ERR_PACK(ERR_LIB_EVP, 0, EVP_R_INVALID_DIGEST) : rc;
    }

  *ctx = EVP_MD_CTX_new ();
  if (!*ctx)
    {
      prlog (PR_ERR, "ERROR: failed to allocate memory\n");
      rc = ERR_get_error();
      return !rc ? ERR_R_MALLOC_FAILURE : rc;
    }

  return !EVP_DigestInit_ex ((EVP_MD_CTX *) *ctx, md, NULL);
}

int crypto_md_update (crypto_md_ctx_t *ctx, const unsigned char *data, size_t data_len)
{
  /* returns 1 on success and 0 for fail */
  return !EVP_DigestUpdate (ctx, data, data_len);
}

int crypto_md_finish (crypto_md_ctx_t *ctx, unsigned char *hash)
{
  /* returns 1 on success and 0 on fail */
  return !EVP_DigestFinal_ex (ctx, hash, NULL);
}

void crypto_md_free (crypto_md_ctx_t *ctx)
{
  EVP_MD_CTX_free (ctx);
}

int crypto_md_generate_hash (const unsigned char *data, size_t size, int hash_funct,
                  unsigned char **out_hash, size_t *out_hash_size)
{
  int rc;
  crypto_md_ctx_t *ctx = NULL;
  size_t hash_len = 0;

  rc = crypto_md_ctx_init (&ctx, hash_funct);
  if (rc)
    return rc;

  rc = crypto_md_update (ctx, data, size);
  if (rc)
    goto out;

  switch (hash_funct)
    {
      case CRYPTO_MD_SHA1: hash_len = SHA_DIGEST_LENGTH; break;
      case CRYPTO_MD_SHA224: hash_len = SHA224_DIGEST_LENGTH; break;
      case CRYPTO_MD_SHA256: hash_len = SHA256_DIGEST_LENGTH; break;
      case CRYPTO_MD_SHA384: hash_len = SHA384_DIGEST_LENGTH; break;
      case CRYPTO_MD_SHA512: hash_len = SHA512_DIGEST_LENGTH; break;
      default:
        prlog (PR_ERR, "ERROR: Unknown NID (%d)\n", hash_funct);
        rc = ERR_PACK(ERR_LIB_EVP, 0, EVP_R_INVALID_DIGEST);
        goto out;
    }

  *out_hash = OPENSSL_malloc (hash_len);
  if (!*out_hash)
    {
      prlog (PR_ERR, "ERROR: Failed to allocate data\n");
      rc = ERR_R_MALLOC_FAILURE;
      goto out;
    }

  rc = crypto_md_finish (ctx, *out_hash);
  if (rc)
    {
      OPENSSL_free (*out_hash);
      *out_hash = NULL;
      goto out;
    }

  *out_hash_size = hash_len;

out:
  crypto_md_free (ctx);
  return rc;
}

