#include "klang/include/value/klclosure.h"
#include "utils/include/utils/utils.h"
#include <string.h>


static KlGCObject* klkclosure_propagate(KlKClosure* kclo, KlGCObject* gclist);
static KlGCObject* klcclosure_propagate(KlCClosure* cclo, KlGCObject* gclist);
static void klkclosure_delete(KlKClosure* kclo);
static void klcclosure_delete(KlCClosure* cclo);

static KlGCVirtualFunc klkclo_gcvfunc = { .destructor = (KlGCDestructor)klkclosure_delete, .propagate = (KlGCProp)klkclosure_propagate };
static KlGCVirtualFunc klcclo_gcvfunc = { .destructor = (KlGCDestructor)klcclosure_delete, .propagate = (KlGCProp)klcclosure_propagate };


KlKClosure* klkclosure_create(KlMM* klmm, KlKFunction* kfunc, KlValue* stkbase, KlRef** openreflist, KlRef** refs) {
  KlKClosure* kclo = (KlKClosure*)klmm_alloc(klmm, sizeof (KlKClosure) + sizeof (KlRef*) * kfunc->nref);
  if (k_unlikely(!kclo)) return NULL;
  klmm_newlevel(klmm, klmm_to_gcobj(kclo));
  kclo->kfunc = kfunc;
  size_t nref = kfunc->nref;
  kclo->nref = nref;
  KlRefInfo* refinfo = kfunc->refinfo;
  KlRef** ref = kclo->refs;
  KlRefInfo* end = refinfo + nref;
  for (; refinfo != end; ++refinfo, ++ref) {
    if (!refinfo->in_stack) {
      *ref = refs[refinfo->index];
      continue;
    }
    KlRef* newref = klref_get(klmm, openreflist, stkbase + refinfo->index);
    if (k_unlikely(!newref)) {
      klmm_newlevel_abort(klmm);
      klmm_free(klmm, kclo, sizeof (KlKClosure) + sizeof (KlRef*) * kfunc->nref);
      return NULL;
    }
    *ref = newref;
  }

  klmm_newlevel_done(klmm, &klkclo_gcvfunc);
  return kclo;
}

static void klkclosure_delete(KlKClosure* kclo) {
  klmm_free(klmm_gcobj_getmm(klmm_to_gcobj(kclo)), kclo, sizeof (KlKClosure) + sizeof (KlRef*) * kclo->nref);
}

static KlGCObject* klkclosure_propagate(KlKClosure* kclo, KlGCObject* gclist) {
  klmm_gcobj_mark_accessible(klmm_to_gcobj(kclo->kfunc), gclist);
  for (size_t i = 0; i < kclo->nref; ++i) {
    if (klref_closed(kclo->refs[i]))
      klmm_gcobj_mark_accessible(klmm_to_gcobj(kclo->refs[i]), gclist);
  }
  return gclist;
}

KlCClosure* klcclosure_create(KlMM* klmm, KlCFunction* cfunc, KlValue* stkbase, KlRef** openreflist, size_t nref) {
  KlCClosure* cclo = (KlCClosure*)klmm_alloc(klmm, sizeof (KlKClosure) + sizeof (KlRef*) * nref);
  if (k_unlikely(!cclo)) return NULL;
  klmm_newlevel(klmm, klmm_to_gcobj(cclo));
  cclo->cfunc = cfunc;
  cclo->nref = nref;
  KlRef** ref = cclo->refs;
  KlRef** end = ref + nref;
  for (; ref != end; ++ref) {
    KlRef* newref = klref_get(klmm, openreflist, stkbase++);
    if (k_unlikely(!newref)) {
      klmm_newlevel_abort(klmm);
      klmm_free(klmm, cclo, sizeof (KlCClosure) + sizeof (KlRef*) * cclo->nref);
      return NULL;
    }
    *ref = newref;
  }

  klmm_newlevel_done(klmm, &klcclo_gcvfunc);
  return cclo;
}

static void klcclosure_delete(KlCClosure* cclo) {
  klmm_free(klmm_gcobj_getmm(klmm_to_gcobj(cclo)), cclo, sizeof (KlCClosure) + sizeof (KlRef*) * cclo->nref);
}

static KlGCObject* klcclosure_propagate(KlCClosure* cclo, KlGCObject* gclist) {
  for (size_t i = 0; i < cclo->nref; ++i) {
    klmm_gcobj_mark_accessible(klmm_to_gcobj(cclo->refs[i]), gclist);
  }
  return gclist;
}