#pragma once
#include "sexpr.h"
#include "primitive.h"
#include "transform_utils.h"
#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <cmath>

class Layer {
	public:
		enum TYPE { jumper, mixed, power, signal, user };
	private:
		int m_ORDINAL;
		std::string m_CANONICAL_NAME;
		TYPE m_TYPE;
		std::optional<std::string> m_USER_NAME;

		std::vector<std::unique_ptr<Primitive>> m_primitives;
	public:
		// Default the move operations since "m_primitives" is move only
		Layer() = default;
		Layer(Layer&&) = default;
		Layer& operator=(const Layer&&) = delete;
		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;

		Layer(int ORDINAL, std::string CANONICAL_NAME, TYPE _TYPE, std::string USER_NAME) :
			m_ORDINAL{ORDINAL}, m_CANONICAL_NAME{CANONICAL_NAME}, m_TYPE{_TYPE}, m_USER_NAME{USER_NAME} {}
		Layer(const SEXPR::SEXPR_LIST *layer);

		~Layer() {}

		int getOrdinal() const { return m_ORDINAL; }
		std::string getCanonicalName() const { return m_CANONICAL_NAME; }
		TYPE getType() const { return m_TYPE; }
		std::optional<std::string> getUserName() const { return m_USER_NAME; }

		void drawAll(SDL_Renderer *renderer, Transform& t) const;
		void addPrimitive(std::unique_ptr<Primitive> primitive);
		void listPrimitives(void) const;

		BoundingBox getBoundingBox() const;
};
