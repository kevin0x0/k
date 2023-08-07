#include "pargen/include/lr/lalr.h"
#include "pargen/include/lr/item.h"
#include "pargen/include/lr/hashmap/gotomap.h"
#include "pargen/include/lr/set/itemset_set.h"
#include "utils/include/array/addr_array.h"
#include "utils/include/set/bitset.h"

#include <stdlib.h>

static size_t kev_label_symbols(KevSymbol** symbols, size_t symbol_no);
static KevItemSet* kev_get_start_itemset(KevSymbol* start, KevSymbol** lookahead, size_t length);
static KevAddrArray* kev_compute_all_itemsets(KevItemSet* start_iset, KevLALRCollection* collec);
static bool kev_merge_gotos(KevItemSetSet* iset_set, KevAddrArray* itemset_array, KevLALRCollection* collec, KevItemSet* itemset);
static bool kev_merge_itemset(KevItemSet* new, KevItemSet* old, KevItemSet* itemset, KevLALRCollection* collec);
static bool kev_add_new_itemset(KevItemSet* new, KevItemSetSet* iset_set, KevItemSet* itemset, KevLALRCollection* collec);
static bool kev_compute_itemset(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon, KevGotoMap* goto_container);
static bool kev_generate_gotos(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevGotoMap* goto_container);
static void kev_clear_la_symbols_and_closure(KevAddrArray* closure, KevBitSet** la_symbols);
static bool kev_lalr_closure(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon);
static bool kev_lalr_closure_propagate(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon);
/* initialize lookahead for kernel items in itemset */
static inline bool kev_init_kitem_la(KevItemSet* itemset, size_t epsilon);
static KevBitSet* kev_lalr_follows(KevKernelItem* kitem, KevBitSet** firsts, size_t epsilon);

static KevLALRCollection* kev_lalr_get_empty_collec(void);
static void kev_lalr_destroy_collec(KevLALRCollection* collec);
static bool kev_lalr_itemset_equal(KevItemSet* itemset1, KevItemSet* itemset2);

KevLRCollection* kev_lalr_generate(KevSymbol* start, KevSymbol** lookahead, size_t length) {
  KevLALRCollection* collec = kev_lalr_get_empty_collec();
  if (!collec) return NULL;
  /* TODO: add a new rules to construct augmented grammar */
  collec->symbols = kev_lr_get_symbol_array(start, &collec->symbol_no);
  if (!collec->symbols) {
    kev_lalr_destroy_collec(collec);
    return NULL;
  }
  kev_lr_symbol_array_partition(collec->symbols, collec->symbol_no);
  collec->terminal_no = kev_label_symbols(collec->symbols, collec->symbol_no);
  collec->firsts = kev_lr_compute_first_array(collec->symbols, collec->symbol_no, collec->terminal_no);
  if (!collec->firsts) {
    kev_lalr_destroy_collec(collec);
    return NULL;
  }
  KevItemSet* start_iset = kev_get_start_itemset(start, lookahead, length);
  if (!start_iset) {
    kev_lalr_destroy_collec(collec);
    return NULL;
  }
  KevAddrArray* array = kev_compute_all_itemsets(start_iset, collec);
  if (!array) {
    kev_lalr_destroy_collec(collec);
    return NULL;
  }
  return NULL;
}

static size_t kev_label_symbols(KevSymbol** symbols, size_t symbol_no) {
  size_t number = 0;
  size_t i = 0;
  while (symbols[i]->kind == KEV_LR_SYMBOL_TERMINAL) {
    symbols[i]->tmp_id = i;
    ++i;
  }
  number = i;
  for (; i < symbol_no; ++i)
    symbols[i]->tmp_id = i;
  return number;
}

static KevLALRCollection* kev_lalr_get_empty_collec(void) {
  KevLALRCollection* collec = (KevLALRCollection*)malloc(sizeof (KevLALRCollection));
  if (!collec) return NULL;
  collec->firsts = NULL;
  collec->symbols = NULL;
  collec->itemsets = NULL;
  collec->propagation = NULL;
  return collec;
}

