#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PreId.h"
#include "DataFormats/ParticleFlowReco/interface/PreIdFwd.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <string>
#include <vector>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "TLorentzVector.h"

using namespace std;
using namespace reco;

bool checkID( reco::GsfElectronRef e, int iD, double rho, float Esc);
class myLowPtGsfElectronsAnalyzer: public edm::EDAnalyzer {

public:
  
  explicit myLowPtGsfElectronsAnalyzer( const edm::ParameterSet& );
  
  ~myLowPtGsfElectronsAnalyzer() {
  
  }

private:
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  const edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticles_;
  const edm::EDGetTokenT<GenEventInfoProduct> genInfo_;
  const edm::EDGetTokenT< std::vector<reco::GsfElectron> > electrons_;
  const std::vector<edm::InputTag> mvaIdTags_;
  std::vector< edm::EDGetTokenT< edm::ValueMap<float> > >  mvaIds_;
  const std::vector<edm::InputTag> mvaSeedTags_;
  std::vector< edm::EDGetTokenT< edm::ValueMap<float> > >  mvaSeeds_;
  const edm::EDGetTokenT<double> eventrho_;
  //const edm::EDGetTokenT< std::vector<reco::GsfTrack> > gsf_tracks_;
  TH1F *idHist;
  TH1F *nElectron;
  TH2F *idHoverE;
  TH2F *idPt;
  TH1F *EEId;
  TH1F *EBId;
  TH2F *EEIdPt;
  TH2F *EBIdPt;
  TH2F *idsigmaIetaIeta;
  TH2F *EEIdHoverE;
  TH2F *EBIdHoverE;
  TH1F *nEvent;
  TH1F *pt;
  TH1F *nLowPtEle;
  TH1F *vetoPt;
  TH1F *loosePt;
  TH1F *mediumPt;
  TH1F *tightPt;
  TH1F *idWorkingPoints;
  TH1F *electronID;
};

myLowPtGsfElectronsAnalyzer::myLowPtGsfElectronsAnalyzer( const edm::ParameterSet& cfg ) :
  genParticles_{consumes<std::vector<reco::GenParticle> >(edm::InputTag(std::string("genParticles")))},
  genInfo_{consumes<GenEventInfoProduct> (edm::InputTag(std::string("generator")))},
  electrons_{consumes< std::vector<reco::GsfElectron> >(cfg.getParameter<edm::InputTag>("electrons"))},  
  mvaIdTags_(cfg.getParameter< std::vector<edm::InputTag> >("mvaIds")),
  mvaIds_(),
  mvaSeedTags_(cfg.getParameter< std::vector<edm::InputTag> >("mvaSeeds")),
  mvaSeeds_(),
  eventrho_{consumes<double>(edm::InputTag(std::string("fixedGridRhoFastjetAll")))}

