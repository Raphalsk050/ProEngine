#pragma once
#include "PEPCH.h"
#include "Core/Log/PELog.h"
#include "Core/Assert/Assert.h"
#include "Core/Event/Event.h"
#include "Core/Application/Application.h"
#include "Core/Application/EntryPoint.h"
#include "Core/Event/KeyEvent.h"

namespace ProEngine
{
    class ProEngine
    {
    public:
        ProEngine()
        {
            PELog::Init();
        }
    };
}
