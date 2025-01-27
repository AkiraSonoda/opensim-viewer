/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "PotamosGameMode.h"

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include <io.h>
#include "HideWindowsPlatformTypes.h"
#else
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "MeshActor.h"
#include "AssetSubsystem/AssetCache.h"
#include "AssetSubsystem/AssetDecode.h"
#include "AssetSubsystem/PrimAsset.h"
#include "AssetSubsystem/TextureAsset.h"
#include "PotCharacter.h"
#include "DiskSubsystem/IPlatformFileWhio.h"

//#define ONE_OBJECT_TEST

class ObjectCreator : public FRunnable {
public:
	ObjectCreator(APotamosGameMode* mode);
	~ObjectCreator();

	virtual bool Init() override;
	uint32_t Run();
	virtual void Stop() override;

	void TickPool();

private:
	FRunnableThread* thread;
	bool runThis = true;
	APotamosGameMode* mode;

	FCriticalSection poolLock;
	TArray<AMeshActor*> pool;

	FCriticalSection readyLock;
	TArray<AMeshActor*> ready;

	void ObjectReady(AMeshActor* act);
};

APotamosGameMode::APotamosGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	DefaultPawnClass = APotCharacter::StaticClass();
}

void APotamosGameMode::HandleMatchHasStarted() {
	Super::HandleMatchHasStarted();
	//delete &AssetCache::Get();
	//delete &TextureCache::Get();

	IPlatformFile& inner = FPlatformFileManager::Get().GetPlatformFile();

	FWhioPlatformFile* whio = new FWhioPlatformFile();
	whio->Initialize(&inner, TEXT(""));
	FPlatformFileManager::Get().SetPlatformFile(*whio);

	if (creator)
		delete creator;
	creator = new ObjectCreator(this);

#ifdef ONE_OBJECT_TEST
    FGuid id;
    //FGuid::Parse(TEXT("77540e8e-5064-4cf9-aa77-ad6617d73508"), id);
    //FGuid::Parse(TEXT("8e3377b1-ccc7-4f7b-981d-f30ccae8121d"), id);
    FGuid::Parse(TEXT("664d9466-e74c-477e-bb4f-796845ce894e"), id);
    
    AssetFetchedDelegate d;
    d.BindUObject(this, &APotamosGameMode::CreateNewActor);
    AssetCache::Get().Fetch<PrimAsset>(id, d);
#endif
}

AMeshActor* APotamosGameMode::CreateNewActor(rapidxml::xml_node<>* data) {
	ObjectReadyDelegate d;
	d.BindUObject(this, &APotamosGameMode::HandleObjectReady);

	return CreateNewActor(data, d);
}

void APotamosGameMode::HandleObjectReady(AMeshActor* act) {
	FVector pos(13000.0, -12800, 4000.0);

	//FVector pos(200.0f, 0.0f, 170.0f);

	act->sog->RequestTextures();

	act->SetActorHiddenInGame(false);
	act->SetActorLocationAndRotation(pos /* act->sog->GetRootPart()->groupPosition * 100 */,
	                                 act->sog->GetRootPart()->rotation);
	act->RegisterComponents();
}

AMeshActor* APotamosGameMode::CreateNewActor(rapidxml::xml_node<>* data, ObjectReadyDelegate d, AMeshActor* act) {
	rapidxml::xml_node<>* sopNode = data->first_node();
	if (FString(sopNode->name()) == TEXT("RootPart"))
		sopNode = sopNode->first_node();
	rapidxml::xml_node<>* uuidNode = sopNode->first_node("UUID");
	rapidxml::xml_node<>* inner = uuidNode->first_node();

	FGuid id;
	if (!FGuid::Parse(FString(inner->value()), id))
		return 0;

	if (!act)
		act = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass());

	act->OnObjectReady = d;

	actors.Add(id, act);

	if (!act->Load(data))
		act->Destroy();

	return act;
}

