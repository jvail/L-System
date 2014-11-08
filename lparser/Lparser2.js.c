/* -------------------------------------------------------------------------
   LPARSER, L-System Parser/Mutator
   -------------------------------------------------------------------------
   laurens.lapre@gmail.com
   -------------------------------------------------------------------------

   modified by Jan Vaillant
*/

// c


#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <time.h>
#include <emscripten.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Basic types ------------------------------------------------------------ */


#define u8               unsigned char
#define u16              unsigned short int
#define u32              unsigned long int

#define s8               signed char
#define s16              signed short int
#define s32              signed long int

#define r32              float

#define bool              s16
#ifndef true
#define true             (s16) 1
#endif
#ifndef false
#define false            (s16) 0
#endif


/* Constants -------------------------------------------------------------- */


#define max_string       1024   /* Max char size of filename and large string */
#define max_colors       100    /* Max color indices */
#define vectors_p_poly   15     /* Max vectors per polygon */
#define max_p_object     400    /* Max polygons per object */
#define max_stack        1024L  /* Max size of the [] and {} stacks during drawing */
#define rule_n           200    /* Max number of the rules in the ls file */
#define rule_s           200    /* Max size of the rules in the ls file */

/* Vector indices */
#define _x               0
#define _y               1
#define _z               2

/* Some often used consts */
#define zero             (r32) 0.0
#define one              (r32) 1.0
#define half             (r32) 0.5
#define pi               (r32) 3.141592
#define LF               ((char) 10)
#define CR               ((char) 13)
#define min_bar          "---------------------------------------------------------"


/* Array and vector types ------------------------------------------------- */


/* A large string */
typedef char            large_string[max_string];

/* Vector arrays */
typedef r32             vector[3];
typedef vector          vectors_4[4];
typedef vector          vectors_max[vectors_p_poly];
typedef r32             float_array[max_p_object + 1];

/* Polygon arrays */
typedef s16             polygon_type[4];
typedef polygon_type    polygon_array[max_p_object + 1];


/* Intrinsics ------------------------------------------------------------- */


#define Abs_s16(A)       ((s16) abs((A)))
#define Abs_s32(A)       ((s32) abs((A)))
#define Abs_r32(A)       ((r32) fabs((A)))

#define MIN(A,B)         (((A) < (B)) ? (A) : (B))
#define MAX(A,B)         (((A) > (B)) ? (A) : (B))

#define Vector_copy_max_r32(n,A,B) memcpy(((void *) (B)), ((void *) (A)), n*12L)
#define Vector_copy_r32(A,B)       memcpy(((void *) (B)), ((void *) (A)),   12L)
#define Mem_copy(A,B,C)            memcpy(((void *) (B)), ((void *) (A)), (C))


/* Vector utils and inlines ----------------------------------------------- */


#define Vector_length(A)    ((r32) sqrt(   (r32)(A[_x] * A[_x])\
                                         + (r32)(A[_y] * A[_y])\
                                         + (r32)(A[_z] * A[_z]) ))

#define Scalar_product(A,B) (A[_x] * B[_x] + A[_y] * B[_y] + A[_z] * B[_z])

#define Vector_make(A,x,y,z) {\
  A[_x] = x;\
  A[_y] = y;\
  A[_z] = z;\
}

#define Vector_normalize(A)\
{ r32 Dist = one / Vector_length(A);\
\
  A[_x] *= Dist;\
  A[_y] *= Dist;\
  A[_z] *= Dist;\
}

#define Vector_product(A,B,C) {\
  C[_x] = A[_y] * B[_z] - A[_z] * B[_y];\
  C[_y] = A[_z] * B[_x] - A[_x] * B[_z];\
  C[_z] = A[_x] * B[_y] - A[_y] * B[_x];\
}

#define Vector_min(A,B,C) {\
  C[_x] = A[_x] - B[_x];\
  C[_y] = A[_y] - B[_y];\
  C[_z] = A[_z] - B[_z];\
}

#define Vector_plus(A,B,C) {\
  C[_x] = A[_x] + B[_x];\
  C[_y] = A[_y] + B[_y];\
  C[_z] = A[_z] + B[_z];\
}

#define Vector_dec(A,B) {\
  A[_x] -= B[_x];\
  A[_y] -= B[_y];\
  A[_z] -= B[_z];\
}

#define Vector_neg(A) {\
  A[_x] = (-A[_x]);\
  A[_y] = (-A[_y]);\
  A[_z] = (-A[_z]);\
}

#define Vector_inc(A,B) {\
  A[_x] += B[_x];\
  A[_y] += B[_y];\
  A[_z] += B[_z];\
}

#define Vector_plus_fac(A,B,t,C) {\
  C[_x] = A[_x] + (t) * B[_x];\
  C[_y] = A[_y] + (t) * B[_y];\
  C[_z] = A[_z] + (t) * B[_z];\
}

#define Vector_plus_fac2(A,B,b,C,c,D) {\
  D[_x] = A[_x] + (b) * B[_x] + (c) * C[_x];\
  D[_y] = A[_y] + (b) * B[_y] + (c) * C[_y];\
  D[_z] = A[_z] + (b) * B[_z] + (c) * C[_z];\
}

#define Vector_combine(A,a,B,b,C) {\
  C[_x] = (a) * A[_x] + (b) * B[_x];\
  C[_y] = (a) * A[_y] + (b) * B[_y];\
  C[_z] = (a) * A[_z] + (b) * B[_z];\
}

#define Vector_add(A,d) {\
  A[_x] += d;\
  A[_y] += d;\
  A[_z] += d;\
}

#define Vector_sub(A,d) {\
  A[_x] -= d;\
  A[_y] -= d;\
  A[_z] -= d;\
}

#define Vector_div(A,d) {\
  A[_x] /= d;\
  A[_y] /= d;\
  A[_z] /= d;\
}

#define Vector_mul(A,d) {\
  A[_x] *= d;\
  A[_y] *= d;\
  A[_z] *= d;\
}


/* Init Lparser vars ------------------------------------------------------ */


