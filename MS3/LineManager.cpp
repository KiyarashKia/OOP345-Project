#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

namespace seneca {
    LineManager::LineManager(const std::string& file, std::vector<Workstation*>& stations) {
        std::ifstream in(file);
        if (!in) {
            throw std::runtime_error("Cannot open file: " + file);
        }

        std::string line;
        Utilities util;
        while (std::getline(in, line)) {
            size_t nextPos = 0;
            bool more = true;
            auto stationName = util.extractToken(line, nextPos, more);
            auto nextStationName = more ? util.extractToken(line, nextPos, more) : "";

            Workstation* currentStation = nullptr;
            Workstation* nextStation = nullptr;

            for (auto& station : stations) {
                if (station->getItemName() == stationName) {
                    currentStation = station;
                }
                if (station->getItemName() == nextStationName) {
                    nextStation = station;
                }
            }

            if (currentStation) {
                currentStation->setNextStation(nextStation);
                m_activeLine.push_back(currentStation);

                if (!m_firstStation || stationName == "FirstStationName") { // Assuming you have a way to identify the first station
                    m_firstStation = currentStation;
                }
            }
        }

        in.close();
        reorderStations();
        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations() {
        if (!m_activeLine.empty()) {
            std::vector<Workstation*> sorted;
            auto current = m_firstStation;
            while (current != nullptr) {
                sorted.push_back(current);
                current = current->getNextStation();
            }
            m_activeLine = sorted;
        }
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0; // Static variable to keep track of iteration number
        ++iteration; // Increment iteration count each time run() is called

        // Print the current iteration number
        os << "Line Manager Iteration: " << iteration << std::endl;

        // Move the order from the pending queue to the first station, if available
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front(); // Remove the order from the pending queue
        }

        // Execute fill operation for each station on the line
        for (auto& station : m_activeLine) {
            station->fill(os);
        }

        // Attempt to move CustomerOrder down the line for each station
        bool moved = false; // Track if any orders were moved in this iteration
        for (auto& station : m_activeLine) {
            moved |= station->attemptToMoveOrder(); // Use bitwise OR to set moved to true if any station successfully moves an order
        }

        // If no orders were moved and the pending queue is empty, we might be in an infinite loop
        if (!moved && g_pending.empty()) {
            os << "Warning: No orders were moved, and the pending queue is empty. Possible infinite loop detected." << std::endl;
        }

        // Check if all customer orders have been processed
        size_t totalProcessed = g_completed.size() + g_incomplete.size();
        return totalProcessed == m_cntCustomerOrder;
    }

    void LineManager::display(std::ostream& os) const {
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* station) {
            station->display(os);
            });
    }
        
    
}
