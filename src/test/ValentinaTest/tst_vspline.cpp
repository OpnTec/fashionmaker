/************************************************************************
 **
 **  @file   tst_vspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tst_vspline.h"
#include "../vgeometry/vspline.h"
#include "../vmisc/logging.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VSpline::TST_VSpline(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints()
{
    // Input data taken from real case
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);
    spl.SetApproximationScale(10);

    QPointF begin(957.69885233364062, 943.84482037833141);
    QPointF end(681.33729132409951, 1815.7969526662778);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(957.6988523336406, 943.8448203783314));
    origPoints.append(QPointF(957.7835033607208, 960.6821452672991));
    origPoints.append(QPointF(957.8563218900781, 990.4995347990275));
    origPoints.append(QPointF(957.8215788398634, 1012.8262256910826));
    origPoints.append(QPointF(957.7504105087676, 1027.6925228407606));
    origPoints.append(QPointF(957.6319911650569, 1042.5415942520012));
    origPoints.append(QPointF(957.4606249017198, 1057.371893324951));
    origPoints.append(QPointF(957.2306158117447, 1072.181873459756));
    origPoints.append(QPointF(956.9362679881198, 1086.9699880565609));
    origPoints.append(QPointF(956.5718855238333, 1101.7346905155123));
    origPoints.append(QPointF(956.1317725118738, 1116.4744342367562));
    origPoints.append(QPointF(955.6102330452296, 1131.1876726204378));
    origPoints.append(QPointF(955.0015712168888, 1145.8728590667038));
    origPoints.append(QPointF(954.3000911198401, 1160.5284469756996));
    origPoints.append(QPointF(953.5000968470716, 1175.152889747571));
    origPoints.append(QPointF(952.5958924915716, 1189.7446407824636));
    origPoints.append(QPointF(951.5817821463287, 1204.3021534805239));
    origPoints.append(QPointF(950.452069904331, 1218.8238812418977));
    origPoints.append(QPointF(949.2010598585669, 1233.3082774667305));
    origPoints.append(QPointF(947.8230561020247, 1247.7537955551684));
    origPoints.append(QPointF(946.3123627276927, 1262.1588889073569));
    origPoints.append(QPointF(944.6632838285595, 1276.5220109234424));
    origPoints.append(QPointF(943.3275966669323, 1287.2644821857812));
    origPoints.append(QPointF(942.3939281609411, 1294.4131147543828));
    origPoints.append(QPointF(941.4224593260558, 1301.5503845265412));
    origPoints.append(QPointF(940.4124781738992, 1308.6760981772736));
    origPoints.append(QPointF(939.3632727160957, 1315.790062381599));
    origPoints.append(QPointF(938.2741309642681, 1322.8920838145352));
    origPoints.append(QPointF(937.1443409300405, 1329.9819691511007));
    origPoints.append(QPointF(935.9731906250361, 1337.0595250663137));
    origPoints.append(QPointF(934.7599680608785, 1344.1245582351921));
    origPoints.append(QPointF(933.5039612491914, 1351.1768753327542));
    origPoints.append(QPointF(932.2044582015981, 1358.2162830340185));
    origPoints.append(QPointF(930.8607469297224, 1365.242588014003));
    origPoints.append(QPointF(929.4721154451878, 1372.2555969477262));
    origPoints.append(QPointF(928.0378517596175, 1379.2551165102063));
    origPoints.append(QPointF(926.5572438846355, 1386.2409533764612));
    origPoints.append(QPointF(925.029579831865, 1393.2129142215094));
    origPoints.append(QPointF(923.4541476129298, 1400.1708057203691));
    origPoints.append(QPointF(921.8302352394535, 1407.1144345480584));
    origPoints.append(QPointF(920.1571307230593, 1414.0436073795959));
    origPoints.append(QPointF(918.4341220753709, 1420.9581308899994));
    origPoints.append(QPointF(916.6604973080118, 1427.8578117542872));
    origPoints.append(QPointF(914.8355444326057, 1434.7424566474779));
    origPoints.append(QPointF(912.9585514607759, 1441.6118722445894));
    origPoints.append(QPointF(911.0288064041463, 1448.46586522064));
    origPoints.append(QPointF(909.0455972743401, 1455.3042422506478));
    origPoints.append(QPointF(907.0082120829811, 1462.1268100096313));
    origPoints.append(QPointF(904.9159388416924, 1468.9333751726085));
    origPoints.append(QPointF(902.7680655620982, 1475.7237444145978));
    origPoints.append(QPointF(900.5638802558215, 1482.4977244106174));
    origPoints.append(QPointF(898.3026709344861, 1489.2551218356857));
    origPoints.append(QPointF(895.9837256097155, 1495.99574336482));
    origPoints.append(QPointF(893.6063322931332, 1502.7193956730398));
    origPoints.append(QPointF(891.1697789963628, 1509.425885435363));
    origPoints.append(QPointF(888.6733537310278, 1516.1150193268072));
    origPoints.append(QPointF(886.1163445087515, 1522.7866040223912));
    origPoints.append(QPointF(883.4980393411579, 1529.440446197133));
    origPoints.append(QPointF(880.8177262398704, 1536.0763525260513));
    origPoints.append(QPointF(878.0746932165123, 1542.6941296841637));
    origPoints.append(QPointF(875.2682282827075, 1549.2935843464886));
    origPoints.append(QPointF(872.3976194500792, 1555.8745231880448));
    origPoints.append(QPointF(869.4621547302513, 1562.4367528838495));
    origPoints.append(QPointF(866.4611221348471, 1568.9800801089218));
    origPoints.append(QPointF(863.39380967549, 1575.5043115382791));
    origPoints.append(QPointF(860.259505363804, 1582.0092538469407));
    origPoints.append(QPointF(857.057497211412, 1588.4947137099243));
    origPoints.append(QPointF(853.7870732299382, 1594.960497802248));
    origPoints.append(QPointF(850.447521431006, 1601.40641279893));
    origPoints.append(QPointF(847.0381298262384, 1607.8322653749888));
    origPoints.append(QPointF(843.5581864272597, 1614.2378622054425));
    origPoints.append(QPointF(840.0069792456928, 1620.623009965309));
    origPoints.append(QPointF(836.3837962931617, 1626.9875153296075));
    origPoints.append(QPointF(832.6879255812897, 1633.3311849733554));
    origPoints.append(QPointF(828.9186551217003, 1639.653825571571));
    origPoints.append(QPointF(825.0752729260173, 1645.9552437992727));
    origPoints.append(QPointF(821.1570670058641, 1652.2352463314787));
    origPoints.append(QPointF(817.1633253728643, 1658.4936398432073));
    origPoints.append(QPointF(813.0933360386413, 1664.7302310094765));
    origPoints.append(QPointF(808.9463870148186, 1670.944826505305));
    origPoints.append(QPointF(804.72176631302, 1677.1372330057104));
    origPoints.append(QPointF(800.4187619448687, 1683.3072571857115));
    origPoints.append(QPointF(796.0366619219888, 1689.4547057203263));
    origPoints.append(QPointF(791.5747542560034, 1695.5793852845732));
    origPoints.append(QPointF(787.032326958536, 1701.6811025534698));
    origPoints.append(QPointF(782.4086680412104, 1707.7596642020349));
    origPoints.append(QPointF(777.70306551565, 1713.8148769052868));
    origPoints.append(QPointF(772.9148073934782, 1719.8465473382437));
    origPoints.append(QPointF(768.0431816863188, 1725.8544821759235));
    origPoints.append(QPointF(763.0874764057953, 1731.8384880933447));
    origPoints.append(QPointF(758.0469795635311, 1737.7983717655252));
    origPoints.append(QPointF(752.9209791711501, 1743.733939867484));
    origPoints.append(QPointF(747.7087632402752, 1749.6449990742383));
    origPoints.append(QPointF(742.4096197825304, 1755.531356060807));
    origPoints.append(QPointF(737.0228368095393, 1761.3928175022083));
    origPoints.append(QPointF(731.5477023329252, 1767.2291900734604));
    origPoints.append(QPointF(725.9835043643118, 1773.0402804495811));
    origPoints.append(QPointF(720.3295309153225, 1778.8258953055893));
    origPoints.append(QPointF(714.5850699975811, 1784.5858413165029));
    origPoints.append(QPointF(708.7494096227108, 1790.31992515734));
    origPoints.append(QPointF(702.8218378023353, 1796.027953503119));
    origPoints.append(QPointF(696.8016425480782, 1801.7097330288584));
    origPoints.append(QPointF(690.688111871563, 1807.365070409576));
    origPoints.append(QPointF(684.4805337844132, 1812.9937723202902));
    origPoints.append(QPointF(681.3372913240995, 1815.7969526662778));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_issue412()
{
    // Input data taken from real case
    // See issue #412 https://bitbucket.org/dismine/valentina/issues/412/error-in-detail-using-segment-a-simple
    VPointF p1(869.11748031496063, -61.117228346456692, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(491.16472440944887, 316.83552755905515, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 270, 0, 1, 1, 1);
    spl.SetApproximationScale(10);

    QPointF begin(869.11748031496063, -61.117228346456692);
    QPointF end(758.41768107838425, 206.13572832247544);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(869.1174803149606, -61.11722834645669));
    origPoints.append(QPointF(869.1123162764883, -58.67229138738739));
    origPoints.append(QPointF(869.050533341976, -53.79466425451659));
    origPoints.append(QPointF(868.9273385272631, -48.93206721284792));
    origPoints.append(QPointF(868.743102886662, -44.08487131669338));
    origPoints.append(QPointF(868.4981974744846, -39.25344762036495));
    origPoints.append(QPointF(868.1929933450426, -34.43816717817464));
    origPoints.append(QPointF(867.8278615526483, -29.639401044434408));
    origPoints.append(QPointF(867.4031731516135, -24.85752027345624));
    origPoints.append(QPointF(866.9192991962501, -20.092895919552134));
    origPoints.append(QPointF(866.3766107408703, -15.345899037034071));
    origPoints.append(QPointF(865.775478839786, -10.616900680214039));
    origPoints.append(QPointF(865.1162745473094, -5.90627190340402));
    origPoints.append(QPointF(864.3993689177523, -1.214383760916003));
    origPoints.append(QPointF(863.6251330054265, 3.4583926929380264));
    origPoints.append(QPointF(862.7939378646442, 8.111686403846083));
    origPoints.append(QPointF(861.9061545497175, 12.74512631749618));
    origPoints.append(QPointF(860.9621541149579, 17.358341379576338));
    origPoints.append(QPointF(859.9623076146779, 21.95096053577456));
    origPoints.append(QPointF(858.9069861031894, 26.522612731778867));
    origPoints.append(QPointF(857.796560634804, 31.07292691327727));
    origPoints.append(QPointF(856.6314022638342, 35.60153202595778));
    origPoints.append(QPointF(855.4118820445917, 40.10805701550843));
    origPoints.append(QPointF(854.1383710313885, 44.59213082761722));
    origPoints.append(QPointF(852.8112402785367, 49.05338240797216));
    origPoints.append(QPointF(851.4308608403481, 53.49144070226127));
    origPoints.append(QPointF(849.997603771135, 57.90593465617256));
    origPoints.append(QPointF(848.5118401252091, 62.29649321539405));
    origPoints.append(QPointF(846.9739409568825, 66.66274532561374));
    origPoints.append(QPointF(845.384277320467, 71.00431993251965));
    origPoints.append(QPointF(843.743220270275, 75.32084598179983));
    origPoints.append(QPointF(842.0511408606181, 79.61195241914226));
    origPoints.append(QPointF(840.3084101458085, 83.87726819023496));
    origPoints.append(QPointF(838.5153991801578, 88.1164222407659));
    origPoints.append(QPointF(836.6724790179786, 92.32904351642317));
    origPoints.append(QPointF(834.7800207135825, 96.51476096289475));
    origPoints.append(QPointF(832.8383953212815, 100.67320352586866));
    origPoints.append(QPointF(830.8479738953877, 104.80400015103291));
    origPoints.append(QPointF(828.8091274902131, 108.9067797840755));
    origPoints.append(QPointF(826.7222271600695, 112.98117137068445));
    origPoints.append(QPointF(824.5876439592689, 117.0268038565478));
    origPoints.append(QPointF(822.4057489421236, 121.04330618735354));
    origPoints.append(QPointF(820.1769131629453, 125.03030730878969));
    origPoints.append(QPointF(817.9015076760461, 128.98743616654426));
    origPoints.append(QPointF(815.5799035357379, 132.91432170630523));
    origPoints.append(QPointF(813.2124717963329, 136.8105928737607));
    origPoints.append(QPointF(810.7995835121426, 140.67587861459862));
    origPoints.append(QPointF(808.3416097374795, 144.50980787450703));
    origPoints.append(QPointF(805.8389215266553, 148.3120095991739));
    origPoints.append(QPointF(803.2918899339824, 152.08211273428736));
    origPoints.append(QPointF(800.7008860137721, 155.8197462255353));
    origPoints.append(QPointF(798.0662808203368, 159.52453901860576));
    origPoints.append(QPointF(795.3884454079885, 163.19612005918677));
    origPoints.append(QPointF(792.667750831039, 166.83411829296637));
    origPoints.append(QPointF(789.9045681438006, 170.43816266563252));
    origPoints.append(QPointF(787.0992684005848, 174.00788212287327));
    origPoints.append(QPointF(784.2522226557041, 177.54290561037666));
    origPoints.append(QPointF(781.3638019634705, 181.0428620738306));
    origPoints.append(QPointF(778.4343773781953, 184.50738045892322));
    origPoints.append(QPointF(775.4643199541911, 187.9360897113425));
    origPoints.append(QPointF(772.4540007457698, 191.32861877677647));
    origPoints.append(QPointF(769.4037908072432, 194.6845966009131));
    origPoints.append(QPointF(766.3140611929234, 198.0036521294404));
    origPoints.append(QPointF(763.1851829571224, 201.28541430804643));
    origPoints.append(QPointF(760.0175271541522, 204.52951208241916));
    origPoints.append(QPointF(758.4176810783842, 206.13572832247544));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_TestPuzzle()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/TestPuzzle.val
    // Test issue with method IsPointOnLineSegment.
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);
    spl.SetApproximationScale(10);

    QPointF begin(1168.85828031, 39.9998740157);
    QPointF end(957.69883966, 943.844812978);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(1168.8582803149607, 39.999874015748034));
    origPoints.append(QPointF(1166.223927144099, 43.081713107423184));
    origPoints.append(QPointF(1161.0274928341657, 49.26409940982961));
    origPoints.append(QPointF(1155.9171441728415, 55.46876146261986));
    origPoints.append(QPointF(1150.8921691717505, 61.695505940812126));
    origPoints.append(QPointF(1145.9518558425152, 67.94413951942465));
    origPoints.append(QPointF(1141.0954921967605, 74.21446887347568));
    origPoints.append(QPointF(1136.3223662461087, 80.50630067798342));
    origPoints.append(QPointF(1131.6317660021837, 86.8194416079661));
    origPoints.append(QPointF(1127.0229794766096, 93.15369833844201));
    origPoints.append(QPointF(1122.4952946810095, 99.50887754442931));
    origPoints.append(QPointF(1118.0479996270067, 105.88478590094627));
    origPoints.append(QPointF(1113.680382326225, 112.28123008301112));
    origPoints.append(QPointF(1109.3917307902882, 118.6980167656421));
    origPoints.append(QPointF(1105.1813330308196, 125.13495262385743));
    origPoints.append(QPointF(1101.048477059443, 131.5918443326753));
    origPoints.append(QPointF(1096.9924508877814, 138.06849856711403));
    origPoints.append(QPointF(1093.0125425274584, 144.56472200219181));
    origPoints.append(QPointF(1089.108039990098, 151.08032131292686));
    origPoints.append(QPointF(1085.2782312873235, 157.61510317433738));
    origPoints.append(QPointF(1081.5224044307583, 164.16887426144172));
    origPoints.append(QPointF(1077.8398474320263, 170.741441249258));
    origPoints.append(QPointF(1074.2298483027507, 177.3326108128045));
    origPoints.append(QPointF(1070.691695054555, 183.94218962709942));
    origPoints.append(QPointF(1067.2246756990635, 190.56998436716106));
    origPoints.append(QPointF(1063.8280782478992, 197.21580170800758));
    origPoints.append(QPointF(1060.5011907126852, 203.8794483246572));
    origPoints.append(QPointF(1057.2433011050455, 210.56073089212825));
    origPoints.append(QPointF(1054.0536974366037, 217.2594560854389));
    origPoints.append(QPointF(1050.9316677189831, 223.9754305796074));
    origPoints.append(QPointF(1047.8764999638074, 230.70846104965193));
    origPoints.append(QPointF(1044.8874821827003, 237.45835417059072));
    origPoints.append(QPointF(1041.9639023872846, 244.2249166174421));
    origPoints.append(QPointF(1039.1050485891847, 251.00795506522425));
    origPoints.append(QPointF(1036.3102088000242, 257.80727618895537));
    origPoints.append(QPointF(1033.5786710314258, 264.6226866636538));
    origPoints.append(QPointF(1030.909723295014, 271.4539931643376));
    origPoints.append(QPointF(1028.3026536024117, 278.30100236602505));
    origPoints.append(QPointF(1025.7567499652423, 285.1635209437345));
    origPoints.append(QPointF(1023.2713003951301, 292.0413555724841));
    origPoints.append(QPointF(1020.8455929036979, 298.9343129272922));
    origPoints.append(QPointF(1018.4789155025696, 305.84219968317683));
    origPoints.append(QPointF(1016.1705562033686, 312.7648225151563));
    origPoints.append(QPointF(1013.9198030177188, 319.7019880982489));
    origPoints.append(QPointF(1011.7259439572433, 326.6535031074728));
    origPoints.append(QPointF(1009.5882670335659, 333.61917421784625));
    origPoints.append(QPointF(1007.5060602583101, 340.59880810438744));
    origPoints.append(QPointF(1005.4786116430994, 347.5922114421147));
    origPoints.append(QPointF(1003.5052091995572, 354.59919090604626));
    origPoints.append(QPointF(1001.5851409393073, 361.6195531712002));
    origPoints.append(QPointF(999.7176948739731, 368.6531049125949));
    origPoints.append(QPointF(997.902159015178, 375.69965280524866));
    origPoints.append(QPointF(996.1378213745459, 382.75900352417943));
    origPoints.append(QPointF(994.4239699637, 389.8309637444057));
    origPoints.append(QPointF(992.759892794264, 396.91534014094555));
    origPoints.append(QPointF(991.1448778778615, 404.0119393888174));
    origPoints.append(QPointF(989.5782132261161, 411.12056816303925));
    origPoints.append(QPointF(988.0591868506513, 418.2410331386294));
    origPoints.append(QPointF(986.5870867630903, 425.3731409906063));
    origPoints.append(QPointF(985.161200975057, 432.5166983939879));
    origPoints.append(QPointF(983.7808174981748, 439.6715120237925));
    origPoints.append(QPointF(982.4452243440672, 446.8373885550384));
    origPoints.append(QPointF(981.1537095243581, 454.0141346627438));
    origPoints.append(QPointF(979.9055610506706, 461.201557021927));
    origPoints.append(QPointF(978.7000669346284, 468.3994623076061));
    origPoints.append(QPointF(977.536515187855, 475.6076571947994));
    origPoints.append(QPointF(976.4141938219741, 482.82594835852524));
    origPoints.append(QPointF(975.3323908486091, 490.0541424738017));
    origPoints.append(QPointF(974.2903942793837, 497.292046215647));
    origPoints.append(QPointF(973.2874921259213, 504.5394662590794));
    origPoints.append(QPointF(972.3229723998454, 511.7962092791172));
    origPoints.append(QPointF(970.9420270804521, 522.6972765338926));
    origPoints.append(QPointF(969.2347383321023, 537.2643805338191));
    origPoints.append(QPointF(967.6681635025225, 551.8652965904953));
    origPoints.append(QPointF(966.2366066847012, 566.4984781040671));
    origPoints.append(QPointF(964.9343719716264, 581.1623784746802));
    origPoints.append(QPointF(963.7557634562868, 595.8554511024805));
    origPoints.append(QPointF(962.6950852316704, 610.5761493876139));
    origPoints.append(QPointF(961.746641390766, 625.3229267302262));
    origPoints.append(QPointF(960.9047360265615, 640.0942365304633));
    origPoints.append(QPointF(960.1636732320455, 654.8885321884709));
    origPoints.append(QPointF(959.5177571002063, 669.7042671043953));
    origPoints.append(QPointF(958.9612917240322, 684.5398946783818));
    origPoints.append(QPointF(958.4885811965113, 699.3938683105766));
    origPoints.append(QPointF(958.0939296106322, 714.2646414011255));
    origPoints.append(QPointF(957.7716410593832, 729.1506673501743));
    origPoints.append(QPointF(957.5160196357529, 744.0503995578688));
    origPoints.append(QPointF(957.3213694327292, 758.9622914243553));
    origPoints.append(QPointF(957.1819945433006, 773.884796349779));
    origPoints.append(QPointF(957.0921990604554, 788.8163677342864));
    origPoints.append(QPointF(957.0335899720619, 811.2266912397164));
    origPoints.append(QPointF(957.0831245617323, 841.1256735649813));
    origPoints.append(QPointF(957.2284504515269, 871.0330829292052));
    origPoints.append(QPointF(957.5286354562604, 915.885745295907));
    origPoints.append(QPointF(957.69883966, 943.844812978));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_NullSegment()
{
    // Test check case where a detail contains one spline and one point on a spline.

    const VPointF p1(146.3718263928647, 6.4192815800656247, "A1", 5.0000125984251973, 9.9999874015748045);
    const QPointF p2(-231.44352755905513, -3.7711067716535434);
    const QPointF p3(-194.55587416421278, 355.17175586626462);
    const VPointF p4(182.31062201967654, 383.80617135964712, "A2", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p2, p3, p4, 1);
    spl.SetApproximationScale(10);

    const QPointF begin	(146.3718263928647, 6.419281580065625);
    const QPointF end	(146.3718263928647, 6.419281580065625);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, true);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(182.31062201967654, 383.8061713596471));
    origPoints.append(QPointF(177.90459309841725, 383.4605315892429));
    origPoints.append(QPointF(169.19639545863012, 382.66945046939884));
    origPoints.append(QPointF(160.6130224709154, 381.76040846158674));
    origPoints.append(QPointF(152.1546879228228, 380.73540668455666));
    origPoints.append(QPointF(143.82160560190192, 379.5964462570587));
    origPoints.append(QPointF(135.61398929570254, 378.34552829784275));
    origPoints.append(QPointF(127.53205279177418, 376.98465392565885));
    origPoints.append(QPointF(119.57600987766655, 375.51582425925693));
    origPoints.append(QPointF(111.74607434092933, 373.9410404173871));
    origPoints.append(QPointF(104.04245996911216, 372.2623035187994));
    origPoints.append(QPointF(96.46538054976472, 370.4816146822436));
    origPoints.append(QPointF(89.01504987043666, 368.6009750264699));
    origPoints.append(QPointF(81.69168171867761, 366.62238567022825));
    origPoints.append(QPointF(74.4954898820373, 364.54784773226874));
    origPoints.append(QPointF(67.42668814806535, 362.3793623313413));
    origPoints.append(QPointF(60.485490304311426, 360.1189305861958));
    origPoints.append(QPointF(53.67211013832518, 357.7685536155825));
    origPoints.append(QPointF(46.98676143765628, 355.33023253825127));
    origPoints.append(QPointF(40.42965798985438, 352.805968472952));
    origPoints.append(QPointF(34.00101358246915, 350.19776253843486));
    origPoints.append(QPointF(27.70104200305024, 347.5076158534498));
    origPoints.append(QPointF(21.529957039147334, 344.7375295367468));
    origPoints.append(QPointF(15.48797247831007, 341.88950470707584));
    origPoints.append(QPointF(9.575302108088117, 338.96554248318705));
    origPoints.append(QPointF(3.7921597160311356, 335.96764398383027));
    origPoints.append(QPointF(-1.8612409103112126, 332.8978103277557));
    origPoints.append(QPointF(-7.38468598338927, 329.75804263371316));
    origPoints.append(QPointF(-12.777961715653369, 326.55034202045266));
    origPoints.append(QPointF(-18.04085431955386, 323.2767096067243));
    origPoints.append(QPointF(-23.173150007541068, 319.93914651127807));
    origPoints.append(QPointF(-28.174634992065343, 316.5396538528639));
    origPoints.append(QPointF(-33.04509548557702, 313.08023275023186));
    origPoints.append(QPointF(-37.784317700526444, 309.5628843221318));
    origPoints.append(QPointF(-42.392087849363946, 305.98960968731404));
    origPoints.append(QPointF(-46.86819214453986, 302.3624099645283));
    origPoints.append(QPointF(-51.21241679850455, 298.6832862725247));
    origPoints.append(QPointF(-55.424548023708326, 294.9542397300532));
    origPoints.append(QPointF(-59.50437203260155, 291.1772714558638));
    origPoints.append(QPointF(-63.45167503763454, 287.35438256870657));
    origPoints.append(QPointF(-67.26624325125766, 283.48757418733135));
    origPoints.append(QPointF(-70.94786288592123, 279.5788474304884));
    origPoints.append(QPointF(-74.49632015407558, 275.6302034169275));
    origPoints.append(QPointF(-77.91140126817108, 271.64364326539874));
    origPoints.append(QPointF(-81.19289244065806, 267.62116809465215));
    origPoints.append(QPointF(-84.34057988398683, 263.56477902343767));
    origPoints.append(QPointF(-87.35424981060777, 259.4764771705053));
    origPoints.append(QPointF(-90.2336884329712, 255.3582636546051));
    origPoints.append(QPointF(-92.97868196352746, 251.212139594487));
    origPoints.append(QPointF(-95.58901661472689, 247.0401061089011));
    origPoints.append(QPointF(-98.06447859901981, 242.84416431659736));
    origPoints.append(QPointF(-100.40485412885658, 238.62631533632572));
    origPoints.append(QPointF(-102.60992941668755, 234.38856028683622));
    origPoints.append(QPointF(-104.67949067496303, 230.1329002868789));
    origPoints.append(QPointF(-106.61332411613338, 225.86133645520374));
    origPoints.append(QPointF(-108.41121595264894, 221.57586991056075));
    origPoints.append(QPointF(-110.07295239696006, 217.2785017716999));
    origPoints.append(QPointF(-111.59831966151705, 212.97123315737122));
    origPoints.append(QPointF(-112.98710395877025, 208.65606518632472));
    origPoints.append(QPointF(-114.23909150117002, 204.33499897731036));
    origPoints.append(QPointF(-115.3540685011667, 200.0100356490782));
    origPoints.append(QPointF(-116.3318211712106, 195.68317632037815));
    origPoints.append(QPointF(-117.17213572375209, 191.35642210996033));
    origPoints.append(QPointF(-117.8747983712415, 187.03177413657468));
    origPoints.append(QPointF(-118.43959532612914, 182.71123351897123));
    origPoints.append(QPointF(-118.86631280086539, 178.39680137589994));
    origPoints.append(QPointF(-119.15473700790058, 174.09047882611083));
    origPoints.append(QPointF(-119.30465415968504, 169.7942669883539));
    origPoints.append(QPointF(-119.31585046866913, 165.5101669813792));
    origPoints.append(QPointF(-119.18811214730314, 161.2401799239367));
    origPoints.append(QPointF(-118.92122540803749, 156.98630693477634));
    origPoints.append(QPointF(-118.51497646332243, 152.7505491326482));
    origPoints.append(QPointF(-117.96915152560837, 148.53490763630228));
    origPoints.append(QPointF(-117.2835368073456, 144.34138356448852));
    origPoints.append(QPointF(-116.45791852098446, 140.171978035957));
    origPoints.append(QPointF(-115.49208287897534, 136.02869216945766));
    origPoints.append(QPointF(-114.38581609376854, 131.91352708374055));
    origPoints.append(QPointF(-113.13890437781438, 127.82848389755566));
    origPoints.append(QPointF(-111.75113394356325, 123.77556372965296));
    origPoints.append(QPointF(-110.22229100346546, 119.75676769878248));
    origPoints.append(QPointF(-108.55216176997135, 115.77409692369423));
    origPoints.append(QPointF(-106.74053245553128, 111.8295525231382));
    origPoints.append(QPointF(-104.78718927259555, 107.9251356158644));
    origPoints.append(QPointF(-102.69191843361452, 104.06284732062281));
    origPoints.append(QPointF(-100.45450615103854, 100.24468875616346));
    origPoints.append(QPointF(-98.07473863731795, 96.47266104123635));
    origPoints.append(QPointF(-95.55240210490308, 92.74876529459146));
    origPoints.append(QPointF(-92.88728276624425, 89.0750026349788));
    origPoints.append(QPointF(-90.07916683379182, 85.45337418114838));
    origPoints.append(QPointF(-87.12784051999613, 81.88588105185019));
    origPoints.append(QPointF(-84.03309003730749, 78.37452436583426));
    origPoints.append(QPointF(-80.79470159817629, 74.92130524185059));
    origPoints.append(QPointF(-77.41246141505286, 71.52822479864915));
    origPoints.append(QPointF(-73.88615570038749, 68.19728415497994));
    origPoints.append(QPointF(-70.21557066663057, 64.93048442959301));
    origPoints.append(QPointF(-66.40049252623243, 61.72982674123833));
    origPoints.append(QPointF(-62.440707491643394, 58.5973122086659));
    origPoints.append(QPointF(-58.336001775313804, 55.53494195062573));
    origPoints.append(QPointF(-54.086161589694, 52.54471708586783));
    origPoints.append(QPointF(-49.69097314723432, 49.628638733142196));
    origPoints.append(QPointF(-45.15022266038511, 46.78870801119882));
    origPoints.append(QPointF(-40.463696341596716, 44.026926038787714));
    origPoints.append(QPointF(-35.63118040331946, 41.34529393465888));
    origPoints.append(QPointF(-30.652461058003688, 38.745812817562324));
    origPoints.append(QPointF(-25.52732451809974, 36.23048380624805));
    origPoints.append(QPointF(-20.255556996057955, 33.80130801946605));
    origPoints.append(QPointF(-14.836944704328669, 31.46028657596633));
    origPoints.append(QPointF(-9.271273855362228, 29.209420594498894));
    origPoints.append(QPointF(-3.558330661608968, 27.050711193813743));
    origPoints.append(QPointF(2.3020986644807735, 24.98615949266088));
    origPoints.append(QPointF(8.310227910456657, 23.01776660979031));
    origPoints.append(QPointF(14.466270863868342, 21.147533663952032));
    origPoints.append(QPointF(20.77044131226549, 19.37746177389605));
    origPoints.append(QPointF(27.22295304319776, 17.709552058372367));
    origPoints.append(QPointF(33.82401984421482, 16.145805636130987));
    origPoints.append(QPointF(40.573855502866316, 14.688223625921907));
    origPoints.append(QPointF(47.472673806701934, 13.338807146495132));
    origPoints.append(QPointF(54.5206885432713, 12.099557316600663));
    origPoints.append(QPointF(61.718113500124105, 10.972475254988508));
    origPoints.append(QPointF(69.06516246480999, 9.959562080408663));
    origPoints.append(QPointF(76.56204922487862, 9.062818911611132));
    origPoints.append(QPointF(84.20898756787969, 8.28424686734592));
    origPoints.append(QPointF(92.00619128136279, 7.625847066363029));
    origPoints.append(QPointF(99.95387415287765, 7.08962062741246));
    origPoints.append(QPointF(108.0522499699739, 6.6775686692442155));
    origPoints.append(QPointF(116.30153252020119, 6.391692310608296));
    origPoints.append(QPointF(124.70193559110919, 6.233992670254708));
    origPoints.append(QPointF(133.2536729702476, 6.206470866933452));
    origPoints.append(QPointF(141.95695844516604, 6.311128019394529));
    origPoints.append(QPointF(146.3718263928647, 6.419281580065625));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_RotateTool()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/bugs/IsPointOnLineSegment_RotateTool_issue.val
    // Test issue with method IsPointOnLineSegment.

    const VPointF p1(155.93961723681397, -42.472964170961042, "A", 5.0000125984251973, 9.9999874015748045);
    const VPointF p4(237.32422843061005, 485.80074940371367, "A2", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 231.11199999999994, "231.112", 145.33899999999997, "145.339", 207.44768503937021, "5.48872",
                337.50916535433066, "8.92993");
    spl.SetApproximationScale(10);

    const QPointF begin(237.32422843061005, 485.80074940371367);
    const QPointF end  (46.623829088412336, 167.78988631718659);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, true);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(237.32422843061005, 485.80074940371367));
    origPoints.append(QPointF(234.08145029385315, 483.5518788597857));
    origPoints.append(QPointF(227.70035384747374, 479.05934931212175));
    origPoints.append(QPointF(221.44388986541253, 474.573064051899));
    origPoints.append(QPointF(215.31125909406614, 470.0930124560183));
    origPoints.append(QPointF(209.3016622798313, 465.61918390138055));
    origPoints.append(QPointF(203.41430016910454, 461.1515677648864));
    origPoints.append(QPointF(197.64837350828248, 456.69015342343664));
    origPoints.append(QPointF(192.00308304376176, 452.23493025393225));
    origPoints.append(QPointF(186.4776295219391, 447.7858876332739));
    origPoints.append(QPointF(181.071213689211, 443.34301493836244));
    origPoints.append(QPointF(175.78303629197416, 438.90630154609863));
    origPoints.append(QPointF(170.6122980766252, 434.4757368333833));
    origPoints.append(QPointF(165.55819978956072, 430.05131017711733));
    origPoints.append(QPointF(160.61994217717742, 425.6330109542015));
    origPoints.append(QPointF(155.79672598587183, 421.22082854153655));
    origPoints.append(QPointF(151.0877519620406, 416.8147523160234));
    origPoints.append(QPointF(146.4922208520805, 412.4147716545626));
    origPoints.append(QPointF(142.00933340238794, 408.0208759340553));
    origPoints.append(QPointF(137.63829035935967, 403.6330545314022));
    origPoints.append(QPointF(133.37829246939233, 399.25129682350405));
    origPoints.append(QPointF(129.2285404788825, 394.87559218726165));
    origPoints.append(QPointF(125.18823513422683, 390.50592999957587));
    origPoints.append(QPointF(121.25657718182194, 386.1422996373475));
    origPoints.append(QPointF(117.43276736806445, 381.78469047747734));
    origPoints.append(QPointF(113.71600643935099, 377.43309189686613));
    origPoints.append(QPointF(110.10549514207823, 373.0874932724148));
    origPoints.append(QPointF(106.60043422264278, 368.7478839810241));
    origPoints.append(QPointF(103.2000244274412, 364.4142533995948));
    origPoints.append(QPointF(99.90346650287023, 360.08659090502783));
    origPoints.append(QPointF(96.70996119532643, 355.76488587422386));
    origPoints.append(QPointF(93.61870925120647, 351.4491276840837));
    origPoints.append(QPointF(90.6289114169069, 347.1393057115083));
    origPoints.append(QPointF(87.7397684388244, 342.8354093333984));
    origPoints.append(QPointF(84.95048106335562, 338.5374279266548));
    origPoints.append(QPointF(82.26025003689719, 334.2453508681782));
    origPoints.append(QPointF(79.66827610584568, 329.95916753486955));
    origPoints.append(QPointF(77.17376001659775, 325.67886730362966));
    origPoints.append(QPointF(74.77590251555006, 321.4044395513592));
    origPoints.append(QPointF(72.47390434909917, 317.13587365495914));
    origPoints.append(QPointF(70.26696626364176, 312.87315899133023));
    origPoints.append(QPointF(68.15428900557446, 308.6162849373733));
    origPoints.append(QPointF(66.13507332129386, 304.36524086998907));
    origPoints.append(QPointF(64.20851995719663, 300.12001616607847));
    origPoints.append(QPointF(62.37382965967939, 295.88060020254215));
    origPoints.append(QPointF(60.630203175138746, 291.64698235628106));
    origPoints.append(QPointF(58.97684124997134, 287.419152004196));
    origPoints.append(QPointF(57.412944630573804, 283.1970985231878));
    origPoints.append(QPointF(55.93771406334275, 278.98081129015713));
    origPoints.append(QPointF(54.55035029467483, 274.7702796820048));
    origPoints.append(QPointF(53.250054070966655, 270.5654930756318));
    origPoints.append(QPointF(52.036026138614865, 266.36644084793886));
    origPoints.append(QPointF(50.907467244016075, 262.1731123758268));
    origPoints.append(QPointF(49.86357813356693, 257.9854970361963));
    origPoints.append(QPointF(48.90355955366405, 253.80358420594834));
    origPoints.append(QPointF(48.02661225070406, 249.62736326198362));
    origPoints.append(QPointF(47.2319369710836, 245.45682358120297));
    origPoints.append(QPointF(46.51873446119927, 241.29195454050725));
    origPoints.append(QPointF(45.88620546744774, 237.1327455167972));
    origPoints.append(QPointF(45.333550736225604, 232.97918588697368));
    origPoints.append(QPointF(44.859971013929496, 228.83126502793746));
    origPoints.append(QPointF(44.46466704695606, 224.68897231658934));
    origPoints.append(QPointF(44.14683958170191, 220.5522971298302));
    origPoints.append(QPointF(43.905689364563685, 216.42122884456074));
    origPoints.append(QPointF(43.740417141938, 212.2957568376819));
    origPoints.append(QPointF(43.650223660221506, 208.17587048609442));
    origPoints.append(QPointF(43.634309665810804, 204.06155916669906));
    origPoints.append(QPointF(43.69187590510255, 199.95281225639673));
    origPoints.append(QPointF(43.822123124493345, 195.8496191320881));
    origPoints.append(QPointF(44.02425207037983, 191.75196917067416));
    origPoints.append(QPointF(44.297463489158645, 187.65985174905558));
    origPoints.append(QPointF(44.64095812722639, 183.5732562441332));
    origPoints.append(QPointF(45.05393673097973, 179.49217203280784));
    origPoints.append(QPointF(45.53560004681526, 175.41658849198035));
    origPoints.append(QPointF(46.085148821129636, 171.34649499855146));
    origPoints.append(QPointF(46.623829088412336, 167.78988631718659));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_issue767()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/bugs/Issue_#767.val
    // Test issue with method IsPointOnLineSegment.

    const QPointF begin(3964.650771379471, 3212.2173150777817);
    const QPointF end  (4200.023629188538, 2559.9039118110236);

    QVector<QPointF> points;
    points.append(QPointF(3964.650771379471, 3212.2173150777817));
    points.append(QPointF(4009.6319008613655, 3103.623262231279));
    points.append(QPointF(4064.8193464067945, 2969.0243880817015));
    points.append(QPointF(4095.804988589596, 2891.7621601338465));
    points.append(QPointF(4122.206955892843, 2823.6554697520774));
    points.append(QPointF(4139.097797821786, 2777.9307145219336));
    points.append(QPointF(4149.204800929745, 2749.4023517620412));
    points.append(QPointF(4158.372150687417, 2722.3578709475264));
    points.append(QPointF(4166.645683843604, 2696.6183826459132));
    points.append(QPointF(4174.071237147104, 2672.004997424725));
    points.append(QPointF(4180.694647346716, 2648.338825851485));
    points.append(QPointF(4186.561751191242, 2625.440978493719));
    points.append(QPointF(4191.718385429485, 2603.132565918948));
    points.append(QPointF(4196.210386810242, 2581.2346986946964));
    points.append(QPointF(4200.083592082314, 2559.5684873884893));
    points.append(QPointF(4203.383837994501, 2537.955042567848));
    points.append(QPointF(4206.156961295605, 2516.2154748002986));
    points.append(QPointF(4208.448798734424, 2494.170894653362));
    points.append(QPointF(4210.305187059759, 2471.642412694565));
    points.append(QPointF(4212.4136775031675, 2436.667443899716));
    points.append(QPointF(4214.063800458542, 2386.5603959212317));
    points.append(QPointF(4214.797188438389, 2331.297730910725));
    points.append(QPointF(4214.980535433111, 2269.448333408385));
    points.append(QPointF(4214.980535433071, 2235.242494488189));

    const QVector<QPointF> res = VAbstractCurve::GetSegmentPoints(points, begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(3964.650771379471, 3212.2173150777817));
    origPoints.append(QPointF(4009.6319008613655, 3103.623262231279));
    origPoints.append(QPointF(4064.8193464067945, 2969.0243880817015));
    origPoints.append(QPointF(4095.804988589596, 2891.7621601338465));
    origPoints.append(QPointF(4122.206955892843, 2823.6554697520774));
    origPoints.append(QPointF(4139.097797821786, 2777.9307145219336));
    origPoints.append(QPointF(4149.204800929745, 2749.4023517620412));
    origPoints.append(QPointF(4158.372150687417, 2722.3578709475264));
    origPoints.append(QPointF(4166.645683843604, 2696.6183826459132));
    origPoints.append(QPointF(4174.071237147104, 2672.004997424725));
    origPoints.append(QPointF(4180.694647346716, 2648.338825851485));
    origPoints.append(QPointF(4186.561751191242, 2625.440978493719));
    origPoints.append(QPointF(4191.718385429485, 2603.132565918948));
    origPoints.append(QPointF(4196.210386810242, 2581.2346986946964));
    origPoints.append(QPointF(4200.023629188538, 2559.9039118110236));

    // Begin comparison
    Comparison(res, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::CompareThreeWays()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/TestPuzzle.val
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl1(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);
    spl1.SetApproximationScale(10);

    VSpline spl2(spl1.GetP1(), static_cast<QPointF>(spl1.GetP2()), static_cast<QPointF>(spl1.GetP3()), spl1.GetP4(), 1);
    spl2.SetApproximationScale(10);

    VSpline spl3(spl1.GetP1(), spl1.GetP4(), spl1.GetStartAngle(), "", spl2.GetEndAngle(), "", spl2.GetC1Length(), "",
                 spl2.GetC2Length(), "", 1);
    spl3.SetApproximationScale(10);

    QWARN("Comparing first and second splines.");
    CompareSplines(spl1, spl2);

    QWARN("Comparing second and third splines.");
    CompareSplines(spl2, spl3);

    QWARN("Comparing third and first splines.");
    CompareSplines(spl3, spl1);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestParametrT()
{
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    const qreal halfLength = spl.GetLength()/2.0;
    const qreal resLength = spl.LengthT(spl.GetParmT(halfLength));

    QVERIFY(qAbs(halfLength - resLength) < UnitConvertor(0.5, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestLengthByPoint_data()
{
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    QTest::addColumn<VSpline>("spl");
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<qreal>("length");

    const qreal length = spl.GetLength();
    const qreal testLength = length*(2.0/3.0);
    VSpline spl1, spl2;
    const QPointF p = spl.CutSpline(testLength, spl1, spl2);

    QTest::newRow("Point on spline") << spl << p << testLength;
    QTest::newRow("Wrong point") << spl << QPointF(-10000, -10000) << -1.0;
    QTest::newRow("First point") << spl << p1.toQPointF() << 0.0;
    QTest::newRow("Last point") << spl << p4.toQPointF() << length;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestLengthByPoint()
{
    QFETCH(VSpline, spl);
    QFETCH(QPointF, point);
    QFETCH(qreal, length);

    const qreal resLength = spl.GetLengthByPoint(point);

    QVERIFY(qAbs(resLength - length) < ToPixel(0.5, Unit::Mm));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestFlip_data()
{
    QTest::addColumn<VSpline>("spl");
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QString>("prefix");

    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    QLineF axis(QPointF(600, 30), QPointF(600, 1800));

    QTest::newRow("Vertical axis") << spl << axis << "a2";

    axis = QLineF(QPointF(600, 30), QPointF(1200, 30));

    QTest::newRow("Horizontal axis") << spl << axis << "a2";

    axis = QLineF(QPointF(600, 30), QPointF(600, 1800));
    axis.setAngle(45);

    QTest::newRow("Diagonal axis") << spl << axis << "a2";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestFlip()
{
    QFETCH(VSpline, spl);
    QFETCH(QLineF, axis);
    QFETCH(QString, prefix);

    const VSpline res = spl.Flip(axis, prefix);

    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(res.name().endsWith(prefix), qUtf8Printable(errorMsg));

    QCOMPARE(spl.GetLength(), res.GetLength());
    QCOMPARE(spl.GetC1Length(), res.GetC1Length());
    QCOMPARE(spl.GetC2Length(), res.GetC2Length());
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::CompareSplines(const VSpline &spl1, const VSpline &spl2) const
{
    QCOMPARE(spl1.GetP1().toQPointF().toPoint(), spl2.GetP1().toQPointF().toPoint());
    QCOMPARE(spl1.GetP2().toQPointF().toPoint(), spl2.GetP2().toQPointF().toPoint());
    QCOMPARE(spl1.GetP3().toQPointF().toPoint(), spl2.GetP3().toQPointF().toPoint());
    QCOMPARE(spl1.GetP4().toQPointF().toPoint(), spl2.GetP4().toQPointF().toPoint());

    QCOMPARE(spl1.GetStartAngle(), spl2.GetStartAngle());
    QCOMPARE(spl1.GetEndAngle(), spl2.GetEndAngle());

    QCOMPARE(spl1.GetC1Length(), spl2.GetC1Length());
    QCOMPARE(spl1.GetC2Length(), spl2.GetC2Length());

    QCOMPARE(spl1.GetLength(), spl2.GetLength());

    QCOMPARE(spl1.GetKasm1(), spl2.GetKasm1());
    QCOMPARE(spl1.GetKasm2(), spl2.GetKasm2());

    QCOMPARE(spl1.GetKcurve(), spl2.GetKcurve());

    // Compare points
    Comparison(spl1.GetPoints(), spl2.GetPoints());
}
