#include "FWCore/Framework/interface/Event.h"

#include "EventFilter/L1TRawToDigi/interface/Packer.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include "BMTFTokens.h"

namespace l1t {
   namespace stage2 {
      class BMTFPackerInputs : public Packer 
      {
         public:
            virtual Blocks pack(const edm::Event&, const PackerTokens*) override;
         private:
            std::map<unsigned int, std::vector<uint32_t> > payloadMap_;

            uint32_t wordPhMaker(const L1MuDTChambPhDigi& phInput);
            uint32_t wordThMaker(const L1MuDTChambThDigi& thInput, const bool& qualFlag);

            static const unsigned int phiMask = 0xFFF;
            static const unsigned int phiShift = 11;
            static const unsigned int phiBMask = 0x3FF;
            static const unsigned int phiBShift = 21;
            static const unsigned int qualMask = 0xF;
            static const unsigned int qualShift = 26;
            static const unsigned int rpcMask = 0x1;
            static const unsigned int rpcShift = 27;
            static const unsigned int bxCntMask = 0x3;
            static const unsigned int bxCntShift = 0;

            static const int ownLinks_[];
            //<link, <bx, <st, payload> > > by that way of mapping we benefit from the auto sorting of the map 
            std::map<unsigned int, std::map<int, std::map<unsigned int, uint32_t > > > phiPayloadFromAllLinks_;
            //<link, <bx, payload> > > by that way of mapping we benefit from the auto sorting of the map 
            std::map<unsigned int, std::map<int, std::map<unsigned int, uint32_t > > > thePayloadFromAllLinks_;

      };
   }
}
