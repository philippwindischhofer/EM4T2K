#ifndef IngridSIMVERTEXSUMMARY_H
#define IngridSIMVERTEXSUMMARY_H
#include <iostream>
#include <vector>
#include "TObject.h"

//using namespace std;

//......................................................................

class IngridSimVertexSummary : public TObject
{
public:
    IngridSimVertexSummary();
    virtual ~IngridSimVertexSummary();
    
    void Clear   (Option_t* option="");
    void Print(Option_t*) const;
    
public:
// Flux information
    int nutype;              // Neutrino type (generator-specific numbering
                             // scheme)
    float numomentum[3];     // Neutrino 3-momentum (GeV/c)
    float nuE;               // Neutrino Energy (GeV)
    float norm;							 // Weighting factor of Jnubeam				
    int ng;                  // # of parents (# of generations)
    std::vector<int> gpid;        // particle ID of each ancestor particle, gpid[0]=primary proton
    std::vector<int> gmec;        // particle production mechanism
    std::vector<float> gposx;     // interaction vertex point of each ancestor
    std::vector<float> gposy;     // interaction vertex point of each ancestor
    std::vector<float> gposz;     // interaction vertex point of each ancestor
    std::vector<float> gmomx;     // directional momentum
    std::vector<float> gmomy;     // directional momentum
    std::vector<float> gmomz;     // directional momentum
    std::vector<float> gcosbm;    // cosine of angle between ancestor and beam

// Neutrino interaction information
    int targeta;             // Atomic weight of target nucleus (TO BE FIXED)
    int targetz;             // Atomic number of target nucleus
    int targettype;          // Neutrino target type (generator-specific)
    float pfsurf;            // Fermi surface momentum (GeV/c)
    float vnuclini;          // Nuclear potential for the target initial state (GeV)
    int inttype;             // Neutrino interaction type (generator-specific
    int mod;                 // Interaction vertex module
    float xnu;               // interaction vertex x (from jnubeam)
    float ynu;               // interaction vertex y (from jnubeam)
    float znu;               // interaction vertex z (from jnubeam)
    float totcrsne;					 // Total cross section [10^-38 cm^2]

private:

    ClassDef(IngridSimVertexSummary, 5) //  Simulation (generator + detector mc) 
                                    //  interaction vertex Summary
};

#endif // IngridSIMVERTEXSUMMARY_H
////////////////////////////////////////////////////////////////////////
