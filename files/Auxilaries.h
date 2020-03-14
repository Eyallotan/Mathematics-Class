#ifndef EX3_AUXILARIES_H
#define EX3_AUXILARIES_H
#include <string>
#include <iostream>

using std::size_t;

namespace MtmMath {
    class Dimensions {
        size_t row, col;
    public:
        Dimensions(size_t row_t, size_t col_t) : row(row_t), col(col_t) {}
        std::string to_string() const {
            return "(" + std::to_string(row) + "," + std::to_string(col) + ")";
        }

        bool operator==(Dimensions dim2) const {
            return (row==dim2.row) and (col==dim2.col);
        }

        bool operator!=(Dimensions dim2) const {
            return not(*this==dim2);
        }

        void transpose() {
            size_t prev_row=row;
            row=col;
            col=prev_row;
        }

        size_t getRow() const {
            return row;
        }

        size_t getCol() const {
            return col;
        }
    };

}

#endif //EX3_AUXILARIES_H
