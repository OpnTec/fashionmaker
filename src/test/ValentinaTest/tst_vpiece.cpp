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
void TST_VPiece::ClearLoop()
{
    // Input data taken from real case
    // See file <root>/src/app/share/collection/jacketМ6_30-110.val
    // Check correct seam allowance
    const Unit unit = Unit::Mm;
    QScopedPointer<VContainer> data(new VContainer(nullptr, &unit, VContainer::UniqueNamespace()));
    qApp->setPatternUnit(unit);

    data->UpdateGObject(304, new VPointF(61.866708661417327, 446.92270866141735, "Ф1", 5.0000125984251973,
                                         9.9999874015748045));
    data->UpdateGObject(307, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    data->UpdateGObject(56, new VPointF(802.08718110236236, 850.6707401574804, "Г6", 20.733316535433072,
                                        18.132850393700789));
    data->UpdateGObject(57, new VPointF(690.47666217505162, 804.29700711628709, "З", -11.505637795275591,
                                        31.221543307086616));
    data->UpdateGObject(203, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    QVector<VFSplinePoint> points;

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(203);
        VFSplinePoint p(*point.data(), 0.79455646129695412, 449.62747641208136, 1.6867283804609809, 269.62747641208136);
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(57);
        VFSplinePoint p(*point.data(), 0.4456850846354396, 120.24000000000034, 1.0255399999999999, 300.24000000000035);
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(56);
        VFSplinePoint p(*point.data(), 1.0085299999999999, 184.58891, 1, 4.5889100000000003);
        points.append(p);
    }

    {
        VSplinePath *path = new VSplinePath(points);
        path->SetApproximationScale(10);
        data->UpdateGObject(308, path);
    }

    data->UpdateGObject(309, new VPointF(799.45989815267649, 850.6707401574804, "Г8", -30.431206299212597,
                                         29.487155905511813));
    data->UpdateGObject(310, new VPointF(802.08718110236236, 1653.9337322834645, "Н5", 5.0000125984251973,
                                         9.9999874015748045));

    VPiece detail;
    detail.SetSeamAllowance(true);
    detail.SetSAWidth(7);
    detail.GetPath().Append(VPieceNode(304, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(307, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(308, Tool::NodeSplinePath));
    detail.GetPath().Append(VPieceNode(309, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(310, Tool::NodePoint));
    // Closed
    detail.GetPath()[0].SetFormulaSABefore(QChar('0'));
    detail.GetPath()[detail.GetPath().CountNodes()-1].SetFormulaSAAfter(QChar('0'));

    const QVector<QPointF> pointsEkv = detail.SeamAllowancePoints(data.data());

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(42.46405659601942, 415.2845470563873));
    origPoints.append(QPointF(669.53545906065, 560.2060851721218));
    origPoints.append(QPointF(669.4001315082384, 584.8051149911265));
    origPoints.append(QPointF(669.3971506600346, 591.5358644500567));
    origPoints.append(QPointF(669.4350122627271, 598.0891335031142));
    origPoints.append(QPointF(669.512757618795, 604.4805090719773));
    origPoints.append(QPointF(669.6294287899757, 610.7123442956477));
    origPoints.append(QPointF(669.7840625660031, 616.7869939551731));
    origPoints.append(QPointF(669.9756903141416, 622.7068151490457));
    origPoints.append(QPointF(670.2033378543063, 628.4741680595723));
    origPoints.append(QPointF(670.4660253660076, 634.0914168000103));
    origPoints.append(QPointF(670.7627673353721, 639.5609303670992));
    origPoints.append(QPointF(671.0925725508636, 644.88508369749));
    origPoints.append(QPointF(671.4544441569424, 650.0662588248562));
    origPoints.append(QPointF(671.8473797781523, 655.1068461674331));
    origPoints.append(QPointF(672.2703717242233, 660.0092459230049));
    origPoints.append(QPointF(672.7224072914591, 664.7758696009033));
    origPoints.append(QPointF(673.2024691737167, 669.4091416739334));
    origPoints.append(QPointF(673.7095360004373, 673.9115013689292));
    origPoints.append(QPointF(674.2425830172843, 678.2854045794492));
    origPoints.append(QPointF(674.8005829289502, 682.5333259132697));
    origPoints.append(QPointF(675.3825069209878, 686.6577608536486));
    origPoints.append(QPointF(675.9873258816623, 690.6612280423046));
    origPoints.append(QPointF(676.6140118402975, 694.5462716540186));
    origPoints.append(QPointF(677.2615396418696, 698.3154638589637));
    origPoints.append(QPointF(677.9288888734632, 701.971407344139));
    origPoints.append(QPointF(678.615046057566, 705.5167378711579));
    origPoints.append(QPointF(679.3190071217258, 708.9541268298552));
    origPoints.append(QPointF(680.0397801538563, 712.2862837635739));
    origPoints.append(QPointF(680.7763884409013, 715.5159588051306));
    origPoints.append(QPointF(681.527873788316, 718.6459449906465));
    origPoints.append(QPointF(682.2933001036281, 721.6790803846292));
    origPoints.append(QPointF(683.0717572225838, 724.6182499679669));
    origPoints.append(QPointF(683.8623649413778, 727.4663872197892));
    origPoints.append(QPointF(684.6642772103369, 730.2264753386498));
    origPoints.append(QPointF(685.4688320979097, 732.8756855713231));
    origPoints.append(QPointF(686.7042297790314, 736.7448337001608));
    origPoints.append(QPointF(688.3922766601711, 741.6485656582661));
    origPoints.append(QPointF(690.1014239938168, 746.2438997099595));
    origPoints.append(QPointF(691.8365445991736, 750.5829691271946));
    origPoints.append(QPointF(693.5947375993189, 754.6929542771874));
    origPoints.append(QPointF(695.374143963642, 758.6015018634648));
    origPoints.append(QPointF(697.173895570668, 762.3365598413925));
    origPoints.append(QPointF(698.9939856776302, 765.9262007023783));
    origPoints.append(QPointF(700.8350559377793, 769.3984479446959));
    origPoints.append(QPointF(702.6981018570923, 772.781118895178));
    origPoints.append(QPointF(704.5751154394404, 776.0858639250417));
    origPoints.append(QPointF(707.471075434013, 781.0529292076153));
    origPoints.append(QPointF(711.3732953060202, 787.6314568397516));
    origPoints.append(QPointF(713.2933193388338, 790.9045911620659));
    origPoints.append(QPointF(713.7531687634975, 791.6875604681287));
    origPoints.append(QPointF(714.6678160659043, 793.1880160336813));
    origPoints.append(QPointF(715.5686285678618, 794.6014031881614));
    origPoints.append(QPointF(716.4776602288559, 795.9656869340208));
    origPoints.append(QPointF(717.3946261026655, 797.2821365366428));
    origPoints.append(QPointF(718.3193027545042, 798.5520602886423));
    origPoints.append(QPointF(719.2515331337012, 799.7768007322334));
    origPoints.append(QPointF(720.1912309193016, 800.9577290368121));
    origPoints.append(QPointF(721.1383841967548, 802.0962385692094));
    origPoints.append(QPointF(722.0930583323086, 803.1937377265572));
    origPoints.append(QPointF(723.0553979184381, 804.2516421248051));
    origPoints.append(QPointF(724.0256276824568, 805.2713662661918));
    origPoints.append(QPointF(725.0040522692885, 806.2543148301429));
    origPoints.append(QPointF(725.9910548373568, 807.2018737544394));
    origPoints.append(QPointF(726.9870944370721, 808.1154012887525));
    origPoints.append(QPointF(727.992702176212, 808.9962192129983));
    origPoints.append(QPointF(729.0084762114801, 809.8456044148575));
    origPoints.append(QPointF(730.0350756421151, 810.6647810168413));
    origPoints.append(QPointF(731.0732134148514, 811.4549132304943));
    origPoints.append(QPointF(732.1236483836559, 812.217099098663));
    origPoints.append(QPointF(733.1871766854633, 812.9523652550562));
    origPoints.append(QPointF(734.2646226265674, 813.6616628101706));
    origPoints.append(QPointF(735.3568292693044, 814.345864425637));
    origPoints.append(QPointF(736.4646489305214, 815.0057626152854));
    origPoints.append(QPointF(737.5889337869056, 815.6420692657007));
    origPoints.append(QPointF(738.7305267800202, 816.2554163399695));
    origPoints.append(QPointF(739.890252994274, 816.8463576948574));
    origPoints.append(QPointF(741.0689116577936, 817.4153719140883));
    origPoints.append(QPointF(742.2672688927632, 817.9628660401196));
    origPoints.append(QPointF(743.4860513115749, 818.4891800699529));
    origPoints.append(QPointF(744.7259405221598, 818.994592069369));
    origPoints.append(QPointF(745.9470287398851, 819.4637478954073));
    origPoints.append(QPointF(747.8833122349788, 820.1566508068018));
    origPoints.append(QPointF(750.5850392329681, 821.0169732509104));
    origPoints.append(QPointF(753.3408618817797, 821.7832053775238));
    origPoints.append(QPointF(756.1945063236712, 822.4686248864464));
    origPoints.append(QPointF(759.1474180444699, 823.0731242050008));
    origPoints.append(QPointF(762.2002204282861, 823.5965365148435));
    origPoints.append(QPointF(765.3528023393483, 824.0387539700189));
    origPoints.append(QPointF(768.6044162317405, 824.399818281789));
    origPoints.append(QPointF(771.9537790264915, 824.679985612587));
    origPoints.append(QPointF(775.3991699554742, 824.8797689928807));
    origPoints.append(QPointF(778.9385214242475, 824.999962059787));
    origPoints.append(QPointF(782.5695005442702, 825.041647992741));
    origPoints.append(QPointF(786.289580235633, 825.0061972612904));
    origPoints.append(QPointF(790.0960997334876, 824.8952573520692));
    origPoints.append(QPointF(793.9863149544959, 824.7107371151485));
    origPoints.append(QPointF(826.2455325542998, 822.6315469221747));
    origPoints.append(QPointF(828.6637938138197, 1697.2698270114065));
    origPoints.append(QPointF(42.46405659601942, 415.2845470563873));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
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
