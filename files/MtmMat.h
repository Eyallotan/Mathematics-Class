#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmVec.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMat {
    protected:
        Dimensions dim;
        vector<MtmVec<T> > matrix;
    public:
        /*
         * Matrix constructor, dim_t is the dimension of the matrix and val
         * is the initial value for the matrix elements.
         */
        explicit MtmMat(Dimensions dim_t, const T& val=T());
        MtmMat(const MtmMat& mat);
        ~MtmMat() = default;
        explicit MtmMat(const MtmVec<T>& vec);
        /*
         * Matrix operators:
         */
        MtmMat& operator=(const MtmMat&);
        MtmMat& operator+=(const MtmMat&);
        MtmMat& operator-=(const MtmMat&);
        MtmMat operator-() const;
        MtmVec<T>& operator[](int pos);
        const MtmVec<T>& operator[](int pos) const;
        /*
         * Helper functions for MtmMat
         */
        int getRow() const;
        int getCol() const;
        Dimensions getDim() const;
        void unlockMatrix();
        /*
         * Function that get function object f and uses it's () operator on
         * each element in the matrix columns. It outputs a vector in the
         * size of the matrix columns where each element is the final output
         * by the function object's * operator
         */
        template <typename Func>
        MtmVec<T> matFunc(Func& f) const;
        /*
         * resizes a matrix to dimension dim, new elements gets the value val.
         */
        virtual void resize(Dimensions new_dim, const T& val=T());
        /*
         * reshapes matrix so linear elements value are the same without
         * changing num of elements.
         */
        virtual void reshape(Dimensions newDim);
        /*
         * Performs transpose operation on matrix
         */
        virtual void transpose();
     /*
      * iterator class- An iterator including operator++ for iterating
      * through the matrix, and operator* for accessing elements in the
      * matrix.
      */
    class iterator
    {
    public:
        iterator(MtmMat<T>* mat, int r, int c, Dimensions d);
        virtual void operator++();
        T& operator*();
        bool operator!=(const iterator& j) const;
        bool operator==(const iterator& j) const;
    protected:
        MtmMat<T>* mat_ptr;
        int row;
        int col;
        Dimensions dim;
    };
     /*
        * nonzero_iterator class- An iterator including operator++ for
        * iterating through all non zero elements in the matrix, and
        * operator* for accessing elements in the matrix.
        * Derived from iterator.
     */
        class nonzero_iterator : public iterator
        {
        public:
            nonzero_iterator(MtmMat<T>* mat, int row, int col,Dimensions d);
            void operator++() override;
        };
     /*
     * functions for iterators of MtmMat:
     */
    iterator begin() ;
    iterator end() ;
    nonzero_iterator nzbegin();
    nonzero_iterator nzend();
    };

                        ////////Constructors////////

    template <typename T>
    MtmMat<T>::MtmMat(Dimensions dim_t, const T &val) try: dim(dim_t),
    matrix(dim_t.getRow(),MtmVec<T>(dim_t.getCol(),val)) {
        if (dim_t.getCol()==0||dim_t.getRow()==0) throw
        MtmExceptions::IllegalInitialization();
    }
    catch (std::bad_alloc& e){
        throw MtmExceptions::OutOfMemory();
    }

    /*
     * Conversion constructor from a vector to a regular matrix.
     */
    template <typename T>
    MtmMat<T>::MtmMat(const MtmVec<T>& vec): MtmMat(vec.getDim(),T()){
        int i=0;
        for (iterator it=this->begin();it!=this->end();++it){
            *it=vec[i];
            i++;
        }
    }

    template <typename T>
    MtmMat<T>::MtmMat(const MtmMat& mat) try: dim(mat.dim) , matrix(mat
    .matrix){}
    catch (std::bad_alloc& e) {throw MtmExceptions::OutOfMemory();}

                        ////////Operators////////

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator=(const MtmMat& mat){
        if (this==&mat)
            return *this;
        try {
            matrix = mat.matrix;
        }
        catch (std::bad_alloc& e) {throw MtmExceptions::OutOfMemory();}
        dim=mat.dim;
        return *this;
    }

    template <typename T>
    MtmVec<T>& MtmMat<T>::operator[](int pos){
        size_t pos_unsigned=(size_t)pos;
        if (pos<0||pos_unsigned>=matrix.size()){
            throw MtmExceptions::AccessIllegalElement();
        }
        return matrix[pos_unsigned];
    }

    template <typename T>
    const MtmVec<T>& MtmMat<T>::operator[](int pos) const{
        size_t pos_unsigned=(size_t)pos;
        if (pos<0||pos_unsigned>=matrix.size()){
            throw MtmExceptions::AccessIllegalElement();
        }
        return matrix[pos_unsigned];
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator+=(const MtmMat& mat){
        if (dim!=mat.dim){
            throw MtmExceptions::DimensionMismatch(dim,mat.dim);
        }
        for(int i=0; i<getRow(); i++){
            for(int j=0; j<getCol(); j++)
                matrix[i][j]+=mat[i][j];
        }
        return *this;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat1,const MtmMat<T>& mat2){
        MtmMat<T> temp(mat2);
        return MtmMat<T>(mat1)+=temp;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat,const MtmVec<T>& vec){
        if (mat.getDim()!=vec.getDim()){
            throw MtmExceptions::DimensionMismatch(mat.getDim(),vec.getDim());
        }
        MtmMat<T> temp_mat(vec);
        return temp_mat+mat;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmVec<T>& vec,const MtmMat<T>& mat){
        return mat+vec;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat,const T &val){
        MtmMat<T> temp(mat);
        for(int i=0; i<temp.getRow(); i++){
            for(int j=0; j<temp.getCol(); j++)
                temp[i][j]+=val;
        }
        return temp;
    }

    template <typename T>
    MtmMat<T> operator+(const T &val,const MtmMat<T>& mat){
        return mat+val;
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator-=(const MtmMat<T>& mat){
        if (dim!=mat.dim){
            throw MtmExceptions::DimensionMismatch(dim,mat.dim);
        }
        for(int i=0; i<getRow(); i++){
            for(int j=0; j<getCol(); j++)
                matrix[i][j]-=mat[i][j];
        }
        return *this;
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat1,const MtmMat<T>& mat2){
        MtmMat<T> temp(mat2);
        return MtmMat<T>(mat1)-=temp;
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat,const MtmVec<T>& vec){
        return mat+(-vec);
    }

    template <typename T>
    MtmMat<T> operator-(const MtmVec<T>& vec, const MtmMat<T>& mat){
        return -(mat)+vec;
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat,const T &val){
        MtmMat<T> temp(mat);
        for(int i=0; i<temp.getRow(); i++){
            for(int j=0; j<temp.getCol(); j++)
                temp[i][j]-=val;
        }
        return temp;
    }

    template <typename T>
    MtmMat<T> operator-(const T &val,const MtmMat<T>& mat){
        return (-mat)+val;
    }

    template <typename T>
    MtmMat<T> MtmMat<T>::operator-() const{
        MtmMat<T> res_mat(dim,T());
        for(int i=0; i<getRow(); i++){
            for(int j=0; j<getCol(); j++)
                res_mat[i][j]=-matrix[i][j];
        }
        res_mat.dim=dim;
        return res_mat;
    }

    template <typename T>
    MtmMat<T> operator*(MtmMat<T>& mat, const T &val){
        MtmMat<T> res(mat);
        for(int i=0; i<res.getRow(); i++){
            for(int j=0; j<res.getCol(); j++)
                res[i][j]*=val;
        }
        return res;
    }

    template <typename T>
    MtmMat<T> operator*(const T &val, MtmMat<T>& mat){
        return mat*val;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat1, const MtmMat<T>& mat2){
        MtmMat<T> temp1(mat1);
        MtmMat<T> temp2(mat2);
        if (mat1.getCol()!=mat2.getRow()){
            throw MtmExceptions::DimensionMismatch
            (mat1.getDim(),mat2.getDim());
        }
        Dimensions dim((size_t)mat1.getRow(),(size_t)mat2.getCol());
        MtmMat<T> res_mat(dim,T());
        int row_m1=0, row_m2=0, col_m1=0, col_m2=0;
        for(typename MtmMat<T>::iterator it=res_mat.begin(); it!=res_mat.end();
        ++it) {
            while(row_m2 < mat2.getRow()) {
                *it += temp1[row_m1][col_m1] * temp2[row_m2][col_m2];
                col_m1++;
                row_m2++;
            }
            col_m1=0;
            row_m2=0;
            row_m1++;
            if(row_m1==mat1.getRow()) {
                row_m1=0;
                col_m2++;
            }
        }
        return res_mat;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat, const MtmVec<T>& vec){
        MtmMat<T> temp_mat(vec);
        return mat*temp_mat;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& vec, const MtmMat<T>& mat){
        MtmMat<T> temp_mat(vec);
        return temp_mat*mat;
    }


    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& vec1, const MtmVec<T>& vec2){
        MtmMat<T> mat1(vec1);
        MtmMat<T> mat2(vec2);
        return mat1*mat2;
    }

                            ////////Matrix Functions////////

    template <typename T>
    void MtmMat<T>::transpose() {
        Dimensions new_dim=dim;
        new_dim.transpose();
        MtmMat<T> new_mat(new_dim,T());
        unlockMatrix(); // for handling triangle matrices
        for (int i=0;i<new_mat.getCol();i++){
            for(int j=0;j<new_mat.getRow();j++){
                new_mat[j][i]=matrix[i][j];
            }
        }
        matrix=new_mat.matrix;
        dim=new_dim;
    }


    template <typename T>
    void MtmMat<T>::resize(Dimensions new_dim, const T& val){
        if (new_dim.getCol()==0||new_dim.getRow()==0){
            throw MtmExceptions::ChangeMatFail(dim,new_dim);
        }
        MtmMat<T> new_mat(new_dim,val);
        unlockMatrix(); //for handling triangle matrices
        for(int i=0; i<new_mat.getRow(); i++) {
            for (int j = 0; j < new_mat.getCol(); j++){
                if (i<getRow()&&j<getCol()){
                    new_mat[i][j]=matrix[i][j];
                }
            }
        }
        matrix=new_mat.matrix;
        dim=new_dim;
    }

    template <typename T>
    void MtmMat<T>::reshape(Dimensions newDim) {
        if (dim.getRow()*dim.getCol()!=newDim.getCol()*newDim.getRow()){
            throw MtmExceptions::ChangeMatFail(dim,newDim);
        }
        MtmMat<T> new_mat(newDim,T());
        MtmMat<T>::iterator old_mat_it=this->begin();
        MtmMat<T>::iterator new_mat_it=new_mat.begin();
        while(new_mat_it!=new_mat.end()){
            *new_mat_it=*old_mat_it;
            ++old_mat_it;
            ++new_mat_it;
        }
        matrix=new_mat.matrix;
        dim=newDim;
    }

    template <typename T>
    template <typename Func>
    MtmVec<T> MtmMat<T>::matFunc(Func& f) const{
        MtmVec<T> res((size_t)getCol(),T());
        MtmMat<T> temp = *this;
        temp.transpose(); //work on columns instead of rows
        res.transpose(); //vector returned needs to be a row vector
        for (int i = 0; i < getCol(); i++) {
            Func g;
            res[i] = temp[i].vecFunc(g);
        }
        return res;
    }

                            ////////Helper functions////////

    template <typename T>
    int MtmMat<T>::getRow() const{
        return (int)dim.getRow();
    }

    template <typename T>
    int MtmMat<T>::getCol() const{
        return (int)dim.getCol();
    }

    template <typename T>
    Dimensions MtmMat<T>::getDim() const{
        return dim;
    }

    /*
     * unlock the lock from all cells in the matrix. After using this
     * function, all matrix cell will be available for reading and writing to.
     */
    template <typename T>
    void MtmMat<T>::unlockMatrix(){
        for (int i=0;i<getRow();i++){
            for (int j=0;j<getCol();j++){
                matrix[i].unlockCell(j);
            }
        }
    }

                        ////////Iterators////////

    template <typename T>
    MtmMat<T>::iterator::iterator(MtmMat<T> *mat, int r, int c, Dimensions d)
    : mat_ptr(mat), row(r), col(c) , dim(d){
        if (mat==nullptr) throw MtmExceptions::IllegalInitialization();
        if ((int)d.getRow()==0 || (int)d.getCol()==0) throw
        MtmExceptions::IllegalInitialization();
    }

    template <typename T>
    void MtmMat<T>::iterator::operator++(){
        if (row==(int)dim.getRow()-1){
            row=0;
            ++col;
            return;
        }
        ++row;
    }

    template <typename T>
    T& MtmMat<T>::iterator::operator*(){
        return (*mat_ptr)[row][col];
    }

    template <typename T>
    bool MtmMat<T>::iterator::operator!=(const iterator &j)
    const {
        return (row != j.row || col != j.col);
    }

    template <typename T>
    bool MtmMat<T>::iterator::operator==(const iterator &j)
    const {
        return (row == j.row && col == j.col);
    }

    template <typename T>
    typename MtmMat<T>::iterator MtmMat<T>::begin(){
        return iterator(this,0,0,dim);
    }

    template <typename T>
    typename MtmMat<T>::iterator MtmMat<T>::end(){
        return iterator(this,0,getCol(),dim);
    }

    template <typename T>
    MtmMat<T>::nonzero_iterator::nonzero_iterator(MtmMat<T>* mat, int row,
                                                  int col,Dimensions d):
    MtmMat<T>::iterator(mat,row,col,d){}

    template <typename T>
    void MtmMat<T>::nonzero_iterator::operator++(){
        if (*this==(*this->mat_ptr).nzend()){
            return;
        }
        MtmMat<T>::iterator::operator++();
        while (*this!=(*this->mat_ptr).nzend()) {
            if ((*this->mat_ptr)[this->row].isCellLocked(this->col)){
                MtmMat<T>::iterator::operator++(); //if cell is locked it is 0
            }
            else{
                if ((*this->mat_ptr)[this->row][this->col]==T()){
                    MtmMat<T>::iterator::operator++(); //skip zero elements
                }
                else {
                    return;
                }
            }
        }
    }

    template <typename T>
    typename MtmMat<T>::nonzero_iterator MtmMat<T>::nzbegin(){
        nonzero_iterator it(this,0,0,dim);
        if (matrix[0][0]!=T()){
            return it;
        }
        ++it;
        return it;
    }

    template <typename T>
    typename MtmMat<T>::nonzero_iterator MtmMat<T>::nzend(){
        return nonzero_iterator(this,0,getCol(),dim);
    }


}


#endif //EX3_MTMMAT_H
