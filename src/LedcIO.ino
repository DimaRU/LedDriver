////
///
//

/**
* 12 bits PWM to CIE Luminance conversion
* L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
* L* = 903.3(Y/Yn), Y/Yn <= 0.008856
*/
//lookup table for 256 CIE lab brightness corrected values with 12 bit resolution
/*
const uint16_t CIEL_8_12[] = {
  0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 18, 20, 21, 23, 25, 27, 28, 30, 32, 34, 36, 37, 39, 41, 43, 45, 47, 49, 52, 54, 56, 59, 61, 64, 66, 69, 72, 75, 77, 80, 83, 87, 90, 93, 97, 100, 103, 107, 111, 115, 118, 122, 126,
  131, 135, 139, 144, 148, 153, 157, 162, 167, 172, 177, 182, 187, 193, 198, 204, 209, 215, 221, 227, 233, 239, 246, 252, 259, 265, 272, 279, 286, 293, 300, 308, 315, 323, 330, 338, 346, 354, 362, 371,
  379, 388, 396, 405, 414, 423, 432, 442, 451, 461, 471, 480, 490, 501, 511, 521, 532, 543, 554, 565, 576, 587, 599, 610, 622, 634, 646, 658, 670, 683, 696, 708, 721, 734, 748, 761, 775, 789, 802, 817,
  831, 845, 860, 875, 890, 905, 920, 935, 951, 967, 983, 999, 1015, 1032, 1048, 1065, 1082, 1099, 1117, 1134, 1152, 1170, 1188, 1206, 1225, 1243, 1262, 1281, 1301, 1320, 1340, 1359, 1379, 1400, 1420,
  1441, 1461, 1482, 1504, 1525, 1547, 1568, 1590, 1613, 1635, 1658, 1681, 1704, 1727, 1750, 1774, 1798, 1822, 1846, 1871, 1896, 1921, 1946, 1971, 1997, 2023, 2049, 2075, 2101, 2128, 2155, 2182, 2210,
  2237, 2265, 2293, 2322, 2350, 2379, 2408, 2437, 2467, 2497, 2527, 2557, 2587, 2618, 2649, 2680, 2712, 2743, 2775, 2807, 2840, 2872, 2905, 2938, 2972, 3006, 3039, 3074, 3108, 3143, 3178, 3213, 3248,
  3284, 3320, 3356, 3393, 3430, 3467, 3504, 3542, 3579, 3617, 3656, 3694, 3733, 3773, 3812, 3852, 3892, 3932, 3973, 4013, 4055, 4095 };
*/

