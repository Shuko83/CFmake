/*!
\file LibIndeSig.h
\date 03/01/06
\brief Signal-slot implementation for C++ 
\author  Big
\version 1.1
\note:
    Auteur initial : Tero Pulkkinen - Esa Pulkkinen
    N'utilise aucun import et fonctionne avec tous les compilateurs c++
                         
\note:
    Espace de nommage "LibIndeSig"
*/
#ifndef _LIB_INDE_SIG_H
#define _LIB_INDE_SIG_H

//#include "SwMacros.h"

namespace LibIndeSig {
//------------------------------------------------------------------------------
// Transaction
//------------------------------------------------------------------------------
class iTransaction{
public:
  bool enabled;
  iTransaction() {enabled=true;}
};

/*! \brief Factorisation de portions de code de gestion d'objets : doDelete�*/
template<class sli>
static void doDelete(const sli& sl)
{
    for(sli i=sl; i != 0;) {
      sli k=i->next;
      delete i;
      i=k;
    }
}
/*! \brief Factorisation de portions de code de gestion d'objets : doInsert�*/
template<class sli>
static iTransaction * doInsert(sli& sl,const sli& item,int& cpt_cb)
{
     item->next=sl;
     sl=item;
     cpt_cb++;
     return (iTransaction *)item;
}

/*! \brief Factorisation de portions de code de gestion d'objets : doRemove */
template<class sli>
static void doRemove(sli& sl,void * slr,int& cpt_cb) {
    sli i,r;
    r=(sli)slr;
    //Cas c'est le premier
    if (sl==r) {
        sl=r->next;
        cpt_cb--;
        delete r;
    } else {
        //Cas c'est un autre
        for (i=sl; i && i->next!=r; i=i->next) { };
        if (i!=0) {
            i->next=r->next;
            cpt_cb--;
            delete r;
        }
    }
}

//------------------------------------------------------------------------------
// SIGNAL No Param
//------------------------------------------------------------------------------
/*! \brief SIGNAL No Param:template de slot generique */
class Func0Interface : public iTransaction{
public:
  Func0Interface *next;
  Func0Interface() : iTransaction(),next(0) {}
  virtual void call()=0;
  virtual ~Func0Interface() {}
};

/*! \brief SIGNAL No Param:template de specialisation de la class Func0Interface pour la class T */
template<class T>
class Func0 : public Func0Interface {
public:
  typedef void (T::*funcType)();
public:
  T *obj;
  funcType func;
public:
  Func0(T* obj_, funcType f)
    : Func0Interface(),obj(obj_),func(f) {}
  virtual ~Func0() {}
  void call() { if (enabled) (obj->*func)(); }
};

/*! \brief SIGNAL No Param:template de signal */
class iSignal0{
public:
  typedef Func0Interface *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal0() { sl=0; cpt_cb=0; }
  ~iSignal0() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }

  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()() {
	  for (sli i=sl; i;) { sli next=i->next; i->call(); i=next; }
   }
   
  //Connexion
    //connexion et deconnexion de signal
    template<class T> iTransaction * iconnect(T& obj, void (T::*f)()) {
        return insert(new Func0<T>(&obj,f));    
    }
    template<class T> void idisconnect(T& obj,void (T::*f)()) {
        Func0<T> * init_func = nullptr;
        sli r;
        r=sl;
        if (r!=0) init_func=dynamic_cast<Func0<T>*>(r);
        while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
            r=r->next;
            if (r!=0) init_func=dynamic_cast<Func0<T>*>(r);
        }
        if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};

//------------------------------------------------------------------------------
/*! \brief SIGNAL 1 Param:template de slot generique*/
template<class arg1>
class Func1Interface: public iTransaction{
public:
  Func1Interface<arg1> *next;
  Func1Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x)=0;
  virtual ~Func1Interface() {}
  
  virtual Func1Interface<arg1>* MakeNewCopyFunc1() { return 0; }
};

