#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "Complex.h"
#include <iostream>
#include <assert.h>

using std::vector;
using std::size_t;

namespace MtmMath {
    template<typename T>
    class MtmVec {
    private:
        vector<T> data;
        bool is_col_vec;
        Dimensions dim;
        vector<bool> lock;
    public:
        /*
         * Vector constructor, m is the number of elements in it and val is the
         * initial value for the matrix elements
         */
        explicit MtmVec(size_t m, const T &val = T());
        MtmVec(const MtmVec &v);
        ~MtmVec() = default;
        /*
         * Vector operators:
         */
        MtmVec& operator=(const MtmVec&);
        MtmVec& operator+=(const MtmVec&);
        MtmVec& operator-=(const MtmVec&);
        MtmVec& operator*=(const T &val);
        MtmVec operator-() const;
        T& operator[](int pos);
        const T& operator[](int pos) const;
        /*
         * Function that get function object f and uses it's () operator on
         * each element in the vectors.
         * It outputs the function object's * operator after iterating on all
         * the vector's elements
         */
        template<typename Func>
        T vecFunc(Func &f) const;
        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        void resize(Dimensions dim, const T &val = T());
        /*
         * Helper functions for MtmVec:
         */
        int size() const;
        Dimensions getDim() const;
        bool isColVector() const;
        int getRow() const;
        int getCol() const;
        bool isCellLocked(int pos) const;
        void lockCell(int pos);       //lock a cell and prevent writing to it
        void unlockCell(int pos);     //unlock a cell
        /*
         * Performs transpose operation on matrix
         */
        void transpose();
        /*
         * iterator class- An iterator including operator++ for iterating
         * through the vector, and operator* for accessing elements in the
         * vector.
         */
        class iterator
        {
        public:
            iterator(MtmVec<T>* vec_ptr, int i);
            virtual void operator++();
            T& operator*();
            bool operator!=(const iterator &j) const;
            bool operator==(const iterator &j) const;
        protected:
            MtmVec<T>* vec;
            int i;
        };
        /*
         * nonzero_iterator class- An iterator including operator++ for
         * iterating through all non zero elements in the vector, and
         * operator* for accessing elements in the vector.
         * Derived from iterator.
        */
        class nonzero_iterator : public iterator
        {
        public:
            nonzero_iterator(MtmVec<T>* vec_ptr, int i);
            void operator++() override;
        };
    /*
     * functions for iterators of MtmVec:
     */
        iterator begin();
        iterator end();
        nonzero_iterator nzbegin();
        nonzero_iterator nzend();
    };

                        ////////Constructors////////
    template<typename T>
    MtmVec<T>::MtmVec(size_t m, const T &val) try:
            data(vector<T>(m,val)), is_col_vec(true) , dim(Dimensions(m,1)) ,
            lock(m,true) {
                if (m==0) throw MtmExceptions::IllegalInitialization();
            }
     catch (std::bad_alloc& e) {throw MtmExceptions::OutOfMemory();}

    template<typename T>
    MtmVec<T>::MtmVec(const MtmVec<T>& v) try:
           data(vector<T>(v.data)), is_col_vec(v.is_col_vec) , dim(v.dim) ,
           lock((size_t)v.size(),true) {}
           catch (std::bad_alloc& e) {throw MtmExceptions::OutOfMemory();}

                        ////////Operators////////

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator=(const MtmVec<T>& v){
        if (this==&v) {
            return *this;
        }
    try {
        data = v.data;
        lock = v.lock;
    }
    catch (std::bad_alloc& e) {throw MtmExceptions::OutOfMemory();}
    is_col_vec=v.is_col_vec;
    dim=v.dim;
    return *this;
    }

