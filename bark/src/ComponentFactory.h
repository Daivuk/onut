#pragma once

#include <onut/ForwardDeclaration.h>
#include <string>

ForwardDeclare(Component);

void initComponentFactory();
void shutdownComponentFactory();
ComponentRef createComponentByName(const std::string& name);
