#include <fstream>
#include <filesystem>
#include <random>
#include <set>
#include "Generator.h"
#include "DataExport.h"


namespace jobshop {

    void GeneratorRnd::load(int _seed, int num, vector<JobshopData> &IODs) {

        if (_seed >= 0) {
            GConf.seed = _seed;
        }

        mt19937 genCommon(GConf.seedCommon);
        mt19937 gen(GConf.seed);

        IODs.clear();


        IODs.resize(num);

        // generate OMtime matrix with processing times of operations on machines
        // this also includes the sets of machines capable of precessing operations
        // common all generated data

        vector<vector<int>> OMtime(GConf.numO, vector<int>(GConf.numM, 0));

        if (GConf.RangeOM.second > GConf.numM) throw string("RangeTM.second > numM");

        for( auto &Mtime: OMtime ) {
            //< how many machines will process given operation
            int M = uniform_int_distribution<int>(GConf.RangeOM.first, GConf.RangeOM.second)(genCommon);

            fill(Mtime.begin(), Mtime.end(), 0);

            set<int> MachineSet;
            while( MachineSet.size() < M ) {
                MachineSet.insert(uniform_int_distribution<int>(0, GConf.numM - 1)(genCommon));
            }

            for ( auto m: MachineSet ) {
                Mtime[m] = uniform_int_distribution<int>(GConf.RangeD.first, GConf.RangeD.second)(genCommon);
            }
        }

        // generate all the problems with common OMtime matrix
        for ( int j = 0; j < IODs.size(); j++ ) {

            JobshopData &IOD = IODs[j];

            IOD.name = GConf.nameBase + "_" + jobshop::to_string(j, 3);

            IOD.numJ = uniform_int_distribution<int>(GConf.RangeJ.first, GConf.RangeJ.second)(gen);
            IOD.numM = GConf.numM;
            IOD.numO = GConf.numO;

            // generate jobs and their operations
            IOD.Jobs.resize(IOD.numJ);

            for( int j = 0; j < IOD.Jobs.size(); j++ )  {
                auto &J = IOD.Jobs[j];
                J.idx = j;

                // generate number of operations for a given job
                int noj = uniform_int_distribution<int>(GConf.RangeJO.first, GConf.RangeJO.second)(gen);

                if (GConf.multiOperation) {
                    for( int i = 0; i < noj; i++ ) {
                        J.Ops.push_back(uniform_int_distribution<int>(0, GConf.numO-1)(gen));
                    }
                } else {
                    if (GConf.RangeJO.second > GConf.numO) throw string("RangeJO.second > numO");
                    set<int> OpSet;
                    while (OpSet.size() < noj) {
                        OpSet.insert(uniform_int_distribution<int>(0, GConf.numO-1)(gen));
                    }
                    for ( auto o: OpSet ) {
                        J.Ops.push_back(o);
                    }
                    shuffle(J.Ops.begin(), J.Ops.end(), gen);
                }
            }

            IOD.OMtime = OMtime;
        }
    };


}