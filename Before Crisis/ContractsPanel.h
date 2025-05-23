#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "AudioService.h"
#include "StateService.h"
#include "CharacterManager.h"
#include "Button.h"

struct Contract {
    int    id;
    std::string title;
    int    required_rank;
    int    reward_exp;
	int    reward_money;
    int    min_enemies;
    int    max_enemies;
};

class ContractsPanel final : public IGameplayPanel {
private:
    void loadContracts(const std::string& path);
    void buildButtons();

    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService>  audio_service_;
    std::shared_ptr<StateService> state_service_;
    std::shared_ptr<Character> player_;

    std::unique_ptr<sf::Font> font_;
    std::unique_ptr<sf::Text> title_;
    std::unique_ptr<sf::Text> player_info_;

    std::vector<Contract>  all_contracts_;
    std::vector<std::shared_ptr<Button>> contract_buttons_;

    static inline int required_contract_id_ = 1;
public:
    ContractsPanel(std::shared_ptr<RenderService> render_service,
        std::shared_ptr<AudioService> audio_service,
        std::shared_ptr<StateService> state_service,
        std::shared_ptr<Character> player,
        const std::string& csvPath);

    void update(float dt) override;
    void render() override;
    void handleInput(const sf::Event& evt) override;
    void updatePlayerInfo();
};
