#pragma once
#include "sexpr_parser.h"
#include <string>

class Layer {
	public:
		enum TYPE { jumper, mixed, power, signal, user };
	protected:
		int m_ORDINAL;
		std::string m_CANONICAL_NAME;
		TYPE m_TYPE;
		std::string m_USER_NAME;
	
	public:
		Layer(int ORDINAL, std::string CANONICAL_NAME, TYPE _TYPE, std::string USER_NAME) :
			m_ORDINAL{ORDINAL}, m_CANONICAL_NAME{CANONICAL_NAME}, m_TYPE{_TYPE}, m_USER_NAME{USER_NAME} {}
		Layer(const SEXPR::SEXPR_LIST *layer);

		~Layer() {}

		int getOrdinal() const { return m_ORDINAL; }
		std::string getCanonicalName() const { return m_CANONICAL_NAME; }
		TYPE getType() const { return m_TYPE; }
		std::string getUserName() const { return m_USER_NAME; }
};
