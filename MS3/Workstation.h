// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/28/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#pragma once
#ifndef SENECA_WORKSTATION_H
#define SENECA_WORKSTATION_H

#include <iostream>
#include <deque>
#include "Station.h"
#include "CustomerOrder.h"

namespace seneca {
    extern std::deque<CustomerOrder> g_pending;
    extern std::deque<CustomerOrder> g_completed;
    extern std::deque<CustomerOrder> g_incomplete;

    class Workstation : public Station {
        std::deque<CustomerOrder> m_orders;
        Workstation* m_pNextStation{ nullptr };

    public:
        Workstation(const std::string& src) : Station(src) {};
        Workstation(const Workstation&) = delete;
        Workstation& operator=(const Workstation&) = delete;
        Workstation(Workstation&&) = delete;
        Workstation& operator=(Workstation&&) = delete;

        void fill(std::ostream& os);
        bool attemptToMoveOrder();
        void setNextStation(Workstation* station = nullptr);
        Workstation* getNextStation() const;
        void display(std::ostream& os) const;
        Workstation& operator+=(CustomerOrder&& newOrder);
    };
}

#endif // SENECA_WORKSTATION_H