static r32              zmin = (r32) 1e30, thick = 100, rand_amount = zero;
static r32              min_thick = zero;
static r32              trope_amount = zero;
static u32              max_prod = 4L * 1024L * 1024L; /* Maximum size l-system production string */
static u32              poly_count = 0L; mesh_count = 0L, vert_count = 0L, pcount_limit = 0L;
static s16              num = 0, col = 2, lev = 1, last_col = 0, obj = 0, muts = 0;
static r32              dis = one, ang = (r32) 45.0, dis2 = half, tr = (r32) 0.2;
static vector           sky = {zero, zero, one}, trope;
static vector           last = {one, one, one}, last_v[9];
static vector           axis_x, axis_y, axis_z;
static vector           color_store[max_colors];
static bool             closed_form = false, switch_yz = false, trace = false;
static bool             nowait = false, poly_limit = false;
static large_string     inputname, outputname, directory;
static r32				x_max, x_min, y_max, y_min, z_max, z_min;

/* Settings stack used for solving [] references */
typedef struct s_rec {
    vector                  pos;            /* Position in 3space of turtle
                                             * origin */
    vector                  fow;            /* Forward direction */

    vector                  lef;            /* Left direction */
    vector                  upp;            /* Up direction */
    vector                  last;           /* Last position used for
                                             * connecting cylinders */
    vector                  last_v[9];      /* Last vertices of object used for
                                             * connecting cylinders */
    r32                     dis;            /* Value of F distance */
    r32                     ang;            /* Value of basic angle */
    r32                     thick;          /* Value of thickness */
    r32                     dis2;           /* Value of Z distance */
    r32                     tr;             /* Trope value */
    s16                     col;            /* Current color */
    s16                     last_col;       /* Color of last object */
    s16		                obj;	        /* Current object # */
    s16		        	    last_obj;	    /* Last object # */
} s_rec;

/* Polygon stack used for solving {} references */
typedef struct p_rec {
    s16                     count;          /* Number of vertices */
    vector                 *ver;            /* Vertex store */
} p_rec;


/* Rule and strings storage */
static char             rule[rule_n][rule_s];   /* The rule store */
static char            *object_s, *otemp;   /* The two strings used for
                                             * building the production string */
static s16              size[rule_n];       /* Size of the rule in the rule
                                             * store */
/* Stacks [] and {} */
static s_rec           *stack, org, save;
static s16              scount = 0;
static p_rec           *pstack;
static s16              pscount = 0;

/* Current active transform matrix for drawing */
static vector           C1, C2, C3;

/* Var for rnd */
static r32              rm = (r32) 1.0 / (r32) RAND_MAX;

/* Ouput files */
static FILE            *volume_file = NULL; /* The output geometry file */

/* Object stores */
static polygon_array    poly_store;         /* The store where polygons
                                             * accumulate before saved to disc */
static vector           ver[max_p_object];  /* The store where vertices
                                             * accumulate */

void reset ()
{

  zmin = (r32) 1e30;
  thick = 100;
  rand_amount = zero;
  min_thick = zero;
  trope_amount = zero;
  max_prod = 4L * 1024L * 1024L; /* Maximum size l-system production string */
  poly_count = 0L; mesh_count = 0L, vert_count = 0L, pcount_limit = 0L;
  num = 0; col = 2; lev = 1; last_col = 0; obj = 0; muts = 0;
  dis = one; ang = (r32) 45.0; dis2 = half; tr = (r32) 0.2;
  closed_form = false; switch_yz = false; trace = false;
  nowait = false; poly_limit = false;
  memset(inputname, 0, sizeof inputname);
  memset(outputname, 0, sizeof outputname);
  memset(directory, 0, sizeof directory);
  x_max = 0; x_min = 0; y_max = 0; y_min = 0; z_max; z_min = 0;
  scount = 0;
  pscount = 0;
  free(object_s);
  free(otemp);
  free(stack);
  free(pstack);

}



/* String utils ----------------------------------------------------------- */


void
User_error(char *s,...)
{                                           /* Displays an error messages and
                                             * exits the program */
    large_string            buf;
    va_list                 args;
    char                    c;

    va_start(args, s);
    vsprintf(buf, s, args);
    va_end(args);

    fprintf(stdout, "\n\nError: %s\n\nPress a key to exit ...\n", buf);
    fflush(stdout);
    
    // if (!nowait) {
    //     do {} while (!kbhit());
    // }

    exit(EXIT_FAILURE);
}


void
Message(char *s,...)
{                                           /* Sends a message to the output
                                             * stream */
    large_string            buf;
    va_list                 args;

    va_start(args, s);
    vsprintf(buf, s, args);
    va_end(args);

    fprintf(stdout, "%s", buf);
    fflush(stdout);
    fflush(stdout);
}


void
Fget_string(FILE * f, char *s)
{                                           /* Get a string from a file. Used
                                             * for parsing the LS files */
    s16                     i = 0;
    char                    c;

    s[0] = '\0';
    for (;;) {
        c = (char) getc(f);
        if (feof(f))
            return;
        if (c == CR)
            continue;
        if (c == LF)
            break;
        s[i++] = c;
    }
    s[i] = '\0';
}


void
Seperate_dir(char *full, char *dir)
{
    s16                     i; /* Split the directory from a long file name */

    if ((!strstr(full, "/")) && (!strstr(full, "\\"))) {
        strcpy(dir, ".\\");
        return;
    }

    strcpy(dir, full);
    for (i = strlen(dir) - 1; i > 0; i--) {
        if ((dir[i] == '\\') || (dir[i] == '/')) {
            dir[i] = '\0';
            break;
        }
    }
}


static void
Remove_spaces(large_string in)
{
    large_string out = "";
    s16 i = 0, o = 0;

    for (i = 0; i < strlen(in); i++) {
        if (!isspace(in[i])) {
            out[o] = in[i];
            o++;
        }
    }
    out[o] = '\0';
    strcpy(in, out);
}


/* Feedback percentage counter -------------------------------------------- */


/* These vars are used to calculate the percentages counters for feedback */
static r32              bar_fac = zero;
static s16              old_bar = 0;
static u32              bar_max = 0;


void
Process_start(u32 max)
{                                           /* Start bar 2 with the maximum
                                             * value it's going to get */
    bar_fac = (r32) 100.0 / (r32) max;
    bar_max = max;
}


void
Process_update(u32 now)
{                                           /* Update the percentage counter
                                             * when needed */
    s16                     bar = (s16) (bar_fac * (r32) now);

    if (bar != old_bar) {
        old_bar = bar;
        Message("\r%3d%%\r", bar);
    }
}


void
Process_end(void)
{                                           /* Close bar */
    Message("\r    \r");
}


/* Setting default values ----------------------------------------------- */


