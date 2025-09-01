//
// Created by tsliwins on 02.12.24.
//

#pragma once
#include <vector>
#include <string>

#include "JobshopData.h"


namespace jobshop {
    using namespace std;
    // using namespace nnutils;

    struct GeneratorRnd {

        struct GenConfigType {
            string nameBase;
            int seedCommon;
            int seed;
            int numM;
            int numO;
            bool multiOperation; //< are multiple tasks in the same job permited
            pair<int, int> RangeOM;   //< how many machines for single operation
            pair<int, int> RangeJ;    //< how many jobs
            pair<int, int> RangeJO;   //< how many operations is a single job
            pair<int, int> RangeD;    //< task duration range
        };

        GenConfigType GConf;

        GeneratorRnd( const GenConfigType &_Conf  ) : GConf(_Conf) {
        }

        void load(int _seed, int _num, vector<JobshopData> &IODs);
    };






}

