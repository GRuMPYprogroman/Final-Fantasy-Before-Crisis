#include "ExplorationState.h"
#include <fstream>
#include <sstream>

ExplorationState::ExplorationState(
    std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService>  audioService,
    std::shared_ptr<StateService>  stateService,
    std::shared_ptr<Character> player,
    std::string_view locationsCsv,
    std::string_view enemiesCsv,
    std::string_view lootCsv)
    : render_(renderService)
    , audio_(audioService)
    , states_(stateService)
    , player_(player)
    , locationId_(contractId)
    , rng_(std::random_device{}())
{
    font_ = std::make_unique<sf::Font>(render_->getDefaultFont());

    loadLocations(locationsCsv);
    loadEnemies(enemiesCsv);
    loadLoot(lootCsv);

    locData_ = locationsMap_.at(locationId_);
    enemyWeights_ = enemiesMap_[locData_.enemyTable];
    lootWeights_ = lootMap_[locData_.lootTable];

    initUI();
    addLog("Entering area: " + locData_.name);
    describeRoom();
}

void ExplorationState::loadLocations(std::string_view path) {
    std::ifstream file(path.data());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        LocationData ld;
        int id;
        // id
        std::getline(ss, cell, ';'); id = std::stoi(cell);
        // name
        std::getline(ss, ld.name, ';');
        // description
        std::getline(ss, ld.description, ';');
        // neighbors
        std::getline(ss, cell, ';');
        std::stringstream ns(cell);
        for (int i = 0;i < 4;++i) {
            std::getline(ns, cell, '|');
            ld.neighbors[i] = cell.empty() ? 0 : std::stoi(cell);
        }
        // lootTable
        std::getline(ss, ld.lootTable, ';');
        // enemyTable
        std::getline(ss, ld.enemyTable, ';');
        // backgroundImage
        std::getline(ss, ld.backgroundImage, ';');
        // musicTrack
        std::getline(ss, ld.musicTrack, ';');
        locationsMap_[id] = ld;
    }
}

void ExplorationState::loadEnemies(std::string_view path) {
    std::ifstream file(path.data());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, mtStr, wtStr;
        std::getline(ss, key, ';');
        std::getline(ss, mtStr, ';');
        std::getline(ss, wtStr, ';');
        int w = std::stoi(wtStr);
        MonsterType mt = MonsterType::Goblin;
        if (mtStr == "Goblin") mt = MonsterType::Goblin;
        else if (mtStr == "Spider") mt = MonsterType::Spider;
        else if (mtStr == "Wolf")   mt = MonsterType::Wolf;
        else if (mtStr == "Troll")  mt = MonsterType::Troll;
        else if (mtStr == "Dragon") mt = MonsterType::Dragon;
        enemiesMap_[key].emplace_back(mt, w);
    }
}

void ExplorationState::loadLoot(std::string_view path) {
    std::ifstream file(path.data());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, idStr, wtStr;
        std::getline(ss, key, ';');
        std::getline(ss, idStr, ';');
        std::getline(ss, wtStr, ';');
        int id = std::stoi(idStr);
        int w = std::stoi(wtStr);
        // найдём Item по id (из GameUtils)
        /**for (auto& it : GameUtils::allItems) {
            if (it.id == id) {
                lootMap_[key].emplace_back(it, w);
                break;
            }
        }**/
    }
}

void ExplorationState::initUI() {
    auto& win = render_->getRenderWindow();
    float W = win.getSize().x;
    float H = win.getSize().y;
    float inputH = H * 0.25f, msgH = H - inputH;

    messageText_ = std::make_unique<sf::Text>(*font_);
    inputPrompt_ = std::make_unique<sf::Text>(*font_);

    messageArea_.setSize({ W,msgH });
    messageArea_.setFillColor({ 15,15,15 });
    inputArea_.setSize({ W,inputH });
    inputArea_.setPosition({ 0, msgH });
    inputArea_.setFillColor({ 25,25,25 });

    auto font = render_->getDefaultFont();

    messageText_->setFont(font);
    messageText_->setCharacterSize(fontSize_);
    messageText_->setFillColor(sf::Color::White);
    messageText_->setPosition({ 5, 5 });
    inputPrompt_->setFont(font);
    inputPrompt_->setCharacterSize(fontSize_);
    inputPrompt_->setFillColor(sf::Color::Green);
    inputPrompt_->setPosition({5, msgH + 5});
    inputPrompt_->setString("> ");
}

void ExplorationState::addLog(std::string_view line) {
    auto lines = wrapText(line, messageArea_.getSize().x - 10);
    for (auto& l : lines) logLines_.push_back(l);
    while (logLines_.size() > 50) logLines_.pop_front();
}

