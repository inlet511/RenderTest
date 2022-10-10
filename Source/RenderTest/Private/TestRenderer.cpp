#include "TestRenderer.h"
#include "GlobalShader.h"
#include "Renderer/Private/SceneRendering.h"
#include "Renderer/Private/PostProcess/SceneRenderTargets.h"
#include "RenderGraphResources.h"
#include "EngineModule.h"
#include "RendererInterface.h"
#include "TestShaders.h"


FTestRenderer::FTestRenderer()
	:ThisModule(nullptr)
{
	RenderHandle = GetRendererModule().RegisterPostOpaqueRenderDelegate(FPostOpaqueRenderDelegate::CreateRaw(this, &FTestRenderer::Render));
	ThisModule = &FModuleManager::GetModuleChecked<FRenderTestModule>("RenderTest");
}

FTestRenderer::~FTestRenderer()
{
	GetRendererModule().RemovePostOpaqueRenderDelegate(RenderHandle);
	RenderHandle.Reset();

	ThisModule = nullptr;
}


IMPLEMENT_GLOBAL_SHADER(FMyCS, "/RenderTest/RadiationShaders.usf", "MainCS", SF_Compute);


void FTestRenderer::Render(FPostOpaqueRenderParameters& InParameters)
{

	FRDGBuilder& GraphBuilder = *InParameters.GraphBuilder;
	const FViewInfo& View = *InParameters.View;

	const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	const TShaderMapRef<FMyCS> ComputeShader(GlobalShaderMap);

	RDG_EVENT_SCOPE(GraphBuilder, "Test");




	FRDGTextureDesc TextDesc = FRDGTextureDesc::Create3D(
		FIntVector(512, 512, 512),
		EPixelFormat::PF_R32_FLOAT,
		FClearValueBinding::None,
		TexCreate_RenderTargetable | TexCreate_ShaderResource | TexCreate_UAV);

	FRDGTextureRef VolumeTexture = GraphBuilder.CreateTexture(TextDesc, TEXT("RadiationVolumeTexture"));



	TArray<FVector4f> Data;
	for (int i = 0; i < 5; i++)
	{
		Data.Add(FVector4f(i / 5.0f, i / 5.0f, i / 5.0f, i / 5.0f));
	}

	FRDGBufferRef Buffer = CreateStructuredBuffer(
		GraphBuilder,
		TEXT("RadiationDataBuffer"),
		sizeof(FVector4f),
		5,
		Data.GetData(),
		sizeof(FVector4f) * 5,
		ERDGInitialDataFlags::None
	);


	// Compute Shader Parameters and Textures
	FMyCS::FParameters* CSParams = GraphBuilder.AllocParameters<FMyCS::FParameters>();
	CSParams->OutUAV = GraphBuilder.CreateUAV(VolumeTexture);
	CSParams->InDataBuffer = GraphBuilder.CreateSRV(Buffer);
	uint32 GroupCount = FMath::DivideAndRoundUp((uint32)512, (uint32)32);

	// Add Compute Pass, Render Radiation Data to VolumeTexture
	FComputeShaderUtils::AddPass(
		GraphBuilder,
		RDG_EVENT_NAME("TestComputeVolumeTexture"),
		ComputeShader,
		CSParams,
		FIntVector(GroupCount, GroupCount, 512));

}

