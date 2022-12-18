// skilltreesample.cpp : Defines the entry point for the application.
//

#include "skilltreesample.h"
#include <vector>

#define ATTACKS_TO_RETIRE 200
int attacksElapsedGlobal = 0;

struct SkillTree {
  virtual ~SkillTree() = default;
  virtual bool GainSkill() = 0;
};

bool Chance50() {
  return rand() < (RAND_MAX >> 1);
}
bool Chance75() {
  return Chance50() || Chance50();
}
bool Chance25() {
  return Chance50() && Chance50();
}

enum AttackType {
  //default
  basic,
    
  //t1 sword
  swordswing,
  //t2 sword
  power_attack,
  stable_strike,
  //t3 swords
  weaponblock,
  doublestrike,
  quickswap,
  fastattack,
  
  //fun white-flag attack you unlock after playing for 100 turns
  makepeace
};
char GetAttackChar(AttackType attackType) {
  switch(attackType) {
  case basic: return 'b';
  case swordswing: return 's';
  case power_attack: return 'a';
  case stable_strike: return 'n';
  case weaponblock: return 'w';
  case doublestrike: return 'd';
  case quickswap: return 'q';
  case fastattack: return 'f';
  case makepeace: return 'p';
  }
  return '?';
}
const char* GetAttackString(AttackType attackType) {
  switch(attackType) {
  case basic: return "basic";
  case swordswing: return "sword swing";
  case weaponblock: return "weapon block";
  case power_attack: return "power attack";
  case stable_strike: return "stable strike";
  case doublestrike: return "double strike";
  case quickswap: return "quick swap";
  case fastattack: return "fast attack";
  case makepeace: return "make peace";
  }
  return "?";
}

struct SwordSpecial : public SkillTree {
  // returns true if point spent successfully
  bool GainSkill() override {
    if (tiers_into_tree >= maxskillpoints) return false;
    // learn ability (choice)
    char choice = '\n';
    std::cout << "Skill tree point " << tiers_into_tree + 1 << " unlocked\n";
    switch (tiers_into_tree) {
    case 0: {
      while (t1 == Tier1::unspent) {
        choice = PromptChoice("d: damage", "a: accuracy");
        if (choice == 'd') {
          t1 = Tier1::plus_damage;
          std::cout << "plus damage chosen for tier1\n";
        }
        else if (choice == 'a') {
          t1 = Tier1::plus_accuracy;
          std::cout << "plus accuracy chosen for tier1\n";
        }
      }
    }break;
    case 1: {
      if (t1 == Tier1::plus_damage) {
        t2 = Tier2::power_attack;
        std::cout << "power attack unlocked\n";
      } else if (t1 == Tier1::plus_accuracy) {
        t2 = Tier2::stablestrike;
        std::cout << "stable strike unlocked\n";
      }
    }break;
    case 2: {
      while(t3 == Tier3::unspent) {
        if(t2 == Tier2::power_attack) {
          choice = PromptChoice("d: double strike", "w: weapon block");
          if(choice == 'd') {
            t3 = Tier3::doublestrike;
            std::cout << "double strike chosen for tier3\n";
          } else if(choice == 'w') {
            t3 = Tier3::weaponblock;
            std::cout << "weapon block chosen for tier3\n";
          }
        } else if(t2 == Tier2::stablestrike) {
          choice = PromptChoice("f: fast attack", "q: quick swap");
          if(choice == 'f') {
            t3 = Tier3::fastattack;
            std::cout << "fast attack chosen for tier3\n";
          } else if(choice == 'q') {
            t3 = Tier3::quickswap;
            std::cout << "quick swap chosen for tier3\n";
          }
        }
      }
    }break;
    default: break;
    }

    tiers_into_tree++;
    return true;
  }

  enum class Tier1 { unspent, plus_damage, plus_accuracy } t1 = Tier1::unspent;
  enum class Tier2 { unspent, power_attack, stablestrike } t2 = Tier2::unspent;
  enum class Tier3 { unspent, doublestrike, weaponblock, fastattack, quickswap } t3 = Tier3::unspent;

private:

  int tiers_into_tree = 0;
  const int maxskillpoints = 3;

  char PromptChoice(const char* option1, const char* option2) {
    char choice = '\n';
    std::cout << option1 << "\n" << option2 << "\n -=> choice: ";
    std::cin >> choice;
    std::cin.ignore(100, '\n');
    return choice;
  }
};

