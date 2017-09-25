#include <stdio.h>

#define NATURE_TEXT(nature) (nature ? (nature == 1 ? "NEUTRAL" : "POSITIVE") : "NEGATIVE")
#define NATURE_MODIFER(nature) (nature ? (nature == 1 ? 1 : 1.1) : 0.9)

#define STAT_VALUE(base, level, natureModifier) ((int) (((2 * base + iv) * level / 100 + 5) * natureModifier))
#define BASE_DAMAGE(level, power, attack, defense) (((2 * level / 5 + 2) * power * attack / defense) / 50 + 2)
#define DAMAGE_MODIFIERS(damageVar, roll, stab, se) (\
    damageVar*= ((double)(85 + roll) / 100);\
    if (stab) damageVar*= 1.5;\
)

static unsigned short damageRolls[20][3][32][16];
/*                                |   |  |   |
 *                                |   |  |   +-----> Damage Rolls
 *                                |   |  +---------> Lurantis ATK IV
 *                                |   +------------> Lurantis ATK Nature
 *                                V
 *                           Popplio DEF
 */


static unsigned short encounteredDamageRolls[20][10] = {
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

static inline void computeDamageRoll(int defense, int nature, int iv, int roll) {
    int defIndex = defense - 38;
    double natureModifier = NATURE_MODIFER(nature);

    int attack = STAT_VALUE(105, 24, natureModifier);
    defense*= 2;

    unsigned short effectiveDamage = BASE_DAMAGE(24, 125, attack, defense);
    effectiveDamage*= (((double)(85 + roll) / 100));
    effectiveDamage*= 1.5; // Stab
    effectiveDamage*= 2; // SE

    damageRolls[defIndex][nature][iv][roll] = effectiveDamage;

    // printf("DEF: %d, Nature: %s, IV: %2d, Roll: %2d ----> %hu\n", defense / 2, NATURE_TEXT(nature), iv, roll + 1, effectiveDamage);

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

static inline int checkCombination(int nature, int iv, int evs) {
    double natureModifier = NATURE_MODIFER(nature);
}

static void checkStaticIvsAndStaticNaturesTheory() {

    printf("Checking theory: static IVs & static natures & static EVs (of 0)");

    for (int nature = 0; nature < 3; ++nature) {
        for (int iv = 0; iv < 32; ++iv) {
            printf("Nature: %s, IV: %d - ", NATURE_TEXT(nature), iv);

            if (checkCombination(nature, iv, 0)) {

            }
        }
    }
}


int main() {
    computeAllDamageRolls();
    checkStaticIvsAndStaticNaturesTheory();
    return 0;
}
