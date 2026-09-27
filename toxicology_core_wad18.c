#include "toxicology_core_wad18.h"

/* ═══════════════════════════════════════════════════════════════════════════
   Pure integer arithmetic - NO floating point operations
   All divisions are integer division, all results are exact integers
   ═══════════════════════════════════════════════════════════════════════════ */

hazard_category_t classify_hazard_by_ld50(wad18_t ld50_micrograms_kg) {
    /* Classification boundaries in micrograms/kg */
    /* CAT1: LD50 ≤ 5 mg/kg = 5,000,000 μg/kg */
    if (ld50_micrograms_kg <= 5000000LL) {
        return HAZARD_CAT1;
    }
    /* CAT2: LD50 ≤ 50 mg/kg = 50,000,000 μg/kg */
    else if (ld50_micrograms_kg <= 50000000LL) {
        return HAZARD_CAT2;
    }
    /* CAT3: LD50 ≤ 500 mg/kg = 500,000,000 μg/kg */
    else if (ld50_micrograms_kg <= 500000000LL) {
        return HAZARD_CAT3;
    }
    /* CAT4: LD50 ≤ 2000 mg/kg = 2,000,000,000 μg/kg */
    else if (ld50_micrograms_kg <= 2000000000LL) {
        return HAZARD_CAT4;
    }
    
    return HAZARD_NONE;
}

tox_assessment_t assess_risk(wad18_t ld50_ug_kg, wad18_t dose_ug_kg, wad18_t uf_total __attribute__((unused))) {
    tox_assessment_t result;
    
    result.ld50_micrograms_kg = ld50_ug_kg;
    result.dose_micrograms_kg = dose_ug_kg;
    result.hazard_class = classify_hazard_by_ld50(ld50_ug_kg);
    
    /* Risk quotient = (Dose / LD50) × 10^6 */
    /* Integer division: (Dose × 10^6) / LD50 */
    if (ld50_ug_kg > 0) {
        result.risk_quotient_scaled = (dose_ug_kg * 1000000LL) / ld50_ug_kg;
    } else {
        result.risk_quotient_scaled = 0;  /* Invalid LD50 */
    }
    
    /* Margin of safety = (LD50 / Dose) × 10^6 */
    /* Integer division: (LD50 × 10^6) / Dose */
    if (dose_ug_kg > 0) {
        result.margin_of_safety_scaled = (ld50_ug_kg * 1000000LL) / dose_ug_kg;
    } else {
        result.margin_of_safety_scaled = 0;  /* Invalid dose */
    }
    
    /* Safe if RQ < 1.0 (i.e., RQ_scaled < 10^6) */
    result.is_safe = (result.risk_quotient_scaled < 1000000LL) ? 1 : 0;
    
    return result;
}

wad18_t calculate_rfd(wad18_t noael_ug_kg, wad18_t uf_total) {
    /* RfD = NOAEL / UF (both as integers) */
    /* Result: (NOAEL × 10^6) / UF gives fractional precision */
    if (uf_total <= 0) return 0;
    
    return (noael_ug_kg * 1000000LL) / uf_total;
}

int is_acceptable_risk(wad18_t risk_quotient_scaled) {
    /* RQ < 1.0 means acceptable risk */
    /* RQ_scaled < 10^6 (since scale is 10^6) */
    return (risk_quotient_scaled < 1000000LL) ? 1 : 0;
}

int is_acceptable_mos(wad18_t mos_scaled) {
    /* MOS ≥ 100 means acceptable risk */
    /* MOS_scaled ≥ 100 × 10^6 */
    return (mos_scaled >= 100000000LL) ? 1 : 0;
}
