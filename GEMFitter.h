#ifndef __GEMFitter_h
#define __GEMFitter_h

#include <iostream>
#include <vector>

extern "C" {
#include "../libgem/libgem.h"
}

class GEMVector
{
public:
    GEMVector(int dim);
    GEMVector(int dim, double* vals);
    void SetCoord(int n, double val);
    double GetCoord(int n) const;
    double* GetPtr();
    void Print() const;

    GEMVector& operator+=(const GEMVector& rhs);
    friend GEMVector operator+(GEMVector lhs, const GEMVector& rhs);
    
    GEMVector& operator-=(const GEMVector& rhs);
    friend GEMVector operator-(GEMVector lhs, const GEMVector& rhs);

    GEMVector& operator*=(float rhs);
    friend GEMVector operator*(GEMVector lhs, float rhs);
    friend GEMVector operator*(float rhs, GEMVector lhs);

private:
    int dim;
    std::vector<double> vals;
};

class GEMLine
{
public:
    GEMLine(int dim);
    GEMLine(GEMVector refPoint, GEMVector dirVect);
    GEMVector GetRefPoint();
    GEMVector GetDirVect();

private:
    GEMVector refPoint;
    GEMVector dirVect;
};

class GEMFitter
{
public:
    GEMFitter(int dim);
    void AddPoint(GEMVector& pt);
    void PerformFit(double scalecrit, double convcrit);
    int GetNumberLines();
    GEMLine GetLine(int n);

private:
    int dim;
    struct gem_ws* emws;
    struct dataset* ds;
};

#endif
