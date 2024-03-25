#include <stdexcept>

#include "CustomerOrder.h"
#include "Utilities.h"
#include <iomanip>

namespace seneca {

    size_t CustomerOrder::m_widthField = 1; // initialize static member variable

    CustomerOrder::CustomerOrder(const std::string& record) : m_name(""), m_product(""), m_cntItem(0), m_lstItem(nullptr) {
        Utilities util;
        size_t next_pos = 0;
        size_t identifierPos = 0;
        bool more = true;

        m_name = util.extractToken(record, next_pos, more);
        m_product = util.extractToken(record, next_pos, more);
        identifierPos = next_pos;

        while (more) {
            util.extractToken(record, next_pos, more);
            m_cntItem++;
        }

        if (m_widthField < util.getFieldWidth())
            m_widthField = util.getFieldWidth();


        next_pos = identifierPos;
        more = true;

        m_lstItem = new Item * [m_cntItem];

        for (size_t i = 0; i < m_cntItem; ++i) {
            std::string itemName = util.extractToken(record, next_pos, more);
            m_lstItem[i] = new Item(itemName);
        }
    }

    CustomerOrder::CustomerOrder(const CustomerOrder& other) {
        throw std::runtime_error("Ilegal Operatiorn: Copy Constructor");
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept
        : m_name(std::move(other.m_name)), m_product(std::move(other.m_product)),
        m_cntItem(other.m_cntItem), m_lstItem(other.m_lstItem) {

        other.m_cntItem = 0;
        other.m_lstItem = nullptr;
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);

            for (size_t i = 0; i < m_cntItem; ++i)
                delete m_lstItem[i];
            delete[] m_lstItem;
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;

            other.m_cntItem = 0;
            other.m_lstItem = nullptr;
        }
        return *this;
    }



    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i)
            delete m_lstItem[i];
        delete[] m_lstItem;
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (!m_lstItem[i]->m_isFilled)
                return false;
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled)
                return false;
        }
        return true;
    }

    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (station.getItemName() == m_lstItem[i]->m_itemName && !m_lstItem[i]->m_isFilled) {
                if (station.getQuantity() > 0) {
                    m_lstItem[i]->m_isFilled = true;
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    station.updateQuantity();
                    os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    break;
                }
                else {
                    os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                }
            }
        }
    }

    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;
        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber << "] ";
            os << std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName << " - ";
            os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }

}