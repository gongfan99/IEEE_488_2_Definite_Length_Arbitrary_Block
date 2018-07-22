#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "visa.h"
#include "IEEE_488_2_Definite_Length_Arbitrary_Block.h"

int main(int argc, char* argv[]) {
  using dataType = double;
  std::string rsrcName = "TCPIP0::192.168.0.10::inst0::INSTR"; // or "GPIB::1::INSTR"
  ViSession defaultRM, instr;
  try {
    ViUInt32 retCount;
    ViStatus status = viOpenDefaultRM(&defaultRM);
    if (status < 0) throw std::runtime_error("Error: VISA library is not installed.");
    
    // error function
    auto errChk = [&defaultRM](ViStatus ni_status){
      if (ni_status < 0){
        char ErrorMsg[256]; // should be at least 256 bytes
        viStatusDesc(defaultRM, ni_status, ErrorMsg);
        throw std::runtime_error(ErrorMsg);
      }
    };
    
    errChk(viOpen(defaultRM, (ViRsrc)rsrcName.data(), VI_NULL, VI_NULL, &instr));
    
    std::string cmd;
    cmd = ":FORMat:TRACe:DATA REAL,64\n";
    errChk(viWrite(instr, (ViBuf)cmd.data(), (ViUInt32)cmd.size(), (ViPUInt32)&retCount));
    
    cmd = ":FORMat:BORDer SWAPped\n";
    errChk(viWrite(instr, (ViBuf)cmd.data(), (ViUInt32)cmd.size(), (ViPUInt32)&retCount));
    
    cmd = ":SENSe:SWEep:POINts?\n";
    errChk(viWrite(instr, (ViBuf)cmd.data(), (ViUInt32)cmd.size(), (ViPUInt32)&retCount));
    
    std::string sweepPointsStr;
    sweepPointsStr.assign(9, NULL);
    errChk(viRead(instr, (ViPBuf)sweepPointsStr.data(), 9, (ViPUInt32)&retCount));
    int sweepPoints = std::stoi(sweepPointsStr);
    
    cmd = ":CALCulate:DATA1?\n";
    errChk(viWrite(instr, (ViBuf)cmd.data(), (ViUInt32)cmd.size(), (ViPUInt32)&retCount));
    
    std::string readBuf;
    readBuf.resize(sweepPoints * sizeof(dataType) * 2 + 16);
    errChk(viRead(instr, (ViPBuf)readBuf.data(), readBuf.size(), (ViPUInt32)&retCount));
    
    std::vector<dataType> out = parse_IEEE_488_2_Definite_Length_Arbitrary_Block<dataType>(readBuf);
    
    std::ofstream myfile;
    myfile.open("data.csv");
    for (int i = 0; i < out.size() / 2; ++i){
      myfile << out[2 * i] << "," << out[2 * i + 1] << "\n";
    }
    myfile.close();
  } catch (const std::exception& e){
    std::cout << "Error: " << e.what() << std::endl;
  }
  
  viClose(instr);
  viClose(defaultRM);
}