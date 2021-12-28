#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define	EXIT_FAILURE 1
#define	X 0
#define	Y 1
typedef	enum { FALSE, TRUE } bool;

#define	DIM 2              
typedef	int tPointi[DIM];  

typedef	struct tVertexStructure tsVertex;
typedef	tsVertex *tVertex;
struct	tVertexStructure {
   int		vnum;	
   tPointi	v;	
   bool 	ear;	
   tVertex 	next,prev;
};


tVertex	vertices  = NULL;
int	nvertices = 0;	




int	AreaPoly2( void );
void	Triangulate( void );
void	EarInit( void );
bool	Diagonal( tVertex a, tVertex b );
bool	Diagonalie( tVertex a, tVertex b );
bool	InCone( tVertex a, tVertex b );

int     Area2( tPointi a, tPointi b, tPointi c );
int     AreaSign( tPointi a, tPointi b, tPointi c );
bool	Xor( bool x, bool y );
bool	Left( tPointi a, tPointi b, tPointi c );
bool	LeftOn( tPointi a, tPointi b, tPointi c );
bool	Collinear( tPointi a, tPointi b, tPointi c );
bool	Between( tPointi a, tPointi b, tPointi c );
bool	Intersect( tPointi a, tPointi b, tPointi c, tPointi d );
bool	IntersectProp( tPointi a, tPointi b, tPointi c, tPointi d );

tVertex	MakeNullVertex( void );
void	ReadVertices( void );
void	PrintVertices( void );
void	PrintDiagonal( tVertex a, tVertex b );
void	PrintPoly( void );


#define NEW(p, type) if ((p=(type *) malloc (sizeof(type))) == NULL) {\
      printf ("NEW: Out of Memory!\n");\
      exit(EXIT_FAILURE);\
    }

#define ADD( head, p )  if ( head )  { \
       p->next = head; \
       p->prev = head->prev; \
       head->prev = p; \
       p->prev->next = p; \
    } \
    else { \
       head = p; \
       head->next = head->prev = p; \
    }


int main()
{
   ReadVertices();
   PrintVertices();
   printf("%%Area of polygon = %g\n", 0.5 * AreaPoly2() );
   Triangulate();
   printf("showpage\n%%%%EOF\n");
   return 0;
}


int     Area2( tPointi a, tPointi b, tPointi c )
{
   return
          (b[X] - a[X]) * (c[Y] - a[Y]) -
          (c[X] - a[X]) * (b[Y] - a[Y]);
}



bool	Xor( bool x, bool y )
{
   
   return   !x ^ !y;
}


bool	IntersectProp( tPointi a, tPointi b, tPointi c, tPointi d )
{
   
   if (
      Collinear(a,b,c) ||
      Collinear(a,b,d) ||
      Collinear(c,d,a) ||
      Collinear(c,d,b)
      )
      return FALSE;

   return
         Xor( Left(a,b,c), Left(a,b,d) )
      && Xor( Left(c,d,a), Left(c,d,b) );
}


bool	Left( tPointi a, tPointi b, tPointi c )
{ 
   return  AreaSign( a, b, c ) > 0;
}

bool	LeftOn( tPointi a, tPointi b, tPointi c )
{
   return  AreaSign( a, b, c ) >= 0;
}

bool	Collinear( tPointi a, tPointi b, tPointi c )
{
   return  AreaSign( a, b, c ) == 0;
}


bool	Between( tPointi a, tPointi b, tPointi c )
{
   tPointi	ba, ca;

   if ( ! Collinear( a, b, c ) )
      return  FALSE;

  
   if ( a[X] != b[X] ) 
      return ((a[X] <= c[X]) && (c[X] <= b[X])) ||
             ((a[X] >= c[X]) && (c[X] >= b[X]));
   else
      return ((a[Y] <= c[Y]) && (c[Y] <= b[Y])) ||
             ((a[Y] >= c[Y]) && (c[Y] >= b[Y]));
}


bool	Intersect( tPointi a, tPointi b, tPointi c, tPointi d )
{
   if      ( IntersectProp( a, b, c, d ) )
      return  TRUE;
   else if (   Between( a, b, c )
            || Between( a, b, d )
            || Between( c, d, a )
            || Between( c, d, b )
           )
      return  TRUE;
   else    return  FALSE;
}


bool   Diagonalie( tVertex a, tVertex b )
{
   tVertex c, c1;

  
   c = vertices;
   do {
      c1 = c->next;
     
      if (    ( c != a ) && ( c1 != a )
           && ( c != b ) && ( c1 != b )
           && Intersect( a->v, b->v, c->v, c1->v )
         )
         return FALSE;
      c = c->next;
   } while ( c != vertices );
   return TRUE;
}


void   EarInit( void )
{
   tVertex v0, v1, v2;  

  
   v1 = vertices;
   printf("newpath\n");
   do {
      v2 = v1->next;
      v0 = v1->prev;
      v1->ear = Diagonal( v0, v2 );
      v1 = v1->next;
   } while ( v1 != vertices );
   printf("closepath stroke\n\n");

}




