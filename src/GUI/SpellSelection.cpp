#include "GUI/SpellSelection.h"

SpellSelection::SpellSelection(SpellManager* manager) {
	m_spellManager = manager;
	m_spellManager->setSpellSelection(this);
	reload();
	selectSlot(m_spellManager->getSelectedSpellID());
}

SpellSelection::~SpellSelection() {
	m_spellSlots.clear();
}

void SpellSelection::activateSlot(int spellNr, const sf::Time& cooldown) {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size()) - 1) return;
	if (m_spellSlots[m_selectedSlot].getSpellID() == SpellID::VOID) return;
	m_spellSlots[spellNr].playAnimation(cooldown);
}

void SpellSelection::selectSlot(int spellNr) {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size()) - 1) return;
	if (m_spellSlots[m_selectedSlot].getSpellID() == SpellID::VOID) return;
	m_spellSlots[m_selectedSlot].deselect();
	m_spellSlots[spellNr].select();
	m_selectedSlot = spellNr;
}

void SpellSelection::update(const sf::Time& frametime) {
	for (int i = 0; i < static_cast<int>(m_spellSlots.size()); i++) {
		m_spellSlots[i].update(frametime);
		if (m_spellSlots[i].isClicked() && !m_spellSlots[i].isLocked()) {
			m_spellManager->getOwner()->getScreen()->getCharacterCore()->setWeaponSpell(m_spellSlots[i].getKey());
			m_spellManager->setAndExecuteSpell(i);
		}
	}
}

void SpellSelection::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	for (auto& it : m_spellSlots) {
		it.render(target);
	}
}

void SpellSelection::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	for (auto& it : m_spellSlots) {
		it.renderAfterForeground(target);
	}
}

void SpellSelection::reload() {
	m_spellSlots.clear();
	auto& lockedMagic = m_spellManager->getOwner()->getLevel()->getLockedMagic();
	float offset = SpellSlot::ICON_OFFSET;
	for (auto& it : m_spellManager->getSpellMap()) {
		SpellSlot slot(it->getSpellData());
		slot.setPosition(sf::Vector2f(
			SPELLSELECTION_OFFSET.x + offset,
			WINDOW_HEIGHT - (SpellSlot::ICON_SIZE + SPELLSELECTION_OFFSET.y + SpellSlot::ICON_OFFSET)));
		
		slot.setLocked(contains(lockedMagic, slot.getSpellType()));
		
		m_spellSlots.push_back(slot);
		offset += (SPELLSLOT_SPACING + SpellSlot::SIZE);
	}
	m_selectedSlot = 0;
	if (!m_spellSlots.empty()) {
		selectSlot(0);
	}
}

bool SpellSelection::isSlotLocked(int spellNr) const {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size())) {
		return false;
	}
	return m_spellSlots.at(spellNr).isLocked();
}

std::vector<SpellSlot>& SpellSelection::getSlots() {
	return m_spellSlots;
}

void SpellSelection::show() {
	m_isVisible = true;
}

void SpellSelection::hide() {
	m_isVisible = false;
}