// skilltreesample.cpp : Defines the entry point for the application.
//

#include "skilltreesample.h"

using namespace std;

struct SkillTree {
  virtual ~SkillTree() = default;
  virtual bool GainSkill() = 0;
};

struct SwordSpecial : public SkillTree {
  // returns true if point spent successfully
  bool GainSkill() override {
    if (tiers_into_tree >= maxskillpoints) return false;
    // learn ability (choice)
    char choice = '\n';
    std::cout << "Choose a place to spend skill point " << tiers_into_tree + 1 << '\n';
    switch (tiers_into_tree) {
    case 0: {
      while (t1 == Tier1::unspent) {
        std::cout << "d: damage\n";
        std::cout << "a: accuracy\n";
        std::cout << "choice: ";
        std::cin >> choice;
        if (choice == 'd') {
          t1 = Tier1::plus_damage;
          std::cout << "plus damage chosen for tier1\n";
        }
        else if (choice == 'a') {
          t1 = Tier1::plus_accuracy;
          std::cout << "plus accuracy chosen for tier1\n";
        }
        std::cin.ignore(100, '\n');
      }
    }break;
    case 1: {
      if (t1 == Tier1::plus_damage) {
        t2 = Tier2::power_attack;
      } else if (t1 == Tier1::plus_accuracy) {
        t2 = Tier2::stablestrike;
      }
    }break;
    case 2: {
    
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
};

struct Player {
  void attack() {
    // attack logic and choices
    if (chosen_skilltree) {
      std::cout << "sword attack! (d: " << GetSwordDamage() << " a: " << GetSwordAccuracy() << ")\n";
    }
    else {
      std::cout << "basic attack!\n";
    }

    LevelUp();
  }
private:
  void LevelUp() {
    std::cout << "level up!\n";
    skillpoints++;
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
  int skillpoints = 0;
  int pointsspent = 0;
  int damage = 1;
  int accuracy = 1;
  SkillTree* chosen_skilltree = nullptr;
};

int main()
{
  Player our_player;

  our_player.attack();
  our_player.attack();
  our_player.attack();

  return 0;
}
