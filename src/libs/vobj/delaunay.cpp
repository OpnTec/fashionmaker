/*
**  delaunay.c : compute 2D delaunay triangulation in the plane.
**  Copyright (C) 2005  Wael El Oraiby <wael.eloraiby@gmail.com>
**
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtGlobal>

#include "../vmisc/diagnostic.h"
#include "delaunay.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wold-style-cast")
QT_WARNING_DISABLE_CLANG("-Wold-style-cast")
QT_WARNING_DISABLE_GCC("-Wcast-qual")

#if PREDICATE == EXACT_PREDICATE
extern void exactinit();
extern real incircle(real* pa, real* pb, real* pc, real* pd);
#endif

#define ON_RIGHT	1
#define ON_SEG		0
#define ON_LEFT		-1

#define OUTSIDE		-1
#define	ON_CIRCLE	0
#define INSIDE		1

struct	delaunay_s;
struct	face_s;
struct	halfedge_s;
struct	point2d_s;


#ifdef USE_DOUBLE
#	define REAL_ZERO	0.0
#	define REAL_ONE		1.0
#	define REAL_TWO		2.0
#	define REAL_FOUR	4.0
#	define TOLERANCE	(1024.0 * 1024.0)
#else
#	define REAL_ZERO	0.0f
#	define REAL_ONE		1.0f
#	define REAL_TWO		2.0f
#	define REAL_FOUR	4.0f
#	define TOLERANCE	(16.0f )
#endif

#define EPSILON		(REAL_ONE / TOLERANCE)

typedef struct point2d_s	point2d_t;
typedef struct face_s		face_t;
typedef struct halfedge_s	halfedge_t;
typedef struct delaunay_s	delaunay_t;
typedef real mat3_t[3][3];

struct point2d_s
{
    real			x, y;			/* point coordinates */
    halfedge_t*		he;			/* point halfedge */
    quint32		idx;			/* point index in input buffer */
};

struct face_s
{
/*	real			radius;
    real			cx, cy;
    point2d_t*		p[3];
*/
    halfedge_t*		he;			/* a pointing half edge */
    quint32		num_verts;		/* number of vertices on this face */
};

struct halfedge_s
{
    point2d_t*		vertex;			/* vertex */
    halfedge_t*		pair;			/* pair */
    halfedge_t*		next;			/* next */
    halfedge_t*		prev;			/* next^-1 */
    face_t*			face;			/* halfedge face */
};

struct delaunay_s
{
    halfedge_t*		rightmost_he;		/* right most halfedge */
    halfedge_t*		leftmost_he;		/* left most halfedge */
    point2d_t**		points;			/* pointer to points */
    face_t*			faces;			/* faces of delaunay */
    quint32		num_faces;		/* face count */
    quint32		start_point;		/* start point index */
    quint32		end_point;		/* end point index */
};


/*
* 3x3 matrix determinant
*/
//static real det3( mat3_t *m )
//{
//	real		res;

//	res		= ((*m)[0][0]) * (((*m)[1][1]) * ((*m)[2][2]) - ((*m)[1][2]) * ((*m)[2][1]))
//			- ((*m)[0][1]) * (((*m)[1][0]) * ((*m)[2][2]) - ((*m)[1][2]) * ((*m)[2][0]))
//			+ ((*m)[0][2]) * (((*m)[1][0]) * ((*m)[2][1]) - ((*m)[1][1]) * ((*m)[2][0]));

//	return res;
//}

/*
* allocate a point
*/
static point2d_t* point_alloc()
{
    point2d_t*	p;

    p	= (point2d_t*)malloc(sizeof(point2d_t));
    assert( p != NULL );
// cppcheck-suppress memsetClassFloat
    memset(p, 0, sizeof(point2d_t));

    return p;
}

/*
* free a point
*/
static void point_free( point2d_t* p )
{
    assert( p != NULL );
    free(p);
}

/*
* allocate a halfedge
*/
static halfedge_t* halfedge_alloc()
{
    halfedge_t*		d;

    d	= (halfedge_t*)malloc(sizeof(halfedge_t));
    assert( d != NULL );
    memset(d, 0, sizeof(halfedge_t));

    return d;
}

/*
* free a halfedge
*/
static void halfedge_free( halfedge_t* d )
{
    assert( d != NULL );
    memset(d, 0, sizeof(halfedge_t));
    free(d);
}

