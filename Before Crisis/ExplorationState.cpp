#include "ExplorationState.h"
#include <fstream>
#include <sstream>
#include "iostream"

ExplorationState::ExplorationState(
    std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService>  audioService,
    std::shared_ptr<StateService>  stateService,
    std::shared_ptr<Character> player,
    int locationId,
    std::function<void(bool)> onFinish)
    : render_(renderService),
      audio_(audioService),
      states_(stateService),
      player_(player),
      locationId_(locationId),
      mt_randomizer_(std::random_device{}()),
	  onFinish_(std::move(onFinish))
{
    std::string locationsCsv = "../tables/Locations.csv";

    font_ = std::make_unique<sf::Font>(render_->getDefaultFont());

    loadLocations(locationsCsv);

    current_loc = locationsMap_.at(1);
    current_hp_ = player_->GetStats().hp;

    initUI();
    addLog("Entering area: " + locationsMap_.at(1).title);
    describeRoom();
}

void ExplorationState::loadLocations(std::string_view path) {
    std::ifstream file(path.data());
    if (!file.is_open())
    {
        std::cout << "file cannot be opened";
        return;
    }

    std::string line;
    std::getline(file, line);

    std::string pathToLocation;
    std::string pathToEnemies;
    std::string minEnemies;
    std::string maxEnemies;

    while (std::getline(file, line, ';')) {
        std::stringstream ss(line);

        int id;
        ss >> id;

        if (id != locationId_) continue;
        std::getline(file, line);
        ss.clear();
        ss.str(line);

        std::string otherData;
        std::getline(ss, otherData, ';');
        std::getline(ss, otherData, ';');
        std::getline(ss, otherData, ';');
        std::getline(ss, otherData, ';');
        std::getline(ss, minEnemies, ';');
        std::getline(ss, maxEnemies, ';');
        std::getline(ss, pathToLocation, ';');
        std::getline(ss, pathToEnemies, ';');

        std::cout << pathToLocation << pathToEnemies << '\n';
        file.close();
        break;
    }

    loadMap(*this, pathToLocation);
    loadEnemies(*this, std::stoi(minEnemies), std::stoi(maxEnemies),pathToEnemies);

}

void loadMap(ExplorationState& obj,std::string_view pathToLocation)
{
    std::string line;
    std::ifstream fileLoc(pathToLocation.data());
    if (!fileLoc.is_open()) return;

    std::getline(fileLoc, line);

    while (std::getline(fileLoc, line))
    {
        std::stringstream ss(line);

        std::string idStr, title, description, neighborsStr;
        std::getline(ss, idStr, ';');
        std::getline(ss, title, ';');
        std::getline(ss, description, ';');
        std::getline(ss, neighborsStr);

        Location newLoc;
        newLoc.id = std::stoi(idStr);
        newLoc.title = title;
        newLoc.description = description;

        std::stringstream neighborStream(neighborsStr);
        int neighbor;
        while (neighborStream >> neighbor) {
            newLoc.neighbors.push_back(neighbor);
        }

        obj.locationsMap_.emplace(newLoc.id, newLoc);
    }
}

void loadEnemies(ExplorationState& obj, int min_enemies, int max_enemies, std::string_view pathToEnemies)
{
    int enemies_amount;
    std::uniform_int_distribution rand_int{ min_enemies, max_enemies };

    enemies_amount = rand_int(obj.mt_randomizer_);

    rand_int = std::uniform_int_distribution{ 0,1 };

    std::ifstream file(pathToEnemies.data());
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line);

    std::vector<Monster> possible_enemies;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string name, hp, dmg, exp;
        std::getline(ss, name, ';');
        std::getline(ss, hp, ';');
        std::getline(ss, dmg, ';');
        std::getline(ss, exp, ';');

        Monster new_enemy{ std::stoi(hp),std::stoi(dmg), std::stoi(exp),name };
        possible_enemies.push_back(new_enemy);
    }

    for (auto& loc: obj.locationsMap_)
    {
	    if (loc.first != 1 && rand_int(obj.mt_randomizer_))
	    {
            loc.second.has_enemy = true;
            obj.enemies_.emplace(loc.first, obj.pickRandomEnemy(possible_enemies));

	    }
    }

}

Monster ExplorationState::pickRandomEnemy(std::vector<Monster>& possible_enemies)
{
    int size = possible_enemies.size();
    std::uniform_int_distribution which_enemy(0, size-1);
    return possible_enemies.at(which_enemy(mt_randomizer_));
}

void ExplorationState::initUI() {
    float width = render_->getRenderWindow().getSize().x;
    float height = render_->getRenderWindow().getSize().y;
    float inputH = height * 0.25f;
	float msgH = height - inputH;

    messageText_ = std::make_unique<sf::Text>(*font_);
    inputPrompt_ = std::make_unique<sf::Text>(*font_);

    messageArea_.setSize({ width,msgH });
    messageArea_.setFillColor({ 15,15,15 });
    inputArea_.setSize({ width,inputH });
    inputArea_.setPosition({ 0, msgH });
    inputArea_.setFillColor({ 25,25,25 });

    auto font = render_->getDefaultFont();

    messageText_->setCharacterSize(fontSize_);
    messageText_->setFillColor(sf::Color::White);
    messageText_->setPosition({ 5, 5 });
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
        std::string cant = line.empty() ? word : line + " " + word;
        messageText_->setString(cant);
        if (messageText_->getGlobalBounds().size.x > maxWidth) {
            if (!line.empty()) out.push_back(line);
            line = word;
        }
        else line = cant;
    }

    if (!line.empty()) out.push_back(line);
    return out;
}

void ExplorationState::describeRoom() {
    addLog(current_loc.description);
}

void ExplorationState::finishMission(bool victory)
{
    onFinish_(victory);
    states_->popState();
}

void ExplorationState::processCommand(std::string cmd) {
    std::stringstream ss(cmd);
    int loc;

    if (ss >> loc && loc <= current_loc.neighbors.back() && loc > 0) {
        if (loc != current_loc.id)
        {
            addLog("You enter: " + locationsMap_.at(loc).title);
            current_loc = locationsMap_.at(loc);
        }
    }
    else
    {
        addLog("Unknown command...");
    }

    if (current_loc.has_enemy)
    {
        states_->pushState(std::make_unique<CombatState>(
            render_, audio_, states_, player_, enemies_.at(current_loc.id),
            [this](bool victory) {
                if (victory)
                {
                    addLog("You defeated the foe.");
                    enemies_.erase(current_loc.id);
                    locationsMap_.at(current_loc.id).has_enemy = false;
                    if (enemies_.empty()) finishMission(true);
                    describeRoom();
                }
                else
                {
                    addLog("You Lost");
                    finishMission(false);
                }},current_hp_));
    }
}

void ExplorationState::update(float& deltaTime){}

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

    messageText_->setString("You can go to:\n");
    messageText_->setPosition({ 5, y });
    render_->getRenderWindow().draw(*messageText_);
    y += fontSize_ + 2;

    for (auto& loc : current_loc.neighbors)
    {
        messageText_->setString(std::to_string(loc) + ". " + locationsMap_.at(loc).title + "\n");
        messageText_->setPosition({ 5, y });
        render_->getRenderWindow().draw(*messageText_);
        y += fontSize_ + 2;

    }

    render_->getRenderWindow().draw(inputArea_);
    inputPrompt_->setString("> " + inputBuffer_);
    render_->getRenderWindow().draw(*inputPrompt_);
}

void ExplorationState::handleInput(const sf::Event& evt) {
    if (evt.is<sf::Event::TextEntered>()) {
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
