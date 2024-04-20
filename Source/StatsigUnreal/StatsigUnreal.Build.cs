using UnrealBuildTool;
using System.IO;
using System.Linq;

public class StatsigUnreal : ModuleRules
{
	public StatsigUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		OptimizeCode = CodeOptimization.Never;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDefinitions.Add("STATSIG_UNREAL_PLUGIN=1");
		
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

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"StatsigUnreal/statsig-cpp-client-sdk/src/statsig",
				"StatsigUnreal/statsig-cpp-client-sdk/src/statsig/internal",
			}
		);
	}
}