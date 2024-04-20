using UnrealBuildTool;
using System.IO;
using System.Linq;

public class StatsigUnreal : ModuleRules
{
	public StatsigUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		OptimizeCode = CodeOptimization.Never;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDefinitions.Add("STATSIG_DISABLE_EXCEPTIONS=1");
		PublicDefinitions.Add("STATSIG_DISABLE_FILESYSTEM=1");
		PublicDefinitions.Add("STATSIG_UNREAL=1");
		PublicDefinitions.Add("B=1");
		
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
				// "StatsigUnreal/Private/compat/primitives",
				"StatsigUnreal/statsig-cpp-client-sdk/src/statsig",
				// "StatsigUnreal/Private/compat",
				"StatsigUnreal/statsig-cpp-client-sdk/src/statsig/internal",
			}
		);

		// PublicIncludePaths.AddRange(
		// 	new string[]
		// 	{
		// 		// Path.Combine(ModuleDirectory, "Private/compat/primitives"),
		// 		Path.Combine(ModuleDirectory, "statsig-cpp-client-sdk/src"),
		// 		// Path.Combine(ModuleDirectory, "Private/compat"),
		// 		Path.Combine(ModuleDirectory, "statsig-cpp-client-sdk/src/statsig/internal"),
		// 	}
		// );
	}
}