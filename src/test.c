#include "consensus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]) {
  assert(CONSENSUS_INIT_OK == consensus_init());

  const size_t count = 16;
  unsigned char secrets[count][CONSENSUS_SECRET_LENGTH];
  unsigned char hashes[count][CONSENSUS_HASH_LENGTH];
  int i;
  for(i = 0; i < count; i++) {
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

  puts("Check hashes");
  assert(CONSENSUS_CHECK_OK == consensus_check_hashes(count, hashes));
  puts("Verify hashes");
  assert(CONSENSUS_VERIFY_OK == consensus_verify_secrets(count, hashes, secrets));

  unsigned char shared[CONSENSUS_SECRET_LENGTH];
  puts("Generate shared");
  assert(CONSENSUS_GENERATE_OK == consensus_generate_shared(count, secrets, shared));
  for(i = 0; i < CONSENSUS_SECRET_LENGTH; i++)
    printf("%02X", shared[i]);
  puts("");
  
}
