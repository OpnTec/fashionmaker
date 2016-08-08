/*****************************************************************************/
/*                                                                           */
/*  Routines for Arbitrary Precision Floating-point Arithmetic               */
/*  and Fast Robust Geometric Predicates                                     */
/*  (predicates.cpp)                                                         */
/*                                                                           */
/*  May 18, 1996                                                             */
/*                                                                           */
/*  Placed in the public domain by                                           */
/*  Jonathan Richard Shewchuk                                                */
/*  School of Computer Science                                               */
/*  Carnegie Mellon University                                               */
/*  5000 Forbes Avenue                                                       */
/*  Pittsburgh, Pennsylvania  15213-3891                                     */
/*  jrs@cs.cmu.edu                                                           */
/*                                                                           */
/*  This file contains C implementation of algorithms for exact addition     */
/*    and multiplication of floating-point numbers, and predicates for       */
/*    robustly performing the orientation and incircle tests used in         */
/*    computational geometry.  The algorithms and underlying theory are      */
/*    described in Jonathan Richard Shewchuk.  "Adaptive Precision Floating- */
/*    Point Arithmetic and Fast Robust Geometric Predicates."  Technical     */
/*    Report CMU-CS-96-140, School of Computer Science, Carnegie Mellon      */
/*    University, Pittsburgh, Pennsylvania, May 1996.  (Submitted to         */
/*    Discrete & Computational Geometry.)                                    */
/*                                                                           */
/*  This file, the paper listed above, and other information are available   */
/*    from the Web page http://www.cs.cmu.edu/~quake/robust.html .           */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  Using this code:                                                         */
/*                                                                           */
/*  First, read the short or long version of the paper (from the Web page    */
/*    above).                                                                */
/*                                                                           */
/*  Be sure to call exactinit() once, before calling any of the arithmetic   */
/*    functions or geometric predicates.  Also be sure to turn on the        */
/*    optimizer when compiling this file.                                    */
/*                                                                           */
/*                                                                           */
/*  Several geometric predicates are defined.  Their parameters are all      */
/*    points.  Each point is an array of two or three floating-point         */
/*    numbers.  The geometric predicates, described in the papers, are       */
/*                                                                           */
/*    incircle(pa, pb, pc, pd)                                               */
/*    incirclefast(pa, pb, pc, pd)                                           */
/*                                                                           */
/*  Those with suffix "fast" are approximate, non-robust versions.  Those    */
/*    without the suffix are adaptive precision, robust versions.  There     */
/*    are also versions with the suffices "exact" and "slow", which are      */
/*    non-adaptive, exact arithmetic versions, which I use only for timings  */
/*    in my arithmetic papers.                                               */
/*                                                                           */
/*                                                                           */
/*  An expansion is represented by an array of floating-point numbers,       */
/*    sorted from smallest to largest magnitude (possibly with interspersed  */
/*    zeros).  The length of each expansion is stored as a separate integer, */
/*    and each arithmetic function returns an integer which is the length    */
/*    of the expansion it created.                                           */
/*                                                                           */
/*  Several arithmetic functions are defined.  Their parameters are          */
/*                                                                           */
/*    e, f           Input expansions                                        */
/*    elen, flen     Lengths of input expansions (must be >= 1)              */
/*    h              Output expansion                                        */
/*    b              Input scalar                                            */
/*                                                                           */
/*  The arithmetic functions are                                             */
/*                                                                           */
/*    expansion_sum(elen, e, flen, f, h)                                     */
/*    expansion_sum_zeroelim1(elen, e, flen, f, h)                           */
/*    expansion_sum_zeroelim2(elen, e, flen, f, h)                           */
/*    fast_expansion_sum(elen, e, flen, f, h)                                */
/*    fast_expansion_sum_zeroelim(elen, e, flen, f, h)                       */
/*    linear_expansion_sum(elen, e, flen, f, h)                              */
/*    linear_expansion_sum_zeroelim(elen, e, flen, f, h)                     */
/*    scale_expansion(elen, e, b, h)                                         */
/*    scale_expansion_zeroelim(elen, e, b, h)                                */
/*    compress(elen, e, h)                                                   */
/*                                                                           */
/*  All of these are described in the long version of the paper; some are    */
/*    described in the short version.  All return an integer that is the     */
/*    length of h.  Those with suffix _zeroelim perform zero elimination,    */
/*    and are recommended over their counterparts.  The procedure            */
/*    fast_expansion_sum_zeroelim() (or linear_expansion_sum_zeroelim() on   */
/*    processors that do not use the round-to-even tiebreaking rule) is      */
/*    recommended over expansion_sum_zeroelim().  Each procedure has a       */
/*    little note next to it (in the code below) that tells you whether or   */
/*    not the output expansion may be the same array as one of the input     */
/*    expansions.                                                            */
/*                                                                           */
/*                                                                           */
/*  If you look around below, you'll also find macros for a bunch of         */
/*    simple unrolled arithmetic operations, and procedures for printing     */
/*    expansions (commented out because they don't work with all C           */
/*    compilers) and for generating random floating-point numbers whose      */
/*    significand bits are all random.  Most of the macros have undocumented */
/*    requirements that certain of their parameters should not be the same   */
/*    variable; for safety, better to make sure all the parameters are       */
/*    distinct variables.  Feel free to send email to jrs@cs.cmu.edu if you  */
/*    have questions.                                                        */
/*                                                                           */
/*****************************************************************************/

