//
// Created by tsliwins on 1.09.2025.
//

#include "Generator.h"
#include "JobshopData.h"
#include <random>
#include <iostream>
#include <string>
#include <fstream>
#include "DataExport.h"
#include <filesystem>

namespace jobshop {
    using namespace std;


   std::string to_string(int n, int width)
    {
        std::ostringstream oss;
        oss.width(width);
        oss.fill('0');
        oss << n;
        return oss.str();
    }

    // Function template to handle file opening with directory creation
    template <typename FileStream>
    FileStream openFileWithDirs(const std::string& filePath,
                          std::ios_base::openmode mode = (std::is_same_v<FileStream, std::ofstream> ?  FileStream::out : FileStream::in ) ) {
        namespace fs = std::filesystem;


        try {
            // Extract directory part from the given file path
            fs::path fullPath(filePath);
            fs::path directory = fullPath.parent_path();

            if constexpr (std::is_same_v<FileStream, std::ofstream>) {
                // Create directories if they do not exist
                if (!directory.empty() && !fs::exists(directory)) {
                    if (!fs::create_directories(directory)) {
                        throw string("Failed to create directories: ") + directory.c_str();
                    }
                }
            }

            FileStream fileStream(filePath, mode);
            if (!fileStream.is_open()) {
                std::error_code ec;
                ec.assign(errno, std::system_category());
                std::cerr << "Error: " << ec.message() << " (code " << ec.value() << ")\n";
                throw string("Failed to open file: ") + filePath.c_str() + " (" + ec.message() + ", " + std::to_string(ec.value()) + ")";
            }

            return fileStream;

        } catch (const std::exception& e) {
            throw string("Exception occurred: ") + e.what();
        }

        return FileStream();
    }

    void dataExport_fjs( const vector<jobshop::JobshopData>& DIOs, string dir, string fjsFileName ) {

        int idx = 0;
        for ( auto &DIO: DIOs ) {

            auto ofs = openFileWithDirs<ofstream>(dir + "/" + fjsFileName + (fjsFileName=="" ? "" : "_" ) + to_string(idx++, 6) + ".fjs");

            // computing average number of machines used by each operation
            vector<vector<pair<int,int>>> operationMachines(DIO.OMtime.size()); // list of machines eligible for each operation pair<machine, duration>

            for ( int o = 0; o < DIO.OMtime.size(); o++ ) {
                for ( int m = 0; m < DIO.OMtime[o].size(); m++ ) {
                    int dur = DIO.OMtime[o][m];
                    if (dur > 0) {
                        operationMachines[o].push_back(make_pair(m+1, dur));
                    }
                }
            }

            int sum = 0, num = 0;
            for ( auto &J: DIO.Jobs ) {
                for ( auto &o: J.Ops ) {
                    sum += operationMachines[o].size();
                    num += 1;
                }
            }

            float avgNumMO = (float)sum / num;

            ofs << DIO.numJ << " " << DIO.numM << " " << avgNumMO << endl;

            for ( auto &J: DIO.Jobs ) {
                ofs << J.Ops.size();
                for ( auto &o: J.Ops ) {
                    ofs << " " << operationMachines[o].size();
                    for ( auto &MD: operationMachines[o] ) {
                        ofs << " " << MD.first << " " << MD.second;
                    }
                }
                ofs << endl;
            }
            ofs << flush;

        }
    }
}