/*
* free all delaunay halfedges
*/
void del_free_halfedges( delaunay_t *del )
{
    quint32		i;
    halfedge_t		*d, *sig;

    /* if there is nothing to do */
    if( del->points == NULL )
        return;

    for( i = 0; i <= (del->end_point - del->start_point); i++ )
    {
        /* free all the halfedges around the point */
        d	= del->points[i]->he;
        if( d != NULL )
        {
            do {
                sig	= d->next;
                halfedge_free( d );
                d	= sig;
            } while( d != del->points[i]->he );
            del->points[i]->he	= NULL;
        }
    }
}

/*
 * allocate memory for a face
 */
//static face_t* face_alloc()
//{
//	face_t	*f = (face_t*)malloc(sizeof(face_t));
//	assert( f != NULL );
//	memset(f, 0, sizeof(face_t));
//	return f;
//}

/*
 * free a face
 */
//static void face_free(face_t *f)
//{
//	assert( f != NULL );
//	free(f);
//}

/*
* compare 2 points when sorting
*/
static int cmp_points( const void *_pt0, const void *_pt1 )
{
    point2d_t		*pt0, *pt1;

    pt0	= (point2d_t*)(*((point2d_t**)_pt0));
    pt1	= (point2d_t*)(*((point2d_t**)_pt1));

    if( pt0->x < pt1->x )
        return -1;
    else if( pt0->x > pt1->x )
        return 1;
    else if( pt0->y < pt1->y )
        return -1;
    else if( pt0->y > pt1->y )
        return 1;
    assert(0 && "2 or more points share the same exact coordinate");
    return 0; /* Should not be given! */
}

/*
* classify a point relative to a segment
*/
static int classify_point_seg( point2d_t *s, point2d_t *e, point2d_t *pt )
{
    point2d_t		se, spt;
    real		res;

    se.x	= e->x - s->x;
    se.y	= e->y - s->y;

    spt.x	= pt->x - s->x;
    spt.y	= pt->y - s->y;

    res	= (( se.x * spt.y ) - ( se.y * spt.x ));
    if( res < REAL_ZERO )
        return ON_RIGHT;
    else if( res > REAL_ZERO )
        return ON_LEFT;

    return ON_SEG;
}

/*
* classify a point relative to a halfedge, -1 is left, 0 is on, 1 is right
*/
static int del_classify_point( halfedge_t *d, point2d_t *pt )
{
    point2d_t		*s, *e;

    s		= d->vertex;
    e		= d->pair->vertex;

    return classify_point_seg(s, e, pt);
}

/*
* return the absolute value
*/
//static real dabs( real a )
//{
//	if( a < REAL_ZERO )
//		return (-a);
//	return a;
//}

/*
* compute the circle given 3 points
*/
#if PREDICATE == LOOSE_PREDICATE
static void compute_circle( point2d_t *pt0, point2d_t *pt1, point2d_t *pt2, real *cx, real *cy, real *radius )
{
    mat3_t	ma, mbx, mby, mc;
    real	x0y0, x1y1, x2y2;
    real	a, bx, by, c;

    /* calculate x0y0, .... */
    x0y0		= pt0->x * pt0->x + pt0->y * pt0->y;
    x1y1		= pt1->x * pt1->x + pt1->y * pt1->y;
    x2y2		= pt2->x * pt2->x + pt2->y * pt2->y;

    /* setup A matrix */
    ma[0][0]	= pt0->x;
    ma[0][1]	= pt0->y;
    ma[1][0]	= pt1->x;
    ma[1][1]	= pt1->y;
    ma[2][0]	= pt2->x;
    ma[2][1]	= pt2->y;
    ma[0][2]	= ma[1][2] = ma[2][2] = REAL_ONE;

    /* setup Bx matrix */
    mbx[0][0]	= x0y0;
    mbx[1][0]	= x1y1;
    mbx[2][0]	= x2y2;
    mbx[0][1]	= pt0->y;
    mbx[1][1]	= pt1->y;
    mbx[2][1]	= pt2->y;
    mbx[0][2]	= mbx[1][2] = mbx[2][2] = REAL_ONE;

    /* setup By matrix */
    mby[0][0]	= x0y0;
    mby[1][0]	= x1y1;
    mby[2][0]	= x2y2;
    mby[0][1]	= pt0->x;
    mby[1][1]	= pt1->x;
    mby[2][1]	= pt2->x;
    mby[0][2]	= mby[1][2] = mby[2][2] = REAL_ONE;

    /* setup C matrix */
    mc[0][0]	= x0y0;
    mc[1][0]	= x1y1;
    mc[2][0]	= x2y2;
    mc[0][1]	= pt0->x;
    mc[1][1]	= pt1->x;
    mc[2][1]	= pt2->x;
    mc[0][2]	= pt0->y;
    mc[1][2]	= pt1->y;
    mc[2][2]	= pt2->y;

    /* compute a, bx, by and c */
    a	= det3(&ma);
    bx	= det3(&mbx);
    by	= -det3(&mby);
    c	= -det3(&mc);

    *cx	= bx / (REAL_TWO * a);
    *cy	= by / (REAL_TWO * a);
    *radius	= sqrt(bx * bx + by * by - REAL_FOUR * a * c) / (REAL_TWO * dabs(a));
}
#endif