/*! \brief SIGNAL 1 Param:template de specialisation de la class Func1Interface pour la class T */
template<class T, class Arg1>
class Func1 : public Func1Interface<Arg1> {
public:
  typedef void (T::*funcType)(Arg1);
public:
  T *obj;
  funcType func;
public:
  Func1(T* obj_, funcType f)
    : Func1Interface(),obj(obj_),func(f) {}
  void call(Arg1 x) { if (enabled) (obj->*func)(x); }
  
  virtual Func1Interface<Arg1>* MakeNewCopyFunc1() { 
   
    Func1Interface<Arg1>* newInst = new Func1<T,Arg1>(obj,func);
    
    if (this->next!=0) {
        newInst->next = this->next->MakeNewCopyFunc1(); 
    }    

    return newInst;
  }    
};
/*! \brief SIGNAL 1 Param:template de signal */
template<class arg1>
class iSignal1{
public:
  typedef Func1Interface<arg1> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal1() { sl=0; cpt_cb=0; }
  ~iSignal1() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x) {
     for (sli i=sl; i;) { sli next=i->next; i->call(x); i=next; }
   }   

  iSignal1(const iSignal1<arg1>& x) {
    sl = x.sl->MakeNewCopyFunc1();
  }

  iSignal1<arg1>&     operator=(const iSignal1<arg1>& x) {
    sl = x.sl->MakeNewCopyFunc1();
    return *this;
  } 
  
    //connexion et deconnexion de signal
    template<class T, class Arg1>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1)) {
            return insert(new Func1<T,Arg1>(&obj,f));    
    }
    template<class T, class Arg1> 
        void idisconnect(T& obj,void (T::*f)(Arg1)) {
			Func1<T, Arg1> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func1<T,Arg1>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func1<T,Arg1>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
      
};
//------------------------------------------------------------------------------
/*! \brief SIGNAL 2 Param:template de slot generique */
template<class arg1, class arg2>
class Func2Interface : public iTransaction{
public:
  Func2Interface<arg1,arg2> *next;
  Func2Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y)=0;
  virtual ~Func2Interface() {}
};
/*! \brief SIGNAL 2 Param:template de specialisation de la class Func2Interface pour la class T */
template<class T, class Arg1, class Arg2>
class Func2 : public Func2Interface<Arg1,Arg2> {
public:
  typedef void (T::*funcType)(Arg1,Arg2);
public:
  T *obj;
  funcType func;
public:
  Func2(T* obj_, funcType f)
    : Func2Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y) { if (enabled) (obj->*func)(x,y); }
};
/*! \brief SIGNAL 2 Param:template de signal */
template<class arg1, class arg2>
class iSignal2{
public:
  typedef Func2Interface<arg1,arg2> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal2() { sl=0; cpt_cb=0; }
  ~iSignal2() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y) {
	  for (sli i = sl; i;) { sli next = i->next; i->call(x,y); i = next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2)) {
            return insert(new Func2<T,Arg1,Arg2>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2)) {
            Func2<T,Arg1,Arg2> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func2<T,Arg1,Arg2>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func2<T,Arg1,Arg2>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};

/*! \brief SIGNAL 3 Param:template de slot generique */
template<class arg1, class arg2,class arg3>
class Func3Interface : public iTransaction{
public:
  Func3Interface<arg1,arg2,arg3> *next;
  Func3Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y, arg3 z)=0;
  virtual ~Func3Interface() {}
};
/*! \brief SIGNAL 3 Param:template de specialisation de la class Func2Interface pour la class T */
template<class T, class Arg1, class Arg2, class Arg3>
class Func3 : public Func3Interface<Arg1,Arg2,Arg3> {
public:
  typedef void (T::*funcType)(Arg1,Arg2,Arg3);
public:
  T *obj;
  funcType func;
public:
  Func3(T* obj_, funcType f)
    : Func3Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y,Arg3 z) { if (enabled) (obj->*func)(x,y,z); }
};

/*! \brief SIGNAL 3 Param:template de signal*/
template<class arg1, class arg2,class arg3>
class iSignal3{
public:
  typedef Func3Interface<arg1,arg2,arg3> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal3() { sl=0; cpt_cb=0; }
  ~iSignal3() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y, arg3 z) {
     for (sli i=sl; i;) { sli next=i->next; i->call(x,y,z); i=next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2, class Arg3>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2,Arg3)) {
            return insert(new Func3<T,Arg1,Arg2,Arg3>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2, class Arg3> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2,Arg3)) {
			Func3<T, Arg1, Arg2, Arg3> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func3<T,Arg1,Arg2,Arg3>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func3<T,Arg1,Arg2,Arg3>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};
