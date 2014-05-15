/************************************************************************
 **
 **  @file   options.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses.
 **
 *************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QFlags>
#include <QString>
#include <QMetaType>
#include <QtGlobal>

#define SceneSize 50000

namespace Valentina
{
    /**
     * @brief The Scene enum
     */
    enum Scene { Point, Line, Spline, Arc, SplinePath, Detail };
    Q_DECLARE_FLAGS(Scenes, Scene)

    /**
     * @brief The Tool enum
     */
    enum Tool
    {
        ArrowTool,
        SinglePointTool,
        EndLineTool,
        LineTool,
        AlongLineTool,
        ShoulderPointTool,
        NormalTool,
        BisectorTool,
        LineIntersectTool,
        SplineTool,
        CutSplineTool,
        CutArcTool,
        ArcTool,
        SplinePathTool,
        CutSplinePathTool,
        PointOfContact,
        DetailTool,
        NodePoint,
        NodeArc,
        NodeSpline,
        NodeSplinePath,
        Height,
        Triangle,
        PointOfIntersection,
        UnionDetails
    };
    Q_DECLARE_FLAGS(Tools, Tool)

    /**
     * @brief The Source enum
     */
    enum Source { FromGui, FromFile, FromTool };
    Q_DECLARE_FLAGS(Sources, Source)

    /**
     * @brief The Draw enum
     */
    enum Draw { Calculation, Modeling };
    Q_DECLARE_FLAGS(Draws, Draw)

    /**
     * @brief The Unit enum
     */
    enum Unit { Mm, Cm, Inch };
    Q_DECLARE_FLAGS(Units, Unit)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Scenes )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Tools )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Sources )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Draws )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Units )
Q_DECLARE_METATYPE(Valentina::Unit)

namespace Pattern
{
    /**
     * @brief The Scene enum
     */
    enum Measurement { Standard, Individual };
    Q_DECLARE_FLAGS(Measurements, Measurement)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Pattern::Measurements )

// measurements
extern const QString headGirth;
extern const QString midNeckGirth;
extern const QString neckBaseGirth;
extern const QString headAndNeckLength;
extern const QString centerFrontWaistLength;
extern const QString centerBackWaistLength;
extern const QString shoulderLength;
extern const QString sideWaistLength;
extern const QString trunkLength;
extern const QString shoulderGirth;
extern const QString upperChestGirth;
extern const QString bustGirth;
extern const QString underBustGirth;
extern const QString waistGirth;
extern const QString highHipGirth;
extern const QString hipGirth;
extern const QString upperFrontChestWidth;
extern const QString frontChestWidth;
extern const QString acrossFrontShoulderWidth;
extern const QString acrossBackShoulderWidth;
extern const QString upperBackWidth;
extern const QString backWidth;
extern const QString bustpointToBustpoint;
extern const QString halterBustpointToBustpoint;
extern const QString neckToBustpoint;
extern const QString crotchLength;
extern const QString riseHeight;
extern const QString shoulderDrop;
extern const QString shoulderSlopeDegrees;
extern const QString frontShoulderSlopeLength;
extern const QString backShoulderSlopeLength;
extern const QString frontShoulderToWaistLength;
extern const QString backShoulderToWaistLength;
extern const QString frontNeckArc;
extern const QString backNeckArc;
extern const QString frontUpperChestArc;
extern const QString backUpperChestArc;
extern const QString frontWaistArc;
extern const QString backWaistArc;
extern const QString frontUpperHipArc;
extern const QString backUpperHipArc;
extern const QString frontHipArc;
extern const QString backHipArc;
extern const QString chestSlope;
extern const QString backSlope;
extern const QString frontWaistSlope;
extern const QString backWaistSlope;
extern const QString frontNeckToUpperChestHeight;
extern const QString frontNeckToBustHeight;
extern const QString armscyeGirth;
extern const QString elbowGirth;
extern const QString upperArmGirth;
extern const QString wristGirth;
extern const QString scyeDepth;
extern const QString shoulderAndArmLength;
extern const QString underarmLength;
extern const QString cervicaleToWristLength;
extern const QString shoulderToElbowLength;
extern const QString armLength;
extern const QString handWidth;
extern const QString handLength;
extern const QString handGirth;
extern const QString thighGirth;
extern const QString midThighGirth;
extern const QString kneeGirth;
extern const QString calfGirth;
extern const QString ankleGirth;
extern const QString kneeHeight;
extern const QString ankleHeight;
extern const QString footWidth;
extern const QString footLength;
extern const QString height;
extern const QString cervicaleHeight;
extern const QString cervicaleToKneeHeight;
extern const QString waistHeight;
extern const QString highHipHeight;
extern const QString hipHeight;
extern const QString waistToHipHeight;
extern const QString waistToKneeHeight;
extern const QString crotchHeight;

// variables name
extern const QString line_;
extern const QString angleLine_;
extern const QString arc_;
extern const QString spl_;
extern const QString splPath;

// functions
extern const QString sin_F;
extern const QString cos_F;
extern const QString tan_F;
extern const QString asin_F;
extern const QString acos_F;
extern const QString atan_F;
extern const QString sinh_F;
extern const QString cosh_F;
extern const QString tanh_F;
extern const QString asinh_F;
extern const QString acosh_F;
extern const QString atanh_F;
extern const QString log2_F;
extern const QString log10_F;
extern const QString log_F;
extern const QString ln_F;
extern const QString exp_F;
extern const QString sqrt_F;
extern const QString sign_F;
extern const QString rint_F;
extern const QString abs_F;
extern const QString min_F;
extern const QString max_F;
extern const QString sum_F;
extern const QString avg_F;

#endif // OPTIONS_H
