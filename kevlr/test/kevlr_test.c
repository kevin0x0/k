#include "kevlr/include/lr.h"

#include <time.h>

bool conflict_report(void* object, KevLRConflict* conflict, KevLRCollection* collec) {
  printf("All conflict items here:\n");
  kev_lr_print_itemset(stdout, collec, conflict->conflct_items, false);
  return false;
}

int main(int argc, char** argv) {
  KevSymbol* end = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "$");
  KevSymbol* id = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "id");
  KevSymbol* plus = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "+");
  KevSymbol* minus = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "-");
  KevSymbol* mul = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "*");
  KevSymbol* div = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "/");
  KevSymbol* op5 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "|");
  KevSymbol* op6 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "&");
  KevSymbol* op7 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "^");
  KevSymbol* op8 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "||");
  KevSymbol* op9 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "&&");
  KevSymbol* op10 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "<<");
  KevSymbol* op11 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, ">>");
  KevSymbol* op12 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "<");
  KevSymbol* op13 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, ">");
  KevSymbol* op14 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, ">=");
  KevSymbol* op15 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "<=");
  KevSymbol* op16 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "==");
  KevSymbol* op17 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "!=");
  KevSymbol* op18 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "=");
  KevSymbol* op19 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "->");
  KevSymbol* op20 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "[");
  KevSymbol* op21 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "]");
  KevSymbol* op22 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, ".");
  KevSymbol* op23 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "++");
  KevSymbol* op24 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "--");
  KevSymbol* op25 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "!");
  KevSymbol* op26 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "~");
  KevSymbol* op27 = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "&");
  KevSymbol* openp = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "(");
  KevSymbol* closep = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, ")");
  KevSymbol* E = kev_lr_symbol_create(KEV_LR_SYMBOL_NONTERMINAL, "E");

  KevSymbol* body1[] = { E, plus, E };
  KevSymbol* body2[] = { E, minus, E };
  KevSymbol* body3[] = { E, mul, E };
  KevSymbol* body4[] = { E, div, E };
  KevSymbol* body5[] = { plus, E };
  KevSymbol* body6[] = { minus, E };
  KevSymbol* body7[] = { openp, E, closep };
  KevSymbol* body8[] = { id };
  KevSymbol* body9[] =  { E, op5, E };
  KevSymbol* body10[] = { E, op6, E };
  KevSymbol* body11[] = { E, op7, E };
  KevSymbol* body12[] = { E, op8, E };
  KevSymbol* body13[] = { E, op9, E };
  KevSymbol* body14[] = { E, op10, E };
  KevSymbol* body15[] = { E, op11, E };
  KevSymbol* body16[] = { E, op12, E };
  KevSymbol* body17[] = { E, op13, E };
  KevSymbol* body18[] = { E, op14, E };
  KevSymbol* body19[] = { E, op15, E };
  KevSymbol* body20[] = { E, op16, E };
  KevSymbol* body21[] = { E, op17, E };
  KevSymbol* body22[] = { E, op18, E };
  KevSymbol* body23[] = { E, op19, E };
  KevSymbol* body24[] = { E, op20, E, op21 };
  KevSymbol* body25[] = { E, op22 };
  KevSymbol* body26[] = { E, op23 };
  KevSymbol* body27[] = { E, op24 };
  KevSymbol* body28[] = { op23, E };
  KevSymbol* body29[] = { op24, E };
  KevSymbol* body30[] = { op25, E };
  KevSymbol* body31[] = { op26, E };
  KevSymbol* body32[] = { op27, E };

  KevRule* rule1 = kev_lr_rule_create(E, body1, (sizeof body1 / sizeof body1[0]));
  KevRule* rule2 = kev_lr_rule_create(E, body2, (sizeof body2 / sizeof body2[0]));
  KevRule* rule3 = kev_lr_rule_create(E, body3, (sizeof body3 / sizeof body3[0]));
  KevRule* rule4 = kev_lr_rule_create(E, body4, (sizeof body4 / sizeof body4[0]));
  KevRule* rule5 = kev_lr_rule_create(E, body5, (sizeof body5 / sizeof body5[0]));
  KevRule* rule6 = kev_lr_rule_create(E, body6, (sizeof body6 / sizeof body6[0]));
  KevRule* rule7 = kev_lr_rule_create(E, body7, (sizeof body7 / sizeof body7[0]));
  KevRule* rule8 = kev_lr_rule_create(E, body8, (sizeof body8 / sizeof body8[0]));
  KevRule* rule9 = kev_lr_rule_create(E, body9, (sizeof body9 / sizeof body9[0]));
  KevRule* rule10 = kev_lr_rule_create(E, body10, (sizeof body10 / sizeof body10[0]));
  KevRule* rule11 = kev_lr_rule_create(E, body11, (sizeof body11 / sizeof body11[0]));
  KevRule* rule12 = kev_lr_rule_create(E, body12, (sizeof body12 / sizeof body12[0]));
  KevRule* rule13 = kev_lr_rule_create(E, body13, (sizeof body13 / sizeof body13[0]));
  KevRule* rule14 = kev_lr_rule_create(E, body14, (sizeof body14 / sizeof body14[0]));
  KevRule* rule15 = kev_lr_rule_create(E, body15, (sizeof body15 / sizeof body15[0]));
  KevRule* rule16 = kev_lr_rule_create(E, body16, (sizeof body16 / sizeof body16[0]));
  KevRule* rule17 = kev_lr_rule_create(E, body17, (sizeof body17 / sizeof body17[0]));
  KevRule* rule18 = kev_lr_rule_create(E, body18, (sizeof body18 / sizeof body18[0]));
  KevRule* rule19 = kev_lr_rule_create(E, body19, (sizeof body19 / sizeof body19[0]));
  KevRule* rule20 = kev_lr_rule_create(E, body20, (sizeof body20 / sizeof body20[0]));
  KevRule* rule21 = kev_lr_rule_create(E, body21, (sizeof body21 / sizeof body21[0]));
  KevRule* rule22 = kev_lr_rule_create(E, body22, (sizeof body22 / sizeof body22[0]));
  KevRule* rule23 = kev_lr_rule_create(E, body23, (sizeof body23 / sizeof body23[0]));
  KevRule* rule24 = kev_lr_rule_create(E, body24, (sizeof body24 / sizeof body24[0]));
  KevRule* rule25 = kev_lr_rule_create(E, body25, (sizeof body25 / sizeof body25[0]));
  KevRule* rule26 = kev_lr_rule_create(E, body26, (sizeof body26 / sizeof body26[0]));
  KevRule* rule27 = kev_lr_rule_create(E, body27, (sizeof body27 / sizeof body27[0]));
  KevRule* rule28 = kev_lr_rule_create(E, body28, (sizeof body28 / sizeof body28[0]));
  KevRule* rule29 = kev_lr_rule_create(E, body29, (sizeof body29 / sizeof body29[0]));
  KevRule* rule30 = kev_lr_rule_create(E, body30, (sizeof body30 / sizeof body30[0]));
  KevRule* rule31 = kev_lr_rule_create(E, body31, (sizeof body31 / sizeof body31[0]));
  KevRule* rule32 = kev_lr_rule_create(E, body32, (sizeof body32 / sizeof body32[0]));

  end->id = 0;
  id->id = 1;
  plus->id = 2;
  minus->id = 3;
  mul->id = 4;
  div->id = 5;
  op5->id = 6;
  op6->id = 7;
  op7->id = 8;
  op8->id = 9;
  op9->id = 10;
  op10->id = 11;
  op11->id = 12;
  op12->id = 13;
  op13->id = 14;
  op14->id = 15;
  op15->id = 16;
  op16->id = 17;
  op17->id = 18;
  op18->id = 19;
  op19->id = 20;
  op20->id = 21;
  op21->id = 22;
  op22->id = 23;
  op23->id = 24;
  op24->id = 25;
  op25->id = 26;
  op26->id = 27;
  op27->id = 28;
  openp->id = 29;
  closep->id = 30;
  E->id = 31;

  rule1->id = 0;
  rule2->id = 1;
  rule3->id = 2;
  rule4->id = 3;
  rule5->id = 4;
  rule6->id = 5;
  rule7->id = 6;
  rule8->id = 7;
  rule9->id = 8;
  rule10->id = 9;
  rule11->id = 10;
  rule12->id = 11;
  rule13->id = 12;
  rule14->id = 13;
  rule15->id = 14;
  rule16->id = 15;
  rule17->id = 16;
  rule18->id = 17;
  rule19->id = 18;
  rule20->id = 19;
  rule21->id = 20;
  rule22->id = 21;
  rule23->id = 22;
  rule24->id = 23;
  rule25->id = 24;
  rule26->id = 25;
  rule27->id = 26;
  rule28->id = 27;
  rule29->id = 28;
  rule30->id = 29;
  rule31->id = 30;
  rule32->id = 31;

  clock_t t = clock();
  for (size_t i = 0; i < 1000; ++i) {
    KevLRCollection* collec = kev_lr_collection_create_lr1(E, &end, 1);
    //kev_lr_print_collection(stdout, collec, true);

    //KevLRTable* table = kev_lr_table_create(collec, NULL);
    //kev_lr_print_goto_table(stdout, table);
    //kev_lr_print_action_table(stdout, table);
    //kev_lr_table_delete(table);
    kev_lr_collection_delete(collec);
  }
  printf("total time used:%fs\n", ((float)(clock() - t)) / (float)CLOCKS_PER_SEC);

  kev_lr_symbol_delete(end);
  kev_lr_symbol_delete(id);
  kev_lr_symbol_delete(plus);
  kev_lr_symbol_delete(minus);
  kev_lr_symbol_delete(mul);
  kev_lr_symbol_delete(div);
  kev_lr_symbol_delete(openp);
  kev_lr_symbol_delete(closep);
  kev_lr_symbol_delete(E);
  kev_lr_symbol_delete(op5);
  kev_lr_symbol_delete(op6);
  kev_lr_symbol_delete(op7);
  kev_lr_symbol_delete(op8);
  kev_lr_symbol_delete(op9);
  kev_lr_symbol_delete(op10);
  kev_lr_symbol_delete(op11);
  kev_lr_symbol_delete(op12);
  kev_lr_symbol_delete(op13);
  kev_lr_symbol_delete(op14);
  kev_lr_symbol_delete(op15);
  kev_lr_symbol_delete(op16);
  kev_lr_symbol_delete(op17);
  kev_lr_symbol_delete(op18);
  kev_lr_symbol_delete(op19);
  kev_lr_symbol_delete(op20);
  kev_lr_symbol_delete(op21);
  kev_lr_symbol_delete(op22);
  kev_lr_symbol_delete(op23);
  kev_lr_symbol_delete(op24);
  kev_lr_symbol_delete(op25);
  kev_lr_symbol_delete(op26);
  kev_lr_symbol_delete(op27);

  kev_lr_rule_delete(rule1);
  kev_lr_rule_delete(rule2);
  kev_lr_rule_delete(rule3);
  kev_lr_rule_delete(rule4);
  kev_lr_rule_delete(rule5);
  kev_lr_rule_delete(rule6);
  kev_lr_rule_delete(rule7);
  kev_lr_rule_delete(rule8);
  kev_lr_rule_delete(rule9);
  kev_lr_rule_delete(rule10);
  kev_lr_rule_delete(rule11);
  kev_lr_rule_delete(rule12);
  kev_lr_rule_delete(rule13);
  kev_lr_rule_delete(rule14);
  kev_lr_rule_delete(rule15);
  kev_lr_rule_delete(rule16);
  kev_lr_rule_delete(rule17);
  kev_lr_rule_delete(rule18);
  kev_lr_rule_delete(rule19);
  kev_lr_rule_delete(rule20);
  kev_lr_rule_delete(rule21);
  kev_lr_rule_delete(rule22);
  kev_lr_rule_delete(rule23);
  kev_lr_rule_delete(rule24);
  kev_lr_rule_delete(rule25);
  kev_lr_rule_delete(rule26);
  kev_lr_rule_delete(rule27);
  kev_lr_rule_delete(rule28);
  kev_lr_rule_delete(rule29);
  kev_lr_rule_delete(rule30);
  kev_lr_rule_delete(rule31);
  kev_lr_rule_delete(rule32);



  //KevSymbol* S = kev_lr_symbol_create(KEV_LR_SYMBOL_NONTERMINAL, "S");
  //KevSymbol* L = kev_lr_symbol_create(KEV_LR_SYMBOL_NONTERMINAL, "L");
  //KevSymbol* R = kev_lr_symbol_create(KEV_LR_SYMBOL_NONTERMINAL, "R");
  //KevSymbol* id = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "id");
  //KevSymbol* star = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "*");
  //KevSymbol* assign = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "=");
  //KevSymbol* end = kev_lr_symbol_create(KEV_LR_SYMBOL_TERMINAL, "$");
  //KevSymbol* body1[3] = { L, assign, R };
  //KevSymbol* body2[2] = { star, R };
  //KevRule* rule1 = kev_lr_rule_create(S, &R, 1);
  //KevRule* rule2 = kev_lr_rule_create(L, &id, 1);
  //KevRule* rule3 = kev_lr_rule_create(R, &L, 1);
  //KevRule* rule4 = kev_lr_rule_create(S, body1, 3);
  //KevRule* rule5 = kev_lr_rule_create(L, body2, 2);
  //KevRule* rule6 = kev_lr_rule_create(R, NULL, 0);
  //KevLRCollection* collec = kev_lr_collection_create_slr(S, &end, 1);
  //
  //KevBitSet** follows = kev_lr_util_compute_follows(collec->symbols, collec->firsts, collec->symbol_no, collec->terminal_no, collec->start, &end, 1);
  //
  //for (size_t i = collec->terminal_no; i < collec->symbol_no; ++i) {
  //  fprintf(stdout, "%s ", collec->symbols[i]->name);
  //  kev_lr_print_terminal_set(stdout, collec, collec->firsts[i]);
  //  fputc('\n', stdout);
  //}
  //for (size_t i = collec->terminal_no; i < collec->symbol_no; ++i) {
  //  kev_bitset_delete(follows[i]);
  //}
  //free(follows);
  //
  //kev_lr_print_collection(stdout, collec, true);
  //
  //S->id = 0;
  //L->id = 1;
  //R->id = 2;
  //id->id = 3;
  //star->id = 4;
  //assign->id = 5;
  //end->id = 6;
  //
  //rule1->id = 0;
  //rule2->id = 1;
  //rule3->id = 2;
  //rule4->id = 3;
  //rule5->id = 4;
  //rule6->id = 5;
  //
  //KevLRConflictHandler* handler = kev_lr_conflict_handler_create(NULL, conflict_report);
  //KevLRTable* table = kev_lr_table_create(collec, handler);
  //fputc('\n', stdout);
  //fputc('\n', stdout);
  //fputc('\n', stdout);
  //kev_lr_print_symbols(stdout, collec);
  //fputc('\n', stdout);
  //fprintf(stdout, "%d ", 0);
  //kev_lr_print_rule(stdout, rule1);
  //fputc('\n', stdout);
  //fprintf(stdout, "%d ", 1);
  //kev_lr_print_rule(stdout, rule2);
  //fputc('\n', stdout);
  //fprintf(stdout, "%d ", 2);
  //kev_lr_print_rule(stdout, rule3);
  //fputc('\n', stdout);
  //fprintf(stdout, "%d ", 3);
  //kev_lr_print_rule(stdout, rule4);
  //fputc('\n', stdout);
  //fprintf(stdout, "%d ", 4);
  //kev_lr_print_rule(stdout, rule5);
  //fputc('\n', stdout);
  //fputc('\n', stdout);
  //kev_lr_print_goto_table(stdout, table);
  //fputc('\n', stdout);
  //kev_lr_print_action_table(stdout, table);
  //kev_lr_conflict_handler_delete(handler);
  //kev_lr_collection_delete(collec);
  //kev_lr_table_delete(table);
  //
  //kev_lr_rule_delete(rule1);
  //kev_lr_rule_delete(rule2);
  //kev_lr_rule_delete(rule3);
  //kev_lr_rule_delete(rule4);
  //kev_lr_rule_delete(rule5);
  //kev_lr_rule_delete(rule6);
  //kev_lr_symbol_delete(L);
  //kev_lr_symbol_delete(R);
  //kev_lr_symbol_delete(S);
  //kev_lr_symbol_delete(id);
  //kev_lr_symbol_delete(star);
  //kev_lr_symbol_delete(assign);
  //kev_lr_symbol_delete(end);
  return 0;
}
