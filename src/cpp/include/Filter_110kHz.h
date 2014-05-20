/** 
*Filter name: Filtername
* 
*@file Filter_110kHz.h 
*@abstract 
* 
*@copyright (c)2007 Rohde & Schwarz GmbH & Co. KG, Munich 
*@author Markus Herdin, Johannes Dommel 
*@version 
* 
*/ 

#ifndef _Filter_110kHz_H 
#define _Filter_110kHz_H 

#include "TSMWIQInterfaceTypes.h"
//Filter parameter 
TSMW_IQIF_FILTER_PARAM_t  Filter_110kHzParam = {
       1 ,               //Number of filter
    1638 ,               // Number of filter coefficients
       2 ,               // Filter coefficient oversampling factor
   -1.57 ,               // Filter gain correction value in dB
      24 ,               // No of bits the result is shifted
     410 ,               // Group delay of this filter in taps
9.974747e+001 ,               // Downsampling factor this filter was designed for
};

//Filter coefficients
long Filter_110kHzCoeff[] = {73,73,73,72,72,72,71,71,70,70,69,68,67,66,65,63,62,60,59,57,55,53,51,48,46,43,40,37,34,31,28,24,20,17,13,8,4,-1,-5,-10,-16,-21,-26,-32,-38,-44,-50,-57,-64,-71,-78,-85,-93,-101,-109,-118,-126,-135,-144,-154,-163,-173,-183,-194,-204,-215,-227,-238,-250,-262,-274,-287,-300,-313,-327,-341,-355,-370,-385,-400,-415,-431,-447,-464,-481,-498,-516,-534,-552,-571,-590,-609,-629,-649,-669,-690,-711,-733,-755,-777,-800,-823,-847,-871,-896,-920,-946,-971,-997,-1024,-1051,-1078,-1106,-1134,-1163,-1192,-1222,-1252,-1282,-1313,-1344,-1376,-1409,-1441,-1475,-1508,-1542,-1577,-1612,-1648,-1684,-1720,-1757,-1795,-1833,-1871,-1910,-1950,-1989,-2030,-2071,-2112,-2154,-2197,-2240,-2283,-2327,-2371,-2416,-2462,-2508,-2554,-2601,-2649,-2697,-2745,-2794,-2844,-2894,-2945,-2996,-3047,-3099,-3152,-3205,-3259,-3313,-3368,-3423,-3479,-3535,-3592,-3649,-3707,-3765,-3824,-3883,-3943,-4003,-4064,-4126,-4187,-4250,-4313,-4376,-4440,-4504,-4569,-4634,-4700,-4766,-4833,-4900,-4968,-5036,-5104,-5174,-5243,-5313,-5384,-5455,-5526,-5598,-5670,-5743,-5816,-5889,-5963,-6038,-6113,-6188,-6263,-6339,-6416,-6493,-6570,-6648,-6725,-6804,-6883,-6962,-7041,-7121,-7201,-7281,-7362,-7443,-7525,-7606,-7688,-7771,-7853,-7936,-8019,-8103,-8187,-8270,-8355,-8439,-8524,-8609,-8694,-8779,-8864,-8950,-9036,-9122,-9208,-9294,-9381,-9468,-9554,-9641,-9728,-9815,-9902,-9989,-10077,-10164,-10251,-10339,-10426,-10513,-10601,-10688,-10776,-10863,-10950,-11037,-11125,-11212,-11299,-11386,-11472,-11559,-11646,-11732,-11818,-11904,-11990,-12076,-12161,-12246,-12331,-12416,-12500,-12584,-12668,-12752,-12835,-12918,-13000,-13082,-13164,-13246,-13327,-13407,-13487,-13567,-13646,-13725,-13803,-13881,-13958,-14035,-14111,-14186,-14261,-14336,-14409,-14482,-14555,-14627,-14698,-14768,-14838,-14907,-14975,-15042,-15109,-15175,-15240,-15304,-15368,-15430,-15492,-15553,-15613,-15672,-15730,-15787,-15843,-15898,-15952,-16005,-16057,-16108,-16158,-16207,-16254,-16301,-16346,-16390,-16433,-16475,-16516,-16555,-16593,-16630,-16666,-16700,-16733,-16764,-16795,-16823,-16851,-16877,-16901,-16925,-16946,-16967,-16985,-17003,-17018,-17032,-17045,-17056,-17065,-17073,-17079,-17083,-17086,-17087,-17087,-17084,-17080,-17074,-17067,-17057,-17046,-17033,-17018,-17001,-16983,-16962,-16940,-16915,-16889,-16861,-16830,-16798,-16764,-16728,-16689,-16649,-16606,-16562,-16515,-16466,-16415,-16362,-16307,-16249,-16190,-16128,-16064,-15997,-15929,-15858,-15785,-15709,-15631,-15551,-15469,-15384,-15297,-15207,-15115,-15020,-14924,-14824,-14722,-14618,-14511,-14402,-14291,-14176,-14059,-13940,-13818,-13694,-13567,-13437,-13305,-13170,-13033,-12893,-12750,-12604,-12456,-12306,-12152,-11996,-11837,-11676,-11511,-11344,-11175,-11002,-10827,-10649,-10468,-10285,-10098,-9909,-9717,-9523,-9325,-9125,-8921,-8715,-8506,-8295,-8080,-7863,-7642,-7419,-7193,-6964,-6733,-6498,-6260,-6020,-5777,-5530,-5281,-5029,-4774,-4517,-4256,-3992,-3726,-3456,-3184,-2909,-2631,-2349,-2066,-1779,-1489,-1196,-901,-602,-301,3,311,621,933,1249,1568,1889,2214,2541,2871,3204,3540,3879,4220,4565,4912,5262,5615,5970,6329,6690,7054,7420,7790,8162,8537,8915,9295,9678,10064,10453,10844,11238,11634,12034,12435,12840,13247,13657,14069,14484,14901,15321,15743,16168,16595,17025,17458,17892,18330,18769,19211,19656,20103,20552,21003,21457,21913,22371,22832,23295,23760,24227,24696,25168,25642,26117,26595,27075,27557,28041,28527,29015,29505,29997,30491,30987,31484,31984,32485,32988,33493,34000,34508,35018,35529,36043,36558,37074,37592,38112,38633,39155,39679,40205,40732,41260,41790,42320,42853,43386,43921,44456,44993,45532,46071,46611,47153,47695,48238,48783,49328,49874,50421,50969,51517,52067,52617,53168,53719,54271,54824,55377,55931,56486,57040,57596,58151,58707,59263,59820,60377,60934,61491,62049,62606,63164,63722,64280,64837,65395,65953,66510,67067,67625,68181,68738,69294,69850,70406,70961,71516,72070,72624,73177,73730,74282,74833,75384,75934,76483,77031,77579,78125,78671,79216,79759,80302,80844,81384,81923,82462,82999,83534,84069,84602,85133,85664,86193,86720,87246,87770,88293,88814,89334,89851,90368,90882,91394,91905,92414,92921,93426,93928,94429,94928,95425,95920,96412,96902,97390,97876,98360,98841,99320,99796,100270,100741,101210,101677,102140,102602,103060,103516,103969,104420,104867,105312,105754,106193,106629,107062,107493,107920,108344,108765,109183,109598,110010,110419,110824,111226,111625,112020,112412,112801,113186,113568,113947,114322,114693,115061,115426,115786,116143,116497,116847,117193,117535,117873,118208,118539,118866,119190,119509,119824,120136,120444,120747,121047,121342,121634,121921,122204,122484,122759,123030,123296,123559,123817,124071,124321,124567,124808,125045,125277,125505,125729,125949,126164,126374,126580,126782,126979,127172,127360,127544,127723,127898,128068,128234,128395,128551,128703,128850,128992,129130,129263,129392,129516,129635,129749,129859,129964,130065,130160,130251,130337,130419,130495,130567,130634,130697,130754,130807,130855,130898,130936,130970,130999,131023,131042,131057,131066,131071,131071,131066,131057,131042,131023,130999,130970,130936,130898,130855,130807,130754,130697,130634,130567,130495,130419,130337,130251,130160,130065,129964,129859,129749,129635,129516,129392,129263,129130,128992,128850,128703,128551,128395,128234,128068,127898,127723,127544,127360,127172,126979,126782,126580,126374,126164,125949,125729,125505,125277,125045,124808,124567,124321,124071,123817,123559,123296,123030,122759,122484,122204,121921,121634,121342,121047,120747,120444,120136,119824,119509,119190,118866,118539,118208,117873,117535,117193,116847,116497,116143,115786,115426,115061,114693,114322,113947,113568,113186,112801,112412,112020,111625,111226,110824,110419,110010,109598,109183,108765,108344,107920,107493,107062,106629,106193,105754,105312,104867,104420,103969,103516,103060,102602,102140,101677,101210,100741,100270,99796,99320,98841,98360,97876,97390,96902,96412,95920,95425,94928,94429,93928,93426,92921,92414,91905,91394,90882,90368,89851,89334,88814,88293,87770,87246,86720,86193,85664,85133,84602,84069,83534,82999,82462,81923,81384,80844,80302,79759,79216,78671,78125,77579,77031,76483,75934,75384,74833,74282,73730,73177,72624,72070,71516,70961,70406,69850,69294,68738,68181,67625,67067,66510,65953,65395,64837,64280,63722,63164,62606,62049,61491,60934,60377,59820,59263,58707,58151,57596,57040,56486,55931,55377,54824,54271,53719,53168,52617,52067,51517,50969,50421,49874,49328,48783,48238,47695,47153,46611,46071,45532,44993,44456,43921,43386,42853,42320,41790,41260,40732,40205,39679,39155,38633,38112,37592,37074,36558,36043,35529,35018,34508,34000,33493,32988,32485,31984,31484,30987,30491,29997,29505,29015,28527,28041,27557,27075,26595,26117,25642,25168,24696,24227,23760,23295,22832,22371,21913,21457,21003,20552,20103,19656,19211,18769,18330,17892,17458,17025,16595,16168,15743,15321,14901,14484,14069,13657,13247,12840,12435,12034,11634,11238,10844,10453,10064,9678,9295,8915,8537,8162,7790,7420,7054,6690,6329,5970,5615,5262,4912,4565,4220,3879,3540,3204,2871,2541,2214,1889,1568,1249,933,621,311,3,-301,-602,-901,-1196,-1489,-1779,-2066,-2349,-2631,-2909,-3184,-3456,-3726,-3992,-4256,-4517,-4774,-5029,-5281,-5530,-5777,-6020,-6260,-6498,-6733,-6964,-7193,-7419,-7642,-7863,-8080,-8295,-8506,-8715,-8921,-9125,-9325,-9523,-9717,-9909,-10098,-10285,-10468,-10649,-10827,-11002,-11175,-11344,-11511,-11676,-11837,-11996,-12152,-12306,-12456,-12604,-12750,-12893,-13033,-13170,-13305,-13437,-13567,-13694,-13818,-13940,-14059,-14176,-14291,-14402,-14511,-14618,-14722,-14824,-14924,-15020,-15115,-15207,-15297,-15384,-15469,-15551,-15631,-15709,-15785,-15858,-15929,-15997,-16064,-16128,-16190,-16249,-16307,-16362,-16415,-16466,-16515,-16562,-16606,-16649,-16689,-16728,-16764,-16798,-16830,-16861,-16889,-16915,-16940,-16962,-16983,-17001,-17018,-17033,-17046,-17057,-17067,-17074,-17080,-17084,-17087,-17087,-17086,-17083,-17079,-17073,-17065,-17056,-17045,-17032,-17018,-17003,-16985,-16967,-16946,-16925,-16901,-16877,-16851,-16823,-16795,-16764,-16733,-16700,-16666,-16630,-16593,-16555,-16516,-16475,-16433,-16390,-16346,-16301,-16254,-16207,-16158,-16108,-16057,-16005,-15952,-15898,-15843,-15787,-15730,-15672,-15613,-15553,-15492,-15430,-15368,-15304,-15240,-15175,-15109,-15042,-14975,-14907,-14838,-14768,-14698,-14627,-14555,-14482,-14409,-14336,-14261,-14186,-14111,-14035,-13958,-13881,-13803,-13725,-13646,-13567,-13487,-13407,-13327,-13246,-13164,-13082,-13000,-12918,-12835,-12752,-12668,-12584,-12500,-12416,-12331,-12246,-12161,-12076,-11990,-11904,-11818,-11732,-11646,-11559,-11472,-11386,-11299,-11212,-11125,-11037,-10950,-10863,-10776,-10688,-10601,-10513,-10426,-10339,-10251,-10164,-10077,-9989,-9902,-9815,-9728,-9641,-9554,-9468,-9381,-9294,-9208,-9122,-9036,-8950,-8864,-8779,-8694,-8609,-8524,-8439,-8355,-8270,-8187,-8103,-8019,-7936,-7853,-7771,-7688,-7606,-7525,-7443,-7362,-7281,-7201,-7121,-7041,-6962,-6883,-6804,-6725,-6648,-6570,-6493,-6416,-6339,-6263,-6188,-6113,-6038,-5963,-5889,-5816,-5743,-5670,-5598,-5526,-5455,-5384,-5313,-5243,-5174,-5104,-5036,-4968,-4900,-4833,-4766,-4700,-4634,-4569,-4504,-4440,-4376,-4313,-4250,-4187,-4126,-4064,-4003,-3943,-3883,-3824,-3765,-3707,-3649,-3592,-3535,-3479,-3423,-3368,-3313,-3259,-3205,-3152,-3099,-3047,-2996,-2945,-2894,-2844,-2794,-2745,-2697,-2649,-2601,-2554,-2508,-2462,-2416,-2371,-2327,-2283,-2240,-2197,-2154,-2112,-2071,-2030,-1989,-1950,-1910,-1871,-1833,-1795,-1757,-1720,-1684,-1648,-1612,-1577,-1542,-1508,-1475,-1441,-1409,-1376,-1344,-1313,-1282,-1252,-1222,-1192,-1163,-1134,-1106,-1078,-1051,-1024,-997,-971,-946,-920,-896,-871,-847,-823,-800,-777,-755,-733,-711,-690,-669,-649,-629,-609,-590,-571,-552,-534,-516,-498,-481,-464,-447,-431,-415,-400,-385,-370,-355,-341,-327,-313,-300,-287,-274,-262,-250,-238,-227,-215,-204,-194,-183,-173,-163,-154,-144,-135,-126,-118,-109,-101,-93,-85,-78,-71,-64,-57,-50,-44,-38,-32,-26,-21,-16,-10,-5,-1,4,8,13,17,20,24,28,31,34,37,40,43,46,48,51,53,55,57,59,60,62,63,65,66,67,68,69,70,70,71,71,72,72,72,73,73,73};

#endif //_Filter_110kHz_H 