static
Add_default_colors(void)
{
    s16 i;
    
    for (i = 0; i < max_colors; i++) {
        Vector_make(color_store[i], 0.5, 0.5, 0.5);
    }

    Vector_make(color_store[2], 0.8, 0.4, 0.4);
    Vector_make(color_store[3], 0.8, 0.8, 0.4);
    Vector_make(color_store[4], 0.4, 0.8, 0.4);
    Vector_make(color_store[5], 0.4, 0.8, 0.8);
    Vector_make(color_store[6], 0.4, 0.4, 0.8);
    Vector_make(color_store[7], 0.8, 0.4, 0.8);
    Vector_make(color_store[8], 0.2, 0.5, 0.2);
    Vector_make(color_store[9], 0.2, 0.5, 0.5);
    Vector_make(color_store[10], 0.2, 0.2, 0.5);
    Vector_make(color_store[11], 0.5, 0.2, 0.5);
    Vector_make(color_store[12], 0.6, 0.2, 0.2);
    Vector_make(color_store[13], 0.5, 0.5, 0.5);
    Vector_make(color_store[14], 0.75, 0.75, 0.75);
    Vector_make(color_store[15], 1.0, 1.0, 1.0);
 }


/* Output data file procs ------------------------------------------------- */


static
Open_datafile(void)
{                                           /* open and setup the different
                                             * output files depending on the
                                             * flags */

	Message("VRML file          : %s\n", outputname);
	volume_file = fopen("output.wrl", "wt");

	if (!volume_file)
	    User_error("Cannot open file [%s]", outputname);

  fprintf(volume_file, "#VRML V2.0 utf8\n");     /* VRML header */
  fprintf(volume_file, "#Generated by Lparser2 on %s\n", __DATE__); 
  fprintf(volume_file, "#for more information goto laurenslapre.nl\n"); 

	x_min = 10000.0; /* Set bounding box */
	y_min = 10000.0;
	z_min = 10000.0;
	x_max = -10000.0;
	y_max = -10000.0;
	z_max = -10000.0;
} 


static void
Close_datafile(void)
{                                           /* Close the different open output
                                             * files */
    
	fprintf(volume_file, "Viewpoint {\n"); 
	fprintf(volume_file, "\tposition %f %f %f\n", 0.0, y_min + (y_max - y_min) / 2.0, z_min + (z_max - z_min) * 2.0);
	fprintf(volume_file, "}\n");

	fclose(volume_file);
    
    Message("Meshes             : %ld\n", mesh_count);
    Message("Polygons           : %ld\n", poly_count);
    Message("Vertices           : %ld\n", vert_count);

	Message("Bounding box min   : %f %f %f\n", x_min, y_min, z_min);
	Message("Bounding box max   : %f %f %f\n", x_max, y_max, z_max);
}


static void
Save_object(s16 vertices, s16 polygons, s16 color)
{                                           /* Save an object from store to
                                             * disc */
    s32                     t, i, max;

    fprintf(volume_file, "Shape {\n");
    fprintf(volume_file, "\tappearance Appearance {\n");
    fprintf(volume_file, "\t\tmaterial Material {\n");
    fprintf(volume_file, "\t\t\tdiffuseColor %f %f %f\n", color_store[color][_x], color_store[color][_y], color_store[color][_z]);
    fprintf(volume_file, "\t\t\tshininess 0.75\n");
    fprintf(volume_file, "\t\t\tspecularColor 1.0 1.0 1.0\n");
	fprintf(volume_file, "\t\t\t}\n");	
	fprintf(volume_file, "\t\t}\n");
    
	mesh_count++;
		
/* Write vertices */
    fprintf(volume_file, "\tgeometry IndexedFaceSet {\n");
    fprintf(volume_file, "\t\tcoord Coordinate {\n");
    fprintf(volume_file, "\t\t\tpoint [\n");
    for (t = 1; t < vertices; t++)
        fprintf(volume_file, "\t\t\t\t%f %f %f,\n", ver[t][_x], ver[t][(switch_yz ? _z : _y)], ver[t][(switch_yz ? _y : _z)]);
    for (t = vertices; t <= vertices; t++)
        fprintf(volume_file, "\t\t\t\t%f %f %f\n", ver[t][_x], ver[t][(switch_yz ? _z : _y)], ver[t][(switch_yz ? _y : _z)]);
    fprintf(volume_file, "\t\t\t]\n");
    fprintf(volume_file, "\t\t}\n");

/* Check bounding box */
	for (t = 1; t < vertices; t++) {
		x_min = (ver[t][_x] > x_min ? x_min : ver[t][_x]);
		y_min = (ver[t][(switch_yz ? _z : _y)] > y_min ? y_min : ver[t][(switch_yz ? _z : _y)]);
		z_min = (ver[t][(switch_yz ? _y : _z)] > z_min ? z_min : ver[t][(switch_yz ? _y : _z)]);
		x_max = (ver[t][_x] < x_max ? x_max : ver[t][_x]);
		y_max = (ver[t][(switch_yz ? _z : _y)] < y_max ? y_max : ver[t][(switch_yz ? _z : _y)]);
		z_max = (ver[t][(switch_yz ? _y : _z)] < z_max ? z_max : ver[t][(switch_yz ? _y : _z)]);
	}
    
    vert_count += vertices;
    
/* Write polygons */
    fprintf(volume_file, "\t\tcoordIndex [\n");
    for (t = 1; t < polygons; t++) {
        fprintf(volume_file, "\t\t\t\t");
        if (poly_store[t][2] == poly_store[t][3])
            max = 2;
        else
            max = 3;
        for (i = 0; i <= max; i++)
            fprintf(volume_file, "%d, ", poly_store[t][i] - 1);
        fprintf(volume_file, "-1,\n");
    }
    for (t = polygons; t <= polygons; t++) {
        fprintf(volume_file, "\t\t\t\t");
        if (poly_store[t][2] == poly_store[t][3])
            max = 2;
        else
            max = 3;
        for (i = 0; i <= max; i++)
            fprintf(volume_file, "%d, ", poly_store[t][i] - 1);
        fprintf(volume_file, "-1\n");
    }

    poly_count += polygons;

    fprintf(volume_file, "\t\t\t]\n");
	
	fprintf(volume_file, "\t\t\tsolid FALSE\n");
			
	if (closed_form)
    	fprintf(volume_file, "\t\t\tcreaseAngle 1\n");  /* Switch on smooth shading */
	else 	
	    fprintf(volume_file, "\t\t\tcreaseAngle 0\n");
    
	fprintf(volume_file, "\t}\n");
    fprintf(volume_file, "}\n");
}


/* Add object ------------------------------------------------------------- */


