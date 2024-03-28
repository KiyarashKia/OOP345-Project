// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/28/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#pragma once
#ifndef SENECA_LINEMANAGER_H
#define SENECA_LINEMANAGER_H

#include <vector>
#include <string>
#include "Workstation.h"

namespace seneca {
    class LineManager {
        std::vector<Workstation*> m_activeLine;
        size_t m_cntCustomerOrder{};
        Workstation* m_firstStation{ nullptr };

    public:
        LineManager(const std::string& file, std::vector<Workstation*>& stations);
        void reorderStations();
        bool run(std::ostream& os);
        void display(std::ostream& os) const;
    };
}

#endif // SENECA_LINEMANAGER_H