/*
* test if a point is inside a circle given by 3 points, 1 if inside, 0 if outside
*/
static int in_circle( point2d_t *pt0, point2d_t *pt1, point2d_t *pt2, point2d_t *p )
{
#if PREDICATE == EXACT_PREDICATE
    real res	= incircle(&(pt0->x), &(pt1->x), &(pt2->x), &(p->x));
    if( res > REAL_ZERO )
        return INSIDE;
    else if( res < REAL_ZERO )
        return OUTSIDE;

    return ON_CIRCLE;
#endif
#if PREDICATE == LOOSE_PREDICATE
    real cx, cy, radius;
    compute_circle(pt0, pt1, pt2, &cx, &cy, &radius);

    real	distance	= sqrt((p->x - cx) * (p->x - cx) + (p->y - cy) * (p->y - cy));
    if( distance < radius - EPSILON )
        return INSIDE;
    else if(distance > radius + EPSILON )
        return OUTSIDE;
    return ON_CIRCLE;
#endif
#if PREDICATE == FAST_PREDICATE
    mat3_t	ma, mbx, mby, mc;
    real	x0y0, x1y1, x2y2;
    real	a, bx, by, c, res;

    /* calculate x0y0, .... */
    x0y0		= pt0->x * pt0->x + pt0->y * pt0->y;
    x1y1		= pt1->x * pt1->x + pt1->y * pt1->y;
    x2y2		= pt2->x * pt2->x + pt2->y * pt2->y;

    /* setup A matrix */
    ma[0][0]	= pt0->x;
    ma[0][1]	= pt0->y;
    ma[1][0]	= pt1->x;
    ma[1][1]	= pt1->y;
    ma[2][0]	= pt2->x;
    ma[2][1]	= pt2->y;
    ma[0][2]	= ma[1][2] = ma[2][2] = REAL_ONE;

    /* setup Bx matrix */
    mbx[0][0]	= x0y0;
    mbx[1][0]	= x1y1;
    mbx[2][0]	= x2y2;
    mbx[0][1]	= pt0->y;
    mbx[1][1]	= pt1->y;
    mbx[2][1]	= pt2->y;
    mbx[0][2]	= mbx[1][2] = mbx[2][2] = REAL_ONE;

    /* setup By matrix */
    mby[0][0]	= x0y0;
    mby[1][0]	= x1y1;
    mby[2][0]	= x2y2;
    mby[0][1]	= pt0->x;
    mby[1][1]	= pt1->x;
    mby[2][1]	= pt2->x;
    mby[0][2]	= mby[1][2] = mby[2][2] = REAL_ONE;

    /* setup C matrix */
    mc[0][0]	= x0y0;
    mc[1][0]	= x1y1;
    mc[2][0]	= x2y2;
    mc[0][1]	= pt0->x;
    mc[1][1]	= pt1->x;
    mc[2][1]	= pt2->x;
    mc[0][2]	= pt0->y;
    mc[1][2]	= pt1->y;
    mc[2][2]	= pt2->y;

    /* compute a, bx, by and c */
    a	= det3(&ma);
    bx	= det3(&mbx);
    by	= -det3(&mby);
    c	= -det3(&mc);

    res	= a * (p->x * p->x + p->y * p->y ) - bx * p->x - by * p->y + c;


    if( res < REAL_ZERO )
        return INSIDE;
    else if( res > REAL_ZERO )
        return OUTSIDE;

    return ON_CIRCLE;
#endif
}

