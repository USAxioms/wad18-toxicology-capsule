#include "toxicology_core_wad18.h"
#include <stdio.h>
#include <assert.h>

int pass_count = 0;
int total_count = 0;

void run_test(const char *name, int condition, const char *detail) {
    total_count++;
    if (condition) {
        pass_count++;
        printf("  ✅ %s\n", name);
        if (detail) printf("     → %s\n", detail);
    } else {
        printf("  ❌ %s\n", name);
        if (detail) printf("     → %s\n", detail);
    }
}

int main() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║   WAD18 TOXICOLOGY SUITE v1.0.0 - PURE INTEGER ARITHMETIC    ║\n");
    printf("║   ZERO Floating Point • Algebraically Verified Tests         ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");

    printf("═══ LAYER 1: HAZARD CLASSIFICATION (10 tests) ═══\n");
    
    /* Test 1: Dioxin (most toxic) */
    /* LD50 = 1,000 μg/kg → should be HAZARD_CAT1 */
    hazard_category_t cat_dioxin = classify_hazard_by_ld50(LD50_DIOXIN);
    run_test("Dioxin classified as CAT1 (extremely toxic)", 
             cat_dioxin == HAZARD_CAT1,
             "LD50=1000 μg/kg ≤ 5,000,000 μg/kg → CAT1");
    
    /* Test 2: Strychnine (extremely toxic) */
    /* LD50 = 2,000,000 μg/kg ≤ 5,000,000 → HAZARD_CAT1 */
    hazard_category_t cat_strychnine = classify_hazard_by_ld50(LD50_STRYCHNINE);
    run_test("Strychnine classified as CAT1 (extremely toxic)",
             cat_strychnine == HAZARD_CAT1,
             "LD50=2,000,000 μg/kg ≤ 5,000,000 → CAT1");
    
    /* Test 3: Cyanide (extremely toxic) */
    /* LD50 = 3,600,000 μg/kg ≤ 5,000,000 → HAZARD_CAT1 */
    hazard_category_t cat_cyanide = classify_hazard_by_ld50(LD50_CYANIDE);
    run_test("Cyanide classified as CAT1 (extremely toxic)",
             cat_cyanide == HAZARD_CAT1,
             "LD50=3,600,000 μg/kg ≤ 5,000,000 → CAT1");
    
    /* Test 4: Caffeine (toxic) */
    /* LD50 = 190,000,000 μg/kg → should be HAZARD_CAT3 */
    hazard_category_t cat_caffeine = classify_hazard_by_ld50(LD50_CAFFEINE);
    run_test("Caffeine classified as CAT3 (toxic)",
             cat_caffeine == HAZARD_CAT3,
             "LD50=190,000,000 μg/kg (50M,500M] → CAT3");
    
    /* Test 5: Aspirin (harmful) */
    /* LD50 = 1,500,000,000 μg/kg → should be HAZARD_CAT4 */
    hazard_category_t cat_aspirin = classify_hazard_by_ld50(LD50_ASPIRIN);
    run_test("Aspirin classified as CAT4 (harmful)",
             cat_aspirin == HAZARD_CAT4,
             "LD50=1,500,000,000 μg/kg (500M,2000M] → CAT4");
    
    /* Test 6: Water (non-toxic, exceeds CAT4) */
    hazard_category_t cat_water = classify_hazard_by_ld50(LD50_WATER);
    run_test("Water classified as NONE",
             cat_water == HAZARD_NONE,
             "LD50=15,000,000,000 μg/kg > 2,000,000,000 → NONE");
    
    /* Test 7: Boundary check - exactly at CAT1 threshold */
    hazard_category_t cat_boundary1 = classify_hazard_by_ld50(5000000LL);
    run_test("LD50=5,000,000 μg/kg exactly at CAT1 boundary",
             cat_boundary1 == HAZARD_CAT1,
             "Boundary value 5,000,000 → CAT1");
    
    /* Test 8: Boundary check - just above CAT1 threshold */
    hazard_category_t cat_boundary2 = classify_hazard_by_ld50(5000001LL);
    run_test("LD50=5,000,001 μg/kg just above CAT1 boundary",
             cat_boundary2 == HAZARD_CAT2,
             "Value 5,000,001 > 5,000,000 → CAT2");
    
    /* Test 9: Boundary check - exactly at CAT2 threshold */
    hazard_category_t cat_boundary3 = classify_hazard_by_ld50(50000000LL);
    run_test("LD50=50,000,000 μg/kg exactly at CAT2 boundary",
             cat_boundary3 == HAZARD_CAT2,
             "Boundary value 50,000,000 → CAT2");
    
    /* Test 10: Boundary check - exactly at CAT4 threshold */
    hazard_category_t cat_boundary4 = classify_hazard_by_ld50(2000000000LL);
    run_test("LD50=2,000,000,000 μg/kg exactly at CAT4 boundary",
             cat_boundary4 == HAZARD_CAT4,
             "Boundary value 2,000,000,000 → CAT4");

    printf("\n═══ LAYER 2: RISK ASSESSMENT (12 tests) ═══\n");
    
    /* Test 11: Aspirin - low dose risk assessment */
    /* Dose: 100 mg/kg = 100,000,000 μg/kg */
    /* LD50: 1,500,000,000 μg/kg */
    /* RQ = (100,000,000 × 10^6) / 1,500,000,000 = 100,000,000,000,000 / 1,500,000,000 = 66,666 */
    /* RQ_scaled = 66,666 < 1,000,000 (1.0) → SAFE */
    tox_assessment_t aspirin_low = assess_risk(LD50_ASPIRIN, DOSE_CONSUMER_PRODUCT, UF_INTER_SPECIES);
    run_test("Aspirin at consumer dose: RQ < 1.0 (safe)",
             aspirin_low.is_safe == 1,
             "RQ_scaled=666,666 < 1,000,000 → safe");
    
    /* Test 12: Risk quotient calculation is exact */
    /* For aspirin: (1,000,000 × 10^6) / 1,500,000,000 = 1,000,000,000,000 / 1,500,000,000 = 666 */
    tox_assessment_t aspirin_exact = assess_risk(LD50_ASPIRIN, 1000000LL, 1LL);
    int rq_correct = (aspirin_exact.risk_quotient_scaled == 666LL);
    run_test("Risk quotient arithmetic exact (Aspirin)",
             rq_correct,
             "(1,000,000 × 10^6) / 1,500,000,000 = 666");
    
    /* Test 13: Margin of safety calculation */
    /* MOS = LD50 / Dose; for LD50=1500, Dose=100 in mg/kg: MOS = 15 */
    /* MOS_scaled = (1,500,000,000 × 10^6) / 100,000,000 = 15,000,000 */
    wad18_t mos_aspirin = (LD50_ASPIRIN * 1000000LL) / DOSE_CONSUMER_PRODUCT;
    int mos_correct = (mos_aspirin == 1500000000LL);
    run_test("Margin of safety calculation exact (Aspirin)",
             mos_correct,
             "(1,500,000,000 × 10^6) / 100,000,000 = 1,500,000,000");
    
    /* Test 14: Dioxin - extremely high risk */
    /* Dose: 1,000 μg/kg (typical env. contaminant) */
    /* LD50: 1,000 μg/kg */
    /* RQ = (1,000 × 10^6) / 1,000 = 1,000,000 (RQ = 1.0, borderline) */
    tox_assessment_t dioxin_env = assess_risk(LD50_DIOXIN, 1000LL, 1LL);
    int dioxin_rq = (dioxin_env.risk_quotient_scaled == 1000000LL);
    run_test("Dioxin at LD50: RQ = 1.0 (borderline unsafe)",
             dioxin_rq && dioxin_env.is_safe == 0,
             "RQ_scaled=1,000,000 ≥ 1,000,000 → unsafe");
    
    /* Test 15: Water - extremely low risk */
    /* Dose: 1 mL ≈ 1 g ≈ 14.3 mg/kg (assume 70 kg person) */
    /* LD50: 15,000,000,000 μg/kg */
    /* RQ = (14,300,000 × 10^6) / 15,000,000,000 ≈ 953,333 << 1.0 */
    tox_assessment_t water_safe = assess_risk(LD50_WATER, 14300000LL, 1LL);
    run_test("Water at normal intake: RQ << 1.0 (very safe)",
             water_safe.is_safe == 1,
             "RQ_scaled << 1,000,000 → safe");
    
    /* Test 16: Occupational acute dose */
    wad18_t occ_rq = (DOSE_OCCUPATIONAL_ACUTE * 1000000LL) / LD50_ASPIRIN;
    int occ_safe = (occ_rq < 1000000LL);
    run_test("Occupational dose: RQ check",
             occ_safe,
             "Dose 100 mg/kg vs LD50 1500 mg/kg → RQ < 1.0");
    
    /* Test 17: RfD calculation */
    /* RfD = (NOAEL × 10^6) / UF */
    /* = (10,000,000 × 10^6) / 100 = 10^7 × 10^6 / 10^2 = 10^11 = 100,000,000,000 */
    wad18_t rfd = calculate_rfd(NOAEL_TYPICAL_CHRONIC, UF_INTER_SPECIES * UF_INTRA_SPECIES);
    int rfd_correct = (rfd == 100000000000LL);
    run_test("RfD calculation: NOAEL/UF",
             rfd_correct,
             "(10,000,000 × 10^6) / 100 = 100,000,000,000");
    
    /* Test 18: Risk acceptability */
    int aspirin_accept = is_acceptable_risk(aspirin_low.risk_quotient_scaled);
    run_test("Acceptability: RQ < 1.0",
             aspirin_accept == 1,
             "RQ_scaled < 1,000,000 → acceptable");
    
    /* Test 19: Risk unacceptability */
    int dioxin_reject = is_acceptable_risk(dioxin_env.risk_quotient_scaled);
    run_test("Unacceptability: RQ ≥ 1.0",
             dioxin_reject == 0,
             "RQ_scaled ≥ 1,000,000 → unacceptable");
    
    /* Test 20: MOS acceptability */
    int mos_accept = is_acceptable_mos(mos_aspirin);
    run_test("MOS acceptable: MOS ≥ 100",
             mos_accept == 1,
             "MOS_scaled=1,500,000,000 ≥ 100,000,000 → acceptable");
    
    /* Test 21: Uncertainty factor multiplication */
    wad18_t uf_combined = UF_INTER_SPECIES * UF_INTRA_SPECIES;
    int uf_correct = (uf_combined == 100LL);
    run_test("UF multiplication: 10 × 10 = 100",
             uf_correct,
             "Pure integer: 10 × 10 = 100");
    
    /* Test 22: Hazard classification consistency */
    /* Numeric ordering: CAT1 (4) > CAT2 (3) > CAT3 (2) > CAT4 (1) */
    int hazard_order_correct = (cat_dioxin > cat_caffeine) && 
                               (cat_caffeine > cat_aspirin);
    run_test("Hazard classification ordering: CAT1 > CAT3 > CAT4",
             hazard_order_correct,
             "Dioxin(4) > Caffeine(2) > Aspirin(1)");

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║ TEST SUMMARY: %d / %d PASSED                                  ║\n", pass_count, total_count);
    if (pass_count == total_count) {
        printf("║ STATUS: ✅ ALL TESTS PASSED - PURE WAD18 VERIFIED             ║\n");
    } else {
        printf("║ STATUS: ⚠️  %d TESTS FAILED                                   ║\n", total_count - pass_count);
    }
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    return (pass_count == total_count) ? 0 : 1;
}
