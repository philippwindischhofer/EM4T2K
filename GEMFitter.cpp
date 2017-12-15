/*
 * Copyright 2017 Philipp Windischhofer (philipp.windischhofer@polytechnique.edu)
 * libgem2ROOT
 */

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
    remove_degenerated_objects_gem(emws, degen_param * ds -> nb_points);
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

std::vector<GEMTrack> GEMFitter::GetTracks()
{
    std::vector<GEMTrack> retval;
    int number_lines = GetNumberLines();
    int number_points = ds -> nb_points;

    // first get all the lines and construct the individual tracks, still without their hits assigned
    for(int i = 0; i < number_lines; i++)
    {
	GEMLine cur = GetLine(i);
	retval.push_back(GEMTrack(cur));
    }

    // now, iterate over all points and assign each to the line with the greatest weight
    for(int cur_point = 0; cur_point < number_points; cur_point++)
    {
	int matched_line = 0;
	double max = 0;

	for(int cur_line = 0; cur_line < number_lines; cur_line++)
	{
	    double cur = emws -> fit_weights[cur_line] -> coefs[cur_point];
	    if(cur > max)
	    {
		matched_line = cur_line;
		max = cur;
	    }
	}

	// matched_line now contains the index of the line to which this point / hit should be assigned
	GEMVector hit(dim, ds -> points[cur_point] -> coords);
	retval[matched_line].AddHit(hit);
    }
    
    return retval;
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

GEMLine::GEMLine(int dim) : refPoint(dim), dirVect(dim), color(kRed), style(2)
{ }

GEMLine::GEMLine(GEMVector refPoint, GEMVector dirVect) : refPoint(refPoint), dirVect(dirVect), color(kRed), style(2)
{ }

GEMLine::GEMLine(const GEMLine& line) : refPoint(line.GetRefPoint()), dirVect(line.GetDirVect()), color(kRed), style(2)
{ }

GEMVector GEMLine::GetRefPoint() const
{
    return refPoint;
}

GEMVector GEMLine::GetDirVect() const
{
    return dirVect;
}

double GEMLine::GetColor()
{
    return color;
}

double GEMLine::GetStyle()
{
    return style;
}

void GEMLine::SetColor(double newcolor)
{
    color = newcolor;
}

void GEMLine::SetStyle(double newstyle)
{
    style = newstyle;
}

//---------------------------

GEMTrack::GEMTrack(GEMLine& line) : line(line)
{ }

void GEMTrack::AddHit(GEMVector& hit)
{
    hits.push_back(hit);
}

void GEMTrack::AddHits(std::vector<GEMVector>& hits)
{
    for(int i = 0; i < hits.size(); i++)
    {
	AddHit(hits.at(i));
    }
}

std::vector<GEMVector> GEMTrack::GetHits()
{
    return hits;
}

GEMLine GEMTrack::GetLine()
{
    return line;
}
