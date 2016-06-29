#include "BMTFPackerInputs.h"


namespace l1t 
{
   namespace stage2 
   {
      const int BMTFPackerInputs::ownLinks_[]={4,5,12,13,20,21,22,23,28,29};
      Blocks BMTFPackerInputs::pack(const edm::Event& event, const PackerTokens* toks)
      {  

        auto muonPhToken = static_cast<const BMTFTokens*>(toks)->getInputMuonTokenPh();
        auto muonThToken = static_cast<const BMTFTokens*>(toks)->getInputMuonTokenTh();

        Blocks blocks;
  
        edm::Handle<L1MuDTChambPhContainer> phInputs;
        event.getByToken(muonPhToken, phInputs);
        edm::Handle<L1MuDTChambThContainer> thInputs;
        event.getByToken(muonThToken, thInputs);


        for( L1MuDTChambPhContainer::Phi_Container::const_iterator iphi =  phInputs->getContainer()->begin(); iphi != phInputs->getContainer()->end(); ++iphi)
        {
          // if ( iphi->scNum() != board_id )
          //   continue;
          //<link, <bx, <st, payload> > > 
          std::map<unsigned int, uint32_t > phWordPerStation;
          phWordPerStation[iphi->stNum()] = wordPhMaker(*iphi);
          std::map<int, std::map<unsigned int, uint32_t > > phWordPerBX;
          phWordPerBX[iphi->bxNum()] = phWordPerStation;
          //up to now we are using only the own links for the emulator.
          phiPayloadFromAllLinks_[ownLinks_[2+iphi->whNum()+iphi->Ts2Tag()]] = phWordPerBX;

        }
      

        for( L1MuDTChambThContainer::The_Container::const_iterator ithe =  thInputs->getContainer()->begin(); ithe != thInputs->getContainer()->end(); ++ithe)
        {
          // if ( ithe->scNum() != board_id )
          //   continue;
          std::map<unsigned int, uint32_t > thWordPerStation;
          thWordPerStation[ithe->stNum()] = wordThMaker(*ithe, false);
          std::map<int, std::map<unsigned int, uint32_t > > thWordPerBX;
          thWordPerBX[ithe->bxNum()] = thWordPerStation;
          thePayloadFromAllLinks_[ownLinks_[2+ithe->whNum()]] = thWordPerBX;
            //qualities are coming in the high
          thWordPerStation[ithe->stNum()] = wordThMaker(*ithe, true);
          thWordPerBX[ithe->bxNum()] = thWordPerStation;
          thePayloadFromAllLinks_[ownLinks_[2+ithe->whNum()+1]] = thWordPerBX;
        }
          //create the payload...
        for(auto &iLink: phiPayloadFromAllLinks_)
        {
          for(auto &iBX:  iLink.second)
          {
            for(auto &iSt: iBX.second)
              payloadMap_[iLink.first].push_back(iSt.second);
          
            if (iBX.second.size() < 4)
            {
              for(unsigned jj=0; jj < 4-iBX.second.size(); jj++)
                payloadMap_[iLink.first].push_back(0);
            }
            
            payloadMap_[iLink.first].push_back(0);
            if (thePayloadFromAllLinks_.find(iLink.first) != thePayloadFromAllLinks_.end())
            {
              if (thePayloadFromAllLinks_[iLink.first].find(iBX.first) != thePayloadFromAllLinks_[iLink.first].end())
              {
                for(auto &iSt: thePayloadFromAllLinks_[iLink.first][iBX.first])
                  payloadMap_[iLink.first].back() |= ( iSt.second & 7 ) << 7;
              }
            }

            payloadMap_[iLink.first].push_back(0);
          }//iBX

        }
        

        for (auto &ip: payloadMap_)
          blocks.push_back(Block(ip.first, ip.second));
         

         return blocks;
      }

      uint32_t BMTFPackerInputs::wordPhMaker(const L1MuDTChambPhDigi& phInput)
      {
        uint32_t temp(0);
        temp = (phInput.phi() & phiMask) << phiShift
              |(phInput.phiB() & phiBMask) << phiBShift
              |(phInput.code() & qualMask) << qualShift
              |(phInput.RpcBit() & rpcMask) << rpcShift
              |(0) << 29
              |(phInput.BxCnt() & bxCntMask) << bxCntShift;

        return temp;
      }

            
      uint32_t BMTFPackerInputs::wordThMaker(const L1MuDTChambThDigi& thInput, const bool& qualFlag)
      {
        uint32_t temp(0);
        if (!qualFlag)
        {
          for (int i=0; i<7; i++)
            temp |= (thInput.position(i) & 0x1) << 1;
        }
        else
        {
          for (int i=0; i<7; i++)
            temp |= (thInput.quality(i) & 0x1) << 1;
        }

        return temp;
      }

  }//ns stage2
}//ns l1t
DEFINE_L1T_PACKER(l1t::stage2::BMTFPackerInputs);