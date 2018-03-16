#define X_DELEGATE_TEMPLATE_PARAMS    X_MAKE_PARAMS1(X_DELEGATE_NUM_ARGS, class T)
// class T0, class T1, class T2, ...
#define X_DELEGATE_TEMPLATE_ARGS      X_MAKE_PARAMS1(X_DELEGATE_NUM_ARGS, T)
// T0, T1, T2, ...
#define X_DELEGATE_FUNCTION_PARAMS    X_MAKE_PARAMS2(X_DELEGATE_NUM_ARGS, T, a)
// T0 a0, T1 a1, T2 a2, ...
#define X_DELEGATE_FUNCTION_ARGS      X_MAKE_PARAMS1(X_DELEGATE_NUM_ARGS, a)
// a0, a1, a2, ...

// Comma if nonzero number of arguments
#if X_DELEGATE_NUM_ARGS == 0
#define X_DELEGATE_COMMA
#else
#define X_DELEGATE_COMMA    ,
#endif

//-------------------------------------------------------------------------
// class Delegate<R (T1, T2, ..., TN)>
//template <class R, X_MAKE_PARAMS1_1 (class T)> <class R, class T1, class T2,
//class Delegate<R ()> 
template <class R X_DELEGATE_COMMA X_DELEGATE_TEMPLATE_PARAMS, class MT_Policy>
class Delegate<R (X_DELEGATE_TEMPLATE_ARGS), MT_Policy>
{
   // Declaractions
private:
   class DelegateImplBase
   {
      // Fields
   public:
#ifndef USING_LIGHT_DELEGATE
      DelegateImplBase* Previous; // linked list
#endif
      // Constructor/Destructor
   protected:
      DelegateImplBase() 
#ifndef USING_LIGHT_DELEGATE
         : Previous(NULL) 
#endif
      { }
      
      DelegateImplBase(const DelegateImplBase& other) 
#ifndef USING_LIGHT_DELEGATE
         : Previous(NULL) 
#endif
      { }
   public:
      virtual ~DelegateImplBase() { }

      // Methods
   public:
      virtual DelegateImplBase* Clone() const = 0;
      virtual R Invoke(X_DELEGATE_FUNCTION_PARAMS) const = 0;
   };

   template <class TFunctor>
   struct Invoker
   {
      static R Invoke(const TFunctor& f X_DELEGATE_COMMA X_DELEGATE_FUNCTION_PARAMS)
      {
         return (const_cast<TFunctor&>(f))(X_DELEGATE_FUNCTION_ARGS);
      }
   };

   template <class TPtr, class TFunctionPtr>
   struct Invoker<pair<TPtr, TFunctionPtr> >
   {
      static R Invoke(const pair<TPtr, TFunctionPtr>& mf X_DELEGATE_COMMA X_DELEGATE_FUNCTION_PARAMS)
      {
         return ((*mf.first).*mf.second)(X_DELEGATE_FUNCTION_ARGS);
      }
   };

   template <class TFunctor>
   class DelegateImpl : public DelegateImplBase
   {
      // Fields
   public:
      TFunctor Functor;

      // Constructor
   public:
      DelegateImpl(const TFunctor& f) : Functor(f)
      {
      }
      //DelegateImpl(const DelegateImpl& other) : Functor(other.Functor)
      //{
      //}

      // Methods
   public:
      virtual DelegateImplBase* Clone() const
      {
         return new (nothrow) DelegateImpl(*this);
      }
      virtual R Invoke(X_DELEGATE_FUNCTION_PARAMS) const
      {
         return Invoker<TFunctor>::Invoke(this->Functor X_DELEGATE_COMMA X_DELEGATE_FUNCTION_ARGS);
      }
   };

   // Fields
private:
   DelegateImplBase* _last;
   mutable MT_Policy _mutex;

   // Constructor/Destructor
public:
   Delegate()
   {
      this->_last = NULL;
   }

   template <class TFunctor>
   Delegate(const TFunctor& f)
   {
      this->_last = NULL;
      *this = f;
   }

