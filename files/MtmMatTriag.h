#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;

namespace MtmMath {

    template<typename T>
    class MtmMatTriag : public MtmMatSq<T> {
    private:
        bool is_upper;
    public:
        /*
         * Triangular Matrix constructor, m is the number of rows and columns
         * in the matrix, val is the initial value for the matrix elements
         * and isUpper_ is whether it is upper
         * Rectangular matrix (true means it is)
         */
        explicit MtmMatTriag(size_t m, const T &val = T(),
                bool isUpper_t = true);
        MtmMatTriag(const MtmMatTriag& mat);
        explicit MtmMatTriag (const MtmMat<T>&);

        void resize(Dimensions new_dim, const T& val=T()) override;
        void transpose() override;
        void lockUpper(); //lock upper triangle of the matrix
        void lockLower(); //lock lower triangle of the matrix
    };

                        ////////Constructors////////
/*
 * Constructor for Triangle matrix. The constructor assigns val to the upper
 * or lower triangle according to is_upper , and "locks" the other triangle to
 * prevent writing to it.
 * reading from locked elements will be done with const iterators.
 */
    template<typename T>
    MtmMatTriag<T>::MtmMatTriag(size_t m, const T &val, bool isUpper_t) :
            MtmMatSq<T>(m, val), is_upper(isUpper_t) {
        int mat_size = this->getCol();
        for (int i = 0; i < mat_size; i++) {
            for (int j = 0; j < mat_size; j++) {
                if (isUpper_t && i > j) {
                    this->matrix[i][j] = T();
                    this->matrix[i].lockCell(j);
                }
                if (!isUpper_t && i < j) {
                    this->matrix[i][j] = T();
                    this->matrix[i].lockCell(j);
                }
            }
        }
    }

    /*
     * copy constructor for triangle matrix
     */
    template<typename T>
    MtmMatTriag<T>::MtmMatTriag(const MtmMatTriag& mat):
    MtmMatSq<T>::MtmMatSq(mat), is_upper(mat.is_upper){
        if (is_upper) {
            lockLower();
        }
        else {
            lockUpper();
        }
    }

    /*
     * Conversion constructor from regular to triangle matrix. If the matrix
     * is not a triangle matrix, MtmExceptions::IllegalInitialization() will
     * be thrown. The relevent part of the matrix will be locked to prevent
     * writing to.
     */
   template<typename T>
   MtmMatTriag<T>::MtmMatTriag(const MtmMat<T>& mat) :
    MtmMatSq<T>((size_t)mat.getRow(),T()), is_upper(true){
        if (mat.getCol()!=mat.getRow()){
            throw MtmExceptions::IllegalInitialization();
        }
        bool is_upper_t= true;
        bool is_lower_t= true;
        for (int i=0;i<mat.getRow();i++){
            for (int j=0;j<mat.getCol();j++){
                if (j<i&&mat[i][j]!=T()) is_upper_t=false; //checks if Upper
                if (j>i&&mat[i][j]!=T()) is_lower_t=false; //checks if Lower
                this->matrix[i][j]=mat[i][j];
            }
        }
        if (!is_upper_t&&!is_lower_t) {throw
        MtmExceptions::IllegalInitialization();}
        if (is_upper_t&&!is_lower_t) {
            this->is_upper=true;
            this->lockLower();
        }
        if (is_lower_t) {
            this->is_upper=false;
            this->lockUpper();
        }
    }

                        ////////Triangle matrix functions////////
/*
 * resize for triangle matrix. If the new dimension is bigger then the old
 * one, the new elements will be assigned with val, and the relevent
 * elemnents will be locked to prevent writing to.
 */
    template <typename T>
    void MtmMatTriag<T>::resize(Dimensions new_dim, const T &val)  {
        Dimensions old_dim=this->dim;
        MtmMatSq<T>::resize(new_dim, val);
        if(old_dim.getRow()>new_dim.getRow()){
            return;
        }
        bool is_upper_t=this->is_upper;
        int mat_size = this->getCol();
        for (int i = 0; i < mat_size; i++) {
            for (int j = 0; j < mat_size; j++) {
                if (is_upper_t && i > j) {
                    this->matrix[i][j] = T();
                }
                if (!is_upper_t && i < j) {
                    this->matrix[i][j] = T();
                }
            }
        }
        if (is_upper_t) {
            lockLower();
        }
        else {
            lockUpper();
        }
    }

    template <typename T>
    void MtmMatTriag<T>::transpose(){
        MtmMat<T>::transpose();
        is_upper = !is_upper;
        if(is_upper) {
            lockLower();
        }
        else {
            lockUpper();
        }

    }
                        ////////Helper functions////////
/*
 * Mark the upper triangle of the matrix as locked, i.e if the user tries to
 * write to the cell , an IllegalAccess exception will be thrown. Reading
 * from these elements will be dont using const iterators.
 */
    template <typename T>
    void MtmMatTriag<T>::lockUpper() {
        for (int i = 0; i < this->getRow(); i++) {
            for (int j = 0; j < this->getCol(); j++) {
                if (j > i) {
                    this->matrix[i].lockCell(j);
                }
            }
        }
    }

/*
 * Mark the lower triangle of the matrix as locked, i.e if the user tries to
 * write to the cell , an IllegalAccess exception will be thrown. Reading
 * from these elements will be dont using const iterators.
 */
    template <typename T>
    void MtmMatTriag<T>::lockLower(){
        for (int i = 0; i < this->getRow(); i++) {
            for (int j = 0; j < this->getCol(); j++) {
                if (j < i) {
                    this->matrix[i].lockCell(j);
                }
            }
        }
    }


}

#endif //EX3_MTMMATTRIAG_H
