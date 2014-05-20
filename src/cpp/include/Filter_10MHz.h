/**
*Filter name: Filter_10MHz
*
*@file Filter_10MHz.h
*@abstract
*
*@copyright (c)2007 Rohde & Schwarz GmbH & Co. KG, Munich
*@author Markus Herdin, Johannes Dommel
*@version
*
*/

#ifndef _Filter_10MHz_H
#define _Filter_10MHz_H

#include "TSMWIQInterfaceTypes.h"
//Filter parameter
TSMW_IQIF_FILTER_PARAM_t  Filter_10MHzParam = {
       1 ,               //Number of filter
    2112 ,               // Number of filter coefficients
      32 ,               // Filter coefficient oversampling factor
   -1.60 ,               // Filter gain correction value in dB
      18 ,               // No of bits the result is shifted
       0 ,               // Group delay of this filter in taps
2.194443e+000 ,               // Downsampling factor this filter was designed for
};

//Filter coefficients
long Filter_10MHzCoeff[] = {67,77,89,100,113,126,139,154,169,184,200,217,235,253,272,291,311,332,353,375,398,421,444,
469,493,518,544,570,597,624,651,679,707,736,765,794,823,852,882,912,942,971,1001,1031,1061,1091,1120,1150,1179,1208,
1236,1264,1292,1319,1346,1372,1397,1422,1447,1470,1493,1515,1536,1556,1575,1593,1610,1625,1640,1654,1666,1677,1686,
1695,1702,1707,1711,1714,1715,1714,1712,1708,1703,1696,1687,1677,1665,1651,1635,1618,1599,1578,1556,1531,1505,1477,
1448,1417,1384,1349,1313,1274,1235,1193,1151,1106,1060,1013,964,914,862,809,755,700,643,586,527,467,407,345,283,220,
157,93,29,-36,-102,-167,-233,-298,-364,-430,-495,-560,-625,-689,-753,-816,-879,-940,-1001,-1061,-1120,-1178,-1234,
-1289,-1343,-1395,-1446,-1495,-1542,-1587,-1631,-1672,-1712,-1749,-1785,-1818,-1849,-1877,-1903,-1927,-1948,-1966,
-1982,-1995,-2005,-2013,-2018,-2020,-2020,-2016,-2010,-2001,-1989,-1974,-1956,-1936,-1912,-1886,-1857,-1825,-1791,
-1753,-1713,-1670,-1625,-1577,-1526,-1473,-1418,-1360,-1301,-1238,-1174,-1108,-1039,-969,-897,-824,-749,-672,-594,
-514,-434,-352,-270,-186,-102,-18,67,153,238,324,410,495,581,666,750,833,916,998,1079,1158,1236,1313,1388,1462,1533,
1603,1671,1736,1799,1860,1918,1974,2026,2076,2124,2168,2209,2247,2281,2313,2340,2365,2386,2403,2417,2427,2434,2437,
2436,2431,2422,2410,2394,2374,2350,2323,2292,2257,2218,2176,2130,2081,2028,1972,1912,1849,1783,1714,1641,1566,1488,
1407,1324,1238,1149,1059,966,871,775,677,577,475,373,269,165,59,-47,-153,-260,-367,-474,-580,-686,-792,-897,-1001,
-1104,-1206,-1307,-1405,-1502,-1598,-1691,-1782,-1870,-1956,-2040,-2120,-2198,-2272,-2343,-2411,-2475,-2536,-2592,
-2645,-2694,-2739,-2780,-2816,-2849,-2876,-2900,-2918,-2932,-2942,-2946,-2946,-2942,-2932,-2918,-2899,-2875,-2846,
-2813,-2775,-2732,-2685,-2633,-2576,-2515,-2450,-2380,-2307,-2229,-2147,-2061,-1971,-1878,-1781,-1680,-1577,-1470,
-1361,-1249,-1134,-1016,-897,-775,-651,-526,-400,-271,-142,-12,118,250,381,512,644,775,905,1035,1163,1290,1416,1540,
1662,1783,1900,2016,2128,2238,2344,2448,2547,2643,2736,2824,2908,2988,3063,3133,3199,3260,3315,3366,3411,3451,3485,
3514,3537,3555,3566,3572,3572,3566,3554,3537,3513,3483,3448,3406,3359,3306,3247,3182,3112,3037,2956,2869,2778,2681,
2579,2473,2362,2246,2126,2002,1874,1743,1607,1469,1327,1182,1034,884,732,578,422,264,105,-54,-215,-376,-537,-699,
-859,-1020,-1179,-1337,-1494,-1649,-1803,-1954,-2102,-2248,-2391,-2531,-2667,-2799,-2928,-3052,-3172,-3287,-3397,
-3502,-3602,-3696,-3785,-3868,-3945,-4015,-4079,-4137,-4188,-4232,-4270,-4301,-4324,-4340,-4350,-4352,-4346,-4334,
-4314,-4286,-4252,-4210,-4161,-4104,-4040,-3970,-3892,-3807,-3715,-3617,-3512,-3401,-3283,-3159,-3029,-2894,-2753,
-2606,-2454,-2297,-2136,-1970,-1800,-1626,-1448,-1267,-1083,-896,-706,-515,-321,-126,71,268,466,665,863,1061,1258,
1455,1649,1843,2034,2222,2408,2591,2771,2947,3119,3286,3449,3607,3760,3907,4048,4184,4313,4436,4551,4660,4761,4855,
4941,5020,5090,5152,5206,5251,5288,5315,5334,5345,5346,5338,5321,5295,5259,5215,5162,5099,5028,4947,4858,4760,4654,
4539,4415,4284,4144,3997,3842,3679,3510,3333,3150,2960,2765,2563,2356,2144,1927,1706,1480,1250,1017,781,543,302,59,
-186,-431,-678,-924,-1171,-1416,-1661,-1905,-2147,-2386,-2623,-2857,-3087,-3313,-3535,-3753,-3965,-4172,-4373,-4567,
-4756,-4937,-5110,-5277,-5435,-5585,-5726,-5858,-5981,-6095,-6199,-6293,-6377,-6451,-6514,-6566,-6608,-6638,-6658,
-6666,-6663,-6648,-6622,-6585,-6536,-6476,-6404,-6321,-6226,-6121,-6004,-5877,-5738,-5589,-5430,-5260,-5080,-4891,
-4691,-4483,-4266,-4040,-3805,-3563,-3313,-3056,-2792,-2522,-2245,-1963,-1676,-1384,-1088,-788,-485,-179,129,439,750,
1062,1374,1686,1997,2307,2614,2920,3222,3521,3815,4105,4390,4670,4943,5210,5469,5721,5965,6200,6426,6643,6849,7046,
7231,7406,7569,7720,7859,7986,8100,8200,8288,8362,8421,8467,8499,8517,8520,8508,8482,8441,8385,8315,8229,8130,8015,
7886,7742,7585,7413,7227,7027,6814,6588,6349,6097,5833,5557,5270,4971,4662,4343,4013,3675,3327,2972,2608,2238,1860,
1477,1089,695,298,-103,-507,-913,-1321,-1729,-2137,-2545,-2952,-3357,-3759,-4158,-4553,-4943,-5327,-5705,-6077,-6441,
-6797,-7144,-7481,-7809,-8125,-8430,-8723,-9003,-9271,-9524,-9763,-9987,-10196,-10389,-10566,-10726,-10869,-10995,
-11102,-11192,-11263,-11315,-11348,-11362,-11357,-11332,-11287,-11222,-11138,-11034,-10910,-10766,-10602,-10419,
-10216,-9994,-9752,-9492,-9213,-8916,-8601,-8268,-7918,-7552,-7169,-6770,-6356,-5927,-5484,-5028,-4559,-4078,-3585,
-3081,-2568,-2045,-1514,-975,-429,122,679,1239,1803,2370,2937,3505,4073,4639,5203,5763,6319,6870,7414,7951,8480,
8999,9508,10006,10492,10965,11424,11867,12295,12706,13100,13475,13831,14167,14482,14775,15047,15295,15519,15719,
15895,16044,16168,16265,16336,16378,16393,16380,16339,16269,16170,16042,15885,15698,15483,15238,14963,14660,14328,
13967,13577,13160,12714,12241,11741,11215,10662,10085,9482,8855,8206,7533,6839,6124,5389,4635,3863,3074,2269,1449,
616,-230,-1087,-1954,-2829,-3712,-4601,-5495,-6392,-7291,-8190,-9088,-9984,-10876,-11762,-12641,-13511,-14371,
-15220,-16055,-16875,-17679,-18465,-19232,-19977,-20700,-21399,-22072,-22718,-23335,-23923,-24478,-25001,-25489,
-25942,-26357,-26735,-27072,-27369,-27623,-27834,-28001,-28123,-28197,-28225,-28204,-28133,-28013,-27841,-27618,
-27343,-27014,-26633,-26197,-25706,-25161,-24561,-23905,-23194,-22427,-21604,-20725,-19791,-18801,-17756,-16656,
-15501,-14292,-13029,-11713,-10345,-8924,-7452,-5930,-4358,-2738,-1070,644,2403,4207,6053,7941,9869,11835,13839,
15879,17953,20059,22195,24361,26554,28772,31014,33278,35561,37861,40178,42507,44849,47200,49558,51921,54287,56654,
59019,61381,63737,66085,68423,70748,73059,75352,77627,79880,82109,84313,86489,88635,90748,92828,94871,96876,98841,
100763,102641,104474,106258,107992,109675,111305,112880,114398,115859,117260,118600,119878,121093,122242,123326,
124342,125291,126170,126979,127717,128384,128978,129499,129947,130321,130621,130846,130996,131071,131071,130996,
130846,130621,130321,129947,129499,128978,128384,127717,126979,126170,125291,124342,123326,122242,121093,119878,
118600,117260,115859,114398,112880,111305,109675,107992,106258,104474,102641,100763,98841,96876,94871,92828,90748,
88635,86489,84313,82109,79880,77627,75352,73059,70748,68423,66085,63737,61381,59019,56654,54287,51921,49558,47200,
44849,42507,40178,37861,35561,33278,31014,28772,26554,24361,22195,20059,17953,15879,13839,11835,9869,7941,6053,4207,
2403,644,-1070,-2738,-4358,-5930,-7452,-8924,-10345,-11713,-13029,-14292,-15501,-16656,-17756,-18801,-19791,-20725,
-21604,-22427,-23194,-23905,-24561,-25161,-25706,-26197,-26633,-27014,-27343,-27618,-27841,-28013,-28133,-28204,
-28225,-28197,-28123,-28001,-27834,-27623,-27369,-27072,-26735,-26357,-25942,-25489,-25001,-24478,-23923,-23335,
-22718,-22072,-21399,-20700,-19977,-19232,-18465,-17679,-16875,-16055,-15220,-14371,-13511,-12641,-11762,-10876,
-9984,-9088,-8190,-7291,-6392,-5495,-4601,-3712,-2829,-1954,-1087,-230,616,1449,2269,3074,3863,4635,5389,6124,
6839,7533,8206,8855,9482,10085,10662,11215,11741,12241,12714,13160,13577,13967,14328,14660,14963,15238,15483,15698,
15885,16042,16170,16269,16339,16380,16393,16378,16336,16265,16168,16044,15895,15719,15519,15295,15047,14775,14482,
14167,13831,13475,13100,12706,12295,11867,11424,10965,10492,10006,9508,8999,8480,7951,7414,6870,6319,5763,5203,4639,
4073,3505,2937,2370,1803,1239,679,122,-429,-975,-1514,-2045,-2568,-3081,-3585,-4078,-4559,-5028,-5484,-5927,-6356,
-6770,-7169,-7552,-7918,-8268,-8601,-8916,-9213,-9492,-9752,-9994,-10216,-10419,-10602,-10766,-10910,-11034,-11138,
-11222,-11287,-11332,-11357,-11362,-11348,-11315,-11263,-11192,-11102,-10995,-10869,-10726,-10566,-10389,-10196,
-9987,-9763,-9524,-9271,-9003,-8723,-8430,-8125,-7809,-7481,-7144,-6797,-6441,-6077,-5705,-5327,-4943,-4553,-4158,
-3759,-3357,-2952,-2545,-2137,-1729,-1321,-913,-507,-103,298,695,1089,1477,1860,2238,2608,2972,3327,3675,4013,4343,
4662,4971,5270,5557,5833,6097,6349,6588,6814,7027,7227,7413,7585,7742,7886,8015,8130,8229,8315,8385,8441,8482,8508,
8520,8517,8499,8467,8421,8362,8288,8200,8100,7986,7859,7720,7569,7406,7231,7046,6849,6643,6426,6200,5965,5721,5469,
5210,4943,4670,4390,4105,3815,3521,3222,2920,2614,2307,1997,1686,1374,1062,750,439,129,-179,-485,-788,-1088,-1384,
-1676,-1963,-2245,-2522,-2792,-3056,-3313,-3563,-3805,-4040,-4266,-4483,-4691,-4891,-5080,-5260,-5430,-5589,-5738,
-5877,-6004,-6121,-6226,-6321,-6404,-6476,-6536,-6585,-6622,-6648,-6663,-6666,-6658,-6638,-6608,-6566,-6514,-6451,
-6377,-6293,-6199,-6095,-5981,-5858,-5726,-5585,-5435,-5277,-5110,-4937,-4756,-4567,-4373,-4172,-3965,-3753,-3535,
-3313,-3087,-2857,-2623,-2386,-2147,-1905,-1661,-1416,-1171,-924,-678,-431,-186,59,302,543,781,1017,1250,1480,1706,
1927,2144,2356,2563,2765,2960,3150,3333,3510,3679,3842,3997,4144,4284,4415,4539,4654,4760,4858,4947,5028,5099,5162,
5215,5259,5295,5321,5338,5346,5345,5334,5315,5288,5251,5206,5152,5090,5020,4941,4855,4761,4660,4551,4436,4313,4184,
4048,3907,3760,3607,3449,3286,3119,2947,2771,2591,2408,2222,2034,1843,1649,1455,1258,1061,863,665,466,268,71,-126,
-321,-515,-706,-896,-1083,-1267,-1448,-1626,-1800,-1970,-2136,-2297,-2454,-2606,-2753,-2894,-3029,-3159,-3283,-3401,
-3512,-3617,-3715,-3807,-3892,-3970,-4040,-4104,-4161,-4210,-4252,-4286,-4314,-4334,-4346,-4352,-4350,-4340,-4324,
-4301,-4270,-4232,-4188,-4137,-4079,-4015,-3945,-3868,-3785,-3696,-3602,-3502,-3397,-3287,-3172,-3052,-2928,-2799,
-2667,-2531,-2391,-2248,-2102,-1954,-1803,-1649,-1494,-1337,-1179,-1020,-859,-699,-537,-376,-215,-54,105,264,422,578,
732,884,1034,1182,1327,1469,1607,1743,1874,2002,2126,2246,2362,2473,2579,2681,2778,2869,2956,3037,3112,3182,3247,
3306,3359,3406,3448,3483,3513,3537,3554,3566,3572,3572,3566,3555,3537,3514,3485,3451,3411,3366,3315,3260,3199,3133,
3063,2988,2908,2824,2736,2643,2547,2448,2344,2238,2128,2016,1900,1783,1662,1540,1416,1290,1163,1035,905,775,644,512,
381,250,118,-12,-142,-271,-400,-526,-651,-775,-897,-1016,-1134,-1249,-1361,-1470,-1577,-1680,-1781,-1878,-1971,
-2061,-2147,-2229,-2307,-2380,-2450,-2515,-2576,-2633,-2685,-2732,-2775,-2813,-2846,-2875,-2899,-2918,-2932,-2942,
-2946,-2946,-2942,-2932,-2918,-2900,-2876,-2849,-2816,-2780,-2739,-2694,-2645,-2592,-2536,-2475,-2411,-2343,-2272,
-2198,-2120,-2040,-1956,-1870,-1782,-1691,-1598,-1502,-1405,-1307,-1206,-1104,-1001,-897,-792,-686,-580,-474,-367,
-260,-153,-47,59,165,269,373,475,577,677,775,871,966,1059,1149,1238,1324,1407,1488,1566,1641,1714,1783,1849,1912,
1972,2028,2081,2130,2176,2218,2257,2292,2323,2350,2374,2394,2410,2422,2431,2436,2437,2434,2427,2417,2403,2386,2365,
2340,2313,2281,2247,2209,2168,2124,2076,2026,1974,1918,1860,1799,1736,1671,1603,1533,1462,1388,1313,1236,1158,1079,
998,916,833,750,666,581,495,410,324,238,153,67,-18,-102,-186,-270,-352,-434,-514,-594,-672,-749,-824,-897,-969,-1039,
-1108,-1174,-1238,-1301,-1360,-1418,-1473,-1526,-1577,-1625,-1670,-1713,-1753,-1791,-1825,-1857,-1886,-1912,-1936,
-1956,-1974,-1989,-2001,-2010,-2016,-2020,-2020,-2018,-2013,-2005,-1995,-1982,-1966,-1948,-1927,-1903,-1877,-1849,
-1818,-1785,-1749,-1712,-1672,-1631,-1587,-1542,-1495,-1446,-1395,-1343,-1289,-1234,-1178,-1120,-1061,-1001,-940,
-879,-816,-753,-689,-625,-560,-495,-430,-364,-298,-233,-167,-102,-36,29,93,157,220,283,345,407,467,527,586,643,700,
755,809,862,914,964,1013,1060,1106,1151,1193,1235,1274,1313,1349,1384,1417,1448,1477,1505,1531,1556,1578,1599,1618,
1635,1651,1665,1677,1687,1696,1703,1708,1712,1714,1715,1714,1711,1707,1702,1695,1686,1677,1666,1654,1640,1625,1610,
1593,1575,1556,1536,1515,1493,1470,1447,1422,1397,1372,1346,1319,1292,1264,1236,1208,1179,1150,1120,1091,1061,1031,
1001,971,942,912,882,852,823,794,765,736,707,679,651,624,597,570,544,518,493,469,444,421,398,375,353,332,311,291,272,
253,235,217,200,184,169,154,139,126,113,100,89,77,67};

#endif //_Filter_10MHz_H
