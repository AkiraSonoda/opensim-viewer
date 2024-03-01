/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */

using UnrealBuildTool;
using System.Collections.Generic;

public class PotamosEditorTarget : TargetRules
{
	public PotamosEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("Potamos");
	}

}
