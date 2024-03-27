#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

namespace seneca {
    LineManager::LineManager(const std::string& file, std::vector<Workstation*>& stations) {
        Utilities util;
        std::ifstream in(file);
        if (!in.is_open()) throw std::runtime_error("Cannot open file: " + file);

        std::string eachLine, cur_Wstation, next_Wstation;
        size_t pos = 0;
        bool more = false;
        std::vector<std::pair<Workstation*, Workstation*>> stationLinks;


        while (std::getline(in, eachLine)) {
            if (!eachLine.empty()) {
                cur_Wstation = util.extractToken(eachLine, pos, more);
                next_Wstation = more ? util.extractToken(eachLine, pos, more) : "";

                Workstation* first_src = *std::find_if(stations.begin(), stations.end(),
                    [&cur_Wstation](Workstation* ws) { return ws->getItemName() == cur_Wstation; });
                Workstation* sec_src = nullptr;
                if (!next_Wstation.empty()) {
                    sec_src = *std::find_if(stations.begin(), stations.end(),
                        [&next_Wstation](Workstation* ws) { return ws->getItemName() == next_Wstation; });
                }


                stationLinks.emplace_back(first_src, sec_src);
            }
            pos = 0;
            more = false;
        }

 
        for (auto& link : stationLinks) {
            if (link.second) {
                link.first->setNextStation(link.second);
            }
            else {
                link.first->setNextStation(nullptr); 
            }
        }


        for (auto& station : stations) {
            bool isStartingPoint = true;
            for (auto& link : stationLinks) {
                if (link.second == station) {
                    isStartingPoint = false;
                    break;
                }
            }
            if (isStartingPoint) {
                m_firstStation = station;
                break;
            }
        }

        m_cntCustomerOrder = g_pending.size();
    }



    void LineManager::reorderStations() {
        std::vector<Workstation*> sorted;
        auto current = m_firstStation;
        while (current != nullptr) {
            sorted.push_back(current);
            current = current->getNextStation();
        }
        m_activeLine = sorted;
    }


    bool LineManager::run(std::ostream& os) {
        static size_t count = 0u;
        os << "Line Manager Iteration: " << ++count << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for (auto& station : m_activeLine) station->fill(os);
        for (auto& station : m_activeLine) station->attemptToMoveOrder();
        return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;
    }

    void LineManager::display(std::ostream& os) const {
        for (auto& station : m_activeLine) {
            station->display(os);
        }
    
    }
        
    
}
