#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define NATURE_TEXT(nature) (nature ? (nature == 1 ? " NEUTRAL" : "POSITIVE") : "NEGATIVE")
#define NATURE_MODIFER(nature) (nature ? (nature == 1 ? 1 : 1.1) : 0.9)

#define STAT_VALUE(base, level, evs, natureModifier) ((int) (((2 * base + iv + evs / 4) * level / 100 + 5) * natureModifier))
#define BASE_DAMAGE(level, power, attack, defense) (((2 * level / 5 + 2) * power * attack / defense) / 50 + 2)

static int damageRolls[20][3][32][16];
/*                     |   |  |   |
 *                     |   |  |   +-----> Damage Rolls
 *                     |   |  +---------> Lurantis ATK IV
 *                     |   +------------> Lurantis ATK Nature
 *                     V
 *                Popplio DEF
 */


static int encounteredDamageRolls[20][10] = {
        {0}, // 38
        {0}, // 39
        {0}, // 40
        {0}, // 41
        {50, 54, 56, 60, 0}, // 42
        {50, 54, 56, 62, 0}, // 43
        {48, 50, 54, 56, 0}, // 44
        {48, 50, 54, 56, 60, 0}, // 45
        {44, 48, 50, 54, 0}, // 46
        {48, 54, 0}, // 47
        {44, 48, 50, 54, 0}, // 48
        {42, 44, 48, 50, 54, 0}, // 49
        {42, 44, 48, 50, 0}, // 50
        {38, 42, 44, 0}, // 51
        {42, 44, 48, 0}, // 52
        {0}, // 53
        {0}, // 54 --> IMPOSSIBLE TO REACH
        {42, 44, 0}, // 55
        {0}, // 56
        {0}, // 57
};

//static int encounteredDamageRolls[20][10] = {
//        {0}, // 38
//        {0}, // 39
//        {0}, // 40
//        {0}, // 41
//        {0}, // 42
//        {0}, // 43
//        {0}, // 44
//        {36, 0}, // 45
//        {0}, // 46
//        {0}, // 47
//        {30, 32, 0}, // 48
//        {36, 0}, // 49
//        {32, 36, 0}, // 50
//        {30, 32, 36, 0}, // 51
//        {30, 32, 0}, // 52
//        {0}, // 53
//        {0}, // 54 --> IMPOSSIBLE TO REACH
//        {30, 32, 0}, // 55
//        {0}, // 56
//        {0}, // 57
//};

static inline void computeDamageRoll(int defense, int nature, int iv, int roll) {
    int defIndex = defense - 38;
    double natureModifier = NATURE_MODIFER(nature);

    int attack = STAT_VALUE(105, 24, 0, natureModifier);
    defense*= 2; // +2
//    defense*= 3; // +4

    int effectiveDamage = BASE_DAMAGE(24, 125, attack, defense);
    effectiveDamage*= (((double)(85 + roll) / 100));
    effectiveDamage*= 1.5; // Stab
    effectiveDamage*= 2; // SE

    damageRolls[defIndex][nature][iv][roll] = effectiveDamage;

     printf("DEF: %d, Nature: %s, IV: %2d, Roll: %2d ----> %d\n", defense / 2, NATURE_TEXT(nature), iv, roll + 1, effectiveDamage);

}

static void computeAllDamageRolls() {
    for (int popplioDefense = 38; popplioDefense <= 57; popplioDefense++) {
        for (int nature = 0; nature < 3; nature++) {
            for (int iv = 0; iv < 32; iv++) {
                for (int roll = 0; roll < 16; ++roll) {
                    computeDamageRoll(popplioDefense, nature, iv, roll);
                }
            }
        }
    }
}

static bool verboseCombinationCheck = false;

static inline bool checkCombination(int nature, int iv, int evs) {
    for (int i = 0; i < 20; ++i) {
        int *encounteredDamage = encounteredDamageRolls[i];
        while (*encounteredDamage) {
            // Check if this encountered damage roll can be explained by parameters
            int damage = *encounteredDamage;

            bool found = false;
            for (int j = 0; j < 16; ++j) {
                if (damageRolls[i][nature][iv][j] == damage) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (verboseCombinationCheck) {
                    printf("Can't explain: %d roll at %d DEF - ", damage, i + 38);
                }
                return false;
            }


            ++encounteredDamage;
        }
    }
    return true;
}

static void checkStaticIvsAndStaticNaturesTheory() {

    printf("\nChecking theory: static IVs & static natures & static EVs (of 0)\n");
    verboseCombinationCheck = true;
    for (int nature = 0; nature < 3; ++nature) {
        for (int iv = 0; iv < 32; ++iv) {
            printf("Nature: %s, IV: %2d - ", NATURE_TEXT(nature), iv);

            if (checkCombination(nature, iv, 0)) {
                printf("Possible\n");
            } else {
                printf("Impossible\n");
            }
        }
    }
}

static void checkVariableIvsAndStaticNaturesTheory() {
    printf("\nChecking theory: variable IVs & static natures & static EVs (of 0)\n");
    verboseCombinationCheck = false;
    for (int nature = 0; nature < 3; ++nature) {
        printf("Nature: %s - ", NATURE_TEXT(nature));

        bool possible = false;

        for (int iv = 0; iv < 32; ++iv) {
            if (checkCombination(nature, iv, 0)) {
                possible = true;
                break;
            }
        }
        if (possible) {
            printf("Possible\n");
        } else {
            printf("Impossible\n");
        }
    }
}

static void checkStaticIvsAndVariableNaturesTheory() {
    printf("\nChecking theory: static IVs & variable natures & static EVs (of 0)\n");
    verboseCombinationCheck = false;
    for (int iv = 0; iv < 32; ++iv) {
        printf("IV: %d - ", iv);
        bool possible = false;
        for (int nature = 0; nature < 3; ++nature) {
            if (checkCombination(nature, iv, 0)) {
                possible = true;
                break;
            }
        }
        if (possible) {
            printf("Possible\n");
        } else {
            printf("Impossible\n");
        }
    }
}

static void checkVariableIvsAndVariableNaturesTheory() {
    for (int i = 0; i < 20; ++i) {
        int *encounteredDamage = encounteredDamageRolls[i];
        while (*encounteredDamage) {
            // Check if this encountered damage roll can be explained by parameters
            int damage = *encounteredDamage;
            bool possible = false;
            int possibleCount = 0;
            for (int nature = 0; nature < 3; ++nature) {
                for (int iv = 0; iv < 32; ++iv) {
                    for (int roll = 0; roll < 16; ++roll) {
                        if (damageRolls[i][nature][iv][roll] == damage) {
                            possible = true;
                            ++possibleCount;
                            // printf("DAMAGE ROLL %d against %d DEF can be explained by %s@%2d IV damage rolls\n", damage, i + 38, NATURE_TEXT(nature), iv);
                            break;
                        }
                    }
                }
            }
            if (possible) {
                printf("DAMAGE ROLL: %d against %d DEF can be explained by %d out of 1536 combinations\n", damage, i + 38, possibleCount);
            } else {
                printf("DAMAGE ROLL: %d against %d DEF can't be explained by any combination\n", damage, i + 38);
            }
            ++encounteredDamage;
        }
    }
}



int main() {
    computeAllDamageRolls();
    checkStaticIvsAndStaticNaturesTheory();
    checkVariableIvsAndStaticNaturesTheory();
    checkStaticIvsAndVariableNaturesTheory();
    checkVariableIvsAndVariableNaturesTheory();
    return 0;
}
