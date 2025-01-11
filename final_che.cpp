#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
using namespace std;

// 前向宣告
class Skill;
class Character;
class Warrior;
class Wizard;
class Monster;

// 技能類別
class Skill {
private:
    string name;        // 技能名稱
    string description; // 技能描述
    int mpCost;         // MP消耗
    double effect;      // 技能倍率

public:
    Skill(string name, string desc, int mp, double eff) 
        : name(name), description(desc), mpCost(mp), effect(eff) {}
    
    string getName() const { return name; }
    string getDescription() const { return description; }
    int getMpCost() const { return mpCost; }
    double getEffect() const { return effect; }
};

// 角色基礎類別
class Character {
protected:
    string characterName;   // 角色名稱
    int currentHp;          // 當前血量
    int maxHp;              // 最大血量
    int currentMp;          // 當前魔力
    int maxMp;              // 最大魔力
    int attackPower;        // 攻擊力
    vector<Skill> skills;   // 技能列表

public:
    Character(string n, int maxHpValue, int maxMpValue, int atkPower) 
        : characterName(n), currentHp(maxHpValue), maxHp(maxHpValue), 
          currentMp(maxMpValue), maxMp(maxMpValue), attackPower(atkPower) {}
    
    // 基本getter/setter
    string getCharacterName() const { return characterName; }
    int getCurrentHp() const { return currentHp; }
    int getCurrentMp() const { return currentMp; }
    int getAttackPower() const { return attackPower; }
    
    // 狀態修改
    void takeDamage(int dmg) { currentHp = max(0, currentHp - dmg); }
    void useMp(int amount) { currentMp = max(0, currentMp - amount); }
    bool isDead() const { return currentHp <= 0; }
    
    // 技能相關
    void addSkill(Skill skill) { skills.push_back(skill); }
    const vector<Skill>& getSkills() const { return skills; }
    
    // 顯示狀態
    virtual void displayStatus() const {
        cout << characterName << " HP:" << currentHp << "/" << maxHp 
             << " MP:" << currentMp << "/" << maxMp 
             << " Attack:" << attackPower << endl;
    }
    
    // 顯示技能列表
    void displaySkills() const {
        cout << "Skill list:" << endl;
        for (size_t i = 0; i < skills.size(); i++) {
            cout << char('A' + i) << ". " 
                 << skills[i].getName() << " (MP:" 
                 << skills[i].getMpCost() << ")" << endl;
        }
    }
};

// 戰士類別
class Warrior : public Character {
public:
    Warrior(string name) : Character(name, 120, 60, 15) {
        // 戰士技能初始化
        addSkill(Skill("Heavy Strike", "Deals 200% attack damage", 20, 2.0));
        addSkill(Skill("Frenzied Strike", "Deals 300% attack damage", 40, 3.0));
    }

    void displayStatus() const override {
        cout << "Warrior ";
        Character::displayStatus();
    }
};

// 法師類別
class Wizard : public Character {
public:
    Wizard(string name) : Character(name, 80, 120, 10) {
        // 法師技能初始化
        addSkill(Skill("Fireball", "Deals 250% attack damage", 20, 2.5));
        addSkill(Skill("Lightning Bolt", "Deals 350% attack damage", 40, 3.5));
    }

    void displayStatus() const override {
        cout << "Wizard ";
        Character::displayStatus();
    }
};

// 怪物類別
class Monster : public Character {
private:
    int level;  // 怪物等級

public:
    Monster(int lv) : Character("Monster", 50 + lv * 30, 0, 5 + lv * 3), level(lv) {}
    
    void displayStatus() const override {
        cout << "Lv." << level << " ";
        Character::displayStatus();
    }
};

// 戰鬥系統
class BattleSystem {
public:
    // 執行戰鬥
    static bool executeBattle(Character* player, Monster* monster) {
        cout << "\nBattle starts!" << endl;
        monster->displayStatus();
        
        while (true) {
            // 顯示當前狀態
            cout << "\nYour status:" << endl;
            player->displayStatus();
            
            // 玩家回合
            if (!playerTurn(player, monster)) {
                return false;  // 玩家死亡
            }
            
            // 檢查怪物是否死亡
            if (monster->isDead()) {
                cout << "\nYou defeated the monster!" << endl;
                return true;
            }
            
            // 怪物回合
            monsterTurn(player, monster);
            
            // 檢查玩家是否死亡
            if (player->isDead()) {
                cout << "\nYou were defeated..." << endl;
                return false;
            }
            
            cout << "\nPress any key to continue..." << endl;
            _getch();
        }
        return false;
    }

private:
    // 玩家回合
    static bool playerTurn(Character* player, Monster* monster) {
        cout << "\nChoose your action:" << endl;
        cout << "1. Normal Attack" << endl;
        player->displaySkills();
        
        char choice;
        cin >> choice;
        
        if (choice == '1') {
            // 普通攻擊
            int damage = player->getAttackPower();
            monster->takeDamage(damage);
            cout << "You dealt " << damage << " damage to the monster!" << endl;
        }
        else if (choice >= 'A' && choice < char('A' + player->getSkills().size())) {
            // 使用技能
            int skillIndex = choice - 'A';
            const Skill& skill = player->getSkills()[skillIndex];
            
            if (player->getCurrentMp() < skill.getMpCost()) {
                cout << "Not enough MP!" << endl;
                return true;
            }
            
            player->useMp(skill.getMpCost());
            int damage = int(player->getAttackPower() * skill.getEffect());
            monster->takeDamage(damage);
            cout << "You used " << skill.getName() << " and dealt " << damage << " damage to the monster!" << endl;
        }
        else {
            cout << "Invalid choice!" << endl;
            return true;
        }
        
        return true;
    }
    
    // 怪物回合
    static void monsterTurn(Character* player, Monster* monster) {
        int damage = monster->getAttackPower();
        player->takeDamage(damage);
        cout << "The monster dealt " << damage << " damage to you!" << endl;
    }
};

// 主遊戲系統
class Game {
public:
    static void start() {
        cout << "Welcome to the simple RPG game!" << endl;
        
        // 選擇職業
        cout << "\nChoose your class:" << endl;
        cout << "1. Warrior" << endl;
        cout << "2. Wizard" << endl;
        
        char choice;
        cin >> choice;
        
        // 輸入名字
        cout << "Enter your name: ";
        string name;
        cin >> name;
        
        // 創建角色
        Character* player = (choice == '1') ? 
            static_cast<Character*>(new Warrior(name)) : 
            static_cast<Character*>(new Wizard(name));
        
        // 開始遊戲循環
        bool gameOver = false;
        for (int level = 1; level <= 3 && !gameOver; level++) {
            cout << "\n=== Stage " << level << " ===" << endl;
            Monster monster(level);
            
            if (!BattleSystem::executeBattle(player, &monster)) {
                gameOver = true;
            }
        }
        
        // 遊戲結束
        if (!gameOver) {
            cout << "\nCongratulations, you cleared the game!" << endl;
        }
        
        delete player;
    }
};

// 主函數
int main() {
    Game::start();
    return 0;
}
