#ifndef CONSENSUS_H
#define CONSENSUS_H

#include <tomcrypt.h>
#include <string.h>

#define CONSENSUS_SECRET_LENGTH 32
#define CONSENSUS_HASH_LENGTH 32

#define CONSENSUS_INIT_OK 0
#define CONSENSUS_INIT_ERROR -1

#define CONSENSUS_RNG_OK 0

#define CONSENSUS_VERIFY_OK 0
#define CONSENSUS_VERIFY_ERROR -1

#define CONSENSUS_CHECK_OK 0
#define CONSENSUS_CHECK_ERROR -1

#define CONSENSUS_GENERATE_OK 0

int consensus_init(void);
void consensus_hash(unsigned char hash[CONSENSUS_HASH_LENGTH], size_t len, unsigned char data[len]);
int consensus_generate_secret(unsigned char secret[CONSENSUS_SECRET_LENGTH], unsigned char hash[CONSENSUS_HASH_LENGTH]);
int consensus_check_hashes(size_t count, unsigned char hashes[count][CONSENSUS_HASH_LENGTH]);
int consensus_verify_secrets(size_t count,
			     unsigned char hashes[count][CONSENSUS_HASH_LENGTH],
			     unsigned char secrets[count][CONSENSUS_HASH_LENGTH]);
int consensus_generate_shared(size_t count,
			      unsigned char secrets[count][CONSENSUS_SECRET_LENGTH],
			      unsigned char* buffer);
#endif