static void
Define_block(vector p1, vector p2, vector up, s16 c)
{                                           /* Insert basic block. Here we
                                             * build a cube shape directly on
                                             * the input vectors. */
    vector                  dis, d1, d2, d3;
    s16                     i;
    r32                     s, d;

    zmin = MIN(zmin, p1[_z]);
    zmin = MIN(zmin, p2[_z]);

 /* Setup */
    Vector_min(p2, p1, dis);
    d = Vector_length(dis);
    if (d == zero)
        return;
    s = d * thick;
    s = (s < (r32) min_thick) ? min_thick : s;
    s *= (r32) 0.7071;

 /* D1 */
    Vector_copy_r32(dis, d1);
    Vector_normalize(d1);

 /* D2 */
    Vector_copy_r32(up, d2);
    Vector_normalize(d2);

 /* D3 */
    Vector_product(d1, d2, d3);
    Vector_normalize(d3);

 /* Base 1, 3 */
    Vector_plus(d2, d3, d1);
    Vector_normalize(d1);
    Vector_plus_fac(p1, d1, s, ver[1]);
    Vector_plus_fac(p1, d1, -s, ver[3]);

 /* Base 2, 4 */
    Vector_min(d2, d3, d1);
    Vector_normalize(d1);
    Vector_plus_fac(p1, d1, s, ver[2]);
    Vector_plus_fac(p1, d1, -s, ver[4]);

 /* End */
    for (i = 1; i <= 4; i++)
        Vector_plus(ver[i], dis, ver[i + 4]);

 /* Polygons */
    poly_store[1][0] = 1;
    poly_store[1][1] = 5;
    poly_store[1][2] = 6;
    poly_store[1][3] = 2;

    poly_store[2][0] = 2;
    poly_store[2][1] = 6;
    poly_store[2][2] = 7;
    poly_store[2][3] = 3;

    poly_store[3][0] = 3;
    poly_store[3][1] = 7;
    poly_store[3][2] = 8;
    poly_store[3][3] = 4;

    poly_store[4][0] = 4;
    poly_store[4][1] = 8;
    poly_store[4][2] = 5;
    poly_store[4][3] = 1;

    poly_store[5][0] = 1;
    poly_store[5][1] = 2;
    poly_store[5][2] = 3;
    poly_store[5][3] = 4;

    poly_store[6][0] = 8;
    poly_store[6][1] = 7;
    poly_store[6][2] = 6;
    poly_store[6][3] = 5;

    Save_object(8, 6, c);
}


static void
Define_closed(vector p1, vector p2, vector up, s16 c)
{                                           /* Insert connected cylinder shape.
                                             * The lastxxx vars are used to
                                             * store the previous top of the
                                             * cylinder for conntecing a next
                                             * one. Since the vars are stacked
                                             * for [] we can connect correctly
                                             * according to current nesting
                                             * level. */
    vector                  dis, d1, d2, d3, t1, t2;
    s16                     i, ii;
    r32                     s, d, dd = (r32) 1e30;

    zmin = MIN(zmin, p1[_z]);
    zmin = MIN(zmin, p2[_z]);

 /* Setup */
    Vector_min(p2, p1, dis);
    d = Vector_length(dis);
    if (d == zero)
        return;
    s = d * thick;
    s = (s < (r32) min_thick) ? min_thick : s;
    s *= half;

 /* D1 */
    Vector_copy_r32(dis, d1);
    Vector_normalize(d1);

 /* D2 */
    Vector_copy_r32(up, d2);
    Vector_normalize(d2);

 /* D3 */
    Vector_product(d1, d2, d3);
    Vector_normalize(d3);

    Vector_plus(d2, d3, t1);
    Vector_normalize(t1);
    Vector_min(d2, d3, t2);
    Vector_normalize(t2);

    Vector_plus_fac(p1, t1, s, ver[1]);
    Vector_plus_fac(p1, t1, -s, ver[5]);
    Vector_plus_fac(p1, t2, s, ver[3]);
    Vector_plus_fac(p1, t2, -s, ver[7]);
    
    s *= (r32) 0.7071;
    Vector_plus_fac2(p1, t1, s, t2, s, ver[2]);
    Vector_plus_fac2(p1, t1, -s, t2, s, ver[4]);
    Vector_plus_fac2(p1, t1, -s, t2, -s, ver[6]);
    Vector_plus_fac2(p1, t1, s, t2, -s, ver[8]);

 /* End */
    for (i = 1; i <= 8; i++)
        Vector_plus(ver[i], dis, ver[i + 8]);

    if (last_col == c) {
        Vector_min(p1, last, dis);
        d = Vector_length(dis);

        if (d < (r32) 1.0) {
            for (i = 1; i <= 8; i++) {
                Vector_min(ver[1], last_v[i], dis);
                d = Vector_length(dis);
                if (d < dd) {
                    dd = d;
                    ii = i;
                }
            }
            for (i = 1; i <= 8; i++) {
                Vector_copy_r32(last_v[ii], ver[i]);
                ii = (ii + 1) % 9;
                if (ii == 0)
                    ii = 1;
            }
        }
    }

 /* Polygons */
    poly_store[1][0] = 1;
    poly_store[1][1] = 9;
    poly_store[1][2] = 10;
    poly_store[1][3] = 2;

    poly_store[2][0] = 2;
    poly_store[2][1] = 10;
    poly_store[2][2] = 11;
    poly_store[2][3] = 3;

    poly_store[3][0] = 3;
    poly_store[3][1] = 11;
    poly_store[3][2] = 12;
    poly_store[3][3] = 4;

    poly_store[4][0] = 4;
    poly_store[4][1] = 12;
    poly_store[4][2] = 13;
    poly_store[4][3] = 5;

    poly_store[5][0] = 5;
    poly_store[5][1] = 13;
    poly_store[5][2] = 14;
    poly_store[5][3] = 6;

    poly_store[6][0] = 6;
    poly_store[6][1] = 14;
    poly_store[6][2] = 15;
    poly_store[6][3] = 7;

    poly_store[7][0] = 7;
    poly_store[7][1] = 15;
    poly_store[7][2] = 16;
    poly_store[7][3] = 8;

    poly_store[8][0] = 8;
    poly_store[8][1] = 16;
    poly_store[8][2] = 9;
    poly_store[8][3] = 1;

    Save_object(16, 8, c);

    last_col = c;
    Vector_copy_r32(p2, last);
    for (i = 1; i <= 8; i++)
        Vector_copy_r32(ver[i + 8], last_v[i]);
}


/* L-system routines ------------------------------------------------------ */


