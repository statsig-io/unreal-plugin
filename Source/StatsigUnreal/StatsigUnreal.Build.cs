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
				// ... add private dependencies that you statically link with here ...	
			}
		);

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"StatsigUnreal/Private/compat/primitives",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src/statsig/public",
				"StatsigUnreal/Private/compat",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src/statsig/internal",
			}
		);

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "Private/compat/primitives"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src/statsig/public"),
				Path.Combine(ModuleDirectory, "Private/compat"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src/statsig/internal"),
			}
		);
	}
}