//------------------------------------------------------------------------------
/*! \brief SIGNAL 4 Param:template de slot generique */
template<class arg1, class arg2,class arg3,class arg4>
class Func4Interface : public iTransaction{
public:
  Func4Interface<arg1,arg2,arg3,arg4> *next;
  Func4Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y, arg3 z, arg4 t)=0;
  virtual ~Func4Interface() {}
};
/*! \brief SIGNAL 4 Param:template de specialisation de la class Func4Interface pour la class T */
template<class T, class Arg1, class Arg2, class Arg3, class Arg4>
class Func4 : public Func4Interface<Arg1,Arg2,Arg3,Arg4> {
public:
  typedef void (T::*funcType)(Arg1,Arg2,Arg3,Arg4);
public:
  T *obj;
  funcType func;
public:
  Func4(T* obj_, funcType f)
    : Func4Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y,Arg3 z,Arg4 t) { if (enabled) (obj->*func)(x,y,z,t); }
};

/*! \brief SIGNAL 4 Param:template de signal */
template<class arg1, class arg2,class arg3,class arg4>
class iSignal4{
public:
  typedef Func4Interface<arg1,arg2,arg3,arg4> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal4() { sl=0; cpt_cb=0; }
  ~iSignal4() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y, arg3 z, arg4 t) {
     for (sli i=sl; i;) { sli next=i->next; i->call(x,y,z,t); i=next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2,Arg3,Arg4)) {
            return insert(new Func4<T,Arg1,Arg2,Arg3,Arg4>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2,Arg3,Arg4)) {
			Func4<T, Arg1, Arg2, Arg3, Arg4> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func4<T,Arg1,Arg2,Arg3,Arg4>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func4<T,Arg1,Arg2,Arg3,Arg4>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};
//------------------------------------------------------------------------------
/*! \brief SIGNAL 5 Param:template de slot generique */
template<class arg1, class arg2,class arg3,class arg4,class arg5>
class Func5Interface : public iTransaction{
public:
  Func5Interface<arg1,arg2,arg3,arg4,arg5> *next;
  Func5Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u)=0;
  virtual ~Func5Interface() {}
};
/*! \brief SIGNAL 5 Param:template de specialisation de la class Func5Interface pour la class T */
template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
class Func5 : public Func5Interface<Arg1,Arg2,Arg3,Arg4,Arg5> {
public:
  typedef void (T::*funcType)(Arg1,Arg2,Arg3,Arg4,Arg5);
public:
  T *obj;
  funcType func;
public:
  Func5(T* obj_, funcType f)
    : Func5Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y,Arg3 z,Arg4 t,Arg5 u) { if (enabled) (obj->*func)(x,y,z,t,u); }
};

/*! \brief SIGNAL 5 Param:template de signal */
template<class arg1, class arg2,class arg3,class arg4,class arg5>
class iSignal5{
public:
  typedef Func5Interface<arg1,arg2,arg3,arg4,arg5> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal5() { sl=0; cpt_cb=0; }
  ~iSignal5() {
      doDelete<sli>(sl);      
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u) {
     for (sli i=sl; i;) { sli next=i->next; i->call(x,y,z,t,u); i=next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5)) {
            return insert(new Func5<T,Arg1,Arg2,Arg3,Arg4,Arg5>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5)) {
			Func5<T, Arg1, Arg2, Arg3, Arg4, Arg5> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func5<T,Arg1,Arg2,Arg3,Arg4,Arg5>*>(r);
             while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func5<T,Arg1,Arg2,Arg3,Arg4,Arg5>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};
//------------------------------------------------------------------------------
/*! \brief SIGNAL 6 Param:template de slot generique */
template<class arg1, class arg2,class arg3,class arg4,class arg5,class arg6>
class Func6Interface : public iTransaction{
public:
  Func6Interface<arg1,arg2,arg3,arg4,arg5,arg6> *next;
  Func6Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u, arg6 v)=0;
  virtual ~Func6Interface() {}
};
/*! \brief SIGNAL 6 Param:template de specialisation de la class Func6Interface pour la class T */
template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
class Func6 : public Func6Interface<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> {
public:
  typedef void (T::*funcType)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6);