static r32
Rnd(void)
{                                           /* Get a random number */
    return (r32) rand() * rm;
}



#define Util_t(In,C1,C2,C3,Out) {\
  Out[_x] = Scalar_product(C1,In);\
  Out[_y] = Scalar_product(C2,In);\
  Out[_z] = Scalar_product(C3,In);\
}


static void
Set_rot(r32 a, vector n)
{                                           /* Set up a rotation matrix */
    r32                     n11, n22, n33, nxy, nxz, nyz, sina, cosa;

    cosa = (r32) cos(a);
    sina = (r32) sin(a);

    n11 = n[_x] * n[_x];
    n22 = n[_y] * n[_y];
    n33 = n[_z] * n[_z];

    nxy = n[_x] * n[_y];
    nxz = n[_x] * n[_z];
    nyz = n[_y] * n[_z];

    C1[_x] = n11 + (one - n11) * cosa;
    C1[_y] = nxy * (one - cosa) - n[_z] * sina;
    C1[_z] = nxz * (one - cosa) + n[_y] * sina;

    C2[_x] = nxy * (one - cosa) + n[_z] * sina;
    C2[_y] = n22 + (one - n22) * cosa;
    C2[_z] = nyz * (one - cosa) - n[_x] * sina;

    C3[_x] = nxz * (one - cosa) - n[_y] * sina;
    C3[_y] = nyz * (one - cosa) + n[_x] * sina;
    C3[_z] = n33 + (one - n33) * cosa;
}


static r32
Get_value(u32 *j)
{                                           /* Read a (xx) value from a
                                             * production string at location j
                                             * and make it into a real */
    s16                     i = 0;
    r32                     r = zero;
    char                    val[40] = "";

    (*j)++;
    (*j)++;

    for (;;) {
        if (object_s[*j] == ')')
            break;
        val[i] = object_s[*j];
        i++;
        (*j)++;
    }

    val[i] = '\0';
    sscanf(val, "%f", &r);

    return r;
}


static
L_init(void)
{                                           /* Process a ls file and setup */
    FILE                   *f;
    char                    temp[rule_s];
    large_string            keyword, command, r_1, r_2;
    s16                     i, tt, r, g, b;
    r32                     closed;
    
 /* Init mem */
    object_s = (char *) malloc(max_prod);
    otemp = (char *) malloc(max_prod);
    stack = (s_rec *) malloc(sizeof(s_rec) * max_stack);
    pstack = (p_rec *) malloc(sizeof(p_rec) * max_stack);
    if ((object_s == NULL) || (otemp == NULL) || (stack == NULL) || (pstack == NULL))
        User_error("Not enough memory to startup");

 /* File name */
  Message("File %s\n", inputname);
	f = fopen(inputname, "rt");
    if (!f)
        User_error("Cannot find file [%s]", inputname);

    num = 0;
    Add_default_colors();

  /* Read input file */
    do {
        do { 
            Fget_string(f, temp); 
            if (trace)
                Message("R<%s>\n", temp);
        } while ( (!feof(f)) && ((temp[0] == '#') || (temp[0] == '\0'))); /* Skip over comments */

        if (feof(f)) 
            goto skip;
        
        if (strlen(temp) < 5) /* Leading white spaces, not a real line */
            goto skip;

        strcpy(keyword, strtok(temp, " \r\n#"));
        strcpy(command, strtok(NULL, "\r\n#"));
        
        if (trace)
            Message("K[%s] C[%s]\n", keyword, command);

        if (strcmp(keyword, "recursion") == 0) { 	/* Use keyword to determine action */
            sscanf(command, "%hd", &lev);
    
        } else if (strcmp(keyword, "mutation") == 0){
            sscanf(command, "%hd", &muts);

        } else if (strcmp(keyword, "angle") == 0){
            sscanf(command, "%f", &ang);

        } else if (strcmp(keyword, "thickness") == 0){
            sscanf(command, "%f", &thick);

        } else if (strcmp(keyword, "min_thickness") == 0){
            sscanf(command, "%f", &min_thick);

        } else if (strcmp(keyword, "switch_yz") == 0){
            sscanf(command, "%hd", &tt);
            switch_yz = (tt == 1);

        } else if (strcmp(keyword, "trace") == 0){
            sscanf(command, "%hd", &tt);
            trace = (tt == 1);

        } else if (strcmp(keyword, "poly_limit") == 0){
            sscanf(command, "%d", &pcount_limit);
            poly_limit = true;

        } else if (strcmp(keyword, "no_wait") == 0){
            sscanf(command, "%hd", &tt);
            nowait = (tt == 1);

        } else if (strcmp(keyword, "shape") == 0){
            sscanf(command, "%f", &closed);

        } else if (strcmp(keyword, "color") == 0){
            sscanf(command, "%hd %hd %hd %hd", &tt, &r, &g, &b);
            Vector_make(color_store[tt], (r32) r / (r32) 255.0, (r32) g / (r32) 255.0, (r32) b / (r32) 255.0);

        } else if (strcmp(keyword, "axiom") == 0){
            Remove_spaces(command);
            strcpy(object_s, strtok(command, "\r\n\t#"));

        } else if (strcmp(keyword, "rule") == 0){
            Remove_spaces(command);
            strcpy(r_1, strtok(command, "="));
            strcpy(r_2, strtok(NULL, "\r\n\t#"));
            rule[num][0] = r_1[0];            
            rule[num][1] = '=';
            rule[num][2] = '\0';
            strcat(rule[num], r_2); 
            num++;

        } else {
            User_error("Unknown keyword [%s]", keyword);
        }
skip:;
                
    } while (!(feof(f)));

    fclose(f);

  /* Echo input */
    Message("L-system file      : %s\n", inputname);
    Message("Recursion depth    : %d\n", lev);
    Message("Mutations          : %d\n", muts);
    Message("Basic angle        : %g degrees\n", ang);
    Message("Starting thickness : %g%% of length\n", thick);
    Message("Minimum thickness  : %g%% of length\n", min_thick);
    if (switch_yz) 
        Message("Switching y and z\n");
    if (poly_limit) 
        Message("Limiting lsystem to: %ld polygons\n", pcount_limit);
            
    Message("Shape style        : ");
    if (closed == one) {
        Message("Attached cylinders\n");
        closed_form = true;
    } else {
        Message("Blocks\n");
        closed_form = false;
    }

    Message("Axiom              : %s\n", object_s);

 /* Add default rules */
    strcpy(rule[num++], "+=+");
    strcpy(rule[num++], "-=-");
    strcpy(rule[num++], "&=&");
    strcpy(rule[num++], "^=^");
    strcpy(rule[num++], "<=<");
    strcpy(rule[num++], ">=>");

    strcpy(rule[num++], "%=%");
    strcpy(rule[num++], "|=|");
    strcpy(rule[num++], "!=!");
    strcpy(rule[num++], "?=?");
    strcpy(rule[num++], ":=:");
    strcpy(rule[num++], ";=;");
    strcpy(rule[num++], "\'=\'");
    strcpy(rule[num++], "\"=\"");
    strcpy(rule[num++], "c=c");

    strcpy(rule[num++], "[=[");
    strcpy(rule[num++], "]=]");
    strcpy(rule[num++], "{={");
    strcpy(rule[num++], "}=}");

    strcpy(rule[num++], "F=F");
    strcpy(rule[num++], "f=f");
    strcpy(rule[num++], "t=t");
    strcpy(rule[num++], "g=g");
    strcpy(rule[num++], "Z=Z");
    strcpy(rule[num++], "z=z");
    strcpy(rule[num++], "*=*");
    strcpy(rule[num++], "$=$");
    strcpy(rule[num++], "~=~");

    strcpy(rule[num++], ".=.");
    strcpy(rule[num++], "1=1");
    strcpy(rule[num++], "2=2");
    strcpy(rule[num++], "3=3");
    strcpy(rule[num++], "4=4");
    strcpy(rule[num++], "5=5");
    strcpy(rule[num++], "6=6");
    strcpy(rule[num++], "7=7");
    strcpy(rule[num++], "8=8");
    strcpy(rule[num++], "9=9");
    strcpy(rule[num++], "0=0");
    strcpy(rule[num++], "(=(");
    strcpy(rule[num++], ")=)");

    strcpy(rule[num++], "_=_");            /* Closer default */

    for (i = 0; i < num; i++)
        size[i] = strlen(rule[i]) - 2;	  /* Set length of rules */
   
  /* Echo rules */  
    for (i = 0; i < num; i++) {
        if (rule[i][0] == '+')
            break;
        Message("Rule %2d            : %s\n", i + 1, rule[i]);
    }

  /* Set values ready */
    ang = (ang / (r32) 180.0) * pi;
    thick /= 100.0;
    min_thick /= 100.0;
 }


