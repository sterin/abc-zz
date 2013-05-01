#include "Prelude.hh"
#include "ZZ_CmdLine.hh"
#include "Netlist.hh"
#include "ExportImport.hh"
#include "StdLib.hh"
#include "StdPob.hh"

using namespace ZZ;

namespace ZZ { void test(); }



//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm


double testtest(const Netlist& N)
{
    double T0 = cpuTime();
    for (uint n = 0; n < 10; n++){
        For_Gates(N, w)
            if (type(w) == gate_And){
                Wire w0 = w[0];
                w.set(0, w[1]);
                w.set(1, w0);
            }
    }
    double T1 = cpuTime();

    return T1 - T0;
}


int main(int argc, char** argv)
{
    ZZ_Init;

    Netlist N;
    //WriteLn "Time: %t", testtest(N);

    double T0 = cpuTime();
//    readAigerFile("/home/een/ZZ/Bip/bjrb07amba9andenv_smp.aig", N, true);
    readAigerFile("/home/een/ZZ/Bip/s_00005_0007.aig", N, true);
    WriteLn "Parse time: %t", cpuTime() - T0;

    {
        TimeIt;
        testtest(N);
    }

#if 0
    upOrderTest(N);
    double T1 = cpuTime();
    N.save("tmp.gnl");
    WriteLn "Save time: %t", cpuTime() - T1;
#endif

    return 0;
}
