namespace Mystic
{
	using FiberHandle = std::function<void()>;

	class FiberScript : public Script
	{
	public:
		bool IsInitialized() override;
		void Initialize() override;
		ScriptType GetType() override;
		void Destroy() override;
		void Tick() override;

		void Wait(DWORD ms);

		std::queue<FiberHandle>& getTasks()
		{
			return m_fiberTasks;
		}

		void addTask(FiberHandle handle)
		{
			m_fiberTasks.push(handle);
		}

		std::mutex& get_mutex()
		{
			return m_mutex;
		}

	private:
		bool m_Initialized{};
		std::uint32_t m_wake_at{};
		void* m_script_fiber{};
		void* m_main_fiber{};
		std::mutex m_mutex{};

		std::queue<FiberHandle> m_fiberTasks;
	};

	inline std::shared_ptr<FiberScript> g_FiberScript;
}