/*
* initialize delaunay segment
*/
static int del_init_seg( delaunay_t *del, int start )
{
    halfedge_t		*d0, *d1;
    point2d_t		*pt0, *pt1;

    /* init delaunay */
    del->start_point	= static_cast<quint32>(start);
    del->end_point		= static_cast<quint32>(start + 1);

    /* setup pt0 and pt1 */
    pt0			= del->points[start];
    pt1			= del->points[start + 1];

    /* allocate the halfedges and setup them */
    d0	= halfedge_alloc();
    d1	= halfedge_alloc();

    d0->vertex	= pt0;
    d1->vertex	= pt1;

    d0->next	= d0->prev	= d0;
    d1->next	= d1->prev	= d1;

    d0->pair	= d1;
    d1->pair	= d0;

    pt0->he	= d0;
    pt1->he	= d1;

    del->rightmost_he	= d1;
    del->leftmost_he	= d0;


    return 0;
}

/*
* initialize delaunay triangle
*/
static int del_init_tri( delaunay_t *del, int start )
{
    halfedge_t		*d0, *d1, *d2, *d3, *d4, *d5;
    point2d_t		*pt0, *pt1, *pt2;

    /* initiate delaunay */
    del->start_point	= static_cast<quint32>(start);
    del->end_point		= static_cast<quint32>(start + 2);

    /* setup the points */
    pt0					= del->points[start];
    pt1					= del->points[start + 1];
    pt2					= del->points[start + 2];

    /* allocate the 6 halfedges */
    d0	= halfedge_alloc();
    d1	= halfedge_alloc();
    d2	= halfedge_alloc();
    d3	= halfedge_alloc();
    d4	= halfedge_alloc();
    d5	= halfedge_alloc();

    if( classify_point_seg(pt0, pt2, pt1) == ON_LEFT )	/* first case */
    {
        /* set halfedges points */
        d0->vertex	= pt0;
        d1->vertex	= pt2;
        d2->vertex	= pt1;

        d3->vertex	= pt2;
        d4->vertex	= pt1;
        d5->vertex	= pt0;

        /* set points halfedges */
        pt0->he	= d0;
        pt1->he	= d2;
        pt2->he	= d1;

        /* next and next -1 setup */
        d0->next	= d5;
        d0->prev	= d5;

        d1->next	= d3;
        d1->prev	= d3;

        d2->next	= d4;
        d2->prev	= d4;

        d3->next	= d1;
        d3->prev	= d1;

        d4->next	= d2;
        d4->prev	= d2;

        d5->next	= d0;
        d5->prev	= d0;

        /* set halfedges pair */
        d0->pair	= d3;
        d3->pair	= d0;

        d1->pair	= d4;
        d4->pair	= d1;

        d2->pair	= d5;
        d5->pair	= d2;

        del->rightmost_he	= d1;
        del->leftmost_he		= d0;

    } else /* 2nd case */
    {
        /* set halfedges points */
        d0->vertex	= pt0;
        d1->vertex	= pt1;
        d2->vertex	= pt2;

        d3->vertex	= pt1;
        d4->vertex	= pt2;
        d5->vertex	= pt0;

        /* set points halfedges */
        pt0->he	= d0;
        pt1->he	= d1;
        pt2->he	= d2;

        /* next and next -1 setup */
        d0->next	= d5;
        d0->prev	= d5;

        d1->next	= d3;
        d1->prev	= d3;

        d2->next	= d4;
        d2->prev	= d4;

        d3->next	= d1;
        d3->prev	= d1;

        d4->next	= d2;
        d4->prev	= d2;

        d5->next	= d0;
        d5->prev	= d0;

        /* set halfedges pair */
        d0->pair	= d3;
        d3->pair	= d0;

        d1->pair	= d4;
        d4->pair	= d1;

        d2->pair	= d5;
        d5->pair	= d2;

        del->rightmost_he	= d2;
        del->leftmost_he		= d0;
    }

    return 0;
}

