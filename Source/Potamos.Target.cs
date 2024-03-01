/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
using UnrealBuildTool;
using System.Collections.Generic;

public class PotamosTarget : TargetRules
{
	public PotamosTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("Potamos");
	}

}
