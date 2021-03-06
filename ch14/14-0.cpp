#include <cstddef>
#include <stdexcept>
#include <string>
#include <iostream>

template <class T> class Ptr {
public:
    void make_unique() {
        if (*refptr != 1) {
            --*refptr;
            refptr = new size_t(1);
            p = p? clone(p): 0;
        }
    }

    // the rest of the class looks like `Ref_handle' except for its name
    Ptr(): p(0), refptr(new size_t(1)) { }
    Ptr(T* t): p(t), refptr(new size_t(1)) { }
    Ptr(const Ptr& h): p(h.p), refptr(h.refptr) { ++*refptr; }

    Ptr& operator=(const Ptr&);
    ~Ptr();
    operator bool() const { return p; }
    T& operator*() const;
    T* operator->() const;

private:
    T* p;
    std::size_t* refptr;
};

template <class T> T* clone(const T* tp)
{
    return tp->clone();
}

template<class T>
T& Ptr<T>::operator*() const { if (p) return *p; throw std::runtime_error("unbound Ptr"); }

template<class T>
T* Ptr<T>::operator->() const { if (p) return p; throw std::runtime_error("unbound Ptr"); }

template<class T>
Ptr<T>& Ptr<T>::operator=(const Ptr& rhs)
{
    ++*rhs.refptr;
    if (--*refptr == 0) {
        delete refptr;
        delete p;
    }

    refptr = rhs.refptr;
    p = rhs.p;
    return *this;
}

template<class T> Ptr<T>::~Ptr()
{
    if (--*refptr == 0) {
        delete refptr;
        delete p;
    }
}

int main(void)
{
    std::string *s = new std::string("Jonathan");
    Ptr<std::string> p(s);

    std::cout << *p << "\n";
    return 0;
}