   template<class TPtr, class TFunctionPtr>
   Delegate(const TPtr& obj, const TFunctionPtr& mfp)
   {
      this->_last = NULL;
      *this = make_pair(obj, mfp);
   }

   Delegate(const Delegate& d)
   {
      this->_last = NULL;
      *this = d;
   }

   ~Delegate()
   {
      Clear();
   }

   // Properties
public:
   bool IsEmpty() const
   {
      LockBlock<MT_Policy> lock(_mutex);
      return (this->_last == NULL);
   }
#ifndef USING_LIGHT_DELEGATE
   bool IsMulticast() const
   {
      LockBlock<MT_Policy> lock(_mutex);

      return (this->_last != NULL && this->_last->Previous != NULL);
   }
#endif

   // Static Methods
private:
   static DelegateImplBase* CloneDelegateList(DelegateImplBase* list, /*out*/ DelegateImplBase** first)
   {
      DelegateImplBase* list2 = list;
      DelegateImplBase* newList = NULL;
      DelegateImplBase** pNewList = &newList;
      DelegateImplBase* temp = NULL;

      try
      {
         while (list2 != NULL)
         {
            temp = list2->Clone();
            if (temp)
            {
               *pNewList = temp;
#ifndef USING_LIGHT_DELEGATE
               pNewList = &temp->Previous;
               list2 = list2->Previous;
#else
               break;
#endif
            }
            else
            {
               break;
            }

         }
      }
      catch (...)
      {
         FreeDelegateList(newList);
#ifndef BOOST_NO_EXCEPTIONS
         throw;
#endif
      }

      if (first != NULL)
         *first = temp;
      return newList;
   }

   static void FreeDelegateList(DelegateImplBase* list)
   {
#ifndef USING_LIGHT_DELEGATE
      DelegateImplBase* temp = NULL;
      while (list != NULL)
      {
         temp = list->Previous;
         delete list;
         list = temp;
      }
#else
      delete list;
#endif
   }

#ifndef USING_LIGHT_DELEGATE
   static void InvokeDelegateList(DelegateImplBase* list X_DELEGATE_COMMA X_DELEGATE_FUNCTION_PARAMS)
   {
      if (list != NULL)
      {
         if (list->Previous != NULL)
            InvokeDelegateList(list->Previous X_DELEGATE_COMMA X_DELEGATE_FUNCTION_ARGS);
         list->Invoke(X_DELEGATE_FUNCTION_ARGS);
      }
   }
#endif

   // Methods
private:

   //template <class TPtr, class TFunctionPtr>
   //bool Exist(const TPtr& obj, const TFunctionPtr& mfp)
   //{
   //   DelegateImpl<pair<TPtr, TFunctionPtr> >* last = dynamic_cast<DelegateImpl<pair<TPtr, TFunctionPtr> >*>(this->_last);
   //   while (last)
   //   {
   //      if (last->Functor.first == obj && 
   //         last->Functor.second == mfp)
   //      {
   //         return true;
   //      }
   //      else
   //      {
   //         last = dynamic_cast<DelegateImpl<pair<TPtr, TFunctionPtr> >*>(last->Previous);
   //      }
   //   }
   //   return false;
   //}

   // Methods
public:

#ifndef USING_LIGHT_DELEGATE
   template <class TFunctor>
   void Add(const TFunctor& f)
   {
      if (!Exist(f))
      {
         LockBlock<MT_Policy> lock(_mutex);

         DelegateImplBase* del = new (nothrow) DelegateImpl<TFunctor>(f);
         if (del)
         {
            del->Previous = this->_last;
            this->_last = del;
         }
      }
   }

   template<class TPtr, class TFunctionPtr>
   void Add(const TPtr& obj, const TFunctionPtr& mfp)
   {
      Add(make_pair(obj, mfp));
   }

   template <class TFunctor>
   bool Remove(const TFunctor& f)
   {
      LockBlock<MT_Policy> lock(_mutex);

      DelegateImplBase* pLast = this->_last;
      DelegateImplBase** ppLast = &this->_last;
      DelegateImpl<TFunctor>* impl = NULL;

      while (pLast != NULL)
      {
         impl = dynamic_cast<DelegateImpl<TFunctor>*>(pLast);
         if (impl && impl->Functor == f)
         {
            *ppLast = pLast->Previous;
            delete impl;
            return true;
         }
         ppLast = &pLast->Previous;
         pLast = pLast->Previous;
      }
      return false;
   }

