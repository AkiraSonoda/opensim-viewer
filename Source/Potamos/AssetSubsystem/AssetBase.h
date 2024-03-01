/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
DECLARE_DELEGATE(ProcessDelegate)


/**
 * 
 */
class POTAMOS_API AssetBase {
public:
	enum AssetState {
		Waiting = 0,
		Processing = 1,
		Failed = 2,
		Done = 3,
		Retrying = 4
	};

	AssetBase();
	virtual ~AssetBase();
	virtual bool GetFromCache(const TCHAR* file);

	void inline SetFailed() {
		if (stageData.IsValid()) stageData->Empty();
		state = AssetState::Failed;
	}

	ProcessDelegate decode; // On dedicated thread
	ProcessDelegate preProcess; // On game thread!
	ProcessDelegate mainProcess; // On dedicated thread
	ProcessDelegate postProcess; // On game thread

	FGuid id;
	AssetState state;

protected:
	void Decode();

	TSharedPtr<TArray<uint8_t>, ESPMode::ThreadSafe> stageData;

	friend class AssetCache;
};

typedef TSharedRef<AssetBase, ESPMode::ThreadSafe> TSharedAssetRef;
