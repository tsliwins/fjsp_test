#include "Generator.h"
#include "JobshopData.h"
#include <string>
#include "DataExport.h"

using namespace std;

#include <filesystem>

int main() {

    std::filesystem::current_path(".");

    typedef jobshop::GeneratorRnd GeneratorType;

    typedef GeneratorType::GenConfigType GenConfigType;

    vector<jobshop::JobshopData> TotalData, ValidationData, TestData;

    // generate and store random problems
    for ( pair<int, int> JM: {make_pair(10, 5), make_pair(15, 10), make_pair(20, 10), make_pair(30, 10) , make_pair(30, 15)} ) {
            int J = JM.first;
            int numM = JM.second;

            for ( pair<int, int> RangeJO : {make_pair(5,10), make_pair(10,15)}) {
                for (int numO: {15, 30}) {
                    for ( pair<int, int> RangeOM: {make_pair(2, 3), make_pair(2, 5)} ) {
                        pair<int, int> RangeJ = make_pair((int)(0.8f*J), (int)(1.2f*J));

                        string basename = string("")
                                + "n(" + to_string(RangeJ.first) + "-" + to_string(RangeJ.second) + ")" +
                                + "_m(" + to_string(numM) + ")"
                        + "_k_j(" + to_string(RangeJO.first) + "-" + to_string(RangeJO.second) + ")"
                        + "_t(" + to_string(numO) + ")"
                        + "_flex(" + to_string(RangeOM.first) + "-" + to_string(RangeOM.second) + ")";


                        GenConfigType GConfTmp;
                        // //  GConf.dir = "data/jobshop";
                        GConfTmp.nameBase = basename;
                        GConfTmp.seedCommon = 1;
                        GConfTmp.seed = 10000;
                        GConfTmp.numM = numM;
                        GConfTmp.numO = numO;
                        GConfTmp.multiOperation = false;
                        GConfTmp.RangeOM = RangeOM;
                        GConfTmp.RangeJ = RangeJ;
                        GConfTmp.RangeJO = RangeJO;
                        GConfTmp.RangeD = {10, 100};

                        GeneratorType Generator(GConfTmp);

                        Generator.load(1, 1000000, TotalData);
                        Generator.load(2, 10000, ValidationData);
                        Generator.load(3, 10, TestData);



                        jobshop::dataExport_fjs(TotalData, "data/" + basename + "/training", "");
                        jobshop::dataExport_fjs(ValidationData, "data/" + basename + "/validation", "");
                        jobshop::dataExport_fjs(TestData, "data/" + basename + "/test", "");

                    }
                }
            // }
        }
    }

    return 0;



} // main