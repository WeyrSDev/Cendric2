#pragma once

#include <map>

#include "global.h"
#include "Spell.h"
#include "InputController.h"
#include "Spells/FireSpell.h"
#include "Spells/ChopSpell.h"
#include "Spells/IceSpell.h"
#include "Spells/ForcefieldSpell.h"

// a class that decides whether a spell can be cast or not and adds modifiers. It also draws a part of the gui.
class SpellManager
{
public:
	SpellManager();
	~SpellManager();

	void update(sf::Time frameTime);
	void render(sf::RenderTarget &renderTarget);
	void addSpell(SpellBean& spell);
	// returns a spell that can be added to the game screen. If the spell cannot be cast because of cd, returns nullptr.
	Spell* getSpell();
	void setCurrentSpell(SpellID id);

private:
	SpellID m_currentSpell;
	std::map<SpellID, sf::Time> m_coolDownMap;
	std::map<SpellID, SpellBean> m_spellMap;
};