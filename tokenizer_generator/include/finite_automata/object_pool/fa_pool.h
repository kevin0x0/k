#ifndef KEVCC_TOKENIZER_GENERATOR_INCLUDE_FINITE_AUTOMATA_OBJECT_POOL_NFA_POOL_H
#define KEVCC_TOKENIZER_GENERATOR_INCLUDE_FINITE_AUTOMATA_OBJECT_POOL_NFA_POOL_H
#include "tokenizer_generator/include/finite_automata/fa.h"

KevFA* kev_fa_pool_acquire(void);
KevFA* kev_fa_pool_allocate(void);
void kev_fa_pool_deallocate(KevFA* nfa);
void kev_fa_pool_free(void);

#endif
