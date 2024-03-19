// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/18/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include "Station.h"
#include "Utilities.h"
#include <iomanip>

namespace seneca {
    size_t Station::m_widthField = 0;
    int Station::id_generator = 0;

    Station::Station(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;
        try {
            m_itemName = util.extractToken(record, next_pos, more);
            if (more) m_serialNumber = std::stoi(util.extractToken(record, next_pos, more));
            if (more) m_quantity = std::stoul(util.extractToken(record, next_pos, more));
            if (more) m_desc = util.extractToken(record, next_pos, more);

            if (m_itemName.length() > m_widthField) m_widthField = m_itemName.length();

            m_id = ++id_generator;
        }
        catch (const std::string& err) {
            std::cerr << "Error: " << err << std::endl;
        }
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_serialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_quantity;
    }

    void Station::updateQuantity() {
        if (m_quantity > 0) m_quantity--;
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::setw(3) << std::setfill('0') << std::right << m_id << " | ";
        os << std::setw(m_widthField) << std::left << std::setfill(' ') << m_itemName << " | ";
        os << std::setw(6) << std::setfill('0') << std::left << m_serialNumber << " | ";
        if (full) {
            os << std::setw(4) << std::setfill(' ') << m_quantity << " | ";
            os << m_desc << std::endl;
        }
        else {
            os << std::endl;
        }
    }
}
