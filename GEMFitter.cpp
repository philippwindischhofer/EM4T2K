#include "GEMFitter.h"

GEMFitter::GEMFitter(int dim) : min(dim), max(dim)
{
    this -> dim = dim;
    ds = new_dataset_zero(0, dim);
}

void GEMFitter::AddPoint(GEMVector& pt)
{
    add_point_dataset(ds, new_valued_point(dim, pt.GetPtr()));
}

void GEMFitter::Reset()
{
    free_dataset(ds);
    ds = new_dataset_zero(0, dim);
}

void GEMFitter::PerformFit(double convcrit, double scalecrit)
{
    SetScreenSizeLibgem();
    emws = multifit(ds, convcrit, scalecrit, LINE, 0, NULL);
    //remove_degenerated_objects_gem(emws, degen_param * ds -> nb_points);
}

int GEMFitter::GetNumberLines()
{
    return emws -> nb_objects;
}

GEMLine GEMFitter::GetLine(int n)
{
    struct line* fitline = ((struct gline*)(emws -> fit_objects[n] -> container)) -> line;
    GEMVector refPoint(dim, fitline -> ref -> coords);
    GEMVector dirVect(dim, fitline -> dir_vect -> coords);
    return GEMLine(refPoint, dirVect);
}

void GEMFitter::SetScreenSize(GEMVector& min_new, GEMVector& max_new)
{
    for(int i = 0; i < dim; i++)
    {	
	min.SetCoord(i, min_new.GetCoord(i));
	max.SetCoord(i, max_new.GetCoord(i));
    }
}

void GEMFitter::SetScreenSizeLibgem()
{
    for(int i = 0; i < dim; i++)
    {
	ds -> point_min -> coords[i] = min.GetCoord(i);
	ds -> point_max -> coords[i] = max.GetCoord(i);
    }
}

void GEMFitter::PrintDataset()
{
    print_dataset(ds);
}

//--------------------------

GEMVector::GEMVector(int dim)
{
    this -> dim = dim;
    
    for(int i = 0; i < dim; i++)
    {
	vals.push_back(0.0);
    }
}

GEMVector::GEMVector(int dim, double* vals)
{
    this -> dim = dim;
    for(int i = 0; i < dim; i++)
    {
	(this -> vals).push_back(vals[i]);
    }
}

GEMVector& GEMVector::operator+=(const GEMVector& rhs)
{
    for(int i = 0; i < dim; i++)
    {
	vals[i] += rhs.GetCoord(i);
    }

    return *this;
}

GEMVector operator+(GEMVector lhs, const GEMVector& rhs)
{
    lhs += rhs;
    return lhs;
}

GEMVector& GEMVector::operator-=(const GEMVector& rhs)
{
    for(int i = 0; i < dim; i++)
    {
	vals[i] -= rhs.GetCoord(i);
    }

    return *this;
}

GEMVector operator-(GEMVector lhs, const GEMVector& rhs)
{
    lhs -= rhs;
    return lhs;
}

GEMVector& GEMVector::operator*=(float rhs)
{
    for(int i = 0; i < dim; i++)
    {
	vals[i] *= rhs;
    }

    return *this;
}

GEMVector operator*(GEMVector lhs, float rhs)
{
    lhs *= rhs;
    return lhs;
}

GEMVector operator*(float rhs, GEMVector lhs)
{
    lhs *= rhs;
    return lhs;
}

void GEMVector::SetCoord(int n, double val)
{
    vals[n] = val;
}

double GEMVector::GetCoord(int n) const
{
    return vals[n];
}

double* GEMVector::GetPtr()
{
    return &vals[0];
}

void GEMVector::Print() const
{
    for(int i = 0; i < dim; i++)
    {
	std::cout << vals[i] << " ";
    }
    std::cout << std::endl;
}

//---------------------------

GEMLine::GEMLine(int dim) : refPoint(dim), dirVect(dim)
{ }

GEMLine::GEMLine(GEMVector refPoint, GEMVector dirVect) : refPoint(refPoint), dirVect(dirVect)
{ }

GEMVector GEMLine::GetRefPoint()
{
    return refPoint;
}

GEMVector GEMLine::GetDirVect()
{
    return dirVect;
}
