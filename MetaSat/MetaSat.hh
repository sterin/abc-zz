//_________________________________________________________________________________________________
//|                                                                                      -- INFO --
//| Name        : MetaSat.hh
//| Author(s)   : Niklas Een
//| Module      : MetaSat
//| Description : Wrapper for non-proof-logging SAT solvers supporting the assumption interface.
//| 
//| (C) Copyright 2010-2012, The Regents of the University of California
//|________________________________________________________________________________________________
//|                                                                                  -- COMMENTS --
//| 
//|________________________________________________________________________________________________


#ifndef ZZ__MetaSat__MetaSat_hh
#define ZZ__MetaSat__MetaSat_hh

#include "ZZ/Generics/Lit.hh"


// Forward declarations:
namespace Minisat{
    struct Solver;
}


namespace ZZ {
using namespace std;


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// 'MetaSat' base class:


struct MetaSat {
  //________________________________________
  //  Construction:

    virtual ~MetaSat() {}
    virtual void clear(bool dealloc = true) = 0;

  //________________________________________
  //  Problem specification:

    virtual Lit   True() const = 0;
    virtual Lit   addLit() = 0;
    virtual void  addClause_(const Vec<Lit>& ps) = 0;

  //________________________________________
  //  Solving:

    virtual void  setConflictLim(uint64 n_confl) = 0;       // -- Affects the next call to 'solve()' only.
    virtual lbool solve_(const Vec<Lit>& assumps) = 0;
    virtual void  randomizeVarOrder(uint64 seed) = 0;       // -- EXPERIMENTAL.

  //________________________________________
  //  Result extraction:

    virtual bool  okay() const = 0;
    virtual lbool value_(uint x) const = 0;                 // -- Last operation must have been 'solve()', not 'addClause()'.
    virtual void  getModel(Vec<lbool>& m) const = 0;
    virtual void  getConflict(Vec<Lit>& assump_confl) = 0;  // -- Returns a cube (not a clause) which is a subset of 'assumps' passed to 'solve()'.

  //________________________________________
  //  Statistics:

    virtual uint  nClauses() const = 0;
    virtual uint  nLearnts() const = 0;
    virtual uint  nVars () const = 0;

  //________________________________________
  //  Preprocessing:

    virtual void  freeze(uint x) = 0;                      // }- Has no effect except for simplifying solver.
    virtual void  thaw(uint x) = 0;                        // }
    virtual void  preprocess(bool final_call) = 0;         // -- If 'final_call' is TRUE, internal data will be freed to save memory, but no more preprocessing is possible.
    virtual void  getCnf(Vec<Lit>& out_cnf) = 0;           // -- Read back CNF as a sequence of clauses separated by 'lit_Undef's.

  //________________________________________
  //  Debug:

    virtual void  setVerbosity(int verb_level) = 0;        // -- '0' means no output.

  //________________________________________
  //  Convenience:

    void  addClause(const Vec<Lit>& ps)         { addClause_(ps); }
    void  addClause(Lit p)                      { cl_tmp.setSize(1, Lit_NULL); cl_tmp[0] = p; addClause(cl_tmp); }
    void  addClause(Lit p, Lit q)               { cl_tmp.setSize(2, Lit_NULL); cl_tmp[0] = p; cl_tmp[1] = q; addClause(cl_tmp); }
    void  addClause(Lit p, Lit q, Lit r)        { cl_tmp.setSize(3, Lit_NULL); cl_tmp[0] = p; cl_tmp[1] = q; cl_tmp[2] = r; addClause(cl_tmp); }
    void  addClause(Lit p, Lit q, Lit r, Lit s) { cl_tmp.setSize(4, Lit_NULL); cl_tmp[0] = p; cl_tmp[1] = q; cl_tmp[2] = r; cl_tmp[3] = s; addClause(cl_tmp); }

    lbool solve(const Vec<Lit>& assumps) { return solve_(assumps); }
    lbool solve()                        { Vec<Lit> tmp; return solve(tmp); }
    lbool solve(Lit p)                   { Vec<Lit> tmp; tmp.push(p); return solve(tmp); }
    lbool solve(Lit p, Lit q)            { Vec<Lit> tmp; tmp.push(p); tmp.push(q); return solve(tmp); }

    lbool value(uint x) const            { return value_(x); }
    lbool value(Lit  p) const            { return value_(p.id) ^ sign(p); }

protected:
    Vec<Lit> cl_tmp;
};


#define MetaSat_OVERRIDES                                               \
    virtual void  clear(bool dealloc);                                  \
    virtual Lit   True() const;                                         \
    virtual Lit   addLit();                                             \
    virtual void  addClause_(const Vec<Lit>& ps);                       \
    virtual void  setConflictLim(uint64 n_confl);                       \
    virtual lbool solve_(const Vec<Lit>& assumps);                      \
    virtual void  randomizeVarOrder(uint64 seed);                       \
    virtual bool  okay() const;                                         \
    virtual lbool value_(uint x) const;                                 \
    virtual void  getModel(Vec<lbool>& m) const;                        \
    virtual void  getConflict(Vec<Lit>& assump_confl);                  \
    virtual uint  nClauses() const;                                     \
    virtual uint  nLearnts() const;                                     \
    virtual uint  nVars () const;                                       \
    virtual void  freeze(uint x);                                       \
    virtual void  thaw(uint x);                                         \
    virtual void  preprocess(bool final_call);                          \
    virtual void  getCnf(Vec<Lit>& out_cnf);                            \
    virtual void  setVerbosity(int verb_level);


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Sub-classes:


struct minisat2_vec_data {
    void* data;
    int   sz;
    int   cap;
};


struct MiniSat2 : MetaSat {
    MiniSat2();
    virtual ~MiniSat2();

    MetaSat_OVERRIDES

private:
    ::Minisat::Solver* S;
    Lit true_lit;
    minisat2_vec_data tmp_lits;
};


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
}
#endif
