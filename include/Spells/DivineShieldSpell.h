#pragma once

#include "global.h"
#include "Spells/Spell.h"

class DivineShieldSpell final : public Spell {
public:
	DivineShieldSpell(const AttributeData& additionalResistance);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;
	void execOnHit(LevelMovableGameObject* target) override;
	void setDisposed() override;

private:
	AttributeData m_additionalResistance;
};