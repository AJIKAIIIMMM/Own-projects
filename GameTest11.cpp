#include <iostream>
#include <string>
#include <random>
#include <cassert>

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator(PRNG& generator)
{
    std::random_device device;
    generator.engine.seed(device());
}

unsigned random(PRNG& generator, unsigned minValue, unsigned maxValue)
{
    assert(minValue < maxValue);
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
    return distribution(generator.engine);
}

class Creature {
protected:
    std::string m_name;
    char m_symbol;
    int m_hp;
    int m_atk;
    int m_gold;
public:
    Creature(std::string name, char symbol, int hp, int atk, int gold):
        m_name(name), m_symbol(symbol), m_hp(hp), m_atk(atk), m_gold(gold){}

    std::string getName() { return m_name; }
    int getHealth() { return m_hp; }
    int getGold() { return m_gold; }
    char getSymbol() { return m_symbol; }
    int getAtk() { return m_atk; }
    void reduceHealth(int a){
        m_hp -= a;
    }
    bool isDead() { return m_hp <= 0; }
    void addGold(int a) {
        m_gold += a;
    }
};

class Player : public Creature {
    int m_level = 1;
public:
    Player(std::string name) :
        Creature(name, '@', 10, 1, 0) {
        std::cout << "Welcome, " << name << '.' << std::endl;
    }

    void levelUp() { m_level++; m_atk++; }
    int getLevel() { return m_level; }
    bool hasWon() { return m_level == 20; }

    

    
};

class Monster : public Creature {
public:
    enum Type {
        DRAGON,
        ORC,
        SLIME,
        MAX_TYPES
    };

    struct MonsterData {
        std::string name;
        char symbol;
        int health;
        int damage;
        int gold;
    };

    static Monster::MonsterData monsterData[Monster::MAX_TYPES];

    Monster(Type type):
        Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold){}


    static int getRandomMonster(){
        PRNG generator;
        initGenerator(generator);
        return random(generator, 0, (MAX_TYPES - 1));
    }
    
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
{ "dragon", 'D', 20, 4, 100 },
{ "orc", 'o', 4, 2, 25 },
{ "slime", 's', 1, 1, 10 }
};

char fightMonster(Monster& m) {
    std::cout << "You have encountered a " << m.getName() << '(' << m.getSymbol() << ")." << std::endl;
    char answer;
    while (true) {
        std::cout << "Do you want to run (R) or fight (F)?";
        std::cin >> answer;
        answer = toupper(answer);
        if (answer != 'R' && answer != 'F') {
            system("cls");
            std::cout << "Enter correct answer" << std::endl;
            std::cout << "You have encountered a " << m.getName() << '(' << m.getSymbol() << ")." << std::endl;
            continue;
        }
        break;
    }
    return answer;
}

void attackPlayer(Monster& m, Player& p) {
    p.reduceHealth(m.getAtk());
}

void attackMonster(Player& p, Monster& m) {
    m.reduceHealth(p.getAtk());
}

void Game() {
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    Player p(name);
    char temp;
    while (true) {
        Monster m = Monster::Type(Monster::getRandomMonster()); /*static_cast<Monster::Type>(Monster::getRandomMonster());*/
        while (true) {
            temp = fightMonster(m);
            if (temp == 'R') {
                PRNG generator;
                initGenerator(generator);
                bool a = random(generator, 0, 1);
                if (a) {
                    std::cout << "You succesfully fled." << std::endl;
                    break;
                }
                else {
                    std::cout << "Your escape failed." << std::endl;
                    attackPlayer(m, p);
                    std::cout << "The " << m.getName() << " hit you for " << m.getAtk() << std::endl;
                    break;
                }
            }
            else if (temp == 'F') {
                while (true) {
                    std::cout << "You hit the " << m.getName() << " for " << p.getAtk() << " damage." << std::endl;
                    attackMonster(p, m);
                    if (m.isDead()) {
                        std::cout << "You killed the " << m.getName() << '.' << std::endl;
                        p.levelUp();
                        std::cout << "You are now level " << p.getLevel() << '.' << std::endl;
                        p.addGold(m.getGold());
                        std::cout << "You found " << m.getGold() << " gold." << std::endl;
                        break;
                    }
                    attackPlayer(m, p);
                    if (p.isDead()) {
                        break;
                    }
                }
            }
            if (p.isDead()) {
                break;
            }
            if (m.isDead()) break;
        }
        if (p.isDead()) {
            std::cout << "You died at level " << p.getLevel() << " and with " << p.getGold() << " gold.\m"
                << "Too bad you can't take it with you!" << std::endl;
            break;
        }
        else if (p.hasWon()) {
            std::cout << "You reached " << p.getLevel() << "!\n" << "You won!" << std::endl;
            break;
        }
    }
}

int main()
{
    Game();
    return 0;
}


