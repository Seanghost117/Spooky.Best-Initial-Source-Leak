#pragma once
#include "Script.hpp"
#include "Natives.hpp"

namespace Mystic
{
	class AbstractCallback
	{
	public:
		virtual ~AbstractCallback() noexcept = default;
		AbstractCallback(AbstractCallback const&) = delete;
		AbstractCallback& operator=(AbstractCallback const&) = delete;
		AbstractCallback(AbstractCallback&&) = delete;
		AbstractCallback& operator=(AbstractCallback&&) = delete;

		virtual bool IsDone() = 0;
		virtual void OnSuccess() = 0;
		virtual void OnFailure() = 0;
	protected:
		AbstractCallback() = default;
	};

	class ModelCallback : public AbstractCallback
	{
	public:
		explicit ModelCallback(std::uint32_t model, std::function<void()> action):
			m_Model(model),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return STREAMING::HAS_MODEL_LOADED(m_Model);
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			STREAMING::REQUEST_MODEL(m_Model);
		}
	private:
		std::uint32_t m_Model;
		std::function<void()> m_Action;
	};

	class AssetCallback : public AbstractCallback
	{
	public:
		explicit AssetCallback(std::uint32_t asset, std::function<void()> action) :
			m_Asset(asset),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return WEAPON::HAS_WEAPON_ASSET_LOADED(m_Asset);
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			WEAPON::REQUEST_WEAPON_ASSET(m_Asset, 31, false);
		}
	private:
		std::uint32_t m_Asset;
		std::function<void()> m_Action;
	};

	class PTFXAssetCallback : public AbstractCallback
	{
	public:
		explicit PTFXAssetCallback(const char* asset, std::function<void()> action) :
			m_Asset(asset),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(m_Asset);
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET(m_Asset);
		}
	private:
		const char* m_Asset;
		std::function<void()> m_Action;
	};

	class NetworkControlCallback : public AbstractCallback
	{
	public:
		explicit NetworkControlCallback(std::int32_t entity, std::function<void()> action):
			m_Entity(entity),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(m_Entity);
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(m_Entity);
		}
	private:
		std::int32_t m_Entity;
		std::function<void()> m_Action;
	};

	class DelayCallback : public AbstractCallback
	{
	public:
		explicit DelayCallback(std::uint32_t delay, std::function<void()> action) :
			m_StartTime(GetTickCount()),
			m_Delay(delay),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return (GetTickCount() - m_StartTime) >= m_Delay;
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
		}
	private:
		std::uint32_t m_StartTime;
		std::uint32_t m_Delay;
		std::function<void()> m_Action;
	};

	class ModelCollisionCallback : public AbstractCallback
	{
	public:
		explicit ModelCollisionCallback(Hash model, std::function<void()> action) :
			m_Model(model),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			return STREAMING::HAS_COLLISION_FOR_MODEL_LOADED(m_Model);
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			STREAMING::REQUEST_COLLISION_FOR_MODEL(m_Model);
		}
	private:
		Hash m_Model;
		std::function<void()> m_Action;
	};

	class FadeCallback : public AbstractCallback
	{
	public:
		explicit FadeCallback(std::uint32_t fadeInTime, std::uint32_t fadeOutTime, std::function<void()> action) :
			m_FadeIn(fadeInTime),
			m_FadeOut(fadeOutTime),
			m_Action(std::move(action))
		{
		}

		bool IsDone() override
		{
			auto ret = CAM::IS_SCREEN_FADED_OUT();
			if (ret)
			{
				CAM::DO_SCREEN_FADE_IN(m_FadeIn);
			}
			return ret;
		}

		void OnSuccess() override
		{
			if (m_Action)
				std::invoke(m_Action);
		}

		void OnFailure() override
		{
			if (!CAM::IS_SCREEN_FADED_OUT() || !CAM::IS_SCREEN_FADING_OUT())
			{
				CAM::DO_SCREEN_FADE_OUT(m_FadeOut);
			}
		}
	private:
		std::uint32_t m_FadeIn;
		std::uint32_t m_FadeOut;
		std::function<void()> m_Action;
	};

	class CallbackScript : public Script
	{
	public:
		explicit CallbackScript() = default;
		~CallbackScript() noexcept = default;

		bool IsInitialized() override;
		ScriptType GetType() override;
		void Initialize() override;
		void Destroy() override;
		void Tick() override;

		template <typename CallbackType, typename ...TArgs>
		void AddCallback(TArgs&&... args)
		{
			m_Callbacks.push_back(std::make_unique<CallbackType>(std::forward<TArgs>(args)...));
		}
	private:
		bool m_Initialized{};
		std::vector<std::unique_ptr<AbstractCallback>> m_Callbacks;
	};

	inline std::shared_ptr<CallbackScript> g_CallbackScript;
}
