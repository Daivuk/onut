#include "Settings.h"

namespace onut {
	void Settings::load(const std::string& filename) {
	}

	void Settings::save(const std::string& filename) {
		m_isDirty = false;
	}

	void Settings::setResolution(const POINT& resolution) {
		m_resolution = resolution;
		m_isDirty = true;
	}

	void Settings::setGameName(const std::string& gameName) {
		m_gameName = gameName;
		m_isDirty = true;
	}
}
