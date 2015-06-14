/** 
*Filter name: Filtername
* 
*@file Filter_500kHz_b.h 
*@abstract 
* 
*@copyright (c)2007 Rohde & Schwarz GmbH & Co. KG, Munich 
*@author Markus Herdin, Johannes Dommel 
*@version 
* 
*/ 

#ifndef _Filter_500kHz_b_H 
#define _Filter_500kHz_b_H 

#include "TSMWIQInterfaceTypes.h"
//Filter parameter 
TSMW_IQIF_FILTER_PARAM_t  Filter_500kHz_bParam = {
       1 ,               //Number of filter
    1860 ,               // Number of filter coefficients
       2 ,               // Filter coefficient oversampling factor
   -2.60 ,               // Filter gain correction value in dB
      19 ,               // No of bits the result is shifted
     465 ,               // Group delay of this filter in taps
4.388889e+001 ,               // Downsampling factor this filter was designed for
};

//Filter coefficients
long Filter_500kHz_bCoeff[] = {0,0,0,0,0,1,2,3,3,4,5,6,7,7,7,7,7,6,4,3,1,-1,-3,-4,-6,-7,-8,-8,-8,-7,-6,-4,-2,1,3,5,7,9,9,10,9,8,6,4,1,-2,-5,-7,-9,-11,-11,-11,-10,-8,-5,-2,1,5,8,10,12,13,13,12,9,6,3,-1,-5,-9,-12,-14,-15,-15,-14,-11,-8,-4,1,5,10,13,16,17,17,16,13,9,4,-1,-6,-11,-15,-18,-20,-20,-18,-15,-10,-5,1,7,13,18,21,22,22,20,16,11,5,-2,-9,-15,-20,-24,-26,-25,-23,-18,-12,-5,3,11,18,24,27,29,28,25,20,13,4,-5,-13,-21,-27,-31,-33,-31,-28,-21,-13,-4,6,16,25,31,35,37,35,30,23,13,2,-9,-19,-29,-36,-40,-41,-38,-33,-24,-13,-1,12,23,33,41,45,45,42,35,25,12,-1,-15,-28,-39,-47,-50,-50,-46,-37,-25,-11,4,19,33,45,53,56,55,49,39,26,10,-7,-24,-39,-51,-59,-62,-60,-53,-41,-25,-7,12,30,46,59,66,69,65,56,42,24,4,-17,-37,-54,-67,-74,-75,-70,-59,-43,-23,0,23,44,62,75,82,82,75,62,43,20,-5,-30,-53,-72,-85,-91,-89,-80,-64,-42,-17,11,38,62,82,95,100,96,85,66,41,12,-18,-47,-73,-93,-105,-109,-103,-89,-67,-39,-7,26,57,84,104,116,118,110,92,67,35,0,-36,-69,-97,-117,-128,-127,-116,-95,-66,-30,9,47,82,111,131,139,137,122,97,63,24,-18,-60,-96,-126,-145,-152,-146,-128,-98,-60,-16,30,74,112,142,159,164,155,132,98,55,7,-43,-90,-129,-158,-175,-176,-163,-135,-96,-48,5,58,107,148,176,190,188,170,138,93,39,-18,-75,-126,-167,-195,-206,-200,-177,-139,-88,-29,33,94,147,188,214,222,211,183,138,81,16,-51,-115,-169,-210,-234,-238,-222,-187,-135,-72,-1,71,138,193,233,254,254,232,189,131,60,-17,-93,-163,-219,-258,-275,-269,-240,-190,-124,-46,37,118,190,246,283,295,283,247,189,114,28,-60,-145,-219,-275,-308,-315,-296,-251,-185,-101,-8,86,175,250,305,334,335,308,254,178,86,-15,-116,-208,-283,-336,-360,-354,-318,-254,-168,-67,42,148,243,318,367,386,372,326,252,155,44,-72,-183,-280,-355,-400,-412,-388,-332,-246,-139,-18,105,222,320,393,433,436,403,335,237,118,-12,-143,-263,-363,-432,-466,-460,-415,-334,-224,-94,47,184,308,407,473,499,482,425,331,207,64,-86,-230,-356,-454,-514,-531,-503,-432,-323,-185,-30,129,279,407,502,555,562,521,435,311,159,-9,-177,-332,-461,-552,-597,-592,-536,-434,-293,-127,53,230,390,518,603,638,619,547,428,271,89,-103,-288,-451,-577,-655,-679,-645,-555,-418,-243,-45,158,351,515,638,708,718,667,559,402,208,-5,-220,-418,-584,-701,-760,-755,-686,-557,-379,-167,61,287,491,656,766,812,790,700,550,350,119,-125,-361,-569,-731,-832,-864,-822,-710,-536,-314,-63,196,441,651,809,899,914,850,714,515,270,0,-274,-527,-739,-889,-966,-961,-874,-712,-487,-218,72,360,620,831,972,1033,1006,893,703,450,157,-154,-454,-720,-927,-1057,-1099,-1048,-906,-686,-405,-86,244,557,826,1027,1144,1164,1085,912,660,349,4,-345,-668,-939,-1132,-1231,-1226,-1117,-911,-625,-283,88,456,788,1058,1240,1319,1286,1143,901,579,204,-192,-577,-918,-1184,-1352,-1407,-1342,-1162,-881,-522,-113,309,711,1057,1316,1467,1494,1394,1174,851,452,9,-440,-857,-1205,-1455,-1584,-1580,-1440,-1176,-808,-367,111,585,1015,1364,1601,1704,1663,1480,1168,752,267,-247,-746,-1188,-1534,-1754,-1827,-1744,-1512,-1148,-681,-150,401,925,1376,1716,1914,1951,1822,1535,1114,592,13,-575,-1122,-1580,-1910,-2081,-2077,-1895,-1549,-1065,-485,145,772,1341,1803,2118,2256,2204,1962,1550,999,355,-328,-993,-1583,-2046,-2341,-2440,-2333,-2024,-1538,-913,-201,539,1244,1853,2313,2582,2634,2463,2078,1509,803,17,-783,-1528,-2155,-2607,-2843,-2840,-2594,-2123,-1462,-665,202,1066,1853,2494,2933,3129,3060,2728,2157,1392,494,-462,-1395,-2225,-2880,-3300,-3445,-3297,-2865,-2179,-1295,-282,773,1782,2658,3323,3716,3799,3557,3006,2187,1163,20,-1149,-2243,-3168,-3840,-4198,-4203,-3847,-3154,-2176,-989,309,1609,2801,3780,4456,4766,4674,4179,3313,2142,758,-726,-2183,-3491,-4531,-5209,-5457,-5242,-4571,-3489,-2077,-447,1266,2919,4369,5484,6160,6326,5952,5054,3692,1969,22,-1991,-3899,-5534,-6747,-7419,-7475,-6887,-5683,-3943,-1798,586,3011,5272,7168,8521,9193,9097,8208,6567,4282,1518,-1509,-4554,-7354,-9658,-11242,-11932,-11620,-10276,-7956,-4800,-1029,3073,7177,10932,13991,16044,16840,16213,14103,10564,5774,23,-6293,-12697,-18660,-23632,-27081,-28530,-27591,-23998,-17630,-8527,3104,16895,32331,48777,65509,81758,96752,109761,120138,127364,131071,131071,127364,120138,109761,96752,81758,65509,48777,32331,16895,3104,-8527,-17630,-23998,-27591,-28530,-27081,-23632,-18660,-12697,-6293,23,5774,10564,14103,16213,16840,16044,13991,10932,7177,3073,-1029,-4800,-7956,-10276,-11620,-11932,-11242,-9658,-7354,-4554,-1509,1518,4282,6567,8208,9097,9193,8521,7168,5272,3011,586,-1798,-3943,-5683,-6887,-7475,-7419,-6747,-5534,-3899,-1991,22,1969,3692,5054,5952,6326,6160,5484,4369,2919,1266,-447,-2077,-3489,-4571,-5242,-5457,-5209,-4531,-3491,-2183,-726,758,2142,3313,4179,4674,4766,4456,3780,2801,1609,309,-989,-2176,-3154,-3847,-4203,-4198,-3840,-3168,-2243,-1149,20,1163,2187,3006,3557,3799,3716,3323,2658,1782,773,-282,-1295,-2179,-2865,-3297,-3445,-3300,-2880,-2225,-1395,-462,494,1392,2157,2728,3060,3129,2933,2494,1853,1066,202,-665,-1462,-2123,-2594,-2840,-2843,-2607,-2155,-1528,-783,17,803,1509,2078,2463,2634,2582,2313,1853,1244,539,-201,-913,-1538,-2024,-2333,-2440,-2341,-2046,-1583,-993,-328,355,999,1550,1962,2204,2256,2118,1803,1341,772,145,-485,-1065,-1549,-1895,-2077,-2081,-1910,-1580,-1122,-575,13,592,1114,1535,1822,1951,1914,1716,1376,925,401,-150,-681,-1148,-1512,-1744,-1827,-1754,-1534,-1188,-746,-247,267,752,1168,1480,1663,1704,1601,1364,1015,585,111,-367,-808,-1176,-1440,-1580,-1584,-1455,-1205,-857,-440,9,452,851,1174,1394,1494,1467,1316,1057,711,309,-113,-522,-881,-1162,-1342,-1407,-1352,-1184,-918,-577,-192,204,579,901,1143,1286,1319,1240,1058,788,456,88,-283,-625,-911,-1117,-1226,-1231,-1132,-939,-668,-345,4,349,660,912,1085,1164,1144,1027,826,557,244,-86,-405,-686,-906,-1048,-1099,-1057,-927,-720,-454,-154,157,450,703,893,1006,1033,972,831,620,360,72,-218,-487,-712,-874,-961,-966,-889,-739,-527,-274,0,270,515,714,850,914,899,809,651,441,196,-63,-314,-536,-710,-822,-864,-832,-731,-569,-361,-125,119,350,550,700,790,812,766,656,491,287,61,-167,-379,-557,-686,-755,-760,-701,-584,-418,-220,-5,208,402,559,667,718,708,638,515,351,158,-45,-243,-418,-555,-645,-679,-655,-577,-451,-288,-103,89,271,428,547,619,638,603,518,390,230,53,-127,-293,-434,-536,-592,-597,-552,-461,-332,-177,-9,159,311,435,521,562,555,502,407,279,129,-30,-185,-323,-432,-503,-531,-514,-454,-356,-230,-86,64,207,331,425,482,499,473,407,308,184,47,-94,-224,-334,-415,-460,-466,-432,-363,-263,-143,-12,118,237,335,403,436,433,393,320,222,105,-18,-139,-246,-332,-388,-412,-400,-355,-280,-183,-72,44,155,252,326,372,386,367,318,243,148,42,-67,-168,-254,-318,-354,-360,-336,-283,-208,-116,-15,86,178,254,308,335,334,305,250,175,86,-8,-101,-185,-251,-296,-315,-308,-275,-219,-145,-60,28,114,189,247,283,295,283,246,190,118,37,-46,-124,-190,-240,-269,-275,-258,-219,-163,-93,-17,60,131,189,232,254,254,233,193,138,71,-1,-72,-135,-187,-222,-238,-234,-210,-169,-115,-51,16,81,138,183,211,222,214,188,147,94,33,-29,-88,-139,-177,-200,-206,-195,-167,-126,-75,-18,39,93,138,170,188,190,176,148,107,58,5,-48,-96,-135,-163,-176,-175,-158,-129,-90,-43,7,55,98,132,155,164,159,142,112,74,30,-16,-60,-98,-128,-146,-152,-145,-126,-96,-60,-18,24,63,97,122,137,139,131,111,82,47,9,-30,-66,-95,-116,-127,-128,-117,-97,-69,-36,0,35,67,92,110,118,116,104,84,57,26,-7,-39,-67,-89,-103,-109,-105,-93,-73,-47,-18,12,41,66,85,96,100,95,82,62,38,11,-17,-42,-64,-80,-89,-91,-85,-72,-53,-30,-5,20,43,62,75,82,82,75,62,44,23,0,-23,-43,-59,-70,-75,-74,-67,-54,-37,-17,4,24,42,56,65,69,66,59,46,30,12,-7,-25,-41,-53,-60,-62,-59,-51,-39,-24,-7,10,26,39,49,55,56,53,45,33,19,4,-11,-25,-37,-46,-50,-50,-47,-39,-28,-15,-1,12,25,35,42,45,45,41,33,23,12,-1,-13,-24,-33,-38,-41,-40,-36,-29,-19,-9,2,13,23,30,35,37,35,31,25,16,6,-4,-13,-21,-28,-31,-33,-31,-27,-21,-13,-5,4,13,20,25,28,29,27,24,18,11,3,-5,-12,-18,-23,-25,-26,-24,-20,-15,-9,-2,5,11,16,20,22,22,21,18,13,7,1,-5,-10,-15,-18,-20,-20,-18,-15,-11,-6,-1,4,9,13,16,17,17,16,13,10,5,1,-4,-8,-11,-14,-15,-15,-14,-12,-9,-5,-1,3,6,9,12,13,13,12,10,8,5,1,-2,-5,-8,-10,-11,-11,-11,-9,-7,-5,-2,1,4,6,8,9,10,9,9,7,5,3,1,-2,-4,-6,-7,-8,-8,-8,-7,-6,-4,-3,-1,1,3,4,6,7,7,7,7,7,6,5,4,3,3,2,1,0,0,0,0,0};

#endif //_Filter_500kHz_b_H 
