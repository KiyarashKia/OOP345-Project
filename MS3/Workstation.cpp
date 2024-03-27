#include "Workstation.h"
#include <deque>
namespace seneca {

    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
        bool move_result = false;

        // Check if there are orders in the queue
        if (!m_orders.empty()) {
            // Check if the front order's item is filled or if the station's quantity is 0
            if (m_orders.front().isItemFilled(getItemName())) {
                // If there's a next station, move the order to the next station
                if (m_pNextStation) {
                    *m_pNextStation += std::move(m_orders.front());
                }
                // If there's no next station and the order is not fully filled, move to incomplete
                else if (!m_orders.front().isOrderFilled()) {
                    g_incomplete.push_back(std::move(m_orders.front()));
                }
                // If the order is fully filled, move to completed
                else {
                    g_completed.push_back(std::move(m_orders.front()));
                }

                move_result = true;
                m_orders.pop_front();
            }
            // If the station's item quantity is 0, move the order to the next station or incomplete
            else if (getQuantity() <= 0) {
                if (m_pNextStation) {
                    *m_pNextStation += std::move(m_orders.front());
                }
                else {
                    g_incomplete.push_back(std::move(m_orders.front()));
                }

                move_result = true;
                m_orders.pop_front();
            }
        }

        return move_result;
    }

    void Workstation::setNextStation(Workstation* station) {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }

    void Workstation::display(std::ostream& os) const {
        if (m_pNextStation)
            os << getItemName() << " --> " << m_pNextStation->getItemName() << std::endl;
        else
            os << getItemName() << " --> " << "End of Line" << std::endl;
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }

}
