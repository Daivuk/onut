#pragma once
#include <chrono>

namespace onut {
#define FRAME_PER_SECOND 120
	class TimeInfo {
	public:
		int update();

		/**
			@return the time elapsed between this frame and the last one, in seconds.
			Perfect for performing animations
		*/
		float getDeltaTime() const;

		/**
			@return the total time elapsed since the start of the game, in seconds.
		*/
		float getTotalElapsed() const;

	private:
		double									m_deltaTime = 1.f / FRAME_PER_SECOND;
		double									m_totalElapsed = 0.f;
		std::chrono::steady_clock::time_point	m_startTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point	m_lastUpdateTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration		m_currentFrameProgress;
		std::chrono::steady_clock::duration		m_timePerFrame = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(1)) / FRAME_PER_SECOND;
	};
}
