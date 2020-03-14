#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMatSq : public MtmMat<T> {
    public:
        /*
         * Rectangular Matrix constructor, m is the number of rows and columns
         * in the matrix and val is the initial value for the matrix elements.
         */
        explicit MtmMatSq (size_t m, const T& val=T());
        explicit MtmMatSq(const MtmMat<T>&);

        void resize(Dimensions new_dim, const T& val=T()) override;
        void reshape(Dimensions newDim) override;
    };

                        ////////Constructors////////

    template <typename T>
    MtmMatSq<T>::MtmMatSq(size_t m, const T &val) :
    MtmMat<T>(Dimensions(m,m),val)  {}

    /*
     * Conversion constructor from regular to squared matrix. If the matrix
     * is not a squared matrix, MtmExceptions::IllegalInitialization() will
     * be thrown.
     */
    template <typename T>
    MtmMatSq<T>::MtmMatSq(const MtmMat<T>& mat_to_sq) :
    MtmMat<T>(Dimensions((size_t)mat_to_sq.getRow(),(size_t)mat_to_sq
    .getCol()),T()){
        if (mat_to_sq.getRow()!=mat_to_sq.getCol()){
            throw MtmExceptions::IllegalInitialization();
        }
        int mat_size=this->getCol();
        for (int i=0;i<mat_size;i++){
            for (int j=0;j<mat_size;j++){
                this->matrix[i][j]=mat_to_sq[i][j];
            }
        }
    }
                        ////////Squared matrix functions////////

    template <typename T>
    void MtmMatSq<T>::resize(Dimensions new_dim, const T& val) {
        if (new_dim.getCol()!=new_dim.getRow()){
            throw MtmExceptions::ChangeMatFail(this->dim,new_dim);
        }
        MtmMat<T>::resize(new_dim,val);
    }

    /*
     * reshape function not available for derived classes, so
     * MtmExceptions::ChangeMatFail exception will be thrown.
     */
    template <typename T>
    void MtmMatSq<T>::reshape(Dimensions newDim){
        throw MtmExceptions::ChangeMatFail(this->dim,newDim);
    }




}

#endif //EX3_MTMMATREC_H
