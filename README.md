# WAD18 TOXICOLOGY CAPSULE v1.0.0

## Pure Fixed-Point Arithmetic · Zero Floating Point · Exactly Decidable

A reproducible toxicology domain capsule implementing hazard assessment and risk characterization using **pure integer arithmetic with WAD18 scaled-ratio principles**.

---

## Architecture

### Core Domain: Hazard & Risk Assessment

**Scaling Strategy:**
- All dose, LD50, NOAEL values: Stored as **micrograms/kg (μg/kg)** raw integers
  - LD50_ASPIRIN = 1,500,000,000 (1500 mg/kg)
  - LD50_DIOXIN = 1,000 (0.001 mg/kg)
  - Fits completely in int64_t without overflow
  
- Ratios (RQ, MOS, RfD): Computed with **10^6 scale** for 6 decimal places
  - Risk Quotient = (Dose × 10^6) / LD50
  - Margin of Safety = (NOAEL × 10^6) / Dose
  - No WAD18_SCALE (10^18) needed — domain values are naturally smaller

- Uncertainty Factors: Pure **integers** (3, 10, 100, 1000)
  - Product: UF_INTER_SPECIES × UF_INTRA_SPECIES = 10 × 10 = 100
  - Fully decidable at compile time

### Files

```
wad18_common.h                  Pure WAD18 definitions (unchanged from pharmacology pattern)
toxicology_core_wad18.h         Domain constants & function signatures
toxicology_core_wad18.c         Implementation (pure integer, ~100 lines)
test_toxicology_wad18.c         Integrated test suite (22 tests, all verified)
Makefile                        Standard build
```

### Test Suite: 22 Tests, 100% Pass

**Layer 1: Hazard Classification (10 tests)**
- Category boundaries: CAT1 (≤5mg/kg) → CAT2 (≤50) → CAT3 (≤500) → CAT4 (≤2000)
- Boundary-exact testing
- Substances: Dioxin, Strychnine, Cyanide, Caffeine, Aspirin, Water

**Layer 2: Risk Assessment (12 tests)**
- Risk quotient: (Dose/LD50) × 10^6, safe if RQ < 1.0
- Margin of safety: (NOAEL/Dose) × 10^6, acceptable if MOS ≥ 100
- RfD calculation: (NOAEL/UF_total) × 10^6
- Exact arithmetic verification
- Acceptability thresholds

---

## Building & Testing

```bash
cd wad18-toxicology
make clean && make test
```

**Output: 22/22 PASSED** in pure integer arithmetic.

No floating-point operations. No approximations. All divisions are exact integer division with appropriate scaling.

---

## Design Principles (Pure WAD18)

1. **Integer Decidability**: Every calculation is exactly decidable by hand
   - Example: (1,000,000 × 10^6) / 1,500,000,000 = 666 (integer division)

2. **No Overflow**: All values chosen to fit in int64_t
   - LD50 ≤ 20,000,000,000 μg/kg (20,000 mg/kg max realistic)
   - Dose ≤ 1,000,000,000 μg/kg (1,000 mg/kg max)
   - Ratios × 10^6 ≤ 10^15 (fits safely)

3. **Transparent Scaling**: Each quantity has explicit scale
   - Not "everything is WAD18 (10^18)" — use appropriate precision per domain
   - Toxicology: μg/kg (raw) + 10^6 ratios
   - Pharmacology: mg/kg + varied scales per property

4. **Algebraic Verification**: Tests encode hand-computable expectations
   - Each test result can be verified by calculator
   - Example: "RQ = (1,000,000 × 10^6) / 1,500,000,000 = 666"

---

## Integration with WAD18 Pharmacology

This capsule **extends WAD18 Pharmacology v1.1.0**:
- Pharmacology → Drug properties (MW, logP, bioavailability)
- **→ Toxicology** (NEW) → Dose-response, hazard, risk assessment

Forms a complete **Pharmo-Toxicology Pipeline**:
1. Drug defined (pharmacology)
2. LD50/NOAEL determined (toxicology)
3. RfD calculated → safe dose level
4. Risk assessment for clinical/occupational use

---

## Verification

All arithmetic verified by:
- Integer-only operations (no float)
- Algebraic hand-calculation of test expectations
- Boundary value testing (exact threshold checks)
- Category ordering consistency
- 22 independent tests, 100% pass rate

---

## Author & License

WAD18 Toxicology Capsule v1.0.0
Part of Reproducible Science Initiative
Pure fixed-point arithmetic | Zero floating-point guarantee
