/*!
\file SwRefPtr.h
\date 12/04/2006
\brief Class pointer sur SwRef
\author  Big
\version 1.0
*/

#ifndef _SwRefPtr_H
#define _SwRefPtr_H

namespace StreamWork
{
	namespace SwCore
	{
        /*!
        \class SwRefPtr
        \date 12/04/2006
        \brief Class pointer sur SwRef
        \author  BIG
        \note
            Un pointeur correspond a une instance d'une classe derivée de SwRef
            Quand il est detruit, le compteur de reference sur l'instance de la 
            classe dérivée est incrementé
            Quand il est affecté, le compteur de reference sur l'instance de la 
            classe dérivée est decrémenté
        */
        template <class T> 
        class SwRefPtr {
        public:                                        
            //Constructuer de base
            SwRefPtr<T>(T *ptr = 0) : _elt(ptr) 
            {                
                if (_elt != 0) {
                    _elt->_addRef();
                }
                enabled=true;
            }
            //Constructeur de copie
            SwRefPtr(const SwRefPtr<T> & source) : _elt(source._elt) 
            {                
                if (_elt != 0) {
                    _elt->_addRef();
                }
                 enabled=true;
           }
            //Destructeur
            ~SwRefPtr() 
            {
                if (_elt != 0 && enabled) {
                    _elt->_release();
                    _elt = 0;
                }
            }                                   
            //Operateur d'affectation
            SwRefPtr<T> & operator=(const SwRefPtr<T> & source) 
            {
                if (_elt != source._elt) {                    
                    T* old = _elt; //on fait surtout pas release de suite !
                    _elt = source._elt;
                    if (_elt != 0 && enabled) {
                        _elt->_addRef();
                    }
                    //release ici pour empecher la suppression d'un element qui
                    //pourrait etre reference par l'ancien element. Par ex,
                    //si la source est un enfant de l'ancien element
                    if (old != 0 && enabled) {
                        old->_release();
                    }           
                
                }
                return *this;
            }
            //Operateur d'egalité
            SwRefPtr<T> & operator=(T * source) 
            {
                if (_elt != source) {                    
                    T* old = _elt; //on fait surtout pas release de suite !
                    _elt = source;
                    if (_elt != 0 && enabled) {
                        _elt->_addRef();
                    }
                    //release ici pour empecher la suppression d'un element qui
                    //pourrait etre reference par l'ancien element. Par ex,
                    //si la source est un enfant de l'ancien element
                    if (old != 0 && enabled) {
                        old->_release();
                    }           
                
                }
                return *this;
            }
	        //Operateur d'egalite
	        inline bool operator==(const SwRefPtr<T> & source) const 
            {
		        return _elt==source._elt;
	        }
	        //Operateur d'egalite
	        inline bool operator==(T * source) const 
            {
		        return _elt==source;
	        }
	        friend bool operator==(const T* ptr, const SwRefPtr<T> & rp) 
            { 
                return (ptr==rp._elt); 
            }
	        //Operateur d'inegalite
	        inline bool operator!=(const SwRefPtr<T> & source) const 
            {
		        return _elt!=source._elt;
	        }
	        //Operateur d'inegalite
	        inline bool operator!=(T * source) const 
            {
		        return _elt!=source;
	        }
	        //Operateur d'inegalite
	        friend bool operator!=(const T* ptr, const SwRefPtr<T> & right) 
            { 
                return (ptr!=right._elt); 
            }
            //Operateur de comparaison
            bool operator<(const SwRefPtr<T> & right) const 
            { 
                return (_elt < right._elt); 
            }            	        
            //Operateur de dereferencement
            T& operator*() const throw() {
                return *_elt;
            }
            //Operateur d'indirection
            T* operator->() const {
                return _elt;
            }            
            //acces au pointer
	        inline T* get() const{
                return _elt;
            }            
            //Permet de savoir s'il est 0
            bool IsNull() {
                return _elt==0;
            }
            //operateur de negation
            bool operator!() const   
            { 
                return _elt==0; 
            }
            //swap
            void swap(SwRefPtr<T> & right) 
            { 
                T* tmp=_elt; 
                _elt=right._elt; 
                right._elt=tmp; 
            }
                        
	        //Operateur de cast entre pointeur
	        template<typename U> 
            operator SwRefPtr<U> (void) const 
            {
		        return SwRefPtr<U>(dynamic_cast<U *>(_elt));
	        }
	        //Operateur de cast sur le type de base
	        operator T* (void) const 
            {
		        return _elt;
	        }
	        //Operateur d'affectation entre pointeur
	        template<typename U> 
            SwRefPtr<T> & operator=(const SwRefPtr<U> &source) 
            {
		        if (_elt != source.get()) {                    
                    T* old = _elt; //on fait surtout pas release de suite !
                    _elt = dynamic_cast<T *>(source.get());
                    if (_elt != 0 && enabled) {
                        _elt->_addRef();
                    }
                    //release ici pour empecher la suppression d'un element qui
                    //pourrait etre reference par l'ancien element. Par ex,
                    //si la source est un enfant de l'ancien element
                    if (old != 0 && enabled) {
                        old->_release();
                    }           
                
                }
                return *this;                                                                
	        }
	        
        
        private:
            //Instance reference (DOIT HERITE DE SwRef)
            T * _elt;
        public:
            //Action
            bool enabled;
        };  //FIN DE LA CLASS SwRefPtr
        


        template <class T> 
        inline unsigned int qHash(const SwRefPtr<T> & key) 
        {            
            return (unsigned int)key.get();
        }
        
        template<class T> 
        inline void swap(SwRefPtr<T> & ptr1, SwRefPtr<T> & ptr2) 
        { 
            ptr1.swap(ptr2); 
        }
        
        template<class T> 
        inline T* get_pointer(const SwRefPtr<T> & ptr) 
        { 
            return ptr.get(); 
        }
        
        template<class T, class Y> 
        inline SwRefPtr<T> static_pointer_cast(const SwRefPtr<Y> & ptr) 
        { 
            return static_cast<T*>(ptr.get()); 
        }
        
        template<class T, class Y> 
        inline SwRefPtr<T> dynamic_pointer_cast(const SwRefPtr<Y> & ptr) 
        { 
            return dynamic_cast<T*>(ptr.get()); 
        }

        template<class T, class Y> 
        inline SwRefPtr<T> const_pointer_cast(const SwRefPtr<Y> & ptr) 
        { 
            return const_cast<T*>(ptr.get()); 
        }

        template<class T, class Y> 
        inline SwRefPtr<T> static_pointer_cast(Y*  ptr) 
        { 
            return static_cast<T*>(ptr); 
        }
        
        template<class T, class Y> 
        inline SwRefPtr<T> dynamic_pointer_cast(Y * ptr) 
        { 
            return dynamic_cast<T*>(ptr); 
        }

        template<class T, class Y> 
        inline SwRefPtr<T> const_pointer_cast(Y * ptr) 
        { 
            return const_cast<T*>(ptr); 
        }

    }
}

#endif
