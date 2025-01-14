/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

class SceneObjectGroup;

enum SceneObject { ObjectBase = 0, ObjectGroup = 1, ObjectPart = 2 };

/**
 * 
 */
DECLARE_DELEGATE(SceneObjectReadyDelegate)

class POTAMOS_API SceneObjectBase {
public:
	SceneObjectBase();
	virtual ~SceneObjectBase();

	virtual void FetchAssets();
	virtual void RequestTextures() = 0;

	SceneObjectBase* parent;
	SceneObjectGroup* group;

	virtual SceneObject inline Type() {
		return ObjectBase;
	}

protected:
	bool haveAllAssets;

private:
	virtual void AssetReceived(FGuid id, TArray<uint8_t> data);
};
