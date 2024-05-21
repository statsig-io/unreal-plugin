using UnrealBuildTool;
using System.IO;
using System.Linq;

public class StatsigUnreal : ModuleRules
{
	public StatsigUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		IWYUSupport = IWYUSupport.None;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDefinitions.Add("STATSIG_UNREAL_PLUGIN=1");
		PublicDefinitions.Add("STATSIG_WITH_COMPATIBILITY_OVERRIDE=1");

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP",
				"Json"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"DeveloperSettings",
				"Engine",
				"Slate",
				"SlateCore",
			}
		);

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "Private"),
			Path.Combine(ModuleDirectory, "statsig-cpp-client-sdk/src/statsig"),
		});
	}
}