//lookup table for 1024 CIE lab brightness corrected values with 12 bit resolution
const uint16_t CIEL_10_12[] = {
  0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 21,
  21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 39, 40,
  40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 54, 54, 55, 55, 56, 56, 57, 58, 58, 59, 59, 60, 61, 61, 62, 63, 63, 64,
  65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 78, 79, 80, 81, 81, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96,
  97, 98, 98, 99, 100, 101, 102, 103, 104, 105, 106, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
  130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 143, 144, 145, 146, 147, 148, 149, 150, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162, 163, 165, 166, 167,
  168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 183, 185, 186, 187, 189, 190, 191, 193, 194, 195, 197, 198, 200, 201, 202, 204, 205, 206, 208, 209, 211, 212,
  214, 215, 217, 218, 219, 221, 222, 224, 225, 227, 228, 230, 231, 233, 234, 236, 238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255, 257, 258, 260, 262, 263, 265,
  267, 268, 270, 272, 273, 275, 277, 278, 280, 282, 284, 285, 287, 289, 291, 292, 294, 296, 298, 300, 301, 303, 305, 307, 309, 311, 313, 314, 316, 318, 320, 322, 324, 326,
  328, 330, 331, 333, 335, 337, 339, 341, 343, 345, 347, 349, 351, 353, 355, 357, 359, 361, 363, 365, 368, 370, 372, 374, 376, 378, 380, 382, 384, 387, 389, 391, 393, 395,
  397, 400, 402, 404, 406, 408, 411, 413, 415, 417, 420, 422, 424, 426, 429, 431, 433, 436, 438, 440, 443, 445, 447, 450, 452, 454, 457, 459, 462, 464, 467, 469, 471, 474,
  476, 479, 481, 484, 486, 489, 491, 494, 496, 499, 501, 504, 507, 509, 512, 514, 517, 519, 522, 525, 527, 530, 533, 535, 538, 541, 543, 546, 549, 551, 554, 557, 560, 562,
  565, 568, 571, 573, 576, 579, 582, 585, 588, 590, 593, 596, 599, 602, 605, 608, 610, 613, 616, 619, 622, 625, 628, 631, 634, 637, 640, 643, 646, 649, 652, 655, 658, 661,
  664, 667, 671, 674, 677, 680, 683, 686, 689, 692, 696, 699, 702, 705, 708, 712, 715, 718, 721, 725, 728, 731, 734, 738, 741, 744, 748, 751, 754, 758, 761, 764, 768, 771,
  775, 778, 781, 785, 788, 792, 795, 799, 802, 806, 809, 813, 816, 820, 823, 827, 830, 834, 837, 841, 845, 848, 852, 856, 859, 863, 867, 870, 874, 878, 881, 885, 889, 892,
  896, 900, 904, 908, 911, 915, 919, 923, 927, 930, 934, 938, 942, 946, 950, 954, 958, 962, 966, 970, 973, 977, 981, 985, 989, 993, 997, 1002, 1006, 1010, 1014, 1018, 1022,
  1026, 1030, 1034, 1038, 1043, 1047, 1051, 1055, 1059, 1063, 1068, 1072, 1076, 1080, 1085, 1089, 1093, 1097, 1102, 1106, 1110, 1115, 1119, 1123, 1128, 1132, 1137, 1141, 1145,
  1150, 1154, 1159, 1163, 1168, 1172, 1177, 1181, 1186, 1190, 1195, 1199, 1204, 1208, 1213, 1218, 1222, 1227, 1232, 1236, 1241, 1246, 1250, 1255, 1260, 1264, 1269, 1274, 1279,
  1283, 1288, 1293, 1298, 1303, 1307, 1312, 1317, 1322, 1327, 1332, 1337, 1341, 1346, 1351, 1356, 1361, 1366, 1371, 1376, 1381, 1386, 1391, 1396, 1401, 1406, 1411, 1417, 1422,
  1427, 1432, 1437, 1442, 1447, 1453, 1458, 1463, 1468, 1473, 1479, 1484, 1489, 1494, 1500, 1505, 1510, 1516, 1521, 1526, 1532, 1537, 1542, 1548, 1553, 1559, 1564, 1570, 1575,
  1580, 1586, 1591, 1597, 1603, 1608, 1614, 1619, 1625, 1630, 1636, 1642, 1647, 1653, 1658, 1664, 1670, 1676, 1681, 1687, 1693, 1698, 1704, 1710, 1716, 1722, 1727, 1733, 1739,
  1745, 1751, 1757, 1762, 1768, 1774, 1780, 1786, 1792, 1798, 1804, 1810, 1816, 1822, 1828, 1834, 1840, 1846, 1852, 1859, 1865, 1871, 1877, 1883, 1889, 1895, 1902, 1908, 1914,
  1920, 1927, 1933, 1939, 1945, 1952, 1958, 1964, 1971, 1977, 1983, 1990, 1996, 2003, 2009, 2015, 2022, 2028, 2035, 2041, 2048, 2054, 2061, 2067, 2074, 2081, 2087, 2094, 2100,
  2107, 2114, 2120, 2127, 2134, 2140, 2147, 2154, 2161, 2167, 2174, 2181, 2188, 2195, 2201, 2208, 2215, 2222, 2229, 2236, 2243, 2250, 2257, 2263, 2270, 2277, 2284, 2291, 2299,
  2306, 2313, 2320, 2327, 2334, 2341, 2348, 2355, 2362, 2370, 2377, 2384, 2391, 2398, 2406, 2413, 2420, 2428, 2435, 2442, 2449, 2457, 2464, 2472, 2479, 2486, 2494, 2501, 2509,
  2516, 2524, 2531, 2539, 2546, 2554, 2561, 2569, 2576, 2584, 2592, 2599, 2607, 2615, 2622, 2630, 2638, 2645, 2653, 2661, 2669, 2676, 2684, 2692, 2700, 2708, 2716, 2723, 2731,
  2739, 2747, 2755, 2763, 2771, 2779, 2787, 2795, 2803, 2811, 2819, 2827, 2835, 2843, 2851, 2860, 2868, 2876, 2884, 2892, 2900, 2909, 2917, 2925, 2933, 2942, 2950, 2958, 2967,
  2975, 2983, 2992, 3000, 3009, 3017, 3026, 3034, 3042, 3051, 3059, 3068, 3077, 3085, 3094, 3102, 3111, 3119, 3128, 3137, 3145, 3154, 3163, 3171, 3180, 3189, 3198, 3207, 3215,
  3224, 3233, 3242, 3251, 3260, 3268, 3277, 3286, 3295, 3304, 3313, 3322, 3331, 3340, 3349, 3358, 3367, 3376, 3385, 3395, 3404, 3413, 3422, 3431, 3440, 3450, 3459, 3468, 3477,
  3487, 3496, 3505, 3515, 3524, 3533, 3543, 3552, 3562, 3571, 3580, 3590, 3599, 3609, 3618, 3628, 3637, 3647, 3657, 3666, 3676, 3685, 3695, 3705, 3714, 3724, 3734, 3744, 3753,
  3763, 3773, 3783, 3793, 3802, 3812, 3822, 3832, 3842, 3852, 3862, 3872, 3882, 3892, 3902, 3912, 3922, 3932, 3942, 3952, 3962, 3972, 3982, 3992, 4003, 4013, 4023, 4033, 4044,
  4054, 4064, 4074, 4085, 4095 };



void ledSetup() {
   pinMode(NightLedPin, OUTPUT);

 // Setup timer and attach timer to a led pin
  ledcSetup(YellowChannel, LEDC_FREQUENCY, LEDC_RESOLUTION);
  ledcAttachPin(YellowLedPin, YellowChannel);

  ledcSetup(WhiteChannel, LEDC_FREQUENCY, LEDC_RESOLUTION);
  ledcAttachPin(WhiteLedPin, WhiteChannel);
}

void setBrightness(int bright, int channel) {
  const int duty = CIEL_10_12[bright];
  ledcWrite(channel, duty);
}

void ledsOff() {
  setBrightness(0, YellowChannel);
  setBrightness(0, WhiteChannel);
}

void ledsRestore() {
  setBrightness(yellowSlider, YellowChannel);
  setBrightness(whiteSlider, WhiteChannel);
}
