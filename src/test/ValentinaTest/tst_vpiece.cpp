/************************************************************************
 **
 **  @file   tst_vdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "tst_vpiece.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/vpassmark.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"

#include <QtTest>
#include "../vpatterndb/vpiece.h"

namespace
{

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest1()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest2()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, 331.313196850393751);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest3()
{
    QVector<QPointF> points;

    points += QPointF(-862.690254965683266, 340.819961100893522);
    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-818.092955780502393, 371.279575916538420);
    points += QPointF(-823.692975528084730, 369.271805550997954);
    points += QPointF(-829.575336882823422, 366.626948794191208);
    points += QPointF(-835.815139714856855, 362.892089667033019);
    points += QPointF(-862.690254965683266, 340.819961100893522);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest4()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest5()
{
    QVector<QPointF> points;

    points += QPointF(-838.702607797801647, 360.520655492237381);
    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-818.092955780502393, 371.279575916538420);
    points += QPointF(-823.692975528084730, 369.271805550997954);
    points += QPointF(-829.575336882823422, 366.626948794191208);
    points += QPointF(-835.815139714856855, 362.892089667033019);
    points += QPointF(-838.702607797801647, 360.520655492237381);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest6()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> Issue924SeamAllowanceTest1_1()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}
}  // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
TST_VPiece::TST_VPiece(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::Issue620()
{
    // See file <root>/src/app/share/collection/bugs/Issue_#620.vit
    // Check main path
    const Unit unit = Unit::Cm;
    QSharedPointer<VContainer> data(new VContainer(nullptr, &unit, VContainer::UniqueNamespace()));
    qApp->setPatternUnit(unit);

    VPiece detail;
    AbstractTest::PieceFromJson(QStringLiteral("://Issue_620/input.json"), detail, data);

    const QVector<QPointF> pointsEkv = detail.MainPathPoints(data.data());

    QVector<QPointF> origPoints;
    AbstractTest::VectorFromJson(QStringLiteral("://Issue_620/output.json"), origPoints);

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::TestSAPassmark_data()
{
    QTest::addColumn<VPiecePassmarkData>("passmarkData");
    QTest::addColumn<QVector<QPointF>>("seamAllowance");
    QTest::addColumn<QVector<QLineF>>("expectedResult");

    VSAPoint nextSAPoint(-814.7149606299213, -8.844283464566928);
    nextSAPoint.SetSABefore(0);
    nextSAPoint.SetSAAfter(37.795275590551185);

    VSAPoint passmarkSAPoint(-814.7149606299213, 331.31319685039375);
    passmarkSAPoint.SetSABefore(37.795275590551185);
    passmarkSAPoint.SetSAAfter(0);

    VSAPoint previousSAPoint(-813.9961742743915, 331.90352529002166);
    previousSAPoint.SetSABefore(37.795275590551185);
    previousSAPoint.SetSAAfter(37.795275590551185);
    previousSAPoint.SetAngleType(PieceNodeAngle::ByLengthCurve);

    VPiecePassmarkData passmarkData;
    passmarkData.previousSAPoint = previousSAPoint;
    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.nextSAPoint = nextSAPoint;
    passmarkData.saWidth = 37.795275590551185;
    passmarkData.nodeName = QStringLiteral("А4");
    passmarkData.pieceName = QStringLiteral("Test 1");
    passmarkData.passmarkLineType = PassmarkLineType::TMark;
    passmarkData.passmarkAngleType = PassmarkAngleType::Bisector;

    QVector<QLineF> lines = {QLineF(QPointF(-814.7149606299213, 372.2542552634526),
                                    QPointF(-799.6132275149633, 360.8938692151714)),
                             QLineF(QPointF(-803.8733722830686, 355.23071929706197),
                                    QPointF(-795.353082746858, 366.5570191332808))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 1.") << passmarkData << Issue924SeamAllowanceTest1() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::ByPointsIntersection);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 2");

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 2.") << passmarkData << Issue924SeamAllowanceTest2() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 3");

    lines = {QLineF(QPointF(-862.6902549656833, 340.8199611008935),
                    QPointF(-844.3614525403156, 345.42166903309004)),
            QLineF(QPointF(-842.6358120657416, 338.54836812357695),
                   QPointF(-846.0870930148895, 352.29496994260313))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 3.") << passmarkData << Issue924SeamAllowanceTest3() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 4");

    lines = {QLineF(QPointF(-814.7149606299213, 372.2542552634526),
                    QPointF(-799.6132275149633, 360.8938692151714)),
             QLineF(QPointF(-803.8733722830686, 355.23071929706197),
                    QPointF(-795.353082746858, 366.5570191332808))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 4.") << passmarkData << Issue924SeamAllowanceTest4() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 5");

    lines = {QLineF(QPointF(-838.7026077978016, 360.5206554922374),
                    QPointF(-821.3650156381559, 353.002104278923)),
             QLineF(QPointF(-824.1844723431489, 346.50050721905575),
                    QPointF(-818.545558933163, 359.5037013387903))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 5.") << passmarkData << Issue924SeamAllowanceTest5() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 6");

    lines = {QLineF(QPointF(-814.7149606299213, 372.2542552634526),
                    QPointF(-799.6132275149633, 360.8938692151714)),
             QLineF(QPointF(-803.8733722830686, 355.23071929706197),
                    QPointF(-795.353082746858, 366.5570191332808))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 6.") << passmarkData << Issue924SeamAllowanceTest6() << lines;

    passmarkSAPoint.SetAngleType(PieceNodeAngle::ByLength);

    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.pieceName = QStringLiteral("Test 1.1");

    lines = {QLineF(QPointF(-814.7149606299213, 372.2542552634526),
                    QPointF(-799.6132275149633, 360.8938692151714)),
             QLineF(QPointF(-803.8733722830686, 355.23071929706197),
                    QPointF(-795.353082746858, 366.5570191332808))};

    // See file src/app/share/collection/bugs/Issue_#924.val
    QTest::newRow("Test 1.1.") << passmarkData << Issue924SeamAllowanceTest1_1() << lines;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::TestSAPassmark()
{
    QFETCH(VPiecePassmarkData, passmarkData);
    QFETCH(QVector<QPointF>, seamAllowance);
    QFETCH(QVector<QLineF>, expectedResult);

    VPassmark passmark(passmarkData);

    Comparison(passmark.SAPassmark(seamAllowance, PassmarkSide::All), expectedResult);
}
