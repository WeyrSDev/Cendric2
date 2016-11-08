#pragma once

#include <map>

#include "global.h"
#include "GUI/BitmapText.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Enums/QuestState.h"
#include "Enums/SpellID.h"
#include "Structs/SpellModifier.h"

class Item;

/* A screen overlay that supports arbitrary text, split in title and subtitle */
class ScreenOverlay : public virtual GameObject {
public:
	ScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	virtual ~ScreenOverlay() {};

	virtual void update(const sf::Time& frameTime) override;
	virtual void render(sf::RenderTarget& renderTarget) override;

	void setTitle(const std::string& textKey, const std::string& textType = "core");
	void setTitleRaw(const std::string& text);
	void setTitleCharacterSize(int characterSize);
	void setTitleColor(const sf::Color& color);
	void setSubtitle(const std::string& textKey, const std::string& textType = "core");
	void setSubtitleRaw(const std::string& text);
	void setSubtitleCharacterSize(int characterSize);
	void setSubtitleColor(const sf::Color& color);
	void setPermanent(bool permanent);

	GameObjectType getConfiguredType() const override;

	static ScreenOverlay* createQuestScreenOverlay(const std::string& questID, QuestState state);
	static ScreenOverlay* createLocationScreenOverlay(const std::string& locationKey, bool isBossLevel = false, bool isObserved = false);
	// returns nullptr if the configuration for display hints is false.
	static ScreenOverlay* createHintScreenOverlay(const std::string& hintKey);
	static ScreenOverlay* createPermanentItemScreenOverlay(const Item* item);
	static ScreenOverlay* createSpellLearnedScreenOverlay(SpellID id);
	static ScreenOverlay* createModifierLearnedScreenOverlay(const SpellModifier& modifier);
	static ScreenOverlay* createGameOverScreenOverlay();
	static ScreenOverlay* createGamePausedScreenOverlay();
	static ScreenOverlay* createPartyLockedScreenOverlay();
	static ScreenOverlay* createEnemyDefeatedScreenOverlay(std::map<std::string, int>& items, int gold);
	static ScreenOverlay* createArrestedScreenOverlay();

protected: 
	float m_scale;
	bool m_isPermanent;

	BitmapText m_title;
	BitmapText m_subtitle;

	virtual void repositionText();

	sf::Time m_fadeTime;

private:
	sf::Time m_activeTime;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;

	void load();
};