static void
L_mutate(void)
{                                           /* Apply mutations to the rules */
    s16                     i, j, k, rules, ii, max = 1000;
    char                    T, R, S[10] = "";
    char                    rulet[100] = "";

    for (i = 0; i < num; i++) {
        if (rule[i][0] == '+')
            break;
    }
    rules = i;

    switch ((s16) (Rnd() * 6.0)) {

      default:
        return;

      case 1:                              /* Insert */
        i = (s16) (Rnd() * (r32) rules);
        T = rule[i][0];
        j = (s16) (Rnd() * (r32) rules);
        k = (s16) (Rnd() * (r32) strlen(rule[j]));
        k = (k < 2) ? 2 : k;
        strcpy(rulet, &(rule[j][k]));
        rule[j][k] = '[';
        rule[j][k + 1] = T;
        rule[j][k + 2] = ']';
        rule[j][k + 3] = '\0';
        strcat(rule[j], rulet);
        size[j] = strlen(rule[j]) - 2;
        break;

      case 0:
      case 2:                              /* Replace */
        do {
            i = (s16) (Rnd() * (r32) rules);
            j = (s16) (Rnd() * (r32) rules);
            T = rule[i][0];
            R = rule[j][0];
        } while (T == R);
        for (ii = 0; ii < max; ii++) {
            i = (s16) (Rnd() * (r32) rules);
            for (j = 2; j < strlen(rule[i]); j++) {
                if (rule[i][j] == T) {
                    rule[i][j] = R;
                    return;
                }
            }
        }
        break;

      case 3:                              /* Append */
        i = (s16) (Rnd() * (r32) rules);
        S[0] = rule[i][0];
        i = (s16) (Rnd() * (r32) rules);
        strcat(rule[i], S);
        size[i] = strlen(rule[i]) - 2;
        break;

      case 4:                              /* Swap directions */
        for (ii = 0; ii < max; ii++) {
            i = (s16) (Rnd() * (r32) rules);
            for (j = 2; j < strlen(rule[i]); j++) {
                switch ((s16) (Rnd() * 12.0)) {
                  default:
                    return;
                  case 0:
                    if (rule[i][j] == '+') {
                        rule[i][j] = '-';
                        return;
                    }
                    break;
                  case 1:
                    if (rule[i][j] == '-') {
                        rule[i][j] = '+';
                        return;
                    }
                    break;
                  case 2:
                    if (rule[i][j] == '&') {
                        rule[i][j] = '^';
                        return;
                    }
                    break;
                  case 3:
                    if (rule[i][j] == '^') {
                        rule[i][j] = '&';
                        return;
                    }
                    break;
                  case 4:
                    if (rule[i][j] == '>') {
                        rule[i][j] = '<';
                        return;
                    }
                    break;
                  case 5:
                    if (rule[i][j] == '<') {
                        rule[i][j] = '>';
                        return;
                    }
                    break;
                  case 6:
                    if (rule[i][j] == '|') {
                        rule[i][j] = '%';
                        return;
                    }
                    break;
                  case 7:
                    if (rule[i][j] == '%') {
                        rule[i][j] = '|';
                        return;
                    }
                    break;
                  case 8:
                    if (rule[i][j] == ':') {
                        rule[i][j] = ';';
                        return;
                    }
                    break;
                  case 9:
                    if (rule[i][j] == ';') {
                        rule[i][j] = ':';
                        return;
                    }
                    break;
                  case 10:
                    if (rule[i][j] == '\'') {
                        rule[i][j] = '\"';
                        return;
                    }
                    break;
                  case 11:
                    if (rule[i][j] == '\"') {
                        rule[i][j] = '\'';
                        return;
                    }
                    break;
                }
            }
        }
        break;

      case 5:                              /* Swap sizes */
        for (ii = 0; ii < max; ii++) {
            i = (s16) (Rnd() * (r32) rules);
            for (j = 2; j < strlen(rule[i]); j++) {
                switch ((s16) (Rnd() * 6.0)) {
                  default:
                    return;
                  case 0:
                    if (rule[i][j] == 'F') {
                        rule[i][j] = 'Z';
                        return;
                    }
                    break;
                  case 1:
                    if (rule[i][j] == 'Z') {
                        rule[i][j] = 'F';
                        return;
                    }
                    break;
                  case 2:
                    if (rule[i][j] == 'f') {
                        rule[i][j] = 'z';
                        return;
                    }
                    break;
                  case 3:
                    if (rule[i][j] == 'z') {
                        rule[i][j] = 'f';
                        return;
                    }
                    break;
                  case 4:
                    if (rule[i][j] == '!') {
                        rule[i][j] = '?';
                        return;
                    }
                    break;
                  case 5:
                    if (rule[i][j] == '?') {
                        rule[i][j] = '!';
                        return;
                    }
                    break;
                }
            }
        }
        break;

    }
}


