#pragma once
#include <chrono>
#include <vector>
#include <functional>

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

	class IAnim {
	public:
		IAnim() {};
		virtual ~IAnim() {}

		static void update();

	protected:
		static void registerAnim(IAnim* pAnim);
		static void unregisterAnim(IAnim* pAnim);

		virtual bool updateAnim() = 0;

	private:
		static std::vector<IAnim*> s_anims;
	};

	template<typename Ttype>
	class Anim : public IAnim {
	public:
		struct AnimKeyFrame {
			AnimKeyFrame(
				const Ttype& in_goal, 
				float in_duration, 
				TweenType in_tween = TweenType::NONE, 
				const std::function<void()>& in_callback = nullptr) :
				goal(in_goal), 
				duration(in_duration), 
				tween(in_tween),
				callback(in_callback) {
			}
			Ttype					goal;
			float					duration;
			TweenType				tween;
			std::function<void()>	callback;
		};

		Anim() {}
		Anim(const Ttype& rvalue) :
			m_value(rvalue) {}

		virtual ~Anim() {
			if (m_isPlaying) {
				stop(false);
			}
		}

		const Ttype& get() {
			return m_retValue;
		}

	public:
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
			m_retValue = m_value = startValue;

			// Clear previous keyframes
			m_keyFrames.clear();

			// Convert to internal keyframes
			m_oldTime = m_startTime = std::chrono::steady_clock::now();
			double timeIn = 0.0;
			for (auto& keyFrame : keyFrames) {
				timeIn += static_cast<double>(keyFrame.duration);
				
				auto timeInPrecise = m_startTime + 
					std::chrono::microseconds(static_cast<long long>(timeIn * 1000000.0));
				m_keyFrames.push_back(InternalKeyFrame{ keyFrame.goal, timeInPrecise, keyFrame.tween, keyFrame.callback });
			}

			// Start the animation
			m_isPlaying = true;
			m_loop = loop;

			registerAnim(this);
		}

		void stop(bool goToEnd) {
			if (m_isPlaying) {
				unregisterAnim(this);
				m_isPlaying = false;
				if (goToEnd) {
					if (!m_keyFrames.empty()) {
						m_retValue = m_value = m_keyFrames.back().goal;
					}
				}
				m_keyFrames.clear();
			}
		}

	protected:
		virtual bool updateAnim() {
			m_retValue = updateValue();
			return !m_isPlaying;
		}

	private:
		void stopButDontUnregister(bool goToEnd) {
			if (m_isPlaying) {
				m_isPlaying = false;
				if (goToEnd) {
					if (!m_keyFrames.empty()) {
						m_retValue = m_value = m_keyFrames.back().goal;
					}
				}
				m_keyFrames.clear();
			}
		}

		Ttype updateValue() {
			if (m_isPlaying) {
				auto now = std::chrono::steady_clock::now();
				auto& last = m_keyFrames.back();
				if (now >= last.endAt) {
					if (last.callback) {
						last.callback();
					}
					switch (m_loop) {
					case LoopType::NONE: {
						stopButDontUnregister(true);
						return m_value;
					}
					case LoopType::PINGPONG_ONCE:
						if (m_isPingPonging) {
							stopButDontUnregister(true);
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
						auto callback = m_cachedCallback;
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

							// Invert callbacks
							std::swap(keyFrame.callback, callback);
						}
						m_cachedCallback = callback;

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
					if (m_oldTime <= keyFrame.endAt) {
						if (keyFrame.callback) {
							keyFrame.callback();
						}
					}
					from = keyFrame.goal;
					fromTime = keyFrame.endAt;
				}
				m_oldTime = now;
			}
			return m_value;
		}

	private:
		struct InternalKeyFrame {
			InternalKeyFrame(
				const Ttype& in_goal, 
				const std::chrono::system_clock::time_point& in_endAt,
				TweenType in_tween,
				std::function<void()> in_callback) :
				goal(in_goal),
				endAt(in_endAt),
				tween(in_tween),
				callback(in_callback) {
			}
			Ttype									goal;
			std::chrono::system_clock::time_point	endAt;
			TweenType								tween;
			std::function<void()>					callback;
		};

		bool									m_isPlaying = false;
		Ttype									m_value;
		Ttype									m_retValue;
		std::chrono::system_clock::time_point	m_startTime;
		std::chrono::system_clock::time_point	m_oldTime;
		std::vector<InternalKeyFrame>			m_keyFrames;
		LoopType								m_loop;
		bool									m_isPingPonging = false;
		std::function<void()>					m_cachedCallback = nullptr; // Used for pingpong
	};
}