public:
  T *obj;
  funcType func;
public:
  Func6(T* obj_, funcType f)
    : Func6Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y,Arg3 z,Arg4 t,Arg5 u,Arg6 v) { if (enabled) (obj->*func)(x,y,z,t,u,v); }
};

/*! \brief SIGNAL 6 Param:template de signal */
template<class arg1, class arg2,class arg3,class arg4,class arg5,class arg6>
class iSignal6{
public:
  typedef Func6Interface<arg1,arg2,arg3,arg4,arg5,arg6> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal6() { sl=0; cpt_cb=0; }
  ~iSignal6() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u, arg6 v) {
	 for (sli i=sl; i;) { sli next=i->next; i->call(x,y,z,t,u,v); i=next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6)) {
            return insert(new Func6<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6)) {
			Func6<T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func6<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func6<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};
//------------------------------------------------------------------------------
/*! \brief SIGNAL 7 Param:template de slot generique */
template<class arg1, class arg2,class arg3,class arg4,class arg5,class arg6,class arg7>
class Func7Interface : public iTransaction{
public:
  Func7Interface<arg1,arg2,arg3,arg4,arg5,arg6,arg7> *next;
  Func7Interface() : iTransaction(),next(0) {}
  virtual void call(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u, arg6 v, arg7 w)=0;
  virtual ~Func7Interface() {}
};
/*! \brief SIGNAL 7 Param:template de specialisation de la class Func7Interface pour la class T */
template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
class Func7 : public Func7Interface<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7> {
public:
  typedef void (T::*funcType)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7);
public:
  T *obj;
  funcType func;
public:
  Func7(T* obj_, funcType f)
    : Func7Interface(),obj(obj_),func(f) {}
  void call(Arg1 x,Arg2 y,Arg3 z,Arg4 t,Arg5 u,Arg6 v,Arg7 w) { if (enabled) (obj->*func)(x,y,z,t,u,v,w); }
};

/*! \brief SIGNAL 7 Param:template de signal */
template<class arg1, class arg2,class arg3,class arg4,class arg5,class arg6,class arg7>
class iSignal7{
public:
  typedef Func7Interface<arg1,arg2,arg3,arg4,arg5,arg6,arg7> *sli;
private:
  sli sl;
  int cpt_cb;
public:
  iSignal7() { sl=0; cpt_cb=0; }
  ~iSignal7() {
      doDelete<sli>(sl);
  }

  //insertion
  iTransaction * insert(sli item) {
     return doInsert<sli>(sl,item,cpt_cb);
  }
  //liberation
  void remove(void * slr) {
    doRemove<sli>(sl,slr,cpt_cb);
  }

  //Permet de recuperer le nombre de callback attache
  int getNbCallback() { return cpt_cb;}

  //execution
  void operator()(arg1 x, arg2 y, arg3 z, arg4 t, arg5 u, arg6 v, arg7 w) {
     for (sli i=sl; i;) { sli next=i->next; i->call(x,y,z,t,u,v,w); i=next; }
   }
    //connexion et deconnexion de signal
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6,class Arg7>
        iTransaction * iconnect(T& obj, void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7)) {
            return insert(new Func7<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>(&obj,f));    
    }
    template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6,class Arg7> 
        void idisconnect(T& obj,void (T::*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7)) {
			Func7<T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> * init_func = nullptr;
            sli r;
            r=sl;
            if (r!=0) init_func=dynamic_cast<Func7<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>*>(r);
            while (r!=0 && (init_func==0 || (init_func!=0 && (init_func->func!=f || init_func->obj!=&obj))) ) {
                r=r->next;
                if (r!=0) init_func=dynamic_cast<Func7<T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>*>(r);
            }
            if (r!=0) doRemove<sli>(sl,r,cpt_cb);
    }                    
};

} /*namespace LibIndeSig */
#endif

