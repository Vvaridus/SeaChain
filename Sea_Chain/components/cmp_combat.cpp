#include "cmp_combat.h"
#include <system_renderer.h>
#include <system_resources.h>

void CombatComponent::update(double dt) {}

void CombatComponent::render() { 
    //Renderer::queue(&_text); 
}

CombatComponent::CombatComponent(Entity* const p) : Component(p) {

}

void CombatComponent::SetText(const std::string& str) {
  //_string = str;
  //_text.setString(_string);
}