static void kev_lalr_destroy_collec(KevLALRCollection* collec) {
  if (collec->firsts)
    kev_lr_destroy_first_array(collec->firsts, collec->symbol_no);
  if (collec->itemsets)
    /* destroy itemsets */;
  if (collec->symbols)
    free(collec->symbols);
  free(collec);
}

static KevItemSet* kev_get_start_itemset(KevSymbol* start, KevSymbol** lookahead, size_t length) {
  KevBitSet* la_bitset = kev_lr_symbols_to_bitset(lookahead, length);
  KevItemSet* iset = kev_lr_itemset_create();
  if (!iset || !la_bitset) {
    kev_bitset_delete(la_bitset);
    kev_lr_itemset_delete(iset);
    return NULL;
  }
  KevRuleNode* node = start->rules;
  while (node) {
    KevKernelItem* item = kev_lr_kernel_item_create(node->rule, 0);
    if (!item || !(item->lookahead = kev_bitset_create_copy(la_bitset))) {
      kev_lr_itemset_delete(iset);
      kev_bitset_delete(la_bitset);
      return NULL;
    }
    kev_lr_itemset_add_item(iset, item);
    node = node->next;
  }
  kev_bitset_delete(la_bitset);
  return iset;
}

static KevAddrArray* kev_compute_all_itemsets(KevItemSet* start_iset, KevLALRCollection* collec) {
  KevAddrArray* itemset_array = kev_addrarray_create();
  KevItemSetSet* iset_set = kev_itemsetset_create(16, kev_lalr_itemset_equal);
  KevAddrArray* closure = kev_addrarray_create();
  KevBitSet** la_symbols = (KevBitSet**)malloc(sizeof (KevBitSet*) * collec->symbol_no);
  KevGotoMap* goto_container = kev_gotomap_create(16);
  if (!itemset_array || !closure || !la_symbols || !goto_container || !iset_set) {
    kev_addrarray_delete(itemset_array);
    kev_addrarray_delete(closure);
    kev_gotomap_delete(goto_container);
    kev_itemsetset_delete(iset_set);
    free(la_symbols);
    return NULL;
  }

  for (size_t i = 0; i < collec->symbol_no; ++i)
    la_symbols[i] = NULL;
  if (!kev_addrarray_push_back(itemset_array, start_iset) || !kev_itemsetset_insert(iset_set, start_iset)) {
    kev_addrarray_delete(itemset_array);
    kev_addrarray_delete(closure);
    kev_gotomap_delete(goto_container);
    kev_itemsetset_delete(iset_set);
    free(la_symbols);
    return NULL;
  }

  for (size_t i = 0; i < kev_addrarray_size(itemset_array); ++i) {
    KevItemSet* itemset = kev_addrarray_visit(itemset_array, i);
    if (!kev_compute_itemset(itemset, closure, la_symbols, collec->firsts, collec->terminal_no, goto_container)) {
      kev_addrarray_delete(itemset_array);
      kev_addrarray_delete(closure);
      kev_gotomap_delete(goto_container);
      kev_itemsetset_delete(iset_set);
      free(la_symbols);
      return NULL;
    }
    /* TODO: merge gotos and add propagation */
  }
}

static bool kev_merge_gotos(KevItemSetSet* iset_set, KevAddrArray* itemset_array, KevLALRCollection* collec, KevItemSet* itemset) {
  KevItemSetGoto* gotos = itemset->gotos;
  for (; gotos; gotos = gotos->next) {
    KevItemSet* target = gotos->itemset;
    KevItemSetSetNode* node = kev_itemsetset_search(iset_set, target);
    if (node) {
      if (!kev_merge_itemset(target, node->element, itemset, collec)) {
        for (; gotos; gotos = gotos->next) {
          kev_lr_itemset_delete(gotos->itemset);
          gotos->itemset = NULL;
        }
        return false;
      }
      kev_lr_itemset_delete(target);
      gotos->itemset = node->element;
    } else {
      if (!kev_add_new_itemset(target, iset_set, itemset, collec)) {
        for (; gotos; gotos = gotos->next) {
          kev_lr_itemset_delete(gotos->itemset);
          gotos->itemset = NULL;
        }
        return false;
      }
    }
  }
  return true;
}

