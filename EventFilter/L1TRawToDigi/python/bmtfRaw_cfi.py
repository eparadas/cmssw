import FWCore.ParameterSet.Config as cms

bmtfStage2Raw = cms.EDProducer(
    "L1TDigiToRaw",
    Setup = cms.string("stage2::BMTFSetup"),
    InputLabel = cms.InputTag("simBmtfDigis"),
    FedId = cms.vint32(1376,1377),
    FWId = cms.uint32(1),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)