{
  for ( const auto& tag : mvaIdTags_ ) { 
    mvaIds_.push_back( consumes< edm::ValueMap<float> >(tag) ); 
  }
  for ( const auto& tag : mvaSeedTags_ ) { 
    mvaSeeds_.push_back( consumes< edm::ValueMap<float> >(tag) ); 
  }
  
  edm::Service<TFileService> fs;
  idHist = fs->make<TH1F>("idHist" , "Electron ID" , 50 ,0, 9.5 );
  nElectron = fs->make<TH1F>("nElectron", "N Electrons", 2,0,2);
  nEvent = fs->make<TH1F>("nEvent", "N Event", 2,0,2);
  nLowPtEle=fs->make<TH1F>("nLowPtEle", "NEle (pt>1||7>pt>1)", 2,0,2);
  idHoverE = fs->make<TH2F>("idHoverE", "ID vs H/E", 160, 0, 0.3, 100 , 0 , 10 );
  idPt = fs->make<TH2F>("idPt", "Id vs Pt", 500, 0, 500, 100 , 0 , 10 );
  EEId = fs->make<TH1F>("EEId" , "Electron ID[EE]" , 100 , 0 , 10 );
  EBId = fs->make<TH1F>("EBId" , "Electron ID[EB]" , 100 , 0 , 10 );
  EEIdPt = fs->make<TH2F>("EEIdPt", "ID vs Pt[EE]", 500, 0, 500, 100 , 0 , 10 );
  EBIdPt = fs->make<TH2F>("EBIdPt", "ID vs Pt[EB]", 500, 0, 500, 100 , 0 , 10 );
  EEIdHoverE = fs->make<TH2F>("EEIdHoverE", "ID vs H/E[EE]", 160, 0, 0.3, 100 , 0 , 10 );
  EBIdHoverE = fs->make<TH2F>("EBIdHoverE", "ID vs H/E[EB]", 160, 0, 0.3, 100 , 0 , 10 );
  pt = fs->make<TH1F>("pt" , "pt" , 500 , 0 , 500 );
  idsigmaIetaIeta = fs->make<TH2F>("idsigmaIetaIeta", "ID vs SigmaIeIe",  480, 0, 60E-3, 100 , 0 , 10);
  vetoPt = fs->make<TH1F>("vetoPt" , "Veto Pt(ID>2)" , 500 , 0 , 500 );
  loosePt = fs->make<TH1F>("loosePt" , "Loose Pt(ID>3.5)" , 500 , 0 , 500 );
  mediumPt = fs->make<TH1F>("mediumPt" , "Medium Pt(ID>5)" , 500 , 0 , 500 );
  tightPt = fs->make<TH1F>("tightPt" , "Tight Pt(ID>6.5)" , 500 , 0 , 500 );
  idWorkingPoints = fs->make<TH1F>("idWorkingPoints", "ID Working Points", 4, 0 ,4);
  electronID = fs->make<TH1F>("electronID", "Old (94X) Electron ID", 4, 0 ,4);
  //idMissingHits = fs->make<TH2F>("idMissingHits", "ID vs NMissingHits", 5, -0.5, 4.5, 12, -1 , 11);
  //idEinvPinv = fs->make<TH2F>("idEinvPinv", "ID vs E inverse - Pinverse",160, 0, 0.2, 12, -1 , 11);

  // idHoverE = fs->make<TH2F>("n")

}

