#pragma once 
#include "CoreMinimal.h"
#include "Renderer/Private/SceneRendering.h"

struct MyStruct {

};

class FTestRenderer
{
public:
	FTestRenderer();
	~FTestRenderer();


	void Render(FPostOpaqueRenderParameters& InParameters);

private:
	FDelegateHandle RenderHandle;
	class FRenderTestModule* ThisModule;
};