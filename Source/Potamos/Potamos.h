/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogViewer, All, All);

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <thread>
#define usleep(x) std::this_thread::sleep_for(std::chrono::microseconds((unsigned long long )x));
#include "HideWindowsPlatformTypes.h"
#endif

class FPotamos : public IModuleInterface {
public:
	static inline FPotamos& Get() {
		return FModuleManager::LoadModuleChecked<FPotamos>("Potamos");
	}

	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded("Potamos");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