void myLowPtGsfElectronsAnalyzer::analyze( const edm::Event& iEvent, 
					 const edm::EventSetup& iSetup )
{
  edm::Handle< std::vector<reco::GenParticle> > genParticles;
  try{iEvent.getByToken(genParticles_, genParticles);}
  catch (...) {;}
  edm::Handle<double> eventrho;
  iEvent.getByToken(eventrho_, eventrho);
  //iEvent.getByLabel("fixedGridRhoFastjetAll", eventrho);
  cout << "Event Rho = " << *(eventrho.product());
  double Rho = *eventrho.product();
  //double Rho = 5.0;

  edm::Handle<GenEventInfoProduct>  genInfoHandle;
  iEvent.getByToken(genInfo_, genInfoHandle);
  double genWeight=genInfoHandle->weight();

  edm::Handle< std::vector<reco::GsfElectron> > electrons;
  try { iEvent.getByToken(electrons_, electrons); }
  catch (...) {;}
  nEvent->Fill(.5);
  nEvent->Fill(1.5,genWeight);
//############ ID#################
//It Seems there may be two mva IDs in other parts of the code. 
//currently I'm storing this as a vector with only one value
//I'm not super confident in how these IDS are implimented
// but I want to
  
  std::vector< edm::Handle< edm::ValueMap<float> > > mvaIds;

  for ( const auto& token : mvaIds_ ) { 
    edm::Handle< edm::ValueMap<float> > h;
    try { iEvent.getByToken(token, h); }
    catch (...) {;}
    mvaIds.push_back(h);
  }

  std::vector< edm::Handle< edm::ValueMap<float> > > mvaSeeds;
  for ( const auto& token : mvaSeeds_ ) { 
    edm::Handle< edm::ValueMap<float> > h;
    try { iEvent.getByToken(token, h); }
    catch (...) {;}
    mvaSeeds.push_back(h);
  }
  std::vector<reco::GenParticleRef> genElectrons;

  for(unsigned int iGen=0; iGen< genParticles->size(); iGen++){
    reco::GenParticleRef gen (genParticles,iGen);
    if(abs(gen->pdgId())==11 &&
      gen->isDirectHardProcessTauDecayProductFinalState()&&
      gen->pt()>1 &&
      abs(gen->eta())<2.5) {
        genElectrons.push_back(gen);
    }
  }
  for ( unsigned int iter = 0; iter < mvaIds.size(); ++iter ) {
    if ( mvaIds[iter].isValid() &&
	      !mvaIds[iter]->empty() &&
	      electrons.isValid() ) {
      for(unsigned int iElec = 0; iElec<electrons->size(); iElec++){
        reco::GsfElectronRef ele (electrons, iElec);
        nElectron->Fill(.5, genWeight);
        bool matched = false;//For TCP
        //bool matched = true;//For QCD

        for(unsigned int iGenE=0; iGenE<genElectrons.size(); iGenE++){
          reco::GenParticleRef gen = genElectrons[iGenE];
          TLorentzVector e;
          TLorentzVector genE;
          e.SetPtEtaPhiM(ele->pt(), ele->eta(), ele->phi(), ele->mass());
          genE.SetPtEtaPhiM(gen->pt(), gen->eta(), gen->phi(), gen->mass());
          float dr = genE.DeltaR(e);
          if(dr <.1){matched = true;}//For TCP
          //if(dr <.1){matched = false;}//For QCD
        }
        if(matched == true&&
        ele->pt()>1 &&
        ele.isNonnull() &&
        abs(ele->eta())<2.5){
          float E_c=ele->superCluster()->energy();

          for (unsigned int i = 0; i < 5; i++){
            if (checkID(ele, i, Rho, E_c)){
              electronID->Fill(i+.5, genWeight);
            }
          }
          nElectron->Fill(1.5, genWeight);
          pt->Fill(ele->pt(), genWeight);
          nLowPtEle->Fill(.5,genWeight);
          float idVal = (*mvaIds[iter])[ele];
          if (ele->pt()<7){nLowPtEle->Fill(1.5,genWeight);}
          if (idVal >2){
            vetoPt->Fill(ele->pt(),genWeight);
            idWorkingPoints->Fill(0.5,genWeight);
            }
          if (idVal>3.5){
            loosePt->Fill(ele->pt(),genWeight);
            idWorkingPoints->Fill(1.5,genWeight);
            }
          if (idVal>5){
            mediumPt->Fill(ele->pt(),genWeight);
            idWorkingPoints->Fill(2.5,genWeight);
            }
          if (idVal>6.5){
            tightPt->Fill(ele->pt(),genWeight);
            idWorkingPoints->Fill(3.5,genWeight);
            }
          idHist->Fill(idVal, genWeight);
          idHoverE->Fill(ele->hadronicOverEm(), idVal, genWeight);
          idPt->Fill(ele->pt(), idVal, genWeight);
          idsigmaIetaIeta->Fill(ele->full5x5_sigmaIetaIeta(),idVal, genWeight);
          
          if (ele->isEB()){
            EBIdHoverE->Fill(ele->hadronicOverEm(),idVal, genWeight);
            EBIdPt->Fill(ele->pt(),idVal, genWeight);
          }
          else if(ele->isEE()){
            EEIdHoverE->Fill(ele->hadronicOverEm(),idVal, genWeight);
            EEIdPt->Fill(ele->pt(),idVal, genWeight);
          }
        }
      }
    }
  }
}