void   Triangulate( void )
{
   tVertex v0, v1, v2, v3, v4;
   int   n = nvertices;	
   bool earfound;	

   EarInit();
   printf("\nnewpath\n");
  printf("value of n: %d\n",n);

   while ( n > 3 ) {     
     
      v2 = vertices;
      earfound = FALSE;
	  int i = 0;
	  int j = 0;
      do {
		  printf(" i: %d\n",i++);
         if (v2->ear) {
			 printf("j: %d\n",j++);

            earfound = TRUE;
           
            v3 = v2->next; v4 = v3->next;
            v1 = v2->prev; v0 = v1->prev;

           
            PrintDiagonal( v1, v3 );
            
           
            v1->ear = Diagonal( v0, v3 );
            v3->ear = Diagonal( v1, v4 );
            
           
            v1->next = v3;
            v3->prev = v1;
            vertices = v3;
            n--;

			printf(" n: %d\n",n);
            break;  
         }
         v2 = v2->next;
      } while ( v2 != vertices );

      if ( !earfound ) {
         printf("%%Error in Triangulate:  No ear found.\n");
         PrintPoly();
         printf("showpage\n%%%%EOF\n");
         exit(EXIT_FAILURE);
      }
   }
   printf("closepath stroke\n\n");
}



bool   InCone( tVertex a, tVertex b )
{
   tVertex a0,a1;

   a1 = a->next;
   a0 = a->prev;

  
   if( LeftOn( a->v, a1->v, a0->v ) )
       return    Left( a->v, b->v, a0->v )
              && Left( b->v, a->v, a1->v );

  
       return !(    LeftOn( a->v, b->v, a1->v )
                 && LeftOn( b->v, a->v, a0->v ) );
}


bool	Diagonal( tVertex a, tVertex b )
{
   return InCone( a, b ) && InCone( b, a ) && Diagonalie( a, b );
}



void   ReadVertices( void )
{
   tVertex	v;
   int		x, y;
   int		vnum = 0;

   while ( scanf ("%d %d", &x, &y ) != EOF )  {
      v = MakeNullVertex();
      v->v[X] = x;
      v->v[Y] = y;
      v->vnum = vnum++;
   }
   nvertices = vnum;
   printf("total no of vervices:%d \n",vnum);
   if (nvertices < 3) 
      printf("%%Error in ReadVertices: nvertices=%d<3\n", nvertices),
      exit(EXIT_FAILURE);
}

tVertex   MakeNullVertex( void )
{
   tVertex  v;
   
   NEW( v, tsVertex );
   ADD( vertices, v );
   return v;
}


void   PrintPoly( void )
{
   tVertex  v;
   printf("%%Polygon circular list:\n");
   v = vertices;
   do {                                 
      printf( "%% vnum=%5d:\tear=%d\n", v->vnum, v->ear );
      v = v->next;
   } while ( v != vertices );
}


void   PrintVertices( void )
{
  
   tVertex  v;
   int xmin, ymin, xmax, ymax;

  
   v = vertices;
   xmin = xmax = v->v[X];
   ymin = ymax = v->v[Y];
   do {
      if      ( v->v[X] > xmax ) xmax = v->v[X];
      else if ( v->v[X] < xmin ) xmin = v->v[X];
      if      ( v->v[Y] > ymax ) ymax = v->v[Y];
      else if ( v->v[Y] < ymin ) ymin = v->v[Y];
      v = v->next;
   } while ( v != vertices );
   
  
   printf("%%%%BoundingBox: %d %d %d %d\n", xmin, ymin, xmax, ymax);
   printf("%%%%EndComments\n");
   printf(".00 .00 setlinewidth\n");
   printf("%d %d translate\n", -xmin+72, -ymin+72 );
  

  
   printf("\n%% number of vertices = %d\n", nvertices);
   v = vertices;
   do {                                 
      printf( "%% vnum=%5d:\tx=%5d\ty=%5d\n", v->vnum, v->v[X], v->v[Y] );
      v = v->next;
   } while ( v != vertices );

  
   printf("\n%%Polygon:\n");
   printf("newpath\n");
   v = vertices;
   printf("%d\t%d\tmoveto\n", v->v[X], v->v[Y] );
   v = v->next;
   do {                                 
      printf("%d\t%d\tlineto\n", v->v[X], v->v[Y] );
      v = v->next;
   } while ( v != vertices );
   printf("closepath stroke\n");

}

void	PrintDiagonal( tVertex a, tVertex b )
{
   printf("%%Diagonal: (%d,%d)\n", a->vnum, b->vnum );
   printf("%d\t%d\tmoveto\n", a->v[X], a->v[Y] );
   printf("%d\t%d\tlineto\n", b->v[X], b->v[Y] );
}

int	AreaPoly2( void )
{
   int     sum = 0;
   tVertex p, a;

   p = vertices;  
   a = p->next;   
   do {
      sum += Area2( p->v, a->v, a->next->v );
      a = a->next;
   } while ( a->next != vertices );
   return sum;

}
int     AreaSign( tPointi a, tPointi b, tPointi c )
{
    double area2;

    area2 = ( b[0] - a[0] ) * (double)( c[1] - a[1] ) -
            ( c[0] - a[0] ) * (double)( b[1] - a[1] );

   
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}