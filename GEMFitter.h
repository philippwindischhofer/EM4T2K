/*
 * Copyright 2017 Philipp Windischhofer (philipp.windischhofer@polytechnique.edu)
 * libgem2ROOT
 */

#ifndef __GEMFitter_h
#define __GEMFitter_h

#include <iostream>
#include <vector>
#include "RootIncludes.h"

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
    double GetColor();
    double GetStyle();
    void SetColor(double newcolor);
    void SetStyle(double newstyle);

private:
    GEMVector refPoint;
    GEMVector dirVect;
    double color;
    double style;
};

class GEMFitter
{
public:
    GEMFitter(int dim);
    void AddPoint(GEMVector& pt);
    void Reset();
    void PerformFit(double scalecrit, double convcrit);
    int GetNumberLines();
    GEMLine GetLine(int n);
    void SetScreenSize(GEMVector& min_new, GEMVector& max_new);
    void PrintDataset();      

private:
    int dim;
    struct gem_ws* emws;
    struct dataset* ds;
    const static double degen_param = 0.1;
    GEMVector min, max;
    void SetScreenSizeLibgem();
};

#endif
