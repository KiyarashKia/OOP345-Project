// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/18/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include <string>
#include "Utilities.h"

namespace seneca {
	
	char Utilities::m_delimiter = ' ';

	void Utilities::setFieldWidth(size_t newWidth) {
		m_widthField = newWidth;
	}

	size_t Utilities::getFieldWidth() const {
		return m_widthField;
	}

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        auto trimWS = [](std::string token) {
            token.erase(0, token.find_first_not_of(' '));
            size_t lastChar = token.find_last_not_of(' ');
            if (lastChar != std::string::npos) {
                token.erase(lastChar + 1);
            }
            return token;
            };

        if (next_pos >= str.size()) {
            more = false;
            return "";
        }

        size_t pos = str.find(m_delimiter, next_pos);


        if (pos == next_pos) {
            next_pos = pos + 1;
            more = false;
            throw std::string("No token.");
        }

        size_t end_pos = (pos != std::string::npos) ? pos : str.size();

        std::string token = str.substr(next_pos, end_pos - next_pos);
        token = trimWS(token);
        m_widthField = std::max(m_widthField, token.length());

        next_pos = (pos != std::string::npos) ? end_pos + 1 : std::string::npos;

        more = (next_pos != std::string::npos);

        return token;
    }



	void Utilities::setDelimiter(char newDelimiter) {
		m_delimiter = newDelimiter;
	}
	char Utilities::getDelimiter() {
		return m_delimiter;
	}

}