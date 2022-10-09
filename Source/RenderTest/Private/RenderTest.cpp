// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenderTest.h"
#include "TestRenderer.h"
#include "EngineModule.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRenderTestModule"

void FRenderTestModule::StartupModule()
{
	FString ShaderPath = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("RenderTest"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/RenderTest"), ShaderPath);



	// Spawn Renderer in Render thread
	ENQUEUE_RENDER_COMMAND(CreateRadiationRenderer)(
		[this](FRHICommandListImmediate& RHICmdList)
		{
			m_renderer = new FTestRenderer();
		});
}

void FRenderTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenderTestModule, RenderTest)