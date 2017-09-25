#include <stdio.h>

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
    double natureModifier = nature ? (nature == 1 ? 1 : 1.1) : 0.9;

    int attack = (int) (((2 * 105 + iv) * 24 / 100 + 5) * natureModifier);
    defense*= 2;

    unsigned short effectiveDamage = ((2 * 24 / 5 + 2) * 125 * attack / defense) / 50 + 2;
    effectiveDamage*= (((double)(85 + roll) / 100));
    effectiveDamage*= 1.5; // Stab
    effectiveDamage*= 2; // SE

    damageRolls[defIndex][nature][iv][roll] = effectiveDamage;

    printf("DEF: %d, Nature: %s, IV: %2d, Roll: %2d ----> %hu\n", defense / 2, nature ? (nature == 1 ? "NEUTRAL" : "POSITIVE") : "NEGATIVE", iv, roll + 1, effectiveDamage);

}

static inline void computeAllDamageRolls() {
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


int main() {
    computeAllDamageRolls();
    return 0;
}