std::vector<std::string> ExplorationState::wrapText(std::string_view text, float maxWidth) {
    std::vector<std::string> out;
    std::istringstream iss(text.data());
    std::string word, line;
    while (iss >> word) {
        std::string cand = line.empty() ? word : line + " " + word;
        messageText_->setString(cand);
        if (messageText_->getGlobalBounds().size.x > maxWidth) {
            if (!line.empty()) out.push_back(line);
            line = word;
        }
        else line = cand;
    }
    if (!line.empty()) out.push_back(line);
    return out;
}

MonsterType ExplorationState::pickRandomEnemy() {
    int total = 0;
    for (auto& p : enemyWeights_) total += p.second;
    std::uniform_int_distribution<int> d(1, total);
    int r = d(rng_);
    for (auto& p : enemyWeights_) {
        r -= p.second;
        if (r <= 0) return p.first;
    }
    return enemyWeights_.front().first;
}

Item ExplorationState::pickRandomLoot() {
    int total = 0;
    for (auto& p : lootWeights_) total += p.second;
    std::uniform_int_distribution<int> d(1, total);
    int r = d(rng_);
    for (auto& p : lootWeights_) {
        r -= p.second;
        if (r <= 0) return p.first;
    }
    return lootWeights_.front().first;
}

void ExplorationState::describeRoom() {
    addLog(locData_.description);
}

void ExplorationState::tryRandomEvent() {
    std::uniform_int_distribution<int> d(0, 2);
    int ev = d(rng_);
    if (ev == 0 && !lootWeights_.empty()) {
        Item it = pickRandomLoot();
        addLog("Found " + it.name + "!");
        player_->AddItem(it);
    }
    else if (ev == 1 && !enemyWeights_.empty()) {
        MonsterType m = pickRandomEnemy();
        addLog("A wild " + std::string((m)) + " appears!");
        states_->pushState(std::make_unique<CombatState>(
            render_, audio_, states_, player_, m,
            [this](bool victory) {
                if (victory) addLog("You defeated the foe.");
                else        addLog("You fled.");
                describeRoom();
            }
        ));
    }
    else addLog("Nothing happens.");
}

void ExplorationState::processCommand(std::string cmd) {
    if (cmd == "help") {
        addLog("Commands: left,forward,right,backward,attack,use,help");
    }
    else if (cmd == "left" || cmd == "forward" || cmd == "right" || cmd == "backward") {
        addLog("You move " + cmd + ".");
        int idx = cmd == "left" ? 0 : cmd == "forward" ? 1 : cmd == "right" ? 2 : 3;
        int nxt = locData_.neighbors[idx];
        if (nxt == 0) addLog("You can't go that way.");
        else {
            states_->popState();
            states_->pushState(std::make_unique<ExplorationState>(
                render_, audio_, states_, player_, nxt,
                /*paths*/"", "", ""
            ));
        }
    }
    else if (cmd == "attack") {
        addLog("You swing at thin air.");
    }
    else if (cmd == "use") {
        addLog("You rummage.");
    }
    else if (!cmd.empty()) {
        addLog("Unknown: " + cmd);
    }
}

void ExplorationState::render() {
    render_->getRenderWindow().draw(messageArea_);
    float y = 5;
    size_t start = logLines_.size() > 25 ? logLines_.size() - 25 : 0;
    for (size_t i = start;i < logLines_.size();++i) {
        messageText_->setString(logLines_[i]);
        messageText_->setPosition({ 5, y });
        render_->getRenderWindow().draw(*messageText_);
        y += fontSize_ + 2;
    }
    render_->getRenderWindow().draw(inputArea_);
    inputPrompt_->setString("> " + inputBuffer_);
    render_->getRenderWindow().draw(*inputPrompt_);
}

void ExplorationState::handleInput(const sf::Event& evt) {
    if (evt.is<sf::Event::TextEntered>) {
        if (evt.getIf<sf::Event::TextEntered>()->unicode == '\r' || evt.getIf<sf::Event::TextEntered>()->unicode == '\n') {
            processCommand(inputBuffer_);
            inputBuffer_.clear();
        }
        else if (evt.getIf<sf::Event::TextEntered>()->unicode == 8 && !inputBuffer_.empty()) {
            inputBuffer_.pop_back();
        }
        else if (evt.getIf<sf::Event::TextEntered>()->unicode < 128) {
            inputBuffer_ += static_cast<char>(evt.getIf<sf::Event::TextEntered>()->unicode);
        }
    }
}