    /*
     * operator [] gives access to the data stored in the pos given to the
     * function. if the pos is out of the vector's range an
     * AccessIllegalElement() exception will be thrown. If the pos is locked,
     * AccessIllegalElement() exception will be thrown.
     */
    template <typename T>
    T& MtmVec<T>::operator[](int pos) {
        size_t pos_unsigned=(size_t)pos;
        if (pos<0||pos_unsigned>=data.size()||!lock[pos_unsigned]){
            throw MtmExceptions::AccessIllegalElement();
        }
        assert(pos>=0 && pos<(int)data.size());
        return data[pos_unsigned];
    }

    template <typename T>
    const T& MtmVec<T>::operator[](int pos) const {
       size_t pos_unsigned=(size_t)pos;
       if (pos<0||pos_unsigned>=data.size()){
            throw MtmExceptions::AccessIllegalElement();
        }
        assert(pos>=0 && pos<(int)data.size());
        return data[pos_unsigned];
    }

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator+=(const MtmVec& v1) {
        if (dim!=v1.dim||is_col_vec!=v1.is_col_vec){
            throw MtmExceptions::DimensionMismatch(dim,v1.dim);
        }
        for (int i=0;i<(int)data.size();i++){
            data[i]+=v1.data[i];
        }
        return *this;
    }

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator-=(const MtmVec& v1){
        if (dim!=v1.dim||is_col_vec!=v1.is_col_vec){
            throw MtmExceptions::DimensionMismatch(dim,v1.dim);
        }
        for (int i=0;i<(int)data.size();i++){
            data[i]-=v1.data[i];
        }
        return *this;
    }

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& v1,const MtmVec<T>& v2){
        return MtmVec<T>(v1)+=v2;
    }

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& v1,const T& val){
        MtmVec<T> res = v1;
        for (typename MtmVec<T>::iterator it=res.begin();it!=res.end();++it){
            *it+=val;
        }
        return res;
    }

    template <typename T>
    MtmVec<T> operator+(const T& val,const MtmVec<T>& v1){
         return v1+val;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& v1,const MtmVec<T>& v2){
        return MtmVec<T>(v1)-=v2;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& v1,const T& val){
        MtmVec<T> res=v1;
        for (typename MtmVec<T>::iterator it=res.begin();it!=res.end();++it){
            *it-=val;
        }
        return res;
    }

    template <typename T>
    MtmVec<T> operator-(const T& val,const MtmVec<T>& v1){
            MtmVec<T> res = -v1;
            return val + res;
    }

    template <typename T>
    MtmVec<T> MtmVec<T>::operator-() const {
        MtmVec<T> res_vec(data.size(),T());
        for (int i = 0; i < (int)data.size(); i++) {
            res_vec.data[i] = -data[i];
        }
        res_vec.is_col_vec = is_col_vec;
        res_vec.dim = dim;
        return res_vec;
    }

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator*=(const T &val) {
        for(int i=0;i<(int)data.size();i++){
            data[i]*=val;
        }
        return *this;
    }

    template <typename T>
    MtmVec<T> operator*(const MtmVec<T>& v1,const T& val){
        return MtmVec<T>(v1)*=val;
    }

    template <typename T>
    MtmVec<T> operator*(const T& val,const MtmVec<T>& v1){
        return MtmVec<T>(v1)*=val;
    }

                    ////////Vector functions////////

    template <typename T>
    void MtmVec<T>::resize(Dimensions new_dim, const T &val){
        if ((is_col_vec&&new_dim.getCol()!=1) || (!is_col_vec&&new_dim.getRow
        ()!=1) || new_dim.getRow()==0 || new_dim.getCol()==0){
            throw MtmExceptions::ChangeMatFail(dim,new_dim);
        }
        try {
            if (is_col_vec) {
                data.resize(new_dim.getRow(), val);
                lock.resize(new_dim.getRow(),true); //handle triangle matrices
            } else {
                data.resize(new_dim.getCol(), val);
                lock.resize(new_dim.getCol(),true); //handle triangle matrices
            }
            dim = new_dim;
        }
        catch (std::bad_alloc& e){
            throw MtmExceptions::OutOfMemory();
        }
    }

    template <typename T>
    template<typename Func>
    T MtmVec<T>::vecFunc(Func &f) const {
        for (int i=0;i<(int)data.size();i++){
            f(data[i]);
        }
        return *f;
    }

    template <typename T>
    void MtmVec<T>::transpose(){
        is_col_vec=!is_col_vec;
        dim.transpose();
    }

                        ////////Helper functions////////

    template <typename T>
    int MtmVec<T>::getCol() const{
        return (int)dim.getCol();
    }

    template <typename T>
    int MtmVec<T>::getRow() const{
    return (int)dim.getRow();
    }

    template <typename T>
    int MtmVec<T>::size() const{
    return (int)data.size();
    }

    template <typename T>
    Dimensions MtmVec<T>::getDim() const{
    return dim;
    }

    template <typename T>
    bool MtmVec<T>::isColVector() const{
    return is_col_vec;
    }

    template <typename T>
    bool MtmVec<T>::isCellLocked(int pos) const{
    return !lock[(size_t)pos];
    }

    /*
     * Functions for marking a cell in the vector as locked. the pos
     * represents the index in the vector that will be locked, i.e will throw
     * exception when trying to writing to the element stored in this index.
     */
    template <typename T>
    void MtmVec<T>::lockCell(int pos){
        size_t pos_unsigned=(size_t)pos;
        assert(pos>=0 && pos_unsigned<data.size());
        lock[pos_unsigned]=false;
    }

    /*
    * Functions for marking a cell in the vector as unlocked. the pos
    * represents the index in the vector that will be unlocked, i.e will
    * be free for reading and writing to.
    */
    template <typename T>
    void MtmVec<T>::unlockCell(int pos){
        size_t pos_unsigned=(size_t)pos;
        assert(pos>=0 && pos_unsigned<data.size());
        lock[pos_unsigned]=true;
    }

                        ////////Iterators////////

    template <typename T>
    MtmVec<T>::iterator::iterator(MtmVec<T>* vec_ptr, int i) :
    vec (vec_ptr), i(i) {
        if (vec_ptr== nullptr) throw MtmExceptions::IllegalInitialization();
     }

     template <typename T>
     void MtmVec<T>::iterator::operator++() {
         ++i;
     }

     template <typename T>
     T& MtmVec<T>::iterator::operator*(){
        return (*vec)[i];
     }

     template <typename T>
     bool MtmVec<T>::iterator::operator!=(const iterator& j) const{
         return i != j.i;
     }

    template <typename T>
    bool MtmVec<T>::iterator::operator==(const iterator& j) const{
        return i == j.i;
    }

    template <typename T>
    typename MtmVec<T>::iterator MtmVec<T>::begin(){
        return iterator(this,0);
     }

    template <typename T>
    typename MtmVec<T>::iterator MtmVec<T>::end(){
        return iterator(this,(int)data.size());
    }

    template <typename T>
    MtmVec<T>::nonzero_iterator::nonzero_iterator(MtmVec<T>* vec_ptr, int i) :
            MtmVec<T>::iterator(vec_ptr,i) {}

    template <typename T>
    void MtmVec<T>::nonzero_iterator::operator++() {
        this->i++;
        for (int i=this->i; i<this->vec->size();i++) {
            if ((*this->vec)[i] == T()) {
                this->i++;
            }
            else {
                return;
            }
        }
    }


    template <typename T>
    typename MtmVec<T>::nonzero_iterator MtmVec<T>::nzbegin(){
        nonzero_iterator it(this,0);
        if (data[0]!=T()){
            return it;
        }
        ++it;
        return it;
    }

    template <typename T>
    typename MtmVec<T>::nonzero_iterator MtmVec<T>::nzend(){
        return nonzero_iterator(this,(int)data.size());
    }


}
#endif //EX3_MTMVEC_H