void APotamosGameMode::CreateNewActor(FGuid id, TSharedAssetRef data) {
	if (data->state == AssetBase::Failed) {
		UE_LOG(LogTemp, Error, TEXT("Asset fetch failed"));
		return;
	}

	TSharedRef<PrimAsset, ESPMode::ThreadSafe> prim = StaticCastSharedRef<PrimAsset>(data);

	rapidxml::xml_document<> doc;

	// MUST make a copy here because rapidxml::parse modifies the data
	// in place. That would destroy the data cached in the asset and
	// subsequent requests for this asset would get garbage.
	// Learned this the hard way!
	TArray<uint8_t> d(prim->xmlData);

	try {
		doc.parse<0>((char*)d.GetData());
	}
	catch (...) {
		return;
	}

	rapidxml::xml_node<>* groupNode = doc.first_node();
	if (!groupNode || FString(groupNode->name()) != TEXT("SceneObjectGroup"))
		return;

	AMeshActor* act = CreateNewActor(groupNode);
}

void APotamosGameMode::Tick(float deltaSeconds) {
	Super::Tick(deltaSeconds);

	creator->TickPool();

	AssetCache::Get().Tick();
	AssetCache::GetTexCache().Tick();
}

ObjectCreator::ObjectCreator(APotamosGameMode* m) {
	mode = m;

	thread = FRunnableThread::Create(this, TEXT("ObjectCreator"), 0, TPri_BelowNormal);
}

ObjectCreator::~ObjectCreator() {
	runThis = false;
	//thread->Kill();
	thread->WaitForCompletion();
	delete thread;
}

bool ObjectCreator::Init() {
	return true;
}

uint32_t ObjectCreator::Run() {
	UE_LOG(LogGameMode, Warning, TEXT("Made it here"));

#ifndef ONE_OBJECT_TEST
	const char* path = "/tmp/UnrealViewerData/primsload.xml";

	struct stat st;
	if (stat(path, &st) < 0) {
		path = "/UnRealViewer/primsload.xml";
		if (stat(path, &st) < 0) {
			UE_LOG(LogTemp, Warning, TEXT("Scene XML not found"));
			return 0;
		}
	}
	int fd = open(path, O_RDONLY);

	if (fd < 0 || st.st_size == 0)
		return 0;

	uint8_t* fileData = new uint8_t[st.st_size + 1];
	read(fd, fileData, st.st_size);
	fileData[st.st_size] = 0;
	close(fd);

	rapidxml::xml_document<> doc;

	doc.parse<0>((char*)fileData);

	rapidxml::xml_node<>* rootNode = doc.first_node();

	rapidxml::xml_node<>* sog = rootNode->first_node();

	while (runThis) {
		poolLock.Lock();
		if (pool.Num() == 0) {
			poolLock.Unlock();
			usleep(100000);
			continue;
		}

		if (sog) {
			AMeshActor* act = pool[0];
			pool.RemoveAt(0);

			poolLock.Unlock();

			ObjectReadyDelegate d;
			d.BindRaw(this, &ObjectCreator::ObjectReady);

			mode->CreateNewActor(sog, d, act);
			sog = sog->next_sibling();
		} else {
			poolLock.Unlock();
			UE_LOG(LogTemp, Warning, TEXT("All actors created"));
			break;
		}

		//usleep(50);
	}

	delete fileData;

#endif

	return 0;
}

void ObjectCreator::ObjectReady(AMeshActor* act) {
	readyLock.Lock();
	ready.Add(act);
	readyLock.Unlock();
}

void ObjectCreator::Stop() {
	runThis = false;
}

// MUST BE CALLED ON GAME THREAD
void ObjectCreator::TickPool() {
	poolLock.Lock();
	while (pool.Num() < 100) {
		AMeshActor* act = mode->GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass());
		pool.Add(act);
	}
	poolLock.Unlock();

	readyLock.Lock();
	while (ready.Num()) {
		AMeshActor* act = ready[0];
		ready.RemoveAt(0);
		readyLock.Unlock();

		act->BuildObject();
		act->SetActorHiddenInGame(false);
		act->SetActorLocationAndRotation(act->sog->GetRootPart()->groupPosition * 100,
		                                 act->sog->GetRootPart()->rotation);
		act->RegisterComponents();
		act->sog->RequestTextures();
		readyLock.Lock();
	}
	readyLock.Unlock();
}
