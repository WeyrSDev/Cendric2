#include "GUI/QuickSlot.h"
#include "CharacterCore.h"
#include "Level/LevelInterface.h"
#include "Screens/LevelScreen.h"
#include "ResourceManager.h"
#include "GlobalResource.h"

using namespace std;

const float QuickSlot::SIZE = 58.f;
const float QuickSlot::ICON_OFFSET = 4.f;

QuickSlot::QuickSlot(WorldInterface* _interface, const std::string& itemID, Key key) {
	m_interface = _interface;
	m_screen = _interface->getScreen();
	m_core = _interface->getCore();
	m_itemID = itemID;
	m_key = key;

	setBoundingBox(sf::FloatRect(0.f, 0.f, ICON_SIZE, ICON_SIZE));
	setDebugBoundingBox(COLOR_BAD);
	setInputInDefaultView(true);

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);

	m_keyText.setString(key != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[key]) :
		"");
	m_keyText.setCharacterSize(16);
	if (m_keyText.getLocalBounds().width > SIZE - 10.f) m_keyText.setCharacterSize(8);

	m_backgroundRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);

	m_overlayRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));

	m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY);
	m_borderTextureSelected = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY_SELECTED);

	m_highlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT);

	m_borderRect.setSize(sf::Vector2f(SIZE, SIZE));
	m_borderRect.setTexture(m_borderTexture);

	m_iconRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_iconRect.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_ITEMS));

	reload();
}

void QuickSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	m_tooltipWindow.setPosition(sf::Vector2f(pos.x - m_tooltipWindow.getSize().x + ICON_SIZE, pos.y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
	sf::Vector2f positionOffset(QuickSlot::ICON_SIZE / 2.f - m_keyText.getLocalBounds().width / 2.f, QuickSlot::SIZE - QuickSlot::ICON_OFFSET / 2.f);
	m_keyText.setPosition(pos + positionOffset);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + ICON_SIZE - m_amountText.getLocalBounds().width,
		pos.y + ICON_SIZE - m_amountText.getLocalBounds().height));
}

void QuickSlot::setItemID(const std::string& itemID) {
	m_itemID = itemID;
	m_screen->notifyQuickSlotAssignment(itemID, m_key == Key::QuickSlot1 ? 1 : 2);
	reload();
	g_inputController->lockAction();
}

void QuickSlot::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(m_key)) {
		consume();
		g_inputController->lockAction();
	}
	Slot::update(frameTime);
}

void QuickSlot::render(sf::RenderTarget& renderTarget) {
	Slot::render(renderTarget);
	renderTarget.draw(m_amountText);
	renderTarget.draw(m_keyText);
}

void QuickSlot::consume() {
	if (m_isEmpty) return;
	if (LevelInterface* li = dynamic_cast<LevelInterface*>(m_interface)) {
		li->consumeItem(m_itemID);
	}
}

void QuickSlot::reload() {
	// check if item exists
	if (m_itemID.empty() || !contains(*m_core->getItems(), m_itemID)) {
		// the slot is empty
		m_isEmpty = true;
		m_iconRect.setTextureRect(sf::IntRect(0, 0, 0, 0));
		m_keyText.setColor(COLOR_GREY);
		m_amountText.setString("");
		m_itemID = "";
		m_tooltipWindow.setText("(" + g_textProvider->getText("Empty") + ")");
	}
	else {
		// the slot is filled
		m_isEmpty = false;
		Item* item = g_resourceManager->getItem(m_itemID);
		if (item == nullptr || item->getType() == ItemType::VOID) return;
		int amount = m_core->getItems()->at(m_itemID);

		m_iconRect.setTextureRect(sf::IntRect(
			item->getIconTextureLocation().x,
			item->getIconTextureLocation().y,
			static_cast<int>(ICON_SIZE),
			static_cast<int>(ICON_SIZE)));

		m_amountText.setString(to_string(amount));
		m_keyText.setColor(COLOR_WHITE);

		m_amountText.setPosition(sf::Vector2f(
			getPosition().x + ICON_SIZE - m_amountText.getLocalBounds().width,
			getPosition().y + ICON_SIZE - m_amountText.getLocalBounds().height));

		m_tooltipWindow.setText(g_textProvider->getText(m_itemID, "item"));
	}

	m_borderRect.setFillColor(m_isEmpty ? COLOR_MEDIUM_GREY : COLOR_WHITE);
	setPosition(getPosition());
}

void QuickSlot::onLeftClick() {
	consume();
	Slot::onLeftClick();
	g_inputController->lockAction();
}

void QuickSlot::onRightClick() {
	setItemID("");
	Slot::onRightClick();
	g_inputController->lockAction();
}