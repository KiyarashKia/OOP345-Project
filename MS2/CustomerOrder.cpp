// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/24/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include "CustomerOrder.h"
#include "Utilities.h"
#include <utility>
#include <iostream>
#include <iomanip>

namespace seneca {

    size_t CustomerOrder::m_widthField = 1;

 /*   CustomerOrder::CustomerOrder() : m_name(""), m_product(""), m_cntItem(0), m_listItem(nullptr) {}*/ // As constructor is default

    CustomerOrder::CustomerOrder(const std::string& record) : m_name(""), m_product(""), m_cntItem(0), m_listItem(nullptr) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;
        m_name = util.extractToken(record, next_pos, more);
        if (more) m_product = util.extractToken(record, next_pos, more);
        std::vector<Item*> tempItems;
        while (more) {
            util.extractToken(record, next_pos, more);
            m_cntItem++;
        }
        m_cntItem = tempItems.size();
        m_listItem = new Item * [m_cntItem];
        for (size_t i = 0; i < m_cntItem; ++i) {
            m_listItem[i] = tempItems[i];
        }
        m_widthField = std::max(m_widthField, util.getFieldWidth());
    }



    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept
        : m_name(std::move(other.m_name)), m_product(std::move(other.m_product)),
        m_cntItem(other.m_cntItem), m_listItem(other.m_listItem) {

        other.m_cntItem = 0;
        other.m_listItem = nullptr;
    }


    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);

            for (size_t i = 0; i < m_cntItem; ++i)
            delete m_listItem[i];
            delete[] m_listItem;
            
            m_cntItem = other.m_cntItem;
            m_listItem = other.m_listItem;

            other.m_cntItem = 0;
            other.m_listItem = nullptr;
        }
        return *this;
    }

    CustomerOrder::CustomerOrder(const CustomerOrder& other) {
        throw std::runtime_error("Restricted Operation: Copy Constructor");
    }

    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i) {
            delete m_listItem[i];
        }
        delete[] m_listItem;
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (!m_listItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_listItem[i]->m_itemName == itemName && !m_listItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (station.getItemName() == m_listItem[i]->m_itemName && !m_listItem[i]->m_isFilled) {
                if (station.getQuantity() > 0) {
                    m_listItem[i]->m_isFilled = true;
                    m_listItem[i]->m_serialNumber = station.getNextSerialNumber();
                    station.updateQuantity();
                    os << "    Filled " << m_name << ", " << m_product << " [" << m_listItem[i]->m_itemName << "]" << std::endl;
                    break;
                }
                else {
                    os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_listItem[i]->m_itemName << "]" << std::endl;
                }
            }
        }
    }

    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;
        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::setw(6) << std::setfill('0') << m_listItem[i]->m_serialNumber << "] ";
            os << std::setw(m_widthField) << std::setfill(' ') << std::left << m_listItem[i]->m_itemName << " - ";
            os << (m_listItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }

} // namespace seneca