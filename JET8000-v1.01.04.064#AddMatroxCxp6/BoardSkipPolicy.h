#ifndef BOARD_SKIP_POLICY_H
#define BOARD_SKIP_POLICY_H

// –瓂笵璸计 PCB 碞狐糤
class CBoardSkipPolicy
{
public:
    /// <summary>﹍て铬狾砞﹚籔璸计</summary>
    CBoardSkipPolicy()
        : m_Enabled(false), m_InspectCount(1), m_SkipCount(2), m_CycleCount(3), m_BoardCount(0)
    {
    }

    /// <summary>砞﹚铬狾砏玥砞璸计</summary>
    bool Configure(bool enabled, int inspectCount, int skipCount)
    {
        if (inspectCount < 1 || skipCount < 1)
        {
            return false;
        }
        m_Enabled = enabled;
        m_InspectCount = inspectCount;
        m_SkipCount = skipCount;
        m_CycleCount = static_cast<unsigned int>(m_InspectCount)
            + static_cast<unsigned int>(m_SkipCount);
        Reset();
        return true;
    }

    /// <summary>PCB 碞狐璸计ЧΘ近眖穝璸计</summary>
    void BoardCount()
    {
        if (m_BoardCount >= m_CycleCount)
        {
            m_BoardCount = 0;
        }
        ++m_BoardCount;
    }

    /// <summary>絋粄セΩ琌铬狾ぃэ跑璸计</summary>
    bool IsCurrentBoardSkipped() const
    {
        return m_Enabled && m_BoardCount > static_cast<unsigned int>(m_InspectCount);
    }

    /// <summary>砞璸计Ω眖浪代秨﹍</summary>
    void Reset()
    {
        m_BoardCount = 0;
    }

private:
    bool m_Enabled;
    int m_InspectCount;
    int m_SkipCount;
    unsigned int m_CycleCount;
    unsigned int m_BoardCount;
};

#endif
