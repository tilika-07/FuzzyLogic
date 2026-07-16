#ifndef PRODUCT_TNORM_H
#define PRODUCT_TNORM_H
#include "TNorm.h"
class ProdTNorm:public TNorm{
    public:
        double apply(double a,double b) const override;
};
#endif