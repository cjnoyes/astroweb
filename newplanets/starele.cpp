#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include "kep.h"


struct star fixedstarelements[NOFIXEDSTAR] = {
{ "alAnd(Alpheratz)", 2451545.0, 0.036598, 0.507724, 0.000000, 0.000076, -0.000079, -12.00, 0.0, 2.060000e+000 },
{ "alPhe(Ankaa)", 2451545.0, 0.114685, 0.738380, 0.000000, 0.000133, -0.000192, 75.00, 0.0, 2.390000e+000 },
{ "alCas(Schedar)", 2451545.0, 0.176747, 0.986763, 0.000000, 0.000046, -0.000015, -4.00, 0.0, 2.230000e+000 },
{ "beCet(Diphda)", 2451545.0, 0.190195, 0.313926, 0.000000, 0.000119, 0.000016, 13.00, 0.0, 2.040000e+000 },
{ "alEri(Achernar)", 2451545.0, 0.426359, 0.998969, 0.000000, 0.000085, -0.000017, 16.00, 0.0, 4.600000e-001 },
{ "alAri(Hamal)", 2451545.0, 0.554899, 0.409496, 0.000000, 0.000101, -0.000072, -14.00, 0.0, 2.000000e+000 },
{ "th-1Eri(Acamar)", 2451545.0, 0.777813, 0.703450, 0.000000, -0.000028, 0.000009, 12.00, 0.0, 3.420000e+000 },
{ "alCet(Menkar)", 2451545.0, 0.795345, 0.071379, 0.000000, -0.000005, -0.000038, -26.00, 0.0, 2.530000e+000 },
{ "alPer(Mirfak)", 2451545.0, 0.891526, 0.870242, 0.000000, 0.000018, -0.000012, -2.00, 0.0, 1.800000e+000 },
{ "alTau(Aldebara)", 2451545.0, 1.203931, 0.288141, 0.000000, 0.000032, -0.000092, 54.00, 0.0, 8.500000e-001 },
{ "beOri(Rigel)", 2451545.0, 1.372430, 0.143146, 0.000000, 0.000000, -0.000001, 21.00, 0.0, 1.200000e-001 },
{ "alAur(Capella)", 2451545.0, 1.381817, 0.802817, 0.000000, 0.000053, -0.000206, 30.00, 0.0, 8.000000e-002 },
{ "gaOri(Bellatrix)", 2451545.0, 1.418651, 0.110822, 0.000000, -0.000004, -0.000007, 18.00, 0.0, 1.640000e+000 },
{ "beTau(Elnath)", 2451545.0, 1.423717, 0.499293, 0.000000, 0.000012, -0.000085, 9.00, 0.0, 1.650000e+000 },
{ "epOri(Alnilam)", 2451545.0, 1.467008, 0.020978, 0.000000, 0.000000, -0.000001, 26.00, 0.0, 1.700000e+000 },
{ "alOri(Betelgeuse)", 2451545.0, 1.549729, 0.129277, 0.000000, 0.000013, 0.000004, 21.00, 0.0, 5.000000e-001 },
{ "alCar(Canopus)", 2451545.0, 1.675307, 0.919713, 0.000000, 0.000018, 0.000010, 21.00, 0.0, -7.200000e-001 },
{ "alCMa(Sirius)", 2451545.0, 1.767791, 0.291751, 0.000002, -0.000280, -0.000584, -7.60, 0.0, -1.460000e+000 },
{ "epCMa(Adhara)", 2451545.0, 1.826600, 0.505658, 0.000000, 0.000002, 0.000001, 27.00, 0.0, 1.500000e+000 },
{ "alCMi(Procyon)", 2451545.0, 2.004083, 0.091194, 0.000001, -0.000346, -0.000496, -3.00, 0.0, 3.800000e-001 },
{ "beGem(Pollux)", 2451545.0, 2.030323, 0.489149, 0.000000, -0.000345, -0.000022, 3.00, 0.0, 1.140000e+000 },
{ "epCar(Avior)", 2451545.0, 2.192630, 1.038638, 0.000000, -0.000025, 0.000007, 2.00, 0.0, 1.860000e+000 },
{ "laVel(Suhail)", 2451545.0, 2.391085, 0.758042, 0.000000, -0.000013, 0.000006, 18.00, 0.0, 2.210000e+000 },
{ "beCar(Miaplacidus)", 2451545.0, 2.413787, 1.216795, 0.000000, -0.000226, 0.000052, -5.00, 0.0, 1.680000e+000 },
{ "alHya(Alphard)", 2451545.0, 2.476567, 0.151122, 0.000000, -0.000007, 0.000016, -4.00, 0.0, 1.980000e+000 },
{ "alLeo(Regulus)", 2451545.0, 2.654523, 0.208867, 0.000000, -0.000123, 0.000003, 6.00, 0.0, 1.350000e+000 },
{ "alUMa(Dubhe)", 2451545.0, 2.896059, 1.077756, 0.000000, -0.000122, -0.000032, -9.00, 0.0, 1.790000e+000 },
{ "beLeo(Denebola)", 2451545.0, 3.093856, 0.254330, 0.000000, -0.000249, -0.000055, 0.00, 0.0, 2.140000e+000 },
{ "gaCrv(Gienah)", 2451545.0, 3.210560, 0.306165, 0.000000, -0.000082, 0.000011, -4.00, 0.0, 2.590000e+000 },
{ "al-1Cru(Acrux)", 2451545.0, 3.257648, 1.101286, 0.000000, -0.000038, -0.000006, -11.00, 0.0, 1.580000e+000 },
{ "gaCru(Gacrux)", 2451545.0, 3.277578, 0.996813, 0.000000, 0.000021, -0.000127, 21.00, 0.0, 1.630000e+000 },
{ "epUMa(Alioth)", 2451545.0, 3.377339, 0.976684, 0.000000, 0.000097, -0.000003, -9.00, 0.0, 1.770000e+000 },
{ "alVir(Spica)", 2451545.0, 3.513318, 0.194802, 0.000000, -0.000020, -0.000014, 1.00, 0.0, 9.700000e-001 },
{ "etUMa(Alkaid)", 2451545.0, 3.610827, 0.860680, 0.000000, -0.000091, -0.000005, -11.00, 0.0, 1.860000e+000 },
{ "beCen(Hadar)", 2451545.0, 3.681874, 1.053708, 0.000000, -0.000031, -0.000009, 6.00, 0.0, 6.100000e-001 },
{ "thCen(Menkent)", 2451545.0, 3.694349, 0.634776, 0.000000, -0.000312, -0.000252, 1.00, 0.0, 2.060000e+000 },
{ "alBoo(Arcturus)", 2451545.0, 3.733527, 0.334796, 0.000000, -0.000561, -0.000969, -5.00, 0.0, -4.000000e-002 },
{ "alCen(Rigil)", 2451545.0, 3.837971, 1.061778, 0.000004, -0.003623, 0.000339, -22.20, 0.0, -1.000000e-002 },
{ "al-2Lib(Zubenelgen)", 2451545.0, 3.887191, 0.279982, 0.000000, -0.000053, -0.000032, -10.00, 0.0, 2.750000e+000 },
{ "beUMi(Kochab)", 2451545.0, 3.886437, 1.294258, 0.000000, -0.000055, 0.000006, 17.00, 0.0, 2.080000e+000 },
{ "alCrB(Alphecca)", 2451545.0, 4.078345, 0.466260, 0.000000, 0.000066, -0.000043, 2.00, 0.0, 2.230000e+000 },
{ "alSco(Antares)", 2451545.0, 4.317104, 0.461325, 0.000000, -0.000005, -0.000010, -3.00, 0.0, 9.600000e-001 },
{ "alTrA(Atria)", 2451545.0, 4.401129, 1.204761, 0.000000, 0.000019, -0.000016, -3.00, 0.0, 1.920000e+000 },
{ "etOph(Sabik)", 2451545.0, 4.495872, 0.274452, 0.000000, 0.000019, 0.000046, -1.00, 0.0, 2.430000e+000 },
{ "laSco(Shaula)", 2451545.0, 4.597236, 0.647584, 0.000000, -0.000001, -0.000014, -3.00, 0.0, 1.630000e+000 },
{ "alOph(Rasalhague)", 2451545.0, 4.603021, 0.219214, 0.000000, 0.000060, -0.000110, 13.00, 0.0, 2.080000e+000 },
{ "gaDra(Eltanin)", 2451545.0, 4.697580, 0.898652, 0.000000, -0.000006, -0.000009, -28.00, 0.0, 2.230000e+000 },
{ "epSgr(Kaus_Aust.)", 2451545.0, 4.817860, 0.600125, 0.000000, -0.000022, -0.000060, -15.00, 0.0, 1.850000e+000 },
{ "alLyr(Vega)", 2451545.0, 4.873565, 0.676903, 0.000001, 0.000126, 0.000139, -14.00, 0.0, 3.000000e-002 },
{ "siSgr(Nunki)", 2451545.0, 4.953530, 0.458965, 0.000000, 0.000007, -0.000026, -11.00, 0.0, 2.020000e+000 },
{ "alAql(Altair)", 2451545.0, 5.195773, 0.154782, 0.000001, 0.000264, 0.000187, -26.30, 0.0, 7.700000e-001 },
{ "alPav(Peacock)", 2451545.0, 5.347896, 0.990214, 0.000000, 0.000006, -0.000043, 2.00, 0.0, 1.940000e+000 },
{ "alCyg(Deneb)", 2451545.0, 5.416769, 0.790291, 0.000000, 0.000002, 0.000001, -5.00, 0.0, 1.250000e+000 },
{ "epPeg(Enif)", 2451545.0, 5.690585, 0.172351, 0.000000, 0.000015, -0.000000, 5.00, 0.0, 2.390000e+000 },
{ "alGru(Al_na'ir)", 2451545.0, 5.795511, 0.819624, 0.000000, 0.000092, -0.000073, 12.00, 0.0, 1.740000e+000 },
{ "alPsA(Fomalhaut)", 2451545.0, 6.011136, 0.517006, 0.000001, 0.000186, -0.000080, 7.00, 0.0, 1.160000e+000 },
{ "alPeg(Markab)", 2451545.0, 6.042160, 0.265382, 0.000000, 0.000032, -0.000021, -4.00, 0.0, 2.490000e+000 },
{ "alUMi(Polaris)", 2451545.0, 0.662404, 1.557952, 0.000000, 0.001445, -0.000007, -17.00, 0.0, 2.020000e+000 }
};