bool checkID(reco::GsfElectronRef e, int iD, double rho, float Esc) {
  float hoverECut;
  float SigmaIeIeCut;
  float EinvPinvCut;
  //int missingHitsCut;
  float dEtaInSeedCut;
  float dPhiCut;
  //float relIsoWithEACut;
  //bool passconversionVetoCut = true;
  float ePt = e->pt();
  bool result;
  float dEtaInSeed = e->deltaEtaSuperClusterTrackAtVtx()-e->superCluster()->eta()+e->superCluster()->seed()->eta();
  float GsfEleEInverseMinusPInverse = abs((1.0-e->eSuperClusterOverP())/e->ecalEnergy());
  //int mhits = e->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
  if (e->isEB()){
    if (iD==0){
      hoverECut = .05+1.16/Esc+.0324*rho/Esc;
      SigmaIeIeCut =.0126;
      dEtaInSeedCut = .00463;
      dPhiCut = .148;
      EinvPinvCut = .209;
      //missingHitsCut = 2;
      //relIsoWithEACut = .198+.506/ePt;
    }
    else if (iD==1){
      hoverECut = .05+1.16/Esc+.0324*rho/Esc;
      SigmaIeIeCut =.0112;
      dEtaInSeedCut = .00377;
      dPhiCut = .0884;
      EinvPinvCut = .193;
      //missingHitsCut = 1;
      //relIsoWithEACut = .112+.506/ePt;
    }
    else if (iD==3){
      hoverECut = .05+1.16/Esc+.0324*rho/Esc;
      SigmaIeIeCut =.0104;
      dEtaInSeedCut = .00255;
      dPhiCut = .022;
      EinvPinvCut = .159;
      //missingHitsCut = 1;
      //relIsoWithEACut = .026+.506/ePt;
    }
    else if (iD==2){
      hoverECut = .05+1.16/Esc+.0324*rho/Esc;
      SigmaIeIeCut =.0106;
      dEtaInSeedCut = .0032;
      dPhiCut = .0547;
      EinvPinvCut = .184;
      //missingHitsCut = 1;
      //relIsoWithEACut = .0474+.506/ePt;
    } 
  }
  if (e->isEE()){
    if (iD==0){
      hoverECut = .05+2.54/Esc+.183*rho/Esc;
      SigmaIeIeCut =.0457;
      dEtaInSeedCut = .00814;
      dPhiCut = .19;
      EinvPinvCut = .132;
      //missingHitsCut = 3;
      //relIsoWithEACut = .203+.963/ePt;
    }
    else if (iD==1){
      hoverECut = .05+2.54/Esc+.183*rho/Esc;
      SigmaIeIeCut =.0425;
      dEtaInSeedCut = .00674;
      dPhiCut = .169;
      EinvPinvCut = .111;
      //missingHitsCut = 1;
      //relIsoWithEACut = .102+.963/ePt;
    }
    else if (iD==2){
      hoverECut = .05+2.54/Esc+.183*rho/Esc;
      SigmaIeIeCut =.0387;
      dEtaInSeedCut = .00632;
      dPhiCut = .0394;
      EinvPinvCut = .0721;
      //missingHitsCut = 1;
      //relIsoWithEACut = .0658+.963/ePt;
    }
    else if (iD==3){
      hoverECut = .05+2.54/Esc+.183*rho/Esc;
      SigmaIeIeCut =.0353;
      dEtaInSeedCut = .00501;
      dPhiCut = .0236;
      EinvPinvCut = .0197;
      //missingHitsCut = 1;
      //relIsoWithEACut = .0453+.963/ePt;
    } 
  }
  if(e->full5x5_sigmaIetaIeta()<SigmaIeIeCut &&
  e->hadronicOverEm()<hoverECut &&
  dEtaInSeed<dEtaInSeedCut &&
  e->deltaPhiSuperClusterTrackAtVtx()<dPhiCut &&
  GsfEleEInverseMinusPInverse<EinvPinvCut
  ){
    result =  true;
  }
  else{result =  false;}
  return result;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(myLowPtGsfElectronsAnalyzer);
