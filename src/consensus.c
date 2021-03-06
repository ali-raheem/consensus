#include "consensus.h"

consensusResult consensus_init() {
  if(register_hash(&blake2b_256_desc) == -1)
    return CONSENSUS_ERROR_INIT_FAILED;
  return CONSENSUS_OK;
}

void consensus_hash(uint8_t hash[CONSENSUS_HASH_LENGTH],
		    size_t len,
		    uint8_t data[len])
{
  hash_state md;
  blake2b_256_init(&md);
  blake2b_process(&md, data, len);
  blake2b_done(&md, hash);
}

consensusResult consensus_generate_secret(uint8_t secret[CONSENSUS_SECRET_LENGTH],
					  uint8_t hash[CONSENSUS_HASH_LENGTH])
{
  rng_get_bytes(secret, CONSENSUS_SECRET_LENGTH, NULL);
  consensus_hash(hash, CONSENSUS_SECRET_LENGTH, secret);
  return CONSENSUS_OK;
}

consensusResult consensus_check_hashes(size_t count,
				       uint8_t hashes[count][CONSENSUS_HASH_LENGTH])
{
  size_t i, j;
  for(i = 0; i < count - 1; i++) {
    for(j = i + 1; j < count; j++) {
      if(memcmp(hashes[i],
		hashes[j],
		CONSENSUS_HASH_LENGTH) == 0)
	return CONSENSUS_ERROR_DUPLICATE_HASH;
    }
  }
  return CONSENSUS_OK;
}

consensusResult consensus_verify_secrets(size_t count,
			     uint8_t hashes[count][CONSENSUS_HASH_LENGTH],
			     uint8_t secrets[count][CONSENSUS_HASH_LENGTH])
{
  uint8_t hash[CONSENSUS_HASH_LENGTH];
  size_t i, j;
  for(i = 0; i < count; i++){
    consensus_hash(hash,
		   CONSENSUS_SECRET_LENGTH,
		   secrets[i]);
    bool match = false;
    for(j = 0; j < count; j++) {
      if (memcmp(hash,
		 hashes[j],
		 CONSENSUS_HASH_LENGTH) == 0) {
	match = true;
	break;
      }
    }
    if (match)
      return CONSENSUS_ERROR_HASH_NOT_FOUND;
  }
  return CONSENSUS_OK;
}

consensusResult consensus_generate_shared(size_t count,
					  uint8_t secrets[count][CONSENSUS_SECRET_LENGTH],
					  uint8_t* buffer)
{
  memset(buffer, 0, CONSENSUS_SECRET_LENGTH);
  size_t i, j;
  for(j = 0; j < count; j++) {
    for(i = 0; i < CONSENSUS_SECRET_LENGTH; i++) {
      buffer[i] = buffer[i] ^ secrets[j][i];
    }
  }
  return CONSENSUS_OK;
}
