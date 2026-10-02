#ifndef BOARD_SKIP_POLICY_H
#define BOARD_SKIP_POLICY_H

// 每個軌道各自持有，僅由流程擁有者操作。
class CBoardSkipPolicy
{
public:
    /// <summary>初始化跳板狀態</summary>
    CBoardSkipPolicy()
        : m_Enabled(false), m_InspectCount(1), m_SkipCount(2),
          m_SkipPhase(false), m_CompletedCount(0),
          m_BoardActive(false), m_CurrentBoardSkipped(false)
    {
    }

    /// <summary>設定跳板規則並重設計數</summary>
    bool Configure(bool enabled, int inspectCount, int skipCount)
    {
        if (m_BoardActive || inspectCount < 1 || skipCount < 1)
        {
            return false;
        }
        m_Enabled = enabled;
        m_InspectCount = inspectCount;
        m_SkipCount = skipCount;
        Reset();
        return true;
    }

    /// <summary>固定本張板是否跳板</summary>
    bool BeginBoard()
    {
        if (!m_BoardActive)
        {
            m_CurrentBoardSkipped = m_Enabled && m_SkipPhase;
            m_BoardActive = true;
        }
        return m_CurrentBoardSkipped;
    }

    /// <summary>確認本張板是否跳板</summary>
    bool IsCurrentBoardSkipped() const
    {
        return m_BoardActive && m_CurrentBoardSkipped;
    }

    /// <summary>完成本張板並推進跳板計數</summary>
    bool CompleteBoard()
    {
        if (!m_BoardActive)
        {
            return false;
        }
        if (m_Enabled)
        {
            ++m_CompletedCount;
            const int countLimit = m_SkipPhase ? m_SkipCount : m_InspectCount;
            if (m_CompletedCount >= countLimit)
            {
                m_CompletedCount = 0;
                m_SkipPhase = !m_SkipPhase;
            }
        }
        m_BoardActive = false;
        m_CurrentBoardSkipped = false;
        return true;
    }

    /// <summary>重設跳板計數與本張板狀態</summary>
    void Reset()
    {
        m_SkipPhase = false;
        m_CompletedCount = 0;
        m_BoardActive = false;
        m_CurrentBoardSkipped = false;
    }

private:
    bool m_Enabled;
    int m_InspectCount;
    int m_SkipCount;
    bool m_SkipPhase;
    int m_CompletedCount;
    bool m_BoardActive;
    bool m_CurrentBoardSkipped;
};

#endif
