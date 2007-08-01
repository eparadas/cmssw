#ifndef _CSCGAINSCONDITIONS_H
#define _CSCGAINSCONDITIONS_H

#include <memory>
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"

class CSCGainsConditions: public edm::ESProducer, public edm::EventSetupRecordIntervalFinder  {
 public:
  CSCGainsConditions(const edm::ParameterSet&);
  ~CSCGainsConditions();
  
  float mean,min,minchi;
  int seed;long int M;
  int old_chamber_id,old_strip,new_chamber_id,new_strip;
  float old_gainslope,old_intercpt, old_chisq;
  std::vector<int> old_cham_id;
  std::vector<int> old_strips;
  std::vector<float> old_slope;
  std::vector<float> old_intercept;
  std::vector<float> old_chi2;
  float new_gainslope,new_intercpt, new_chisq;
  std::vector<int> new_cham_id;
  std::vector<int> new_strips;
  std::vector<float> new_slope;
  std::vector<float> new_intercept;
  std::vector<float> new_chi2;
  
  void prefillGains();

  typedef const  CSCGains * ReturnType;
  
  ReturnType produceGains(const CSCGainsRcd&);
  
 private:
  // ----------member data ---------------------------
  void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&, edm::ValidityInterval & );
  CSCGains *cngains ;

};

#endif