   template<class TPtr, class TFunctionPtr>
   bool Remove(const TPtr& obj, const TFunctionPtr& mfp)
   {
      return Remove(make_pair(obj, mfp));
   }
#endif

   void Clear()
   {
      LockBlock<MT_Policy> lock(_mutex);

      FreeDelegateList(this->_last);
      this->_last = NULL;
   }

   template <class TFunctor>
   bool Exist(const TFunctor& f) const
   {
      LockBlock<MT_Policy> lock(_mutex);

      DelegateImpl<TFunctor>* last = dynamic_cast<DelegateImpl<TFunctor>*>(this->_last);
      while (last)
      {
         if (last->Functor == f)
         {
            return true;
         }
#ifndef USING_LIGHT_DELEGATE
         else
         {
            last = dynamic_cast<DelegateImpl<TFunctor>*>(last->Previous);
         }
#else
         break;
#endif
      }
      return false;
   }
private:
   template <class TFunctor>
   bool Equals(const TFunctor& f) const
   {
      if (this->_last == NULL 
#ifndef USING_LIGHT_DELEGATE
         || this->_last->Previous != NULL
#endif
         )
         return false;

      DelegateImpl<TFunctor>* impl = dynamic_cast<DelegateImpl<TFunctor>*>(this->_last);
      if (!impl)
         return false;
      return (impl->Functor == f);
   }

   // Operators
public:
   operator bool() const
   {
      return !IsEmpty();
   }

   bool operator!() const
   {
      return IsEmpty();
   }

   template <class TFunctor>
   Delegate& operator=(const TFunctor& f)
   {
      LockBlock<MT_Policy> lock(_mutex);
      FreeDelegateList(this->_last);
      DelegateImplBase* del = new (nothrow) DelegateImpl<TFunctor>(f);
      if (del)
      {
         this->_last = del;
      }
      return *this;
   }

   Delegate& operator=(const Delegate& d)
   {
      if (this != &d)
      {
         LockBlock<MT_Policy> lock(_mutex);
         DelegateImplBase* list = CloneDelegateList(d._last, NULL);
         FreeDelegateList(this->_last);
         this->_last = list;
      }
      return *this;
   }

#ifndef USING_LIGHT_DELEGATE
   template <class TFunctor>
   Delegate& operator+=(const TFunctor& f)
   {
      Add(f);
      return *this;
   }

   template <class TFunctor>
   Delegate& operator-=(const TFunctor& f)
   {
      Remove(f);
      return *this;
   }
#endif

   template <class TFunctor>
   bool operator==(const TFunctor& f)
   {
      return this->Exist(f);
   }

   //template <class TFunctor>
   //friend bool operator==(const TFunctor& f, const Delegate& d)
   //{
   //   return (d == f);
   //}

   template <class TFunctor>
   bool operator!=(const TFunctor& f)
   {
      return !(*this == f);
   }

   //template <class TFunctor>
   //friend bool operator!=(const TFunctor& f, const Delegate& d)
   //{
   //   return (d != f);
   //}

   R operator()(X_DELEGATE_FUNCTION_PARAMS) const
   {
      LockBlock<MT_Policy> lock(_mutex);

      if (this->_last == NULL)
         return R();
#ifndef USING_LIGHT_DELEGATE
      if (this->_last->Previous != NULL)
         InvokeDelegateList(this->_last->Previous X_DELEGATE_COMMA X_DELEGATE_FUNCTION_ARGS);
#endif
      return this->_last->Invoke(X_DELEGATE_FUNCTION_ARGS);
   }
};

#undef X_DELEGATE_TEMPLATE_PARAMS
#undef X_DELEGATE_TEMPLATE_ARGS
#undef X_DELEGATE_FUNCTION_PARAMS
#undef X_DELEGATE_FUNCTION_ARGS
#undef X_DELEGATE_COMMA