/*
* remove an edge given a halfedge
*/
static void del_remove_edge( halfedge_t *d )
{
    halfedge_t	*next, *prev, *pair, *orig_pair;

    orig_pair	= d->pair;

    next	= d->next;
    prev	= d->prev;
    pair	= d->pair;

    assert(next != NULL);
    assert(prev != NULL);

    next->prev	= prev;
    prev->next	= next;


    /* check to see if we have already removed pair */
    if( pair )
        pair->pair	= NULL;

    /* check to see if the vertex points to this halfedge */
    if( d->vertex->he == d )
        d->vertex->he	= next;

    d->vertex	= NULL;
    d->next		= NULL;
    d->prev		= NULL;
    d->pair		= NULL;

    next	= orig_pair->next;
    prev	= orig_pair->prev;
    pair	= orig_pair->pair;

    assert(next != NULL);
    assert(prev != NULL);

    next->prev	= prev;
    prev->next	= next;


    /* check to see if we have already removed pair */
    if( pair )
        pair->pair	= NULL;

    /* check to see if the vertex points to this halfedge */
    if( orig_pair->vertex->he == orig_pair )
        orig_pair->vertex->he	= next;

    orig_pair->vertex	= NULL;
    orig_pair->next		= NULL;
    orig_pair->prev		= NULL;
    orig_pair->pair		= NULL;


    /* finally free the halfedges */
    halfedge_free(d);
    halfedge_free(orig_pair);
}

/*
* pass through all the halfedges on the left side and validate them
*/
static halfedge_t* del_valid_left( halfedge_t* b )
{
    point2d_t		*g, *d, *u, *v;
    halfedge_t		*c, *du, *dg;

    g	= b->vertex;				/* base halfedge point */
    dg	= b;

    d	= b->pair->vertex;			/* pair(halfedge) point */
    b	= b->next;

    u	= b->pair->vertex;			/* next(pair(halfedge)) point */
    du	= b->pair;

    v	= b->next->pair->vertex;	/* pair(next(next(halfedge)) point */

    if( classify_point_seg(g, d, u) == ON_LEFT )
    {
        /* 3 points aren't colinear */
        /* as long as the 4 points belong to the same circle, do the cleaning */
        assert( v != u && "1: floating point precision error");
        while( v != d && v != g && in_circle(g, d, u, v) == INSIDE )
        {
            c	= b->next;
            du	= b->next->pair;
            del_remove_edge(b);
            b	= c;
            u	= du->vertex;
            v	= b->next->pair->vertex;
        }

        assert( v != u && "2: floating point precision error");
        if( v != d && v != g && in_circle(g, d, u, v) == ON_CIRCLE )
        {
            du	= du->prev;
            del_remove_edge(b);
        }
    } else	/* treat the case where the 3 points are colinear */
        du		= dg;

    assert(du->pair);
    return du;
}

/*
* pass through all the halfedges on the right side and validate them
*/
static halfedge_t* del_valid_right( halfedge_t *b )
{
    point2d_t		*rv, *lv, *u, *v;
    halfedge_t		*c, *dd, *du;

    b	= b->pair;
    rv	= b->vertex;
    dd	= b;
    lv	= b->pair->vertex;
    b	= b->prev;
    u	= b->pair->vertex;
    du	= b->pair;

    v	= b->prev->pair->vertex;

    if( classify_point_seg(lv, rv, u) == ON_LEFT )
    {
        assert( v != u && "1: floating point precision error");
        while( v != lv && v != rv && in_circle(lv, rv, u, v) == INSIDE )
        {
            c	= b->prev;
            du	= c->pair;
            del_remove_edge(b);
            b	= c;
            u	= du->vertex;
            v	= b->prev->pair->vertex;
        }

        assert( v != u && "1: floating point precision error");
        if( v != lv && v != rv && in_circle(lv, rv, u, v) == ON_CIRCLE )
        {
            du	= du->next;
            del_remove_edge(b);
        }
    } else
        du	= dd;

    assert(du->pair);
    return du;
}


