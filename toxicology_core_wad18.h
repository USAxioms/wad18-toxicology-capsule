#ifndef TOXICOLOGY_CORE_WAD18_H
#define TOXICOLOGY_CORE_WAD18_H

#include "wad18_common.h"

/* ═══════════════════════════════════════════════════════════════════════════
   TOXICOLOGY DOMAIN: Pure WAD18 Fixed-Point Arithmetic
   
   All values stored as micrograms/kg (μg/kg) raw integers to avoid overflow.
   Ratios computed with 10^6 scale for 6 decimal places of precision.
   
   PURE INTEGER - ZERO FLOATING POINT - EXACTLY DECIDABLE BY INSPECTION
   ═══════════════════════════════════════════════════════════════════════════ */

/* Substances: LD50 values in micrograms/kg (mg/kg × 10^6) */
#define LD50_DIOXIN                 1000LL              /* 0.001 mg/kg */
#define LD50_STRYCHNINE             2000000LL           /* 2 mg/kg */
#define LD50_WARFARIN               12000000LL          /* 12 mg/kg */
#define LD50_CYANIDE                3600000LL           /* 3.6 mg/kg */
#define LD50_CAFFEINE               190000000LL         /* 190 mg/kg */
#define LD50_ASPIRIN                1500000000LL        /* 1500 mg/kg */
#define LD50_SALT                   3000000000LL        /* 3000 mg/kg */
#define LD50_WATER                  15000000000LL       /* 15000 mg/kg (lethal dose) */

/* NOAEL values in micrograms/kg/day */
#define NOAEL_TYPICAL_CHRONIC       10000000LL          /* 10 mg/kg/day */
#define NOAEL_TYPICAL_REPRODUCTIVE  50000000LL          /* 50 mg/kg/day */
#define NOAEL_TYPICAL_DEVELOPMENTAL 100000000LL         /* 100 mg/kg/day */

/* Uncertainty Factors (unitless, as pure integers) */
#define UF_INTER_SPECIES            10LL                /* 10× */
#define UF_INTRA_SPECIES            10LL                /* 10× */
#define UF_SUBCHRONIC_CHRONIC       10LL                /* 10× */
#define UF_LOAEL_TO_NOAEL           3LL                 /* 3× */

/* Exposure doses in micrograms/kg */
#define DOSE_OCCUPATIONAL_ACUTE     100000000LL         /* 100 mg/kg (hypothetical max) */
#define DOSE_ENVIRONMENTAL_CHRONIC  10000LL             /* 0.01 mg/kg/day (typical) */
#define DOSE_CONSUMER_PRODUCT       1000000LL           /* 1 mg/kg (typical) */

/* Hazard classification (raw integer levels, not scaled) */
typedef enum {
    HAZARD_NONE = 0,
    HAZARD_CAT4 = 1,               /* Harmful: 300-2000 mg/kg */
    HAZARD_CAT3 = 2,               /* Toxic: 50-300 mg/kg */
    HAZARD_CAT2 = 3,               /* Very Toxic: 5-50 mg/kg */
    HAZARD_CAT1 = 4                /* Extremely Toxic: <5 mg/kg */
} hazard_category_t;

/* Assessment result (all ratios stored as integers × 10^6) */
typedef struct {
    wad18_t ld50_micrograms_kg;             /* LD50 in μg/kg */
    wad18_t dose_micrograms_kg;             /* Dose in μg/kg */
    wad18_t risk_quotient_scaled;           /* RQ × 10^6 */
    wad18_t margin_of_safety_scaled;        /* MOS × 10^6 */
    hazard_category_t hazard_class;
    int is_safe;                            /* 1 if RQ < 1.0, else 0 */
} tox_assessment_t;

/* Pure integer function declarations */
hazard_category_t classify_hazard_by_ld50(wad18_t ld50_micrograms_kg);
tox_assessment_t assess_risk(wad18_t ld50_ug_kg, wad18_t dose_ug_kg, wad18_t uf_total);
wad18_t calculate_rfd(wad18_t noael_ug_kg, wad18_t uf_total);
int is_acceptable_risk(wad18_t risk_quotient_scaled);
int is_acceptable_mos(wad18_t mos_scaled);

#endif