static bool kev_compute_itemset(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon, KevGotoMap* goto_container) {
  if (!kev_lalr_closure(itemset, closure, la_symbols, firsts, epsilon))
    return false;
  if (!kev_generate_gotos(itemset, closure, la_symbols, goto_container))
    return false;
  return true;
}

static bool kev_generate_gotos(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevGotoMap* goto_container) {
  kev_gotomap_make_empty(goto_container);
  /* for kernel item */
  KevKernelItem* kitem = itemset->items;
  for (; kitem; kitem = kitem->next) {
    KevRule* rule = kitem->rule;
    if (rule->bodylen == kitem->dot) continue;
    KevSymbol* symbol = rule->body[kitem->dot];
    KevKernelItem* item = kev_lr_kernel_item_create(rule, kitem->dot + 1);
    if (!item) return false;
    if (!(item->lookahead = kev_bitset_create_copy(kitem->lookahead))) {
      kev_lr_kernel_item_delete(item);
      return false;
    }
    KevGotoMapNode* node = kev_gotomap_search(goto_container, symbol);
    if (node) {
      kev_lr_itemset_add_item(node->value, item);
    } else {
      KevItemSet* iset = kev_lr_itemset_create();
      if (!iset) {
        kev_lr_kernel_item_delete(item);
        return false;
      }
      kev_lr_itemset_add_item(iset, item);
      if (!kev_lr_itemset_goto(itemset, symbol, iset) ||
          !kev_gotomap_insert(goto_container, symbol, iset)) {
        kev_lr_itemset_delete(iset);
        return false;
      }
    }
  }

  /* for non-kernel item */
  size_t closure_size = kev_addrarray_size(closure);
  for (size_t i = 0; i < closure_size; ++i) {
    KevSymbol* head = *(KevSymbol**)kev_addrarray_visit(closure, i);
    KevRuleNode* rulenode = head->rules;
    for (; rulenode; rulenode = rulenode->next) {
      KevRule* rule = rulenode->rule;
      if (rule->bodylen == 0) continue;
      KevSymbol* symbol = rule->body[0];
      KevKernelItem* item = kev_lr_kernel_item_create(rule, 1);
      if (!item) return false;
      if (!(item->lookahead = kev_bitset_create_copy(la_symbols[head->tmp_id]))) {
        kev_lr_kernel_item_delete(item);
        return false;
      }
      KevGotoMapNode* node = kev_gotomap_search(goto_container, symbol);
      if (node) {
        kev_lr_itemset_add_item(node->value, item);
      } else {
        KevItemSet* iset = kev_lr_itemset_create();
        if (!iset) {
          kev_lr_kernel_item_delete(item);
          return false;
        }
        kev_lr_itemset_add_item(iset, item);
        if (!kev_lr_itemset_goto(itemset, symbol, iset) ||
            !kev_gotomap_insert(goto_container, symbol, iset)) {
          kev_lr_itemset_delete(iset);
          return false;
        }
      }
    }
  }
  return true;
}

static inline bool kev_init_kitem_la(KevItemSet* itemset, size_t epsilon) {
  KevKernelItem* kitem = itemset->items;
  size_t id = epsilon + 1;
  while (kitem) {
    kitem->lookahead = kev_bitset_create(epsilon + 3);
    if (!kitem->lookahead ||
        !kev_bitset_set(kitem->lookahead, id++)) {
      return false;
    }
    kitem = kitem->next;
  }
  return true;
}

