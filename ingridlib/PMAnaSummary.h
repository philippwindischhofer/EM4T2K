#ifndef PMANASUMMARY_H
#define PMANASUMMARY_H
#include <iostream>
#include "TObject.h"
#include "TClonesArray.h"

#include "TRefArray.h"
#include "TRef.h"
#include "vector"
//using namespace std;

#include "IngridHitSummary.h"
#include "IngridTrackSummary.h"


#define INGRIDHIT_MAXHITS 1000  //## Temporary
#define INGRIDRECON_MAXTRACKS 10 //## Temporary

//......................................................................
class PMAnaSummary : public TObject{
 public:

  int       Ntrack;
  int       Ningtrack;
  float     clstime;      // time of cluster defined by most high p.e.
  float     clstimecorr;  // time after correction by measurement time of CT
  float     exptime;      // diff. from expected time
  int       hitcyc;       //
  bool      ontime;       //
  bool      vetowtracking; // Upstream VETO
  bool      edgewtracking; // Fiducial CUT
  float     vact[100];

  std::vector<Float_t>  x;
  std::vector<Float_t>  y;
  std::vector<Float_t>  z;
  std::vector<Float_t>  zx;
  std::vector<Float_t>  zy;

  std::vector<Int_t>    startxpln;
  std::vector<Int_t>    startypln;
  std::vector<Float_t>    startxch;
  std::vector<Float_t>    startych;
  std::vector<Int_t>    endxpln;
  std::vector<Int_t>    endypln;
  std::vector<Float_t>    endxch;
  std::vector<Float_t>    endych;
  std::vector<Float_t>  thetax;
  std::vector<Float_t>  thetay;
  std::vector<Float_t>  angle;
  std::vector<Int_t>    ing_startmod;
  std::vector<Int_t>    ing_endmod;
  std::vector<Int_t>    ing_startpln;
  std::vector<Int_t>    ing_endpln;
  std::vector<Bool_t>   ing_trk;
  std::vector<Bool_t>   pm_stop;
  std::vector<Bool_t>   ing_stop;
  std::vector<Float_t>  sci_range;
  std::vector<Float_t>  iron_range;
  std::vector<Int_t>    iron_pene;
  std::vector<Bool_t>   veto; // Upstream VETO
  std::vector<Bool_t>   edge; // Fiducial CUT
  std::vector<Int_t>    pdg;
  std::vector<Float_t>  mucl;
  std::vector<Float_t>  trkpe;

 
  
  //###########################################
  //###########################################
  PMAnaSummary();
  PMAnaSummary(const PMAnaSummary& basicsum);
  virtual ~PMAnaSummary();
  void Clear   (Option_t* option="");
  void Print();
  void AddIngridHit(IngridHitSummary* hit);
  IngridHitSummary* GetIngridHit(int i) const;
  void AddIngridHitTrk(IngridHitSummary* hit, int track);
  IngridHitSummary* GetIngridHitTrk(int i, int track) const;
  void AddTrack(IngridTrackSummary* trk);
  IngridTrackSummary* GetTrack(int i) const;
  int nhits;
  int nhitTs[INGRIDRECON_MAXTRACKS];
  
 private:
  
  TRef fIngridHit[INGRIDHIT_MAXHITS];
  TRef fTrack[INGRIDRECON_MAXTRACKS];
  TRef fIngridHitTrk[INGRIDHIT_MAXHITS][INGRIDRECON_MAXTRACKS];

 public:
  int ntracks;
  int Nhits(){return nhits;}
  int NhitTs(int trk) {return nhitTs[trk];}
  int Ntracks(){return ntracks;}
  ClassDef(PMAnaSummary, 2) // Ingrid Hit Summary
    };

#endif // PMANASUMMARY_H
////////////////////////////////////////////////////////////////////////
