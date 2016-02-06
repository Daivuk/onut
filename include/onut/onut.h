#pragma once
#include "forward_declaration.h"
OForwardDeclare(Texture)
OForwardDeclare(ContentManager)

#include <string>

OTextureRef OGetTexture(const std::string& name);
