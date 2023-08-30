#ifndef KEVCC_PARGEN_INCLUDE_LR_CONFLICT_H
#define KEVCC_PARGEN_INCLUDE_LR_CONFLICT_H

#include "pargen/include/lr/table.h"

static inline void kev_lr_conflict_set_reducing(KevLRConflict* conflict, KevItem* item);
static inline void kev_lr_conflict_set_shifting(KevLRConflict* conflict);
static inline KevLRConflictHandler kev_lr_confhandler_shifting;
static inline KevLRConflictHandler kev_lr_confhandler_reducing;
/* TODO: immplement priority handler */
static inline KevLRConflictHandler kev_lr_confhandler_priority;


static inline void kev_lr_conflict_set_reducing(KevLRConflict* conflict, KevItem* item) {
  KevLRTableEntry* entry = conflict->entry;
  entry->action = KEV_LR_ACTION_RED;
  entry->info.rule = item->rule;
}

static inline void kev_lr_conflict_set_shifting(KevLRConflict* conflict) {
  KevLRTableEntry* entry = conflict->entry;
  entry->action = KEV_LR_ACTION_SHI;
  size_t target_itemset_id = entry->info.conflict->itemset->id;
  entry->info.itemset_id = target_itemset_id;
}

static inline bool kev_lr_confhandler_reducing(KevLRConflict* conflict, KevLRCollection* collec) {
  if (kev_lr_conflict_RR(conflict)) return false;
  kev_lr_conflict_set_reducing(conflict, conflict->conflct_items->items);
  return true;
}

static inline bool kev_lr_confhandler_shifting(KevLRConflict* conflict, KevLRCollection* collec) {
  if (kev_lr_conflict_RR(conflict)) return false;
  kev_lr_conflict_set_shifting(conflict);
  return true;
}

#endif