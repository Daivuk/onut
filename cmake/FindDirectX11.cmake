# -----------------------------------------------------------------------------
# Find DirectX11 SDK
# Define:
# DirectX11_FOUND
# DirectX11_INCLUDE_DIRS
# DirectX11_LIBRARIES

if(WIN32) # The only platform it makes sense to check for DirectX11 SDK
	set(DirectX11_FOUND TRUE)	
    set(DirectX11_LIBRARIES d3d11 dxguid dxgi d3dcompiler xinput dinput8)
endif(WIN32)
