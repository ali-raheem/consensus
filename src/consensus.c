#include "consensus.h"

int consensus_init() {
  if(register_hash(&blake2b_256_desc) == -1)
    return CONSENSUS_INIT_ERROR;
  return CONSENSUS_INIT_OK;
}

void consensus_hash(unsigned char hash[CONSENSUS_HASH_LENGTH], size_t len, unsigned char data[len]) {
  hash_state md;
  blake2b_256_init(&md);
  blake2b_process(&md, data, len);
  blake2b_done(&md, hash);
}

int consensus_generate_secret(unsigned char secret[CONSENSUS_SECRET_LENGTH], unsigned char hash[CONSENSUS_HASH_LENGTH]){
  rng_get_bytes(secret, CONSENSUS_SECRET_LENGTH, NULL);
  consensus_hash(hash, CONSENSUS_SECRET_LENGTH, secret);
  return CONSENSUS_RNG_OK;
}

int consensus_check_hashes(size_t count, unsigned char hashes[count][CONSENSUS_HASH_LENGTH]) {
  int i, j;
  for(i = 0; i < count - 1; i++) {
    for(j = i + 1; j < count; j++) {
      if(memcmp(hashes[i],
		hashes[j],
		CONSENSUS_HASH_LENGTH) == 0)
	return CONSENSUS_CHECK_ERROR;
    }
  }
  return CONSENSUS_CHECK_OK;
}

int consensus_verify_secrets(size_t count,
			     unsigned char hashes[count][CONSENSUS_HASH_LENGTH],
			     unsigned char secrets[count][CONSENSUS_HASH_LENGTH])
{
  unsigned char hash[CONSENSUS_HASH_LENGTH];
  unsigned int i, j;
  for(i = 0; i < count; i++){
    consensus_hash(hash,
		   CONSENSUS_SECRET_LENGTH,
		   secrets[i]);
    int match = 0;
    for(j = 0; j < count; j++) {
      if (memcmp(hash,
		 hashes[j],
		 CONSENSUS_HASH_LENGTH) == 0) {
	match = 1;
	break;
      }
    }
    if (match == 0)
      return CONSENSUS_VERIFY_ERROR;
  }
  return CONSENSUS_VERIFY_OK;
}

int consensus_generate_shared(size_t count,
			      unsigned char secrets[count][CONSENSUS_SECRET_LENGTH],
			      unsigned char* buffer) {
  memset(buffer, 0, CONSENSUS_SECRET_LENGTH);
  int i, j;
  for(j = 0; j < count; j++) {
    for(i = 0; i < CONSENSUS_SECRET_LENGTH; i++) {
      buffer[i] = buffer[i] ^ secrets[j][i];
    }
  }
  return CONSENSUS_GENERATE_OK;
}