/*
* validate a link
*/
static halfedge_t* del_valid_link( halfedge_t *b )
{
    point2d_t	*g, *g_p, *d, *d_p;
    halfedge_t	*gd, *dd, *new_gd, *new_dd;

    g	= b->vertex;
    gd	= del_valid_left(b);
    g_p	= gd->vertex;

    assert(b->pair);
    d	= b->pair->vertex;
    dd	= del_valid_right(b);
    d_p	= dd->vertex;
    assert(b->pair);

    if( g != g_p && d != d_p )
    {
        int a	= in_circle(g, d, g_p, d_p);

        if( a != ON_CIRCLE )
        {
            if( a == INSIDE )
            {
                g_p	= g;
                gd	= b;
            }
            else
            {
                d_p = d;
                dd	= b->pair;
            }
        }
    }

    /* create the 2 halfedges */
    new_gd	= halfedge_alloc();
    new_dd	= halfedge_alloc();

    /* setup new_gd and new_dd */

    new_gd->vertex	= gd->vertex;
    new_gd->pair	= new_dd;
    new_gd->prev	= gd;
    new_gd->next	= gd->next;
    gd->next->prev	= new_gd;
    gd->next		= new_gd;

    new_dd->vertex	= dd->vertex;
    new_dd->pair	= new_gd;
    new_dd->prev	= dd->prev;
    dd->prev->next	= new_dd;
    new_dd->next	= dd;
    dd->prev		= new_dd;

    return new_gd;
}

/*
* find the lower tangent between the two delaunay, going from left to right (returns the left half edge)
*/
static halfedge_t* del_get_lower_tangent( delaunay_t *left, delaunay_t *right )
{
    halfedge_t	*right_d, *left_d, *new_ld, *new_rd;
    int		sl, sr;

    left_d	= left->rightmost_he;
    right_d	= right->leftmost_he;

    do {
        point2d_t *pl = left_d->prev->pair->vertex;
        point2d_t *pr = right_d->pair->vertex;

        if( (sl = classify_point_seg(left_d->vertex, right_d->vertex, pl)) == ON_RIGHT ) {
            left_d	= left_d->prev->pair;
        }

        if( (sr = classify_point_seg(left_d->vertex, right_d->vertex, pr)) == ON_RIGHT ) {
            right_d	= right_d->pair->next;
        }

    } while( sl == ON_RIGHT || sr == ON_RIGHT );

    /* create the 2 halfedges */
    new_ld	= halfedge_alloc();
    new_rd	= halfedge_alloc();

    /* setup new_gd and new_dd */
    new_ld->vertex	= left_d->vertex;
    new_ld->pair	= new_rd;
    new_ld->prev	= left_d->prev;
    left_d->prev->next	= new_ld;
    new_ld->next	= left_d;
    left_d->prev	= new_ld;

    new_rd->vertex	= right_d->vertex;
    new_rd->pair	= new_ld;
    new_rd->prev	= right_d->prev;
    right_d->prev->next	= new_rd;
    new_rd->next	= right_d;
    right_d->prev	= new_rd;

    return new_ld;
}

/*
* link the 2 delaunay together
*/
static void del_link( delaunay_t *result, delaunay_t *left, delaunay_t *right )
{
    point2d_t		*u, *v, *ml, *mr;
    halfedge_t		*base;

    assert( left->points == right->points );

    /* save the most right point and the most left point */
    ml		= left->leftmost_he->vertex;
    mr		= right->rightmost_he->vertex;

    base		= del_get_lower_tangent(left, right);

    u		= base->next->pair->vertex;
    v		= base->pair->prev->pair->vertex;

    while( del_classify_point(base, u) == ON_LEFT ||
           del_classify_point(base, v) == ON_LEFT )
    {
        base	= del_valid_link(base);
        u	= base->next->pair->vertex;
        v	= base->pair->prev->pair->vertex;
    }

    right->rightmost_he	= mr->he;
    left->leftmost_he	= ml->he;

    /* TODO: this part is not needed, and can be optimized */
    while( del_classify_point( right->rightmost_he, right->rightmost_he->prev->pair->vertex ) == ON_RIGHT )
           right->rightmost_he	= right->rightmost_he->prev;

    while( del_classify_point( left->leftmost_he, left->leftmost_he->prev->pair->vertex ) == ON_RIGHT )
           left->leftmost_he	= left->leftmost_he->prev;

    result->leftmost_he		= left->leftmost_he;
    result->rightmost_he		= right->rightmost_he;
    result->points			= left->points;
    result->start_point		= left->start_point;
    result->end_point		= right->end_point;
}

/*
* divide and conquer delaunay
*/
void del_divide_and_conquer( delaunay_t *del, int start, int end );
void del_divide_and_conquer( delaunay_t *del, int start, int end )
{
    delaunay_t	left, right;

    int n		= (end - start + 1);

    if( n > 3 )
    {
        int i		= (n / 2) + (n & 1);
        left.points		= del->points;
        right.points	= del->points;
        del_divide_and_conquer( &left, start, start + i - 1 );
        del_divide_and_conquer( &right, start + i, end );
        del_link( del, &left, &right );
    } else
        if( n == 3 )
            del_init_tri( del, start );
        else
            if( n == 2 )
                del_init_seg( del, start );
}