static
L_system(void)
{                                           /* Expand l-system into production
                                             * string. Object_s is read with
                                             * the k counter and the next
                                             * generation is build up in otemp
                                             * with the ot counter. */

    u32                     k, st, s, ss, max = max_prod - 50L;
    char                   *ot;
    s16                     S[256], i, l;
    bool                    incomplete = false;

 /*
  * Clear fast access array. This array is to quickly find the rule asociated
  * with a char.
  */
    for (i = 0; i < 256; i++)
        S[i] = num - 1;                    /* Num -1 contains the default rule
                                            * which does nothing */
 /* Each char gets a rule number */
    for (i = num - 1; i >= 0; i--)
        S[(s16) rule[i][0]] = i;

    for (l = 0; l < lev; l++) {            /* For each recursion */
        st = strlen(object_s);
        ot = otemp;
        ss = 0;

        for (k = (u32) 0; k < st; k++) {   /* For each char in the string */
            i = S[object_s[k]];            /* i = rule number attached to
                                            * current char */
            s = size[i];                   /* s = size of current rule */
            ss += s;

            if (ss >= max) {               /* Overflow */
                l = lev;
                incomplete = true;
                break;
            } else {
                Mem_copy(&(rule[i][2]), ot, s);     /* Copy */
                ot += s;
            }
        };

        *ot = '\0';
        strcpy(object_s, otemp);           /* Copy the temp string to object_s
                                            * and repeat cycle */
    };

    Message("Size of string     : %ld chars %s\n", strlen(object_s), incomplete ? "(incomplete)" : "");
}