#include <QtGlobal>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_MSVC(4701)
QT_WARNING_DISABLE_GCC("-Wold-style-cast")
QT_WARNING_DISABLE_GCC("-Wfloat-equal")
QT_WARNING_DISABLE_GCC("-Wmaybe-uninitialized")

/* On some machines, the exact arithmetic routines might be defeated by the  */
/*   use of internal extended precision floating-point registers.  Sometimes */
/*   this problem can be fixed by defining certain values to be volatile,    */
/*   thus forcing them to be stored to memory and rounded off.  This isn't   */
/*   a great solution, though, as it slows the arithmetic down.              */
/*                                                                           */
/* To try this out, write "#define INEXACT volatile" below.  Normally,       */
/*   however, INEXACT should be defined to be nothing.  ("#define INEXACT".) */

#define INEXACT                          /* Nothing */
/* #define INEXACT volatile */

#define REALPRINT doubleprint
#define REALRAND doublerand
#define NARROWRAND narrowdoublerand
#define UNIFORMRAND uniformdoublerand

/* Which of the following two methods of finding the absolute values is      */
/*   fastest is compiler-dependent.  A few compilers can inline and optimize */
/*   the fabs() call; but most will incur the overhead of a function call,   */
/*   which is disastrously slow.  A faster way on IEEE machines might be to  */
/*   mask the appropriate bit, but that's difficult to do in C.              */

#define Absolute(a)  ((a) >= 0.0 ? (a) : -(a))
/* #define Absolute(a)  fabs(a) */


/* Many of the operations are broken up into two pieces, a main part that    */
/*   performs an approximate operation, and a "tail" that computes the       */
/*   roundoff error of that operation.                                       */
/*                                                                           */
/* The operations Fast_Two_Sum(), Fast_Two_Diff(), Two_Sum(), Two_Diff(),    */
/*   Split(), and Two_Product() are all implemented as described in the      */
/*   reference.  Each of these macros requires certain variables to be       */
/*   defined in the calling routine.  The variables `bvirt', `c', `abig',    */
/*   `_i', `_j', `_k', `_l', `_m', and `_n' are declared `INEXACT' because   */
/*   they store the result of an operation that may incur roundoff error.    */
/*   The input parameter `x' (or the highest numbered `x_' parameter) must   */
/*   also be declared `INEXACT'.                                             */

#define Fast_Two_Sum_Tail(a, b, x, y) \
  bvirt = x - a; \
  y = b - bvirt

#define Fast_Two_Sum(a, b, x, y) \
  x = (qreal) (a + b); \
  Fast_Two_Sum_Tail(a, b, x, y)

#define Two_Sum_Tail(a, b, x, y) \
  bvirt = (qreal) (x - a); \
  avirt = x - bvirt; \
  bround = b - bvirt; \
  around = a - avirt; \
  y = around + bround

#define Two_Sum(a, b, x, y) \
  x = (qreal) (a + b); \
  Two_Sum_Tail(a, b, x, y)

#define Two_Diff_Tail(a, b, x, y) \
  bvirt = (qreal) (a - x); \
  avirt = x + bvirt; \
  bround = bvirt - b; \
  around = a - avirt; \
  y = around + bround

#define Two_Diff(a, b, x, y) \
  x = (qreal) (a - b); \
  Two_Diff_Tail(a, b, x, y)

#define Split(a, ahi, alo) \
  c = (qreal) (splitter * a); \
  abig = (qreal) (c - a); \
  ahi = c - abig; \
  alo = a - ahi

