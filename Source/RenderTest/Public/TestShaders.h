#pragma once
#include "CoreMinimal.h"
#include "RendererInterface.h"
#include "RenderGraphResources.h"
#include "Shader.h"
#include "GlobalShader.h"
#include "ShaderParameterMacros.h"
#include "ShaderParameterUtils.h"
#include "ShaderParameterStruct.h"
#include "SceneRenderTargetParameters.h"
#include "RenderResource.h"



class FMyCS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FMyCS)
	SHADER_USE_ROOT_PARAMETER_STRUCT(FMyCS, FGlobalShader)

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture3D<float>, OutUAV)
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FVector4f>, InDataBuffer)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return RHISupportsComputeShaders(Parameters.Platform);
	}
};
IMPLEMENT_GLOBAL_SHADER(FMyCS, "/RenderTest/RadiationShaders.usf", "MainCS", SF_Compute);