static KevBitSet* kev_lalr_follows(KevKernelItem* kitem, KevBitSet** firsts, size_t epsilon) {
  size_t len = kitem->rule->bodylen;
  KevSymbol** rulebody = kitem->rule->body;
  KevBitSet* follows = kev_bitset_create(epsilon + 3);
  if (!follows) return NULL;
  for (size_t i = kitem->dot; i < len; ++i) {
    if (rulebody[i]->kind == KEV_LR_SYMBOL_TERMINAL) {
      kev_bitset_set(follows, rulebody[i]->tmp_id);
      return follows;
    }
    KevBitSet* set = firsts[rulebody[i]->tmp_id];
    if (!kev_bitset_union(follows, set)) {
      kev_bitset_delete(follows);
      return NULL;
    }
    if (!kev_bitset_has_element(set, epsilon)) {
      kev_bitset_clear(follows, epsilon);
      return follows;
    }
  }
  kev_bitset_clear(follows, epsilon);
  if (!kev_bitset_union(follows, kitem->lookahead)) {
    kev_bitset_delete(follows);
    return NULL;
  }
  return follows;
}

static bool kev_lalr_closure(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon) {
  if (kev_init_kitem_la(itemset, epsilon)) return false;
  KevKernelItem* kitem = itemset->items;
  while (kitem) {
    if (kitem->dot == kitem->rule->bodylen) {
      kitem = kitem->next;
      continue;
    }
    KevSymbol* symbol = kitem->rule->body[kitem->dot];
    if (symbol->kind == KEV_LR_SYMBOL_TERMINAL) {
      kitem = kitem->next;
      continue;
    }
    KevBitSet* la = kev_lalr_follows(kitem, firsts, epsilon);
    if (!la) return false;
    size_t index = symbol->tmp_id;
    if (la_symbols[index]) {
      if (!kev_bitset_union(la_symbols[index], la)) {
        kev_bitset_delete(la);
        return false;
      }
      kev_bitset_delete(la);
    } else {
      la_symbols[index] = la;
      if (!kev_addrarray_push_back(closure, symbol))
        return false;
    }
    kitem = kitem->next;
  }
  return kev_lalr_closure_propagate(itemset, closure, la_symbols, firsts, epsilon);
}

static bool kev_lalr_closure_propagate(KevItemSet* itemset, KevAddrArray* closure, KevBitSet** la_symbols, KevBitSet** firsts, size_t epsilon) {
  size_t closure_size = kev_addrarray_size(closure);
  bool propagated = true;
  while (propagated) {
    propagated = false;
    for (size_t i = 0; i < closure_size; ++i) {
      KevSymbol* symbol = *(KevSymbol**)kev_addrarray_visit(closure, i);
      KevRuleNode* node = symbol->rules;
      for (; node; node = node->next) {
        size_t i = 0;
        size_t len = node->rule->bodylen;
        if (len == 0) continue;
        KevSymbol** body = node->rule->body;
        for (; i < len; ++i) {
          if (body[i]->kind == KEV_LR_SYMBOL_TERMINAL || 
              kev_bitset_has_element(la_symbols[body[i]->tmp_id], epsilon))
            break;
        }
        if (i != len) continue;
        if (!kev_bitset_is_subset(la_symbols[symbol->tmp_id], la_symbols[body[0]->tmp_id])) {
          propagated = true;
          if (!kev_bitset_union(la_symbols[body[0]->tmp_id], la_symbols[symbol->tmp_id]))
            return false;
        }
      }
    }
  }
  return true;
}

static bool kev_lalr_itemset_equal(KevItemSet* itemset1, KevItemSet* itemset2) {
  KevKernelItem* kitem1 = itemset1->items;
  KevKernelItem* kitem2 = itemset2->items;
  while (kitem1 && kitem2) {
    if (kitem1->rule != kitem2->rule || kitem1->dot != kitem2->dot)
      return false;
    kitem1 = kitem1->next;
    kitem2 = kitem2->next;
  }
  return !(kitem1 || kitem2);
}

static void kev_clear_la_symbols_and_closure(KevAddrArray* closure, KevBitSet** la_symbols) {
  size_t size = kev_addrarray_size(closure);
  for (size_t i = 0; i < size; ++i) {
    size_t index = ((KevSymbol*)kev_addrarray_visit(closure, i))->tmp_id;
    kev_bitset_delete(la_symbols[index]);
    la_symbols[index] = NULL;
  }
  kev_addrarray_make_empty(closure);
}
