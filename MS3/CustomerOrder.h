// Name: Kiarash Kia
// Seneca Student ID: 108688235
// Seneca email: kkia2@myseneca.ca
// Date of completion: 03/24/2024
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#pragma once
#ifndef SENECA_CUSTOMERORDER_H
#define SENECA_CUSTOMERORDER_H

#include <string>
#include "Station.h"

namespace seneca {
	struct Item {

		std::string m_itemName;
		size_t m_serialNumber{ 0 };
		bool m_isFilled{ false };

		Item(const std::string& src) : m_itemName(src) {};
	};

	class CustomerOrder {
		std::string m_name;
		std::string m_product;
		size_t m_cntItem;
		Item** m_listItem;

		static size_t m_widthField;

	public:
		CustomerOrder() = default;

		CustomerOrder(const std::string& str);
		CustomerOrder(const CustomerOrder& other);
		CustomerOrder(CustomerOrder&& other) noexcept;

		~CustomerOrder();

		CustomerOrder& operator=(CustomerOrder&& other) noexcept;
		CustomerOrder& operator =(const CustomerOrder& other) = delete;

		bool isOrderFilled() const;
		bool isItemFilled(const std::string& itemName) const;
		void fillItem(Station& station, std::ostream& os);
		void display(std::ostream& os) const;
	};
}

#endif // !SENECA_CUSTOMERORDER_H