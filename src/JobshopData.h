#pragma once
#include <string>
#include <vector>


namespace jobshop {
    using namespace std;

    struct JobshopData {

        struct JType {
            int idx = -1;
            vector<int> Ops; //< list of operation types

            bool operator==(const JType &o) const {
                return idx == o.idx;
            }
            bool operator!=(const JType &o) const {
                return !(*this == o) ;
            }

            JType() {
            }

            JType(int _idx) : idx(_idx) {
            }

            JType(int _idx, const vector<int> &ops) : idx(_idx), Ops(ops) {
            }

        };

        struct Dec {
            int m = -1;     //< machine
            int start_t = -1; //< start time
            int end_t = -1;   //< end time
            int j = -1;     //< job
            int iop = -1; //< idx of task in a seqence of tasks of job j

            Dec() {}
            Dec(int _m, int _o, int _e, int _j, int _iop) : m(_m), start_t(_o), end_t(_e), j(_j), iop(_iop) {
            }
        };

        // DataConcept interface

        struct SolutionType {
            double obj;
            vector<Dec> Decs;
            int cluster = -1;

            // interface of the SolutionConcept
            double getObj() const {
                return obj;
            }
            void setObj (double _obj) {
                obj = _obj;
            }
            void setCluster(int c) {
                cluster = c;
            }
        };

        double getObj() const {
            return Solution.getObj();
        }

        const SolutionType& getSolution() {
            return Solution;
        }

        void setSolution(const SolutionType &_solution) {
            Solution = _solution;
        }

        // interface end


        string name;


        int numJ; //< number of jobs (redundant)
        int numO; //< number of operation types
        int numM; //< number of machines

        vector<JType> Jobs;
        vector<vector<int>> OMtime; //< [Operation][Machine] -> processing time; 0 means operation cannot be processed on a given machine

        SolutionType Solution;

        JobshopData() {
        }

    };


}

