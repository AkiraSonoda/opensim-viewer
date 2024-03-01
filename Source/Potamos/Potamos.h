/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogViewer, All, All);

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <thread>
#define usleep(x) std::this_thread::sleep_for(std::chrono::microseconds((unsigned long long )x));
#include "HideWindowsPlatformTypes.h"
#endif
