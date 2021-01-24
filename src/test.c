#include "consensus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]) {

  // consensus should be initialised, it will work without (for now)
  assert(CONSENSUS_INIT_OK == consensus_init());

  // lets have a party size of 16, in practice you only make a single secret/hash pair.
  const size_t count = 16;
  unsigned char secrets[count][CONSENSUS_SECRET_LENGTH];
  unsigned char hashes[count][CONSENSUS_HASH_LENGTH];
  int i;
  for(i = 0; i < count; i++) {
    // but we are similating everyones generation
    assert(CONSENSUS_RNG_OK == consensus_generate_secret(secrets[i], hashes[i]));
    int j;
    printf("Secret:\t");
    for(j= 0; j < CONSENSUS_SECRET_LENGTH; j++)
      printf("%02X", secrets[i][j]);
    printf("\nHash:\t");
    for(j= 0; j < CONSENSUS_HASH_LENGTH; j++)
      printf("%02X", hashes[i][j]);
    printf("\n\n");
  }

  // it is left up to you to distribute the hashes, everyone should have a single copy of each one
  // consensus_check_hashes makes sure there are no duplicates which might be a sign of a scam
  puts("Check hashes");
  assert(CONSENSUS_CHECK_OK == consensus_check_hashes(count, hashes));

  // You should now distibute your secrets and consensus_verify_secrets will make sure each secret corresponds to a hash
  // You could check the quality of secrets, consensus doesn't your secret should be strong enough for entropy to be high
  puts("Verify hashes");
  assert(CONSENSUS_VERIFY_OK == consensus_verify_secrets(count, hashes, secrets));

  unsigned char shared[CONSENSUS_SECRET_LENGTH];
  puts("Generate shared");
  // consensus_generate_shared bitwise XORs each secret together.
  assert(CONSENSUS_GENERATE_OK == consensus_generate_shared(count, secrets, shared));
  for(i = 0; i < CONSENSUS_SECRET_LENGTH; i++)
    printf("%02X", shared[i]);
  puts("");

  // you may want to process the shared secret further, e.g. hash it, use it as a seed to some RNG.
  
}
