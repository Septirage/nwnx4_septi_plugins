/* "crypto.h" defines the look-up tables and KeyExpansion function 
* used in encrypt.cpp and decrypt.cpp
*/
#ifndef CRYPTO_H
#define CRYPTO_H


/* The main KeyExpansion function
* Generates additional keys using the original key
* Total of 11 128-bit keys generated, including the original
* Keys are stored one after the other in expandedKeys
*/


int AESEncryptB(std::string sMessage, unsigned char* key, unsigned char* encryptedMessage);
int AESDecryptB(unsigned char* key, unsigned char* encryptedMessage, unsigned char* decryptedMessage);

struct public_key_class{
	long long modulus;
	long long exponent;
};

struct private_key_class{
	long long modulus;
	long long exponent;
};

// This function will encrypt the data pointed to by message. It returns a pointer to a heap
// array containing the encrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The encrypted data will be 8 times as large as the original data.
long long *rsa_encrypt(const char *message, const unsigned long message_size, const struct public_key_class *pub);

// This function will decrypt the data pointed to by message. It returns a pointer to a heap
// array containing the decrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The variable message_size is the size in bytes of the encrypted message. 
// The decrypted data will be 1/8th the size of the encrypted data.
//char *rsa_decrypt(const long long *message, const unsigned long message_size, const struct private_key_class *pub);


#endif /* CRYPTO_H */