static void build_halfedge_face( delaunay_t *del, halfedge_t *d )
{
    halfedge_t	*curr;

    /* test if the halfedge has already a pointing face */
    if( d->face != NULL )
        return;

    del->faces = (face_t*)realloc(del->faces, (del->num_faces + 1) * sizeof(face_t));

    face_t	*f	= &(del->faces[del->num_faces]);
    curr	= d;
    f->he	= d;
    f->num_verts	= 0;
    do {
        curr->face	= f;
        (f->num_verts)++;
        curr	= curr->pair->prev;
    } while( curr != d );

    (del->num_faces)++;

/*	if( d->face.radius < 0.0 )
    {
        d->face.p[0]	= d->vertex;
        d->face.p[1]	= d->pair->vertex;
        d->face.p[2]	= d->pair->prev->pair->vertex;

        if( classify_point_seg( d->face.p[0], d->face.p[1], d->face.p[2] ) == ON_LEFT )
        {
            compute_circle(d->face.p[0], d->face.p[1], d->face.p[2], &(d->face.cx), &(d->face.cy), &(d->face.radius));
        }
    }
*/
}

/*
* build the faces for all the halfedge
*/
void del_build_faces( delaunay_t *del )
{
    quint32	i;
    halfedge_t	*curr;

    del->num_faces	= 0;
    del->faces		= NULL;

    /* build external face first */
    build_halfedge_face(del, del->rightmost_he->pair);

    for( i = del->start_point; i <= del->end_point; i++ )
    {
        curr	= del->points[i]->he;

        do {
            build_halfedge_face( del, curr );
            curr	= curr->next;
        } while( curr != del->points[i]->he );
    }
}

/*
*/
delaunay2d_t* delaunay2d_from(del_point2d_t *points, quint32 num_points) {
    delaunay2d_t*	res	= NULL;
    delaunay_t	del;
    quint32	i;
    quint32*	faces	= NULL;

    del.num_faces	= 0; //Warning using uninitialized value

#if PREDICATE == EXACT_PREDICATE
    exactinit();
#endif

    /* allocate the points */
    del.points	= (point2d_t**)malloc(num_points * sizeof(point2d_t*));
    assert( del.points != NULL );
    memset(del.points, 0, num_points * sizeof(point2d_t*));

    /* copy the points */
    for( i = 0; i < num_points; i++ )
    {
        del.points[i]		= point_alloc();
        del.points[i]->idx	= i;
        del.points[i]->x	= points[i].x;
        del.points[i]->y	= points[i].y;
    }

    qsort(del.points, num_points, sizeof(point2d_t*), cmp_points);

    if( num_points >= 3 ) {
        quint32 fbuff_size = 0;
        quint32 j = 0;

        del_divide_and_conquer( &del, 0, num_points - 1 );
        del_build_faces( &del );

        for( i = 0; i < del.num_faces; i++ )
            fbuff_size	+= del.faces[i].num_verts + 1;

        faces = (quint32*)malloc(sizeof(quint32) * fbuff_size);

        for( i = 0; i < del.num_faces; i++ )
        {
            halfedge_t	*curr;

            faces[j]	= del.faces[i].num_verts;
            j++;

            curr	= del.faces[i].he;
            do {
                faces[j]	= curr->vertex->idx;
                j++;
                curr	= curr->pair->prev;
            } while( curr != del.faces[i].he );
        }

        del_free_halfedges( &del );

        free(del.faces);
        for( i = 0; i < num_points; i++ )
            point_free(del.points[i]);

        free(del.points);
    }

    res		= (delaunay2d_t*)malloc(sizeof(delaunay2d_t));
    res->num_points	= num_points;
    res->points	= (del_point2d_t*)malloc(sizeof(del_point2d_t) * num_points);
    memcpy(res->points, points, sizeof(del_point2d_t) * num_points);
    res->num_faces	= del.num_faces;
    res->faces	= faces;

    return res;
}

void delaunay2d_release(delaunay2d_t *del) {
    free(del->faces);
    free(del->points);
    free(del);
}

QT_WARNING_POP
