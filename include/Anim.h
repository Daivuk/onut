#pragma once
#include <chrono>
#include <vector>

namespace onut {
	enum class TweenType {
		NONE,
		LINEAR,
		EASE_IN,
		EASE_OUT,
		EASE_BOTH,
		BOUNCE_IN,
		BOUNCE_OUT,
		SPRING_IN,
		SPRING_OUT
	};

	enum class LoopType {
		NONE,
		LOOP,
		PINGPONG_ONCE,
		PINGPONG_LOOP
	};

	float applyTween(const float t, TweenType tween);
	TweenType invertTween(TweenType tween);

	template<typename Ttype>
	class Anim {
	public:
		struct AnimKeyFrame {
			AnimKeyFrame(const Ttype& in_goal, float in_duration, TweenType in_tween = TweenType::NONE) :
				goal(in_goal), 
				duration(in_duration), 
				tween(in_tween) {
			}
			Ttype		goal;
			float		duration;
			TweenType	tween = TweenType::LINEAR;
		};

		Anim() {}
		Anim(const Ttype& rvalue) :
			m_value(rvalue) {}

		const Ttype& get() {
			if (m_isPlaying) {
				auto now = std::chrono::steady_clock::now();
				auto& last = m_keyFrames.back();
				if (now >= last.endAt) {
					switch (m_loop) {
					case LoopType::NONE: {
						stop(true);
						return m_value;
					}
					case LoopType::PINGPONG_ONCE:
						if (m_isPingPonging) {
							stop(true);
							return m_value;
						}
					case LoopType::PINGPONG_LOOP: {
						m_isPingPonging = true;
						auto newStart = last.endAt;

						// Reverse the time and their position
						auto& fromTime = m_startTime;
						auto lastTime = last.endAt;
						auto from = m_value;
						auto oldGoal = last.goal;
						for (auto& keyFrame : m_keyFrames) {
							// Reverse time
							auto oldEnd = keyFrame.endAt;
							auto duration = (oldEnd - fromTime);
							keyFrame.endAt = newStart + (lastTime - oldEnd) + duration;
							fromTime = oldEnd;

							// Reverse tween
							keyFrame.tween = invertTween(keyFrame.tween);

							// Reverse goals
							std::swap(keyFrame.goal, from);
						}

						// Reverse the keyframes
						std::reverse(m_keyFrames.begin(), m_keyFrames.end());

						m_value = oldGoal;
						m_startTime = newStart;
						break;
					}
					case LoopType::LOOP: {
						// Shift all time to continue from the new start
						auto newStart = last.endAt;
						for (auto& keyFrame : m_keyFrames) {
							keyFrame.endAt += newStart - m_startTime;
						}
						m_startTime = newStart;
					}
					}
				}
				auto from = m_value;
				auto fromTime = m_startTime;
				for (auto& keyFrame : m_keyFrames) {
					if (now < keyFrame.endAt) {
						auto duration = std::chrono::duration<double>(keyFrame.endAt - fromTime);
						auto timeIn = std::chrono::duration<double>(now - fromTime);
						auto percent = static_cast<float>(timeIn.count() / duration.count());
						percent = applyTween(percent, keyFrame.tween);

						// Lerp
						auto ret = from + (keyFrame.goal - from) * percent;

						return std::move(ret);
					}
					from = keyFrame.goal;
					fromTime = keyFrame.endAt;
				}
			}
			return m_value;
		}

		const Ttype& operator=(const Ttype& rvalue) const {
			Ttype = rvalue;
			return m_value;
		}
		Ttype operator=(const Ttype& rvalue) {
			Ttype = rvalue;
			return m_value;
		}

		void start(const Ttype& from, const Ttype& goal, float duration, TweenType tween = TweenType::LINEAR, LoopType loop = LoopType::NONE) {
			start(from, m_value, { { goal, duration, tween } }, loop);
		}

		void start(const Ttype& goal, float duration, TweenType tween = TweenType::LINEAR, LoopType loop = LoopType::NONE) {
			start(m_value, { { goal, duration, tween } }, loop);
		}

		void start(const AnimKeyFrame& keyFrames, LoopType loop = LoopType::NONE) {
			start(m_value, { keyFrames }, loop);
		}

		void start(const std::vector<AnimKeyFrame>& keyFrames, LoopType loop = LoopType::NONE) {
			start(m_value, keyFrames, loop);
		}

		void start(const Ttype& startValue, const std::vector<AnimKeyFrame>& keyFrames, LoopType loop = LoopType::NONE) {
			assert(!keyFrames.empty());

			// Update value to the start point if specified
			m_value = startValue;

			// Clear previous keyframes
			m_keyFrames.clear();

			// Convert to internal keyframes
			m_startTime = std::chrono::steady_clock::now();
			double timeIn = 0.0;
			for (auto& keyFrame : keyFrames) {
				timeIn += static_cast<double>(keyFrame.duration);
				
				auto timeInPrecise = m_startTime + 
					std::chrono::microseconds(static_cast<long long>(timeIn * 1000000.0));
				m_keyFrames.push_back(InternalKeyFrame{ keyFrame.goal, timeInPrecise, keyFrame.tween });
			}

			// Start the animation
			m_isPlaying = true;
			m_loop = loop;
		}

		void stop(bool goToEnd) {
			m_isPlaying = false;
			if (goToEnd) {
				if (!m_keyFrames.empty()) {
					m_value = m_keyFrames.back().goal;
				}
			}
			m_keyFrames.clear();
		}

	private:
		struct InternalKeyFrame {
			InternalKeyFrame(const Ttype& in_goal, const std::chrono::system_clock::time_point& in_endAt, TweenType in_tween) :
				goal(in_goal),
				endAt(in_endAt),
				tween(in_tween) {
			}
			Ttype									goal;
			std::chrono::system_clock::time_point	endAt;
			TweenType								tween;
		};

		bool									m_isPlaying = false;
		Ttype									m_value;
		std::chrono::system_clock::time_point	m_startTime;
		std::vector<InternalKeyFrame>			m_keyFrames;
		LoopType								m_loop;
		bool									m_isPingPonging = false;
	};
}
