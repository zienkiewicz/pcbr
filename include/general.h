#pragma once
#include "sexpr.h"
#include <optional>

class General {
	protected:
		std::optional<int> m_links;
		std::optional<int> m_noConnects;
		std::optional<double> m_thickness;
		std::optional<int> m_drawings;
		std::optional<int> m_tracks;
		std::optional<int> m_zones;
		std::optional<int> m_footprints; // modules
		std::optional<int> m_nets;
	public:
		General(const SEXPR::SEXPR_LIST* general);
		~General() = default;
		const std::optional<int>& links() const { return m_links; }
		const std::optional<int>& noConnects() const { return m_noConnects; }
		const std::optional<double>& thickness() const { return m_thickness; }
		const std::optional<int>& drawings() const { return m_drawings; }
		const std::optional<int>& tracks() const { return m_tracks; }
		const std::optional<int>& zones() const { return m_zones; }
		const std::optional<int>& footprints() const { return m_footprints; }
		const std::optional<int>& nets() const { return m_nets; }
};
