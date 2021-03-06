#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define	X 0
#define	Y 1
typedef	enum { FALSE, TRUE } bool;
#define	DIM 2              
typedef	int tPointi[DIM];  
#define	EXIT_FAILURE 1
typedef	struct tVertexStructure tsVertex;
typedef	tsVertex *tVertex;

#define NEW(p, type) \
      if ((p=(type *) malloc (sizeof(type))) == NULL) {\
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

#define FREE(p) if (p) {free ((char *) p); p = NULL; }

//basic vertex structure
struct tVertexStructure {
   int vnum;	
   tPointi v;	
   bool ear;	
   tVertex next,prev;
};


//Initialize
tVertex	vertices  = NULL;
int	nvertices = 0;	

//declaration of all functions
void	ReadVertices();
void	PrintVertices();
void	Triangulate();
void	EarInit();
void	PrintDiagonal( tVertex a, tVertex b );

int   Area2( tPointi a, tPointi b, tPointi c );
int   AreaSign( tPointi a, tPointi b, tPointi c );
bool	Xor( bool x, bool y );
bool	Left( tPointi a, tPointi b, tPointi c );
bool	LeftOn( tPointi a, tPointi b, tPointi c );
bool	Collinear( tPointi a, tPointi b, tPointi c );
bool	Between( tPointi a, tPointi b, tPointi c );
bool	Intersect( tPointi a, tPointi b, tPointi c, tPointi d );
bool	IntersectProp( tPointi a, tPointi b, tPointi c, tPointi d );
int	AreaPoly2();
bool	Diagonal( tVertex a, tVertex b );
bool	Diagonalie( tVertex a, tVertex b );
bool	InCone( tVertex a, tVertex b );

int main()
{
   ReadVertices();
   PrintVertices();
   Triangulate();

   return 0;
}



void   ReadVertices()
{
   tVertex	v;
   
   int vertexCount = 0;
   int x, y;
   printf("-------------------------\n");
   printf("Step 1: Reading started. Provide input of (x,y) position of vertices.\nTo finish press ctrl+d\n\n");
   while (scanf("%d %d", &x, &y) != EOF) {
      NEW(v, tsVertex);
      v->vnum = vertexCount;
      v->v[X] = x;
      v->v[Y] = y;
      v->ear = FALSE;
      ADD(vertices, v);

      vertexCount++;
   }


   

   nvertices = vertexCount;

   //handle exception
   if (nvertices < 3) {
      printf("Total vertices cannot be less than 3.\n");
      exit(EXIT_FAILURE);
   }
   else{
      printf("\nStep 1: Reading completed without error\n");
      printf("-------------------------\n\n");

   }
      
}

void  PrintVertices()
{
   tVertex  v;

   printf("-------------------------\n");
   printf("\nStep 2: Print vertices started: \n\nTotal Vertices = %d\n", nvertices);

   v = vertices;
   
   do {                                 
      printf("Vertex Id= %d, position=(x,y): (%d,%d)\n", v->vnum, v->v[X], v->v[Y] );
      v = v->next;
   } while ( v != vertices );

   printf("\nStep 2: Print vertices completed\n");
   printf("-------------------------\n\n");


}

void  Triangulate()
{
   tVertex v0, v1, v2, v3, v4;
   int   n = nvertices;	
   
   bool earfound;	

   EarInit();
   
   printf("-------------------------\n");
   printf("\nStep 3: Triangulation started:\n\n");

   while ( n > 3 ) {     
     
      v2 = vertices;
      earfound = FALSE;
	  
      do {

         printf("Current vertex: %d,  ear? : %d\n", v2->vnum, v2->ear);

         if (v2->ear) {

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

			   printf("Remaining Vertices Count: %d\n\n",n);

            break;  
         }

         printf("\n");

         v2 = v2->next;

      } while ( v2 != vertices );


      if ( !earfound ) {

         printf("Triangulation error:  No ear found.\n");

         tVertex  v;

         printf("\nCurrent Polygon Vertices:\n");
         v = vertices;
         do {    
            printf( "%% Vertex Id=%5d:\tear=%d, (x,y):(%d,%d)\n", v->vnum, v->ear, v->v[X], v->v[Y] );
            v = v->next;
         } while ( v != vertices );

         exit(EXIT_FAILURE);
      }
      
   }

   printf("\nStep 3: Triangulation completed successfully:\n\n");
   printf("-------------------------\n");

}




void   EarInit()
{
   printf("\nEarInit started");

   tVertex v0, v1, v2;  

   v1 = vertices;
   do {
      v2 = v1->next;
      v0 = v1->prev;
      v1->ear = Diagonal( v0, v2 );
      v1 = v1->next;
   } while ( v1 != vertices );

   printf("\nEarInit completed\n\n");
}

void	PrintDiagonal( tVertex a, tVertex b )
{
   printf("Diagonal Vertices Ids: (%d,%d)\n", a->vnum, b->vnum );
   printf("from %d(%d,%d) to %d(%d,%d)\n", a->vnum, a->v[X], a->v[Y], b->vnum, b->v[X], b->v[Y] );
}


bool Diagonal( tVertex a, tVertex b )
{
   return InCone( a, b ) && InCone( b, a ) && Diagonalie( a, b );
}

bool InCone( tVertex a, tVertex b )
{
   tVertex a0,a1;
   
   a1 = a->next;
   a0 = a->prev;

  if( LeftOn( a->v, a1->v, a0->v ) )
       return    Left( a->v, b->v, a0->v )
              && Left( b->v, a->v, a1->v );

  
       return !( LeftOn( a->v, b->v, a1->v )
                 && LeftOn( b->v, a->v, a0->v ) );
}

bool Diagonalie( tVertex a, tVertex b )
{
   tVertex c, c1;

   c = vertices;
   do {
      c1 = c->next;
     
      if ( ( c != a ) && ( c1 != a )
           && ( c != b ) && ( c1 != b )
           && Intersect( a->v, b->v, c->v, c1->v )
         )
         return FALSE;
      c = c->next;
   } while ( c != vertices );
   return TRUE;
}

bool	Intersect( tPointi a, tPointi b, tPointi c, tPointi d )
{
   if(IntersectProp( a, b, c, d )){
      return  TRUE;
   }
      
   else if (   Between( a, b, c ) || Between( a, b, d )
            || Between( c, d, a ) || Between( c, d, b )
           )
      return  TRUE;
   else   return  FALSE;
}

bool	IntersectProp( tPointi a, tPointi b, tPointi c, tPointi d )
{
   if (
      Collinear(a,b,c) || Collinear(a,b,d) ||
      Collinear(c,d,a) || Collinear(c,d,b)
      )
      return FALSE;

   return
         Xor( Left(a,b,c), Left(a,b,d) )
      && Xor( Left(c,d,a), Left(c,d,b) );
}


bool	Xor( bool x, bool y )
{
   return !x ^ !y;
}

bool Collinear( tPointi a, tPointi b, tPointi c )
{
   return  AreaSign( a, b, c ) == 0;
}

bool Left( tPointi a, tPointi b, tPointi c )
{ 
   return  AreaSign( a, b, c ) > 0;
}

bool LeftOn( tPointi a, tPointi b, tPointi c )
{
   return  AreaSign( a, b, c ) >= 0;
}

bool	Between( tPointi a, tPointi b, tPointi c )
{

   if ( ! Collinear( a, b, c ) )
      return  FALSE;

   if ( a[X] != b[X] ) 
      return ((a[X] <= c[X]) && (c[X] <= b[X])) ||
             ((a[X] >= c[X]) && (c[X] >= b[X]));
   else
      return ((a[Y] <= c[Y]) && (c[Y] <= b[Y])) ||
             ((a[Y] >= c[Y]) && (c[Y] >= b[Y]));
}



int AreaPoly2()
{
   int sum = 0;
   tVertex p, a;

   p = vertices;  
   a = p->next;   
   do {
      sum += Area2( p->v, a->v, a->next->v );
      a = a->next;
   } while ( a->next != vertices );
   return sum;

}


int Area2( tPointi a, tPointi b, tPointi c )
{
   return (b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]);
}


int AreaSign( tPointi a, tPointi b, tPointi c )
{
    double area2 = ( b[0] - a[0] ) * (double)( c[1] - a[1] ) -
            ( c[0] - a[0] ) * (double)( b[1] - a[1] );

   
    if( area2 >  0.5 ) return  1;
    else if( area2 < -0.5 ) return -1;
    else return  0;
}