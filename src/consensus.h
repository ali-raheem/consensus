#pragma once

#include <tomcrypt.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum consensusResult {
  CONSENSUS_OK = 0,
  CONSENSUS_ERROR_INIT_FAILED,
  CONSENSUS_ERROR_DUPLICATE_HASH,
  CONSENSUS_ERROR_HASH_NOT_FOUND
} consensusResult;

#define CONSENSUS_SECRET_LENGTH 32
#define CONSENSUS_HASH_LENGTH 32

consensusResult consensus_init(void);
void consensus_hash(uint8_t hash[CONSENSUS_HASH_LENGTH], size_t len, uint8_t data[len]);
consensusResult consensus_generate_secret(uint8_t secret[CONSENSUS_SECRET_LENGTH], uint8_t hash[CONSENSUS_HASH_LENGTH]);
consensusResult consensus_check_hashes(size_t count, uint8_t hashes[count][CONSENSUS_HASH_LENGTH]);
consensusResult consensus_verify_secrets(size_t count,
			     uint8_t hashes[count][CONSENSUS_HASH_LENGTH],
			     uint8_t secrets[count][CONSENSUS_HASH_LENGTH]);
consensusResult consensus_generate_shared(size_t count,
			      uint8_t secrets[count][CONSENSUS_SECRET_LENGTH],
			      uint8_t * buffer);

