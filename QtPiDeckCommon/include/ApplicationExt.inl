// NOCOVERAGE
#pragma once

#include "Application.hpp"

namespace QtPiDeck::detail {
template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::initialPreparations() {}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::appCreated() {}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::engineCreated(ApplicationEngine& /*engine*/) {}
}