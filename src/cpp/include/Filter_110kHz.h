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
   -1.13 ,               // Filter gain correction value in dB
      24 ,               // No of bits the result is shifted
     410 ,               // Group delay of this filter in taps
9.974747e+001 ,               // Downsampling factor this filter was designed for
};

//Filter coefficients
long Filter_110kHzCoeff[] = {187,191,195,199,202,206,210,214,217,221,225,228,232,235,239,242,245,248,251,255,258,260,263,266,269,271,274,276,279,281,283,285,287,288,290,292,293,294,295,296,297,298,298,299,299,299,299,299,298,298,297,296,295,294,292,290,288,286,284,281,279,276,272,269,265,261,257,253,248,243,238,232,226,220,214,208,201,193,186,178,170,162,153,144,135,125,115,105,94,83,72,60,48,35,22,9,-4,-18,-33,-47,-63,-78,-94,-110,-127,-144,-162,-180,-198,-217,-237,-256,-277,-297,-318,-340,-362,-385,-408,-431,-455,-479,-504,-530,-555,-582,-609,-636,-664,-692,-721,-751,-781,-811,-842,-874,-906,-939,-972,-1005,-1040,-1074,-1110,-1146,-1182,-1219,-1257,-1295,-1334,-1373,-1413,-1454,-1495,-1536,-1579,-1621,-1665,-1709,-1754,-1799,-1845,-1891,-1938,-1986,-2034,-2083,-2132,-2182,-2233,-2284,-2336,-2388,-2441,-2495,-2549,-2604,-2660,-2716,-2773,-2830,-2888,-2947,-3006,-3066,-3126,-3187,-3249,-3311,-3374,-3438,-3502,-3567,-3632,-3698,-3765,-3832,-3899,-3968,-4037,-4106,-4176,-4247,-4319,-4390,-4463,-4536,-4610,-4684,-4759,-4834,-4910,-4987,-5064,-5141,-5219,-5298,-5377,-5457,-5538,-5618,-5700,-5782,-5864,-5947,-6031,-6114,-6199,-6284,-6369,-6455,-6541,-6628,-6716,-6803,-6891,-6980,-7069,-7159,-7248,-7339,-7429,-7521,-7612,-7704,-7796,-7889,-7982,-8075,-8169,-8263,-8357,-8452,-8546,-8642,-8737,-8833,-8929,-9025,-9122,-9219,-9316,-9413,-9510,-9608,-9706,-9804,-9902,-10000,-10099,-10197,-10296,-10395,-10493,-10592,-10691,-10791,-10890,-10989,-11088,-11187,-11286,-11386,-11485,-11584,-11683,-11782,-11881,-11979,-12078,-12176,-12275,-12373,-12471,-12569,-12667,-12764,-12861,-12958,-13055,-13151,-13247,-13343,-13439,-13534,-13629,-13723,-13817,-13911,-14004,-14097,-14189,-14281,-14372,-14463,-14553,-14643,-14732,-14821,-14909,-14996,-15083,-15169,-15255,-15340,-15424,-15507,-15590,-15672,-15753,-15834,-15913,-15992,-16070,-16147,-16223,-16299,-16373,-16446,-16519,-16590,-16661,-16730,-16799,-16866,-16933,-16998,-17062,-17125,-17187,-17248,-17307,-17365,-17422,-17478,-17533,-17586,-17638,-17689,-17738,-17786,-17833,-17878,-17922,-17964,-18005,-18044,-18082,-18118,-18153,-18186,-18218,-18248,-18276,-18303,-18328,-18352,-18373,-18393,-18412,-18428,-18443,-18456,-18467,-18476,-18484,-18489,-18493,-18495,-18495,-18492,-18488,-18482,-18474,-18464,-18452,-18438,-18421,-18403,-18382,-18360,-18335,-18308,-18278,-18247,-18213,-18177,-18139,-18099,-18056,-18011,-17963,-17914,-17861,-17807,-17750,-17691,-17629,-17565,-17498,-17429,-17357,-17283,-17206,-17126,-17044,-16960,-16873,-16783,-16691,-16596,-16498,-16398,-16295,-16189,-16081,-15970,-15856,-15739,-15620,-15498,-15373,-15245,-15114,-14981,-14844,-14705,-14563,-14418,-14271,-14120,-13966,-13810,-13650,-13488,-13323,-13154,-12983,-12809,-12632,-12452,-12268,-12082,-11893,-11700,-11505,-11307,-11105,-10901,-10693,-10482,-10269,-10052,-9832,-9609,-9383,-9154,-8921,-8686,-8447,-8206,-7961,-7713,-7462,-7208,-6950,-6690,-6426,-6160,-5890,-5617,-5340,-5061,-4779,-4493,-4204,-3912,-3617,-3319,-3018,-2713,-2406,-2095,-1781,-1464,-1144,-821,-494,-165,168,503,842,1184,1529,1877,2228,2582,2939,3299,3663,4029,4398,4771,5146,5524,5906,6290,6677,7067,7461,7857,8256,8657,9062,9470,9881,10294,10710,11129,11551,11976,12403,12833,13266,13702,14141,14582,15026,15472,15921,16373,16827,17284,17744,18206,18671,19138,19608,20080,20555,21032,21511,21993,22477,22964,23453,23944,24438,24933,25431,25932,26434,26939,27445,27954,28465,28978,29493,30010,30529,31050,31573,32098,32624,33153,33683,34215,34749,35285,35822,36361,36902,37444,37988,38533,39080,39629,40178,40730,41282,41836,42392,42949,43506,44066,44626,45188,45750,46314,46879,47445,48012,48580,49148,49718,50289,50860,51432,52005,52579,53153,53728,54303,54880,55456,56033,56611,57189,57768,58346,58925,59505,60084,60664,61244,61824,62404,62985,63565,64145,64725,65305,65885,66465,67044,67623,68202,68781,69359,69937,70514,71091,71667,72242,72817,73392,73965,74538,75110,75681,76252,76821,77390,77957,78524,79089,79654,80217,80779,81340,81899,82457,83014,83570,84124,84676,85227,85777,86325,86871,87416,87958,88500,89039,89576,90112,90646,91178,91707,92235,92761,93284,93806,94325,94842,95357,95869,96379,96887,97392,97895,98395,98893,99388,99881,100371,100858,101343,101825,102304,102780,103253,103724,104191,104656,105117,105575,106031,106483,106932,107378,107820,108260,108696,109129,109558,109984,110407,110826,111241,111653,112062,112467,112868,113266,113660,114050,114437,114819,115198,115573,115944,116312,116675,117035,117390,117741,118089,118432,118771,119106,119437,119764,120086,120405,120719,121029,121334,121635,121932,122224,122512,122796,123075,123350,123620,123886,124147,124403,124655,124903,125146,125384,125617,125846,126070,126289,126504,126714,126919,127120,127315,127506,127692,127873,128049,128220,128387,128549,128705,128857,129004,129146,129282,129414,129541,129663,129780,129892,129999,130101,130197,130289,130376,130458,130534,130606,130672,130733,130790,130841,130887,130928,130964,130994,131020,131040,131056,131066,131071,131071,131066,131056,131040,131020,130994,130964,130928,130887,130841,130790,130733,130672,130606,130534,130458,130376,130289,130197,130101,129999,129892,129780,129663,129541,129414,129282,129146,129004,128857,128705,128549,128387,128220,128049,127873,127692,127506,127315,127120,126919,126714,126504,126289,126070,125846,125617,125384,125146,124903,124655,124403,124147,123886,123620,123350,123075,122796,122512,122224,121932,121635,121334,121029,120719,120405,120086,119764,119437,119106,118771,118432,118089,117741,117390,117035,116675,116312,115944,115573,115198,114819,114437,114050,113660,113266,112868,112467,112062,111653,111241,110826,110407,109984,109558,109129,108696,108260,107820,107378,106932,106483,106031,105575,105117,104656,104191,103724,103253,102780,102304,101825,101343,100858,100371,99881,99388,98893,98395,97895,97392,96887,96379,95869,95357,94842,94325,93806,93284,92761,92235,91707,91178,90646,90112,89576,89039,88500,87958,87416,86871,86325,85777,85227,84676,84124,83570,83014,82457,81899,81340,80779,80217,79654,79089,78524,77957,77390,76821,76252,75681,75110,74538,73965,73392,72817,72242,71667,71091,70514,69937,69359,68781,68202,67623,67044,66465,65885,65305,64725,64145,63565,62985,62404,61824,61244,60664,60084,59505,58925,58346,57768,57189,56611,56033,55456,54880,54303,53728,53153,52579,52005,51432,50860,50289,49718,49148,48580,48012,47445,46879,46314,45750,45188,44626,44066,43506,42949,42392,41836,41282,40730,40178,39629,39080,38533,37988,37444,36902,36361,35822,35285,34749,34215,33683,33153,32624,32098,31573,31050,30529,30010,29493,28978,28465,27954,27445,26939,26434,25932,25431,24933,24438,23944,23453,22964,22477,21993,21511,21032,20555,20080,19608,19138,18671,18206,17744,17284,16827,16373,15921,15472,15026,14582,14141,13702,13266,12833,12403,11976,11551,11129,10710,10294,9881,9470,9062,8657,8256,7857,7461,7067,6677,6290,5906,5524,5146,4771,4398,4029,3663,3299,2939,2582,2228,1877,1529,1184,842,503,168,-165,-494,-821,-1144,-1464,-1781,-2095,-2406,-2713,-3018,-3319,-3617,-3912,-4204,-4493,-4779,-5061,-5340,-5617,-5890,-6160,-6426,-6690,-6950,-7208,-7462,-7713,-7961,-8206,-8447,-8686,-8921,-9154,-9383,-9609,-9832,-10052,-10269,-10482,-10693,-10901,-11105,-11307,-11505,-11700,-11893,-12082,-12268,-12452,-12632,-12809,-12983,-13154,-13323,-13488,-13650,-13810,-13966,-14120,-14271,-14418,-14563,-14705,-14844,-14981,-15114,-15245,-15373,-15498,-15620,-15739,-15856,-15970,-16081,-16189,-16295,-16398,-16498,-16596,-16691,-16783,-16873,-16960,-17044,-17126,-17206,-17283,-17357,-17429,-17498,-17565,-17629,-17691,-17750,-17807,-17861,-17914,-17963,-18011,-18056,-18099,-18139,-18177,-18213,-18247,-18278,-18308,-18335,-18360,-18382,-18403,-18421,-18438,-18452,-18464,-18474,-18482,-18488,-18492,-18495,-18495,-18493,-18489,-18484,-18476,-18467,-18456,-18443,-18428,-18412,-18393,-18373,-18352,-18328,-18303,-18276,-18248,-18218,-18186,-18153,-18118,-18082,-18044,-18005,-17964,-17922,-17878,-17833,-17786,-17738,-17689,-17638,-17586,-17533,-17478,-17422,-17365,-17307,-17248,-17187,-17125,-17062,-16998,-16933,-16866,-16799,-16730,-16661,-16590,-16519,-16446,-16373,-16299,-16223,-16147,-16070,-15992,-15913,-15834,-15753,-15672,-15590,-15507,-15424,-15340,-15255,-15169,-15083,-14996,-14909,-14821,-14732,-14643,-14553,-14463,-14372,-14281,-14189,-14097,-14004,-13911,-13817,-13723,-13629,-13534,-13439,-13343,-13247,-13151,-13055,-12958,-12861,-12764,-12667,-12569,-12471,-12373,-12275,-12176,-12078,-11979,-11881,-11782,-11683,-11584,-11485,-11386,-11286,-11187,-11088,-10989,-10890,-10791,-10691,-10592,-10493,-10395,-10296,-10197,-10099,-10000,-9902,-9804,-9706,-9608,-9510,-9413,-9316,-9219,-9122,-9025,-8929,-8833,-8737,-8642,-8546,-8452,-8357,-8263,-8169,-8075,-7982,-7889,-7796,-7704,-7612,-7521,-7429,-7339,-7248,-7159,-7069,-6980,-6891,-6803,-6716,-6628,-6541,-6455,-6369,-6284,-6199,-6114,-6031,-5947,-5864,-5782,-5700,-5618,-5538,-5457,-5377,-5298,-5219,-5141,-5064,-4987,-4910,-4834,-4759,-4684,-4610,-4536,-4463,-4390,-4319,-4247,-4176,-4106,-4037,-3968,-3899,-3832,-3765,-3698,-3632,-3567,-3502,-3438,-3374,-3311,-3249,-3187,-3126,-3066,-3006,-2947,-2888,-2830,-2773,-2716,-2660,-2604,-2549,-2495,-2441,-2388,-2336,-2284,-2233,-2182,-2132,-2083,-2034,-1986,-1938,-1891,-1845,-1799,-1754,-1709,-1665,-1621,-1579,-1536,-1495,-1454,-1413,-1373,-1334,-1295,-1257,-1219,-1182,-1146,-1110,-1074,-1040,-1005,-972,-939,-906,-874,-842,-811,-781,-751,-721,-692,-664,-636,-609,-582,-555,-530,-504,-479,-455,-431,-408,-385,-362,-340,-318,-297,-277,-256,-237,-217,-198,-180,-162,-144,-127,-110,-94,-78,-63,-47,-33,-18,-4,9,22,35,48,60,72,83,94,105,115,125,135,144,153,162,170,178,186,193,201,208,214,220,226,232,238,243,248,253,257,261,265,269,272,276,279,281,284,286,288,290,292,294,295,296,297,298,298,299,299,299,299,299,298,298,297,296,295,294,293,292,290,288,287,285,283,281,279,276,274,271,269,266,263,260,258,255,251,248,245,242,239,235,232,228,225,221,217,214,210,206,202,199,195,191,187};

#endif //_Filter_110kHz_H 