static
L_draw(void)
{                                           /* Process a production string and
                                             * generate form */


    vector                  pos, end, v, fow, upp, lef;
    u32                     i, max = strlen(object_s);
    r32                     r, a;
    s16                     pcount, vcount, j;
    char                    next;
    bool                    poly_on = false;

 /* Setup vectors */
    Vector_make(pos, zero, zero, zero);
    Vector_make(fow, zero, zero, one);
    Vector_make(lef, zero, one, zero);
    Vector_make(upp, one, zero, zero);
    Vector_normalize(trope);

    Open_datafile();

 /* Start values */
    org.col = col;
    org.obj = obj;
    org.dis = dis;
    org.dis2 = dis2;
    org.ang = ang;
    org.thick = thick;
    org.tr = tr;

 /* Feedback */
    Process_start(strlen(object_s));

    for (i = 0; object_s[i] != '\0'; i++) {
        // Process_update(i);

        next = object_s[i + 1];            /* The next char in de string */

        if (poly_limit && (poly_count > pcount_limit)) 
            break;

        switch (object_s[i]) {             /* The current char in the string */

          default:
              break;

          case '+':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(-ang, upp);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Vector_normalize(fow);
            Vector_normalize(lef);
            ang = save.ang;
            break;

          case '-':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(ang, upp);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Vector_normalize(fow);
            Vector_normalize(lef);
            ang = save.ang;
            break;

          case '~':
            if (next == '(')
                r = ((r32) 0.017453) * Get_value(&i);
            else
                r = (r32) 6.0;
            a = (Rnd() * r * (r32) 2.0) - r;
            Set_rot(a, upp);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Vector_normalize(fow);
            Vector_normalize(lef);
            a = (Rnd() * r * (r32) 2.0) - r;
            Set_rot(a, lef);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(fow);
            Vector_normalize(upp);
            a = (Rnd() * r * (r32) 2.0) - r;
            Set_rot(a, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(lef);
            Vector_normalize(upp);
            break;

          case 't':
            if ((fow[_x] == zero) && (fow[_y] == zero))
                break;
            save.tr = tr;
            if (next == '(') 
                tr = Get_value(&i);
            
            Vector_copy_r32(fow, trope);
            Vector_make(trope, -trope[_x], -trope[_y], zero);
            Vector_normalize(trope);
            r = tr * Scalar_product(fow, trope);
            Set_rot(-r, lef);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(fow);
            Vector_normalize(upp);
            tr = save.tr;
            break;

          case '$':
            Vector_min(fow, sky, v);
            if (Vector_length(v) == zero)
                break;
            Vector_product(fow, sky, lef);
            Vector_product(fow, lef, upp);
            if (upp[_z] < zero) {
                Vector_neg(upp);
                Vector_neg(lef);
            }
            break;

          case '&':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(ang, lef);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(fow);
            Vector_normalize(upp);
            ang = save.ang;
            break;

          case '^':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(-ang, lef);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(fow);
            Vector_normalize(upp);
            ang = save.ang;
            break;

          case '<':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(-ang, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(lef);
            Vector_normalize(upp);
            ang = save.ang;
            break;

          case '>':
            save.ang = ang;
            if (next == '(') 
                ang = ((r32) 0.017453) * Get_value(&i);
            Set_rot(ang, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(lef);
            Vector_normalize(upp);
            ang = save.ang;
            break;

          case '%':
            Set_rot(pi, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Util_t(upp, C1, C2, C3, v);
            Vector_copy_r32(v, upp);
            Vector_normalize(lef);
            Vector_normalize(upp);
            break;

          case '|':
            Set_rot(pi, upp);
            Util_t(fow, C1, C2, C3, v);
            Vector_copy_r32(v, fow);
            Util_t(lef, C1, C2, C3, v);
            Vector_copy_r32(v, lef);
            Vector_normalize(fow);
            Vector_normalize(lef);
            break;

          case '!':
            if (next == '(') 
                thick *= Get_value(&i);
            else
                thick *= (r32) 0.7;
            break;

          case '?':
            if (next == '(')
                thick *= Get_value(&i);
            else
                thick /= (r32) 0.7;
            break;

          case ':':
            if (next == '(')
                ang *= Get_value(&i);
            else
                ang *= (r32) 0.9;
            break;

          case ';':
            if (next == '(') 
                ang *= Get_value(&i);
            else 
                ang /= (r32) 0.9;
            break;

          case '\'':
            if (next == '(') {
                r = Get_value(&i);
                dis *= r;
                dis2 *= r;
            } else {
                dis *= (r32) 0.9;
                dis2 *= (r32) 0.9;
            }
            break;

          case '\"':
            if (next == '(') {
                r = Get_value(&i);
                dis *= r;
                dis2 *= r;
            } else {
                dis /= (r32) 0.9;
                dis2 /= (r32) 0.9;
            }
            break;

          case 'Z':
            save.dis2 = dis2;
            if (next == '(') 
                dis2 = Get_value(&i);
            Vector_plus_fac(pos, fow, dis2, end);
            if (closed_form)
                Define_closed(pos, end, upp, col);
            else
                Define_block(pos, end, upp, col);
            Vector_copy_r32(end, pos);
            dis2 = save.dis2;
            break;

          case 'F':
            save.dis = dis;
            if (next == '(') 
                dis = Get_value(&i);
            Vector_plus_fac(pos, fow, dis, end);
            if (closed_form)
                Define_closed(pos, end, upp, col);
            else
                Define_block(pos, end, upp, col);
            Vector_copy_r32(end, pos);
            dis = save.dis;
            break;

          case '[':
            if (scount > max_stack)
                User_error("Ran out of stack");
            Vector_copy_r32(pos, stack[scount].pos);
            Vector_copy_r32(fow, stack[scount].fow);
            Vector_copy_r32(lef, stack[scount].lef);
            Vector_copy_r32(upp, stack[scount].upp);
            stack[scount].col = col;
            stack[scount].dis = dis;
            stack[scount].dis2 = dis2;
            stack[scount].ang = ang;
            stack[scount].thick = thick;
            stack[scount].tr = tr;
            if (closed_form) {
                Vector_copy_r32(last, stack[scount].last);
                stack[scount].last_col = last_col;
                for (j = 1; j <= 8; j++)
                    Vector_copy_r32(last_v[j], stack[scount].last_v[j]);
            }
            scount++;
            break;

          case ']':
            scount--;
            Vector_copy_r32(stack[scount].pos, pos);
            Vector_copy_r32(stack[scount].fow, fow);
            Vector_copy_r32(stack[scount].lef, lef);
            Vector_copy_r32(stack[scount].upp, upp);
            col = stack[scount].col;
            dis = stack[scount].dis;
            dis2 = stack[scount].dis2;
            ang = stack[scount].ang;
            thick = stack[scount].thick;
            tr = stack[scount].tr;
            if (closed_form) {
                Vector_copy_r32(stack[scount].last, last);
                last_col = stack[scount].last_col;
                for (j = 1; j <= 8; j++)
                    Vector_copy_r32(stack[scount].last_v[j], last_v[j]);
            }
            break;

          case '{':
            if (poly_on) {
                pstack[pscount].count = vcount;
                pstack[pscount].ver = (vector *) malloc(vcount * 12L);
                if (pstack[pscount].ver == NULL)
                    User_error("Ran out of memory");
                Vector_copy_max_r32(vcount, ver, pstack[pscount].ver);
                pscount++;
                if (pscount > max_stack)
                    User_error("Ran out of stack");
            }
            poly_on = true;
            vcount = (s16) 1;
            pcount = (s16) 1;
            break;

          case 'f':
            save.dis = dis;
            if (next == '(') 
                dis = Get_value(&i);
            Vector_plus_fac(pos, fow, dis, pos);
            if (poly_on)
                Vector_copy_r32(pos, ver[vcount++]);
            dis = save.dis;
            break;

          case '.':
            if (poly_on)
                Vector_copy_r32(pos, ver[vcount++]);
            break;

          case 'g':
            save.dis = dis;
            if (next == '(') 
                dis = Get_value(&i);
            Vector_plus_fac(pos, fow, dis, pos);
            dis = save.dis;
            break;

          case 'z':
            save.dis2 = dis2;
            if (next == '(') 
                dis2 = Get_value(&i);
            Vector_plus_fac(pos, fow, dis2, pos);
            if (poly_on)
                Vector_copy_r32(pos, ver[vcount++]);
            dis2 = save.dis2;
            break;

          case '}':
            if (vcount > (s16) 3) {
                for (j = 1; j < vcount - 2; j++) {
                    poly_store[pcount][0] = 1;
                    poly_store[pcount][1] = j + 1;
                    poly_store[pcount][2] = j + 2;
                    poly_store[pcount][3] = j + 2;
                    pcount++;
                }
                Save_object(vcount - 1, pcount - 1, col);
            }
            poly_on = false;
            if (pscount > 0) {
                pscount--;
                Vector_copy_max_r32(pstack[pscount].count, pstack[pscount].ver, ver);
                vcount = pstack[pscount].count;
                poly_on = true;
            }
            break;

          case 'c':
            if (next == '(') 
                col = (s16) Get_value(&i);
             else 
                col++;
            break;
        }
    }

    Process_end();

    Close_datafile();
}


/* Lparser main ----------------------------------------------------------- */


void
do_read_ls(const char* ls)
{


  s16 i;

  srand(time(NULL));

  FILE *fp;
  fp = fopen("input.ls", "w+");
  fputs(ls, fp);
  fclose(fp);

  Message("%s\n", min_bar);
  Message("%s v%s (%s)\n", "Lparser", "2.1", __DATE__);
  Message("%s\n", min_bar);

    // if (argc > 1) {                         /* Get filenames */
        strcpy(inputname, "input.ls");
        // Seperate_dir(argv[1], directory);
        // strcpy(outputname, directory);
        strcat(outputname, "output.wrl");
    // } else {
    //     // Seperate_dir(argv[0], directory);
    //     strcpy(inputname, directory);
    //     strcat(inputname, "input.ls");
    //     strcpy(outputname, directory);
    //     strcat(outputname, "output.wrl");
    // }

    L_init();

    if (muts > 0) {
        Message("Mutating           : ..");
        for (i = 0; i < muts; i++) {
            Message("\b\b%-2d", i + 1);
            L_mutate();
        }
        Message("\n");
    }
    
    L_system();
    L_draw();

    EM_ASM(
      var wrl = FS.readFile('output.wrl', { encoding: 'utf8' });
      window.init(wrl);
    );

    /* not sure if that is enough to clear memory */
    reset();
}


/* -------------------------------------------------------------------------
   End of file.
   -------------------------------------------------------------------------
*/

#ifdef __cplusplus
}
#endif