#define Two_Product_Tail(a, b, x, y) \
  Split(a, ahi, alo); \
  Split(b, bhi, blo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

#define Two_Product(a, b, x, y) \
  x = (qreal) (a * b); \
  Two_Product_Tail(a, b, x, y)

/* Two_Product_Presplit() is Two_Product() where one of the inputs has       */
/*   already been split.  Avoids redundant splitting.                        */

#define Two_Product_Presplit(a, b, bhi, blo, x, y) \
  x = (qreal) (a * b); \
  Split(a, ahi, alo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

/* Square() can be done more quickly than Two_Product().                     */

#define Square_Tail(a, x, y) \
  Split(a, ahi, alo); \
  err1 = x - (ahi * ahi); \
  err3 = err1 - ((ahi + ahi) * alo); \
  y = (alo * alo) - err3

#define Square(a, x, y) \
  x = (qreal) (a * a); \
  Square_Tail(a, x, y)

/* Macros for summing expansions of various fixed lengths.  These are all    */
/*   unrolled versions of Expansion_Sum().                                   */

#define Two_One_Sum(a1, a0, b, x2, x1, x0) \
  Two_Sum(a0, b , _i, x0); \
  Two_Sum(a1, _i, x2, x1)

#define Two_One_Diff(a1, a0, b, x2, x1, x0) \
  Two_Diff(a0, b , _i, x0); \
  Two_Sum( a1, _i, x2, x1)

#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Sum(a1, a0, b0, _j, _0, x0); \
  Two_One_Sum(_j, _0, b1, x3, x2, x1)

#define Two_Two_Diff(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Diff(a1, a0, b0, _j, _0, x0); \
  Two_One_Diff(_j, _0, b1, x3, x2, x1)

qreal splitter;     /* = 2^ceiling(p / 2) + 1.  Used to split floats in half. */
qreal epsilon;                /* = 2^(-p).  Used to estimate roundoff errors. */
/* A set of coefficients used to calculate maximum roundoff errors.          */
qreal resulterrbound;
qreal ccwerrboundA, ccwerrboundB, ccwerrboundC;
qreal o3derrboundA, o3derrboundB, o3derrboundC;
qreal iccerrboundA, iccerrboundB, iccerrboundC;
qreal isperrboundA, isperrboundB, isperrboundC;

/*****************************************************************************/
/*                                                                           */
/*  exactinit()   Initialize the variables used for exact arithmetic.        */
/*                                                                           */
/*  `epsilon' is the largest power of two such that 1.0 + epsilon = 1.0 in   */
/*  floating-point arithmetic.  `epsilon' bounds the relative roundoff       */
/*  error.  It is used for floating-point error analysis.                    */
/*                                                                           */
/*  `splitter' is used to split floating-point numbers into two half-        */
/*  length significands for exact multiplication.                            */
/*                                                                           */
/*  I imagine that a highly optimizing compiler might be too smart for its   */
/*  own good, and somehow cause this routine to fail, if it pretends that    */
/*  floating-point arithmetic is too much like real arithmetic.              */
/*                                                                           */
/*  Don't change this routine unless you fully understand it.                */
/*                                                                           */
/*****************************************************************************/

void exactinit()
{
    qreal half;
    qreal check, lastcheck;
    int every_other;

    every_other = 1;
    half = 0.5;
    epsilon = 1.0;
    splitter = 1.0;
    check = 1.0;
    /* Repeatedly divide `epsilon' by two until it is too small to add to    */
    /*   one without causing roundoff.  (Also check if the sum is equal to   */
    /*   the previous sum, for machines that round up instead of using exact */
    /*   rounding.  Not that this library will work on such machines anyway. */
    do
    {
        lastcheck = check;
        epsilon *= half;
        if (every_other)
        {
            splitter *= 2.0;
        }
        every_other = !every_other;
        check = 1.0 + epsilon;
    } while ((check != 1.0) && (check != lastcheck));
    splitter += 1.0;

    /* Error bounds for orientation and incircle tests. */
    resulterrbound = (3.0 + 8.0 * epsilon) * epsilon;
    ccwerrboundA = (3.0 + 16.0 * epsilon) * epsilon;
    ccwerrboundB = (2.0 + 12.0 * epsilon) * epsilon;
    ccwerrboundC = (9.0 + 64.0 * epsilon) * epsilon * epsilon;
    o3derrboundA = (7.0 + 56.0 * epsilon) * epsilon;
    o3derrboundB = (3.0 + 28.0 * epsilon) * epsilon;
    o3derrboundC = (26.0 + 288.0 * epsilon) * epsilon * epsilon;
    iccerrboundA = (10.0 + 96.0 * epsilon) * epsilon;
    iccerrboundB = (4.0 + 48.0 * epsilon) * epsilon;
    iccerrboundC = (44.0 + 576.0 * epsilon) * epsilon * epsilon;
    isperrboundA = (16.0 + 224.0 * epsilon) * epsilon;
    isperrboundB = (5.0 + 72.0 * epsilon) * epsilon;
    isperrboundC = (71.0 + 1408.0 * epsilon) * epsilon * epsilon;
}

/*****************************************************************************/
/*                                                                           */
/*  fast_expansion_sum_zeroelim()   Sum two expansions, eliminating zero     */
/*                                  components from the output expansion.    */
/*                                                                           */
/*  Sets h = e + f.  See the long version of my paper for details.           */
/*                                                                           */
/*  If round-to-even is used (as with IEEE 754), maintains the strongly      */
/*  nonoverlapping property.  (That is, if e is strongly nonoverlapping, h   */
/*  will be also.)  Does NOT maintain the nonoverlapping or nonadjacent      */
/*  properties.                                                              */
/*                                                                           */
/*****************************************************************************/

int fast_expansion_sum_zeroelim(int elen, qreal *e, int flen, qreal *f, qreal *h)  /* h cannot be e or f. */
{
    qreal Q;
    INEXACT qreal Qnew;
    INEXACT qreal hh;
    INEXACT qreal bvirt;
    qreal avirt, bround, around;
    int eindex, findex, hindex;
    qreal enow, fnow;

    enow = e[0];
    fnow = f[0];
    eindex = findex = 0;
    if ((fnow > enow) == (fnow > -enow))
    {
        Q = enow;
        enow = e[++eindex];
    }
    else
    {
        Q = fnow;
        fnow = f[++findex];
    }
    hindex = 0;
    if ((eindex < elen) && (findex < flen))
    {
        if ((fnow > enow) == (fnow > -enow))
        {
            Fast_Two_Sum(enow, Q, Qnew, hh);
            enow = e[++eindex];
        }
        else
        {
            Fast_Two_Sum(fnow, Q, Qnew, hh);
            fnow = f[++findex];
        }
        Q = Qnew;
        if (hh != 0.0)
        {
            h[hindex++] = hh;
        }
        while ((eindex < elen) && (findex < flen))
        {
            if ((fnow > enow) == (fnow > -enow))
            {
                Two_Sum(Q, enow, Qnew, hh);
                enow = e[++eindex];
            }
            else
            {
                Two_Sum(Q, fnow, Qnew, hh);
                fnow = f[++findex];
            }
            Q = Qnew;
            if (hh != 0.0)
            {
                h[hindex++] = hh;
            }
        }
    }
    while (eindex < elen)
    {
        Two_Sum(Q, enow, Qnew, hh);
        enow = e[++eindex];
        Q = Qnew;
        if (hh != 0.0)
        {
            h[hindex++] = hh;
        }
    }
    while (findex < flen)
    {
        Two_Sum(Q, fnow, Qnew, hh);
        fnow = f[++findex];
        Q = Qnew;
        if (hh != 0.0)
        {
            h[hindex++] = hh;
        }
    }
    if ((Q != 0.0) || (hindex == 0))
    {
        h[hindex++] = Q;
    }
    return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  scale_expansion_zeroelim()   Multiply an expansion by a scalar,          */
/*                               eliminating zero components from the        */
/*                               output expansion.                           */
/*                                                                           */
/*  Sets h = be.  See either version of my paper for details.                */
/*                                                                           */
/*  Maintains the nonoverlapping property.  If round-to-even is used (as     */
/*  with IEEE 754), maintains the strongly nonoverlapping and nonadjacent    */
/*  properties as well.  (That is, if e has one of these properties, so      */
/*  will h.)                                                                 */
/*                                                                           */
/*****************************************************************************/

int scale_expansion_zeroelim(int elen, qreal *e, qreal b, qreal *h)   /* e and h cannot be the same. */
{
    INEXACT qreal Q, sum;
    qreal hh;
    INEXACT qreal product1;
    qreal product0;
    int eindex, hindex;
    qreal enow;
    INEXACT qreal bvirt;
    qreal avirt, bround, around;
    INEXACT qreal c;
    INEXACT qreal abig;
    qreal ahi, alo, bhi, blo;
    qreal err1, err2, err3;

    Split(b, bhi, blo);
    Two_Product_Presplit(e[0], b, bhi, blo, Q, hh);
    hindex = 0;
    if (hh != 0) {
    h[hindex++] = hh;
    }
    for (eindex = 1; eindex < elen; eindex++)
    {
        enow = e[eindex];
        Two_Product_Presplit(enow, b, bhi, blo, product1, product0);
        Two_Sum(Q, product0, sum, hh);
        if (hh != 0)
        {
            h[hindex++] = hh;
        }
        Fast_Two_Sum(product1, sum, Q, hh);
        if (hh != 0)
        {
            h[hindex++] = hh;
        }
    }
    if ((Q != 0.0) || (hindex == 0))
    {
        h[hindex++] = Q;
    }
    return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  estimate()   Produce a one-word estimate of an expansion's value.        */
/*                                                                           */
/*  See either version of my paper for details.                              */
/*                                                                           */
/*****************************************************************************/

qreal estimate(int elen, qreal *e)
{
    qreal Q;
    int eindex;

    Q = e[0];
    for (eindex = 1; eindex < elen; eindex++)
    {
        Q += e[eindex];
    }
    return Q;
}

qreal incircleadapt(qreal *pa, qreal *pb, qreal *pc, qreal *pd, qreal permanent)
{
    INEXACT qreal adx, bdx, cdx, ady, bdy, cdy;
    qreal det, errbound;

    INEXACT qreal bdxcdy1, cdxbdy1, cdxady1, adxcdy1, adxbdy1, bdxady1;
    qreal bdxcdy0, cdxbdy0, cdxady0, adxcdy0, adxbdy0, bdxady0;
    qreal bc[4], ca[4], ab[4];
    INEXACT qreal bc3, ca3, ab3;
    qreal axbc[8], axxbc[16], aybc[8], ayybc[16], adet[32];
    int axbclen, axxbclen, aybclen, ayybclen, alen;
    qreal bxca[8], bxxca[16], byca[8], byyca[16], bdet[32];
    int bxcalen, bxxcalen, bycalen, byycalen, blen;
    qreal cxab[8], cxxab[16], cyab[8], cyyab[16], cdet[32];
    int cxablen, cxxablen, cyablen, cyyablen, clen;
    qreal abdet[64];
    int ablen;
    qreal fin1[1152], fin2[1152];
    qreal *finnow, *finother, *finswap;
    int finlength;

    qreal adxtail, bdxtail, cdxtail, adytail, bdytail, cdytail;
    INEXACT qreal adxadx1, adyady1, bdxbdx1, bdybdy1, cdxcdx1, cdycdy1;
    qreal adxadx0, adyady0, bdxbdx0, bdybdy0, cdxcdx0, cdycdy0;
    qreal aa[4], bb[4], cc[4];
    INEXACT qreal aa3, bb3, cc3;
    INEXACT qreal ti1, tj1;
    qreal ti0, tj0;
    qreal u[4], v[4];
    INEXACT qreal u3, v3;
    qreal temp8[8], temp16a[16], temp16b[16], temp16c[16];
    qreal temp32a[32], temp32b[32], temp48[48], temp64[64];
    int temp8len, temp16alen, temp16blen, temp16clen;
    int temp32alen, temp32blen, temp48len, temp64len;
    qreal axtbb[8], axtcc[8], aytbb[8], aytcc[8];
    // cppcheck-suppress variableScope
    int axtbblen, axtcclen, aytbblen, aytcclen;
    qreal bxtaa[8], bxtcc[8], bytaa[8], bytcc[8];
    // cppcheck-suppress variableScope
    int bxtaalen, bxtcclen, bytaalen, bytcclen;
    qreal cxtaa[8], cxtbb[8], cytaa[8], cytbb[8];
    // cppcheck-suppress variableScope
    int cxtaalen, cxtbblen, cytaalen, cytbblen;
    qreal axtbc[8], aytbc[8], bxtca[8], bytca[8], cxtab[8], cytab[8];
    int axtbclen, aytbclen, bxtcalen, bytcalen, cxtablen, cytablen;
    qreal axtbct[16], aytbct[16], bxtcat[16], bytcat[16], cxtabt[16], cytabt[16];
    // cppcheck-suppress variableScope
    int axtbctlen, aytbctlen, bxtcatlen, bytcatlen, cxtabtlen, cytabtlen;
    qreal axtbctt[8], aytbctt[8], bxtcatt[8];
    qreal bytcatt[8], cxtabtt[8], cytabtt[8];
    // cppcheck-suppress variableScope
    int axtbcttlen, aytbcttlen, bxtcattlen, bytcattlen, cxtabttlen, cytabttlen;
    qreal abt[8], bct[8], cat[8];
    // cppcheck-suppress variableScope
    int abtlen, bctlen, catlen;
    qreal abtt[4], bctt[4], catt[4];
    // cppcheck-suppress variableScope
    int abttlen, bcttlen, cattlen;
    INEXACT qreal abtt3, bctt3, catt3;
    qreal negate;

    INEXACT qreal bvirt;
    qreal avirt, bround, around;
    INEXACT qreal c;
    INEXACT qreal abig;
    qreal ahi, alo, bhi, blo;
    qreal err1, err2, err3;
    INEXACT qreal _i, _j;
    qreal _0;

    adx = (qreal) (pa[0] - pd[0]);
    bdx = (qreal) (pb[0] - pd[0]);
    cdx = (qreal) (pc[0] - pd[0]);
    ady = (qreal) (pa[1] - pd[1]);
    bdy = (qreal) (pb[1] - pd[1]);
    cdy = (qreal) (pc[1] - pd[1]);

    Two_Product(bdx, cdy, bdxcdy1, bdxcdy0);
    Two_Product(cdx, bdy, cdxbdy1, cdxbdy0);
    Two_Two_Diff(bdxcdy1, bdxcdy0, cdxbdy1, cdxbdy0, bc3, bc[2], bc[1], bc[0]);
    bc[3] = bc3;
    axbclen = scale_expansion_zeroelim(4, bc, adx, axbc);
    axxbclen = scale_expansion_zeroelim(axbclen, axbc, adx, axxbc);
    aybclen = scale_expansion_zeroelim(4, bc, ady, aybc);
    ayybclen = scale_expansion_zeroelim(aybclen, aybc, ady, ayybc);
    alen = fast_expansion_sum_zeroelim(axxbclen, axxbc, ayybclen, ayybc, adet);

    Two_Product(cdx, ady, cdxady1, cdxady0);
    Two_Product(adx, cdy, adxcdy1, adxcdy0);
    Two_Two_Diff(cdxady1, cdxady0, adxcdy1, adxcdy0, ca3, ca[2], ca[1], ca[0]);
    ca[3] = ca3;
    bxcalen = scale_expansion_zeroelim(4, ca, bdx, bxca);
    bxxcalen = scale_expansion_zeroelim(bxcalen, bxca, bdx, bxxca);
    bycalen = scale_expansion_zeroelim(4, ca, bdy, byca);
    byycalen = scale_expansion_zeroelim(bycalen, byca, bdy, byyca);
    blen = fast_expansion_sum_zeroelim(bxxcalen, bxxca, byycalen, byyca, bdet);

    Two_Product(adx, bdy, adxbdy1, adxbdy0);
    Two_Product(bdx, ady, bdxady1, bdxady0);
    Two_Two_Diff(adxbdy1, adxbdy0, bdxady1, bdxady0, ab3, ab[2], ab[1], ab[0]);
    ab[3] = ab3;
    cxablen = scale_expansion_zeroelim(4, ab, cdx, cxab);
    cxxablen = scale_expansion_zeroelim(cxablen, cxab, cdx, cxxab);
    cyablen = scale_expansion_zeroelim(4, ab, cdy, cyab);
    cyyablen = scale_expansion_zeroelim(cyablen, cyab, cdy, cyyab);
    clen = fast_expansion_sum_zeroelim(cxxablen, cxxab, cyyablen, cyyab, cdet);

    ablen = fast_expansion_sum_zeroelim(alen, adet, blen, bdet, abdet);
    finlength = fast_expansion_sum_zeroelim(ablen, abdet, clen, cdet, fin1);

    det = estimate(finlength, fin1);
    errbound = iccerrboundB * permanent;
    if ((det >= errbound) || (-det >= errbound))
    {
        return det;
    }

    Two_Diff_Tail(pa[0], pd[0], adx, adxtail);
    Two_Diff_Tail(pa[1], pd[1], ady, adytail);
    Two_Diff_Tail(pb[0], pd[0], bdx, bdxtail);
    Two_Diff_Tail(pb[1], pd[1], bdy, bdytail);
    Two_Diff_Tail(pc[0], pd[0], cdx, cdxtail);
    Two_Diff_Tail(pc[1], pd[1], cdy, cdytail);
    if ((adxtail == 0.0) && (bdxtail == 0.0) && (cdxtail == 0.0)
            && (adytail == 0.0) && (bdytail == 0.0) && (cdytail == 0.0))
    {
        return det;
    }

    errbound = iccerrboundC * permanent + resulterrbound * Absolute(det);
    det += ((adx * adx + ady * ady) * ((bdx * cdytail + cdy * bdxtail) - (bdy * cdxtail + cdx * bdytail))
          + 2.0 * (adx * adxtail + ady * adytail) * (bdx * cdy - bdy * cdx))
          + ((bdx * bdx + bdy * bdy) * ((cdx * adytail + ady * cdxtail) - (cdy * adxtail + adx * cdytail))
          + 2.0 * (bdx * bdxtail + bdy * bdytail) * (cdx * ady - cdy * adx))
          + ((cdx * cdx + cdy * cdy) * ((adx * bdytail + bdy * adxtail) - (ady * bdxtail + bdx * adytail))
          + 2.0 * (cdx * cdxtail + cdy * cdytail) * (adx * bdy - ady * bdx));
    if ((det >= errbound) || (-det >= errbound))
    {
        return det;
    }

    finnow = fin1;
    finother = fin2;

    if ((bdxtail != 0.0) || (bdytail != 0.0) || (cdxtail != 0.0) || (cdytail != 0.0))
    {
        Square(adx, adxadx1, adxadx0);
        Square(ady, adyady1, adyady0);
        Two_Two_Sum(adxadx1, adxadx0, adyady1, adyady0, aa3, aa[2], aa[1], aa[0]);
        aa[3] = aa3;
    }
    if ((cdxtail != 0.0) || (cdytail != 0.0) || (adxtail != 0.0) || (adytail != 0.0))
    {
        Square(bdx, bdxbdx1, bdxbdx0);
        Square(bdy, bdybdy1, bdybdy0);
        Two_Two_Sum(bdxbdx1, bdxbdx0, bdybdy1, bdybdy0, bb3, bb[2], bb[1], bb[0]);
        bb[3] = bb3;
    }
    if ((adxtail != 0.0) || (adytail != 0.0) || (bdxtail != 0.0) || (bdytail != 0.0))
    {
        Square(cdx, cdxcdx1, cdxcdx0);
        Square(cdy, cdycdy1, cdycdy0);
        Two_Two_Sum(cdxcdx1, cdxcdx0, cdycdy1, cdycdy0, cc3, cc[2], cc[1], cc[0]);
        cc[3] = cc3;
    }

    if (adxtail != 0.0)
    {
        axtbclen = scale_expansion_zeroelim(4, bc, adxtail, axtbc);
        temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, 2.0 * adx, temp16a);

        axtcclen = scale_expansion_zeroelim(4, cc, adxtail, axtcc);
        temp16blen = scale_expansion_zeroelim(axtcclen, axtcc, bdy, temp16b);

        axtbblen = scale_expansion_zeroelim(4, bb, adxtail, axtbb);
        temp16clen = scale_expansion_zeroelim(axtbblen, axtbb, -cdy, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c, temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }
    if (adytail != 0.0)
    {
        aytbclen = scale_expansion_zeroelim(4, bc, adytail, aytbc);
        temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, 2.0 * ady, temp16a);

        aytbblen = scale_expansion_zeroelim(4, bb, adytail, aytbb);
        temp16blen = scale_expansion_zeroelim(aytbblen, aytbb, cdx, temp16b);

        aytcclen = scale_expansion_zeroelim(4, cc, adytail, aytcc);
        temp16clen = scale_expansion_zeroelim(aytcclen, aytcc, -bdx, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c, temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }
    if (bdxtail != 0.0)
    {
        bxtcalen = scale_expansion_zeroelim(4, ca, bdxtail, bxtca);
        temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, 2.0 * bdx, temp16a);

        bxtaalen = scale_expansion_zeroelim(4, aa, bdxtail, bxtaa);
        temp16blen = scale_expansion_zeroelim(bxtaalen, bxtaa, cdy, temp16b);

        bxtcclen = scale_expansion_zeroelim(4, cc, bdxtail, bxtcc);
        temp16clen = scale_expansion_zeroelim(bxtcclen, bxtcc, -ady, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c, temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }
    if (bdytail != 0.0)
    {
        bytcalen = scale_expansion_zeroelim(4, ca, bdytail, bytca);
        temp16alen = scale_expansion_zeroelim(bytcalen, bytca, 2.0 * bdy, temp16a);

        bytcclen = scale_expansion_zeroelim(4, cc, bdytail, bytcc);
        temp16blen = scale_expansion_zeroelim(bytcclen, bytcc, adx, temp16b);

        bytaalen = scale_expansion_zeroelim(4, aa, bdytail, bytaa);
        temp16clen = scale_expansion_zeroelim(bytaalen, bytaa, -cdx, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c, temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }
    if (cdxtail != 0.0)
    {
        cxtablen = scale_expansion_zeroelim(4, ab, cdxtail, cxtab);
        temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, 2.0 * cdx, temp16a);

        cxtbblen = scale_expansion_zeroelim(4, bb, cdxtail, cxtbb);
        temp16blen = scale_expansion_zeroelim(cxtbblen, cxtbb, ady, temp16b);

        cxtaalen = scale_expansion_zeroelim(4, aa, cdxtail, cxtaa);
        temp16clen = scale_expansion_zeroelim(cxtaalen, cxtaa, -bdy, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                                temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                                                temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                                temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }
    if (cdytail != 0.0)
    {
        cytablen = scale_expansion_zeroelim(4, ab, cdytail, cytab);
        temp16alen = scale_expansion_zeroelim(cytablen, cytab, 2.0 * cdy, temp16a);

        cytaalen = scale_expansion_zeroelim(4, aa, cdytail, cytaa);
        temp16blen = scale_expansion_zeroelim(cytaalen, cytaa, bdx, temp16b);

        cytbblen = scale_expansion_zeroelim(4, bb, cdytail, cytbb);
        temp16clen = scale_expansion_zeroelim(cytbblen, cytbb, -adx, temp16c);

        temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
        temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c, temp32alen, temp32a, temp48);
        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
        finswap = finnow; finnow = finother; finother = finswap;
    }

    if ((adxtail != 0.0) || (adytail != 0.0))
    {
        if ((bdxtail != 0.0) || (bdytail != 0.0) || (cdxtail != 0.0) || (cdytail != 0.0))
        {
            Two_Product(bdxtail, cdy, ti1, ti0);
            Two_Product(bdx, cdytail, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
            u[3] = u3;
            negate = -bdy;
            Two_Product(cdxtail, negate, ti1, ti0);
            negate = -bdytail;
            Two_Product(cdx, negate, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
            v[3] = v3;
            bctlen = fast_expansion_sum_zeroelim(4, u, 4, v, bct);

            Two_Product(bdxtail, cdytail, ti1, ti0);
            Two_Product(cdxtail, bdytail, tj1, tj0);
            Two_Two_Diff(ti1, ti0, tj1, tj0, bctt3, bctt[2], bctt[1], bctt[0]);
            bctt[3] = bctt3;
            bcttlen = 4;
        }
        else
        {
            bct[0] = 0.0;
            bctlen = 1;
            bctt[0] = 0.0;
            bcttlen = 1;
        }

        if (adxtail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, adxtail, temp16a);
            axtbctlen = scale_expansion_zeroelim(bctlen, bct, adxtail, axtbct);
            temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, 2.0 * adx, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow; finnow = finother; finother = finswap;
            if (bdytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, cc, adxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow; finnow = finother; finother = finswap;
            }
            if (cdytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, bb, -adxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow; finnow = finother; finother = finswap;
            }

            temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, adxtail, temp32a);
            axtbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adxtail, axtbctt);
            temp16alen = scale_expansion_zeroelim(axtbcttlen, axtbctt, 2.0 * adx, temp16a);
            temp16blen = scale_expansion_zeroelim(axtbcttlen, axtbctt, adxtail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow; finnow = finother; finother = finswap;
        }
        if (adytail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, adytail, temp16a);
            aytbctlen = scale_expansion_zeroelim(bctlen, bct, adytail, aytbct);
            temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, 2.0 * ady, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow; finnow = finother; finother = finswap;


            temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, adytail, temp32a);
            aytbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adytail, aytbctt);
            temp16alen = scale_expansion_zeroelim(aytbcttlen, aytbctt, 2.0 * ady, temp16a);
            temp16blen = scale_expansion_zeroelim(aytbcttlen, aytbctt, adytail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow; finnow = finother; finother = finswap;
        }
    }
    if ((bdxtail != 0.0) || (bdytail != 0.0))
    {
        if ((cdxtail != 0.0) || (cdytail != 0.0) || (adxtail != 0.0) || (adytail != 0.0))
        {
            Two_Product(cdxtail, ady, ti1, ti0);
            Two_Product(cdx, adytail, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
            u[3] = u3;
            negate = -cdy;
            Two_Product(adxtail, negate, ti1, ti0);
            negate = -cdytail;
            Two_Product(adx, negate, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
            v[3] = v3;
            catlen = fast_expansion_sum_zeroelim(4, u, 4, v, cat);

            Two_Product(cdxtail, adytail, ti1, ti0);
            Two_Product(adxtail, cdytail, tj1, tj0);
            Two_Two_Diff(ti1, ti0, tj1, tj0, catt3, catt[2], catt[1], catt[0]);
            catt[3] = catt3;
            cattlen = 4;
        }
        else
        {
            cat[0] = 0.0;
            catlen = 1;
            catt[0] = 0.0;
            cattlen = 1;
        }

        if (bdxtail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, bdxtail, temp16a);
            bxtcatlen = scale_expansion_zeroelim(catlen, cat, bdxtail, bxtcat);
            temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, 2.0 * bdx, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;
            if (cdytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, aa, bdxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow;
                finnow = finother;
                finother = finswap;
            }
            if (adytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, cc, -bdxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow;
                finnow = finother;
                finother = finswap;
            }

            temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, bdxtail, temp32a);
            bxtcattlen = scale_expansion_zeroelim(cattlen, catt, bdxtail, bxtcatt);
            temp16alen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, 2.0 * bdx, temp16a);
            temp16blen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, bdxtail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow; finnow = finother; finother = finswap;
        }
        if (bdytail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(bytcalen, bytca, bdytail, temp16a);
            bytcatlen = scale_expansion_zeroelim(catlen, cat, bdytail, bytcat);
            temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, 2.0 * bdy, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;


            temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, bdytail, temp32a);
            bytcattlen = scale_expansion_zeroelim(cattlen, catt, bdytail, bytcatt);
            temp16alen = scale_expansion_zeroelim(bytcattlen, bytcatt, 2.0 * bdy, temp16a);
            temp16blen = scale_expansion_zeroelim(bytcattlen, bytcatt, bdytail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;
        }
    }
    if ((cdxtail != 0.0) || (cdytail != 0.0))
    {
        if ((adxtail != 0.0) || (adytail != 0.0) || (bdxtail != 0.0) || (bdytail != 0.0))
        {
            Two_Product(adxtail, bdy, ti1, ti0);
            Two_Product(adx, bdytail, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
            u[3] = u3;
            negate = -ady;
            Two_Product(bdxtail, negate, ti1, ti0);
            negate = -adytail;
            Two_Product(bdx, negate, tj1, tj0);
            Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
            v[3] = v3;
            abtlen = fast_expansion_sum_zeroelim(4, u, 4, v, abt);

            Two_Product(adxtail, bdytail, ti1, ti0);
            Two_Product(bdxtail, adytail, tj1, tj0);
            Two_Two_Diff(ti1, ti0, tj1, tj0, abtt3, abtt[2], abtt[1], abtt[0]);
            abtt[3] = abtt3;
            abttlen = 4;
        }
        else
        {
            abt[0] = 0.0;
            abtlen = 1;
            abtt[0] = 0.0;
            abttlen = 1;
        }

        if (cdxtail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, cdxtail, temp16a);
            cxtabtlen = scale_expansion_zeroelim(abtlen, abt, cdxtail, cxtabt);
            temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, 2.0 * cdx, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;
            if (adytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, bb, cdxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow;
                finnow = finother;
                finother = finswap;
            }
            if (bdytail != 0.0)
            {
                temp8len = scale_expansion_zeroelim(4, aa, -cdxtail, temp8);
                temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail, temp16a);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen, temp16a, finother);
                finswap = finnow; finnow = finother; finother = finswap;
            }

            temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, cdxtail, temp32a);
            cxtabttlen = scale_expansion_zeroelim(abttlen, abtt, cdxtail, cxtabtt);
            temp16alen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, 2.0 * cdx, temp16a);
            temp16blen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, cdxtail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;
        }
        if (cdytail != 0.0)
        {
            temp16alen = scale_expansion_zeroelim(cytablen, cytab, cdytail, temp16a);
            cytabtlen = scale_expansion_zeroelim(abtlen, abt, cdytail, cytabt);
            temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, 2.0 * cdy, temp32a);
            temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp32alen, temp32a, temp48);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len, temp48, finother);
            finswap = finnow;
            finnow = finother;
            finother = finswap;


            temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, cdytail, temp32a);
            cytabttlen = scale_expansion_zeroelim(abttlen, abtt, cdytail, cytabtt);
            temp16alen = scale_expansion_zeroelim(cytabttlen, cytabtt, 2.0 * cdy, temp16a);
            temp16blen = scale_expansion_zeroelim(cytabttlen, cytabtt, cdytail, temp16b);
            temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
            temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a, temp32blen, temp32b, temp64);
            finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len, temp64, finother);
            finswap = finnow; finnow = finother; finother = finswap;
        }
    }

    return finnow[finlength - 1];
}