struct Player {
  void attack() {
    // attack logic and choices
    char choice;
    bool killedEnemy = false;

    // announcement text/cohices
    std::cout << "-=- your turn to attack! -=-\n";
    std::vector<AttackType> attackOptions = GetAttackOptions();
    for(AttackType t : attackOptions) {
      std::cout << GetAttackChar(t) << ": " << GetAttackString(t) << "\n";
    }
    std::cout << " + ";
    std::cin >> choice;
    std::cin.ignore(100, '\n');

    //attack types
    if(choice == GetAttackChar(AttackType::basic)) {
      std::cout << "basic attack!\n";
      if(AttackChance(Chance25)) {
        std::cout << "enemy successfully killed!\n";
        killedEnemy = true;
      } else {
        std::cout << "enemy dodged!\n";
      }
    } else if(choice == GetAttackChar(AttackType::swordswing)) {
      if(AttackChance(Chance50)) {
        std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
        killedEnemy = true;
      } else {
        std::cout << "enemy dodged!\n";
      }
    } else if(choice == GetAttackChar(AttackType::doublestrike)) {
      if(AttackChance(Chance75)) {
        std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
        std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
        killedEnemy = true;
      } else {
        std::cout << "interrupted by enemy!\n";
      }
    } else if(choice == GetAttackChar(AttackType::fastattack)) {
      std::cout << "fast attack! (d: " << GetSwordDamage() * 0.75f << " a: " << GetSwordAccuracy() * 1.25f << ")\n";
      killedEnemy = true;
    } else if(choice == GetAttackChar(AttackType::quickswap)) {
      if(AttackChance(Chance50)) {
        std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
        std::cout << "enemy killed!\n";
        killedEnemy = true;
      } else {
        std::cout << "the enemy has dodged!\n";
      }
      if(Chance75()) {
        std::cout << "you expertly twisted and dodged around and are now facing another enemy!\n";
        this->attack();
      } else {
        std::cout << "you failed to twist around and quick swap!\n";
      }
    } else if(choice == GetAttackChar(AttackType::weaponblock)) {
      if(AttackChance(Chance50)) {
        std::cout << "failed to block!\n";
        std::cout << "you punch the enemy to retaliate!\n";
        if(Chance50()) {
          std::cout << "you successfully retaliate and kill the enemy!\n";
          killedEnemy = true;
        } else {
          std::cout << "you fail to retaliate and take a hit!\n";
        }
      } else {
        std::cout << "block succeeded!\n";
      }
    } else if(choice == GetAttackChar(AttackType::power_attack)) {
      if(AttackChance(Chance75)) {
        std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
        killedEnemy = true;
      } else {
        std::cout << "enemy dodged!\n";
      }
    } else if(choice == GetAttackChar(AttackType::stable_strike)) {
      int attackTries = 2;
      for(int i = 0; i < attackTries; i++) {
        if(AttackChance(Chance25)) {
          std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
          killedEnemy = true;
          break;
        } else if(i < attackTries-1) {
          std::cout << "enemy dodged, trying again!\n";
        } else {
          std::cout << "attack failed!\n";
        }
      }
    } else if(choice== GetAttackChar(AttackType::makepeace)) {
      attacksElapsedGlobal = ATTACKS_TO_RETIRE;
      std::cout << "you put on your white flag and wizard hat, and you make peace with the enemy.\n";
      std::cout << "... [ENTER]";
      std::cin.get();
    } else {
      std::cout << "you waddle around confusedly and take a hit from the enemy.\n";
    }
    if(killedEnemy) {
      LevelUp();
    }
  }
private:
  //Pass in chance25(), chance50(), chance75(), etc. It gets run based on sword accuracy
  bool AttackChance(bool (*chanceFunc)()) {
    for(int i = 0; i < GetSwordAccuracy(); i++) {
      if(chanceFunc()) {
        return true;
      }
    }
    return false;
  }
  void LevelUp() {
    skillpoints++;
    std::cout << "level up! skill points: " << skillpoints << "\n";
    if (!chosen_skilltree) {
      std::cout << "sword spec chosen" << '\n';
      chosen_skilltree = new SwordSpecial();
      pointsspent++;
    }
    if (chosen_skilltree) {
      if (pointsspent < skillpoints) {
        if (chosen_skilltree->GainSkill()) {
          pointsspent++;
        }
      }
    }
  }
  int GetSwordDamage() {
    int total_damage = damage;
    if (chosen_skilltree) {
      SwordSpecial* spec = dynamic_cast<SwordSpecial*>(chosen_skilltree);
      if (spec) {  // will be null if cast failed
        if (spec->t1 == SwordSpecial::Tier1::plus_damage) total_damage += 1;
      }
    }
    return total_damage;
  }
  int GetSwordAccuracy() {
    int total_accuracy = accuracy;
    if (chosen_skilltree) {
      SwordSpecial* spec = dynamic_cast<SwordSpecial*>(chosen_skilltree);
      if (spec) {  // will be null if cast failed
        if (spec->t1 == SwordSpecial::Tier1::plus_accuracy) total_accuracy += 1;
      }
    }
    return total_accuracy;
  }
  std::vector<AttackType> GetAttackOptions() {
    std::vector<AttackType> out;
    out.push_back(AttackType::basic);
    if(chosen_skilltree) {
      SwordSpecial* spec = dynamic_cast<SwordSpecial*>(chosen_skilltree);
      if(spec) {
        //default sword attack: 
        out.push_back(AttackType::swordswing);
        //tier 2 attacks:
        if(spec->t2 == SwordSpecial::Tier2::power_attack) {
          out.push_back(AttackType::power_attack);
        }
        if(spec->t2 == SwordSpecial::Tier2::stablestrike) {
          out.push_back(AttackType::stable_strike);
        }
        //tier 3 attacks:
        if(spec->t3 == SwordSpecial::Tier3::doublestrike) {
          out.push_back(AttackType::doublestrike);
        }
        if(spec->t3 == SwordSpecial::Tier3::weaponblock) {
          out.push_back(AttackType::weaponblock);
        }
        if(spec->t3 == SwordSpecial::Tier3::fastattack) {
          out.push_back(AttackType::fastattack);
        }
        if(spec->t3 == SwordSpecial::Tier3::quickswap) {
          out.push_back(AttackType::quickswap);
        }
      }
    }
    if(attacksElapsedGlobal > 100) {
      out.push_back(AttackType::makepeace);
    }
    return out;
  }
  int skillpoints = 0;
  int pointsspent = 0;
  int damage = 1;
  int accuracy = 1;
  SkillTree* chosen_skilltree = nullptr;
};

int main()
{
  srand(time(NULL));

  Player our_player;

  while(attacksElapsedGlobal < ATTACKS_TO_RETIRE) {
    our_player.attack();
    attacksElapsedGlobal++;
  }
  std::cout << "__..--..__\n";
  std::cout << "you have fought many a hard battle, warrior.\n";
  std::cout << "you may retire now [ENTER].\n";
  std::cin.get();
  std::cout << "YOU. WIN. [ENTER]\n";
  std::cin.get();

  return 0;
}
