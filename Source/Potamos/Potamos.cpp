/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

void FPotamos::StartupModule()
{
	UE_LOG(LogPotamos, Log, TEXT("Potamos module starting up"));
}

void FPotamos::ShutdownModule()
{
	UE_LOG(LogPotamos, Log, TEXT("Potamos module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FPotamos, Potamos, "Potamos");