qreal incircle(qreal *pa, qreal *pb, qreal *pc, qreal *pd)
{
    qreal adx, bdx, cdx, ady, bdy, cdy;
    qreal bdxcdy, cdxbdy, cdxady, adxcdy, adxbdy, bdxady;
    qreal alift, blift, clift;
    qreal det;
    qreal permanent, errbound;

    adx = pa[0] - pd[0];
    bdx = pb[0] - pd[0];
    cdx = pc[0] - pd[0];
    ady = pa[1] - pd[1];
    bdy = pb[1] - pd[1];
    cdy = pc[1] - pd[1];

    bdxcdy = bdx * cdy;
    cdxbdy = cdx * bdy;
    alift = adx * adx + ady * ady;

    cdxady = cdx * ady;
    adxcdy = adx * cdy;
    blift = bdx * bdx + bdy * bdy;

    adxbdy = adx * bdy;
    bdxady = bdx * ady;
    clift = cdx * cdx + cdy * cdy;

    det = alift * (bdxcdy - cdxbdy)
      + blift * (cdxady - adxcdy)
      + clift * (adxbdy - bdxady);

    permanent = (Absolute(bdxcdy) + Absolute(cdxbdy)) * alift
            + (Absolute(cdxady) + Absolute(adxcdy)) * blift
            + (Absolute(adxbdy) + Absolute(bdxady)) * clift;
    errbound = iccerrboundA * permanent;
    if ((det > errbound) || (-det > errbound))
    {
        return det;
    }

    return incircleadapt(pa, pb, pc, pd, permanent);
}

QT_WARNING_POP
