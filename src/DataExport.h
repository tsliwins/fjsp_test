//
// Created by tsliwins on 1.09.2025.
//
#pragma once

namespace jobshop {
    void dataExport_fjs( const vector<jobshop::JobshopData>& DIOs, string dir, string fjsFileName );
    string to_string(int n, int width);
}