/**********************************************************/
/*                                                        */
/* Area/shape preserving extensive and anti-extensive     */
/* operators using Max-Trees                              */
/*                                                        */
/* by Georgios K Ouzounis - Jan. 2004                     */
/* original Max-Tree code by Erik Urbach June 2000.       */
/*                                                        */
/**********************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#include <FreeImage.h>


typedef short bool;
#define false 0
#define true  1

typedef unsigned char ubyte;

typedef unsigned short greyval;



#define NUMLEVELS     65536
#define CONNECTIVITY  4

double kbase, background = 0, alpha=0;

typedef struct FIFOQueue
{
   long *Pixels;
   long Head;
   long Tail; /* First free place in queue, or -1 when the queue is full */
} FIFOQueue;



/*** Max-Tree definition ***/

typedef struct MaxNode MaxNode;
typedef struct MaxNode *MaxTree;

/* MaxNode Status */
#define MTS_Ok       0
#define MTS_Deleted  1

struct MaxNode
{
  long Parent;
  long Status;
  long Area;
  int PeakLevel;
  int kPrime;
  int Level;
  int NewLevel;
};

MaxTree Tree;



/*** Globals used by Max-Tree ***/

long numbernodes[NUMLEVELS]; /* Number of nodes C^k_h at level h */
greyval **P_ORI;  /* Denotes the original gray level value of pixel p in processed image*/
greyval **ORI;    /* Denotes the original ... in the unproceesed input image */

/* STATUS stores the information of the pixel status: the pixel can be
 * NotAnalyzed, InTheQueue or assigned to node k at level h. In this
 * last case STATUS(p)=k. */
#define ST_NotAnalyzed  -1
#define ST_InTheQueue   -2
long *STATUS;

bool NodeAtLevel[NUMLEVELS];

/*** HQueue globals ***/
long  MaxPixelsPerLevel[NUMLEVELS];   /* Original image  */
long PMaxPixelsPerLevel[NUMLEVELS];   /* Processed image */
FIFOQueue HQueue[NUMLEVELS];

/*** Other globals ***/
long ImageWidth;
long ImageHeight;
long ImageSize;
long NumPlanes;

long NumPixelsBelowLevel[NUMLEVELS];
long PNumPixelsBelowLevel[NUMLEVELS];

#define ReadPixel(t,i,h)  (t[NumPixelsBelowLevel[h]+i].NewLevel)

/*** Queue functions ***/
#define HQueueFirst(h)     (HQueue[h].Pixels[HQueue[h].Head++])
#define HQueueAdd(h,p)     HQueue[h].Pixels[HQueue[h].Tail++] = p
#define HQueueNotEmpty(h)  (HQueue[h].Head != HQueue[h].Tail)
void HQueueCreate(void);
void HQueueDelete(void);

/***********************************************/
/* HQueue is an array segmented based on the   */
/* processed image histogram.                  */
/***********************************************/

void HQueueCreate(void)
{
   int i;

   HQueue->Pixels = calloc((size_t)2*ImageSize, sizeof(long));
   assert(HQueue->Pixels != NULL);
   HQueue->Head = HQueue->Tail = 0;
   for (i=1; i<NUMLEVELS; i++)
   {
      HQueue[i].Pixels = HQueue[i-1].Pixels + PMaxPixelsPerLevel[i-1] + MaxPixelsPerLevel[i-1];
      HQueue[i].Head = HQueue[i].Tail = 0;
   }

} /* HQueueCreate */


/***********************************************/
/* Image I/O functions                         */
/***********************************************/



/** Generic image loader
@param lpszPathName Pointer to the full file name
@param flag Optional load flag constant
@return Returns the loaded dib if successful, returns NULL otherwise
*/
FIBITMAP* GenericLoader(const char* lpszPathName, int flag) {
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
//  check the file signature and deduce its format
// (the second argument is currently not used by FreeImage)
  fif = FreeImage_GetFileType(lpszPathName, 0);
  if(fif == FIF_UNKNOWN) {
    // no signature ?
    // try to guess the file format from the file extension
    fif = FreeImage_GetFIFFromFilename(lpszPathName);
  }
  // check that the plugin has reading capabilities ...
  if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
    // ok, let's load the file
    FIBITMAP *dib = FreeImage_Load(fif, lpszPathName, flag);
    // unless a bad file format, we are done !
    return dib;
  }
  return NULL;
}

greyval **ReadTIFF(char *fnm, long *width, long *height, long *numplanes){
   FIBITMAP *dib = GenericLoader(fnm,0);
   unsigned long  bitsperpixel;
   greyval **im;
   unsigned int x,y,i,j,imsize;
   if (dib == NULL) return NULL;
     
   bitsperpixel =  FreeImage_GetBPP(dib);
   *numplanes=1;
   if ((bitsperpixel==24) ||(bitsperpixel==48))
     *numplanes=3;
   *height = FreeImage_GetHeight(dib), 
   *width = FreeImage_GetWidth(dib);
   
   imsize = (*width)*(*height);
   im = calloc((size_t)*numplanes, sizeof(greyval *));
   assert(im!=NULL);
   for (i=0;i<*numplanes;i++){
     im[i] = calloc((size_t)imsize, sizeof(greyval));
     assert(im[i]!=NULL);
   }
   switch(bitsperpixel) {
   case 8 :
     i=0;
     for(y = 0; y <*height; y++) {
       BYTE *bits = (BYTE *)FreeImage_GetScanLine(dib, *height - y -1);
       //printf("y=%d\n",y);
       for(x = 0; x < *width; x++,i++) {
	 im[0][i] = bits[x];
       }
     }
     
     FreeImage_Unload(dib);
     return im;
   case 16 :
     i=0;
     for(y = 0; y < *height; y++) {
       unsigned short *bits = (unsigned short *)FreeImage_GetScanLine(dib,*height - y -1);
       for(x = 0; x < *width; x++,i++) {
	 im[0][i] = bits[x];
       }
     }
     FreeImage_Unload(dib);
     return im; 
   case 24 : 
     i=0;
     for(y = 0; y <*height; y++) {
       BYTE *bits = (BYTE *)FreeImage_GetScanLine(dib, *height - y -1);
       //printf("y=%d\n",y);
       for(x = 0; x < *width; x++,i++) {
         for (j=0;j<*numplanes;j++)
	   im[j][i] = bits[3*x+j];
       }
     }
     
     FreeImage_Unload(dib);
     return im;
   case 48 : 
     i=0;
     for(y = 0; y <*height; y++) {
       unsigned short *bits = (unsigned short *)FreeImage_GetScanLine(dib, *height - y -1);
       //printf("y=%d\n",y);
       for(x = 0; x < *width; x++,i++) {
         for (j=0;j<*numplanes;j++)
	   im[j][i] = bits[3*x+j];
       }
     }
     
     FreeImage_Unload(dib);
     return im;
     
   default : 
     FreeImage_Unload(dib);
     fprintf(stderr, "unsupported format\n"); exit(-1);
     return NULL; 
   }
 
}


void WriteTIFF(char *fname, long width, long height, int bitspp, long numplanes){
  FIBITMAP *outmap;
  long i,j,y,x,plane; 
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(fname);

  if (bitspp == 8){
    ubyte *imagebuf;
    RGBQUAD *pal;
    outmap = FreeImage_AllocateT(FIT_BITMAP,width,height,numplanes*bitspp,0xFF,0xFF,0xFF);
    if (numplanes==1){
      pal = FreeImage_GetPalette(outmap);
      for (i = 0; i < 256; i++) {
	pal[i].rgbRed = i;
	pal[i].rgbGreen = i;
	pal[i].rgbBlue = i;
      }
    }
    i = 0;
    for (j=height-1; j>=0; j--){      
      imagebuf = FreeImage_GetScanLine(outmap,j);
      for (x=0;x<width;x++,i++)
        for (plane=0;plane<numplanes;plane++)
	 imagebuf[numplanes*x+plane]=ORI[plane][i];	
    }

  } else {
    greyval *imagebuf;
    if (numplanes==1)
      outmap = FreeImage_AllocateT(FIT_UINT16,width,height,16,0xFFFF,0xFFFF,0xFFFF);
    else   
      outmap = FreeImage_AllocateT(FIT_RGB16,width,height,48,0xFFFF,0xFFFF,0xFFFF);

    i = 0;
    for (j=height-1; j>=0; j--){      
      imagebuf = (greyval *)FreeImage_GetScanLine(outmap,j);
      for (x=0;x<width;x++,i++)
        for (plane=0;plane<numplanes;plane++)
	 imagebuf[numplanes*x+plane]=ORI[plane][i];	
	
    }
  }

  FreeImage_Save(fif,outmap,fname,0); 
  FreeImage_Unload(outmap);

}


void ReadPGM(char *fname,short buffer)
{
   long width, height;
   int c;

 
   if(buffer==1)
     {
       ORI = ReadTIFF( fname, &width, &height, &NumPlanes);
       ImageWidth = width;
       ImageHeight = height;
       ImageSize = width*height;

     }
   else
     {
       P_ORI = ReadTIFF(fname, &width, &height, &NumPlanes);
       ImageWidth = width;
       ImageHeight = height;
       ImageSize = width*height;
     } 
  
} /* ReadPGM */


greyval FindMax(greyval *buf, long width, long height){
  greyval max = buf[0];
  long i,size = width*height;
  for (i = 1; i<size;i++){
    max = (buf[i]>max) ? buf[i] : max;
  }
  return max;
}


void WritePGM(char *fname, long width, long height, long numplanes)
{


  if (FindMax(ORI[0],width,height)>255)
    WriteTIFF(fname, width, height, 16, numplanes);
  else
    WriteTIFF(fname, width, height, 8, numplanes);

  
} /* WritePGM */

/***********************************************/
/* Setting up the Max-Tree structure           */
/* MaxPixelsPerLevel already initialized       */
/***********************************************/

MaxTree MaxTreeCreate(long size)
{
   MaxTree t;
   int i;

   t = calloc((size_t)2*size, sizeof(MaxNode));
   if (t)
   {
	  *NumPixelsBelowLevel = 0;
      for (i=1; i<NUMLEVELS; i++)
      {
    	 NumPixelsBelowLevel[i] = NumPixelsBelowLevel[i-1] + MaxPixelsPerLevel[i-1] + PMaxPixelsPerLevel[i-1];
      }
   }
   return(t);
} /* MaxTreeCreate */

/***************************************************/
/* Finds a pixel of minimum intensity in the image */
/* MaxPixelsPerLevel : Histogram of original image */
/* PMaxPixelsPerLevel: Histogram of processed image*/
/***************************************************/

long FindHMinPixel(greyval *ORI,greyval *P_ORI)
/*  */
{
   long i, p=0;

   /* initialise level entries */
   for (i=0; i<NUMLEVELS; i++)  
     {
       MaxPixelsPerLevel[i] = 0;
       PMaxPixelsPerLevel[i] = 0;
     }
   /* compute histogram */
   for (i=0; i<ImageSize; i++)
   {
      MaxPixelsPerLevel[ORI[i]]++;      
      PMaxPixelsPerLevel[P_ORI[i]]++;
      if (P_ORI[i]<P_ORI[p])  p = i;
   }
   return(p);
} /* FindHMinPixel */

/***********************************************/
/* Retrieves the neighbors of pixel p          */
/***********************************************/

int GetNeighbors(long p, long *neighbors)
{
   long x;
   int n=0;

   x = p % ImageWidth;
   if (x<(ImageWidth-1))  neighbors[n++] = p+1;
   if (p>=ImageWidth)     neighbors[n++] = p-ImageWidth;
   if (x>0)               neighbors[n++] = p-1;
   p += ImageWidth;
   if (p<ImageSize)       neighbors[n++] = p;
   return(n);
} /* GetNeighbors */


/***********************************************/
/* Max-Tree Creation: Here it is the modified  */
/* image that is being processed               */
/***********************************************/

int flood(greyval *ORI, greyval *P_ORI, int h, 
	  long *thisarea, int *thispeaklevel)
{
   long neighbors[CONNECTIVITY];
   long p, q,idx;
   long area = *thisarea, childarea;
   int numneighbors, i, m, level, peaklevel = h, childpeaklevel;
   MaxNode *node;

   while(HQueueNotEmpty(h)){
     p = HQueueFirst(h);
     STATUS[p] = numbernodes[ORI[p]]; /* in all cases */
     
     if(ORI[p]!=h){/* unstable regions */
       level = ORI[p];
       NodeAtLevel[level]=true;
       
       /* Detect the node */
       node = Tree + (NumPixelsBelowLevel[level] + numbernodes[level]);
       node->Area++;
       peaklevel = (peaklevel<level)? level : peaklevel;
       if(level>h){ /* Anti-extensive case */
	 
	 /* set the singleton node at ORI[p]*/
	 node->Parent = NumPixelsBelowLevel[h] + numbernodes[h];
	 node->Status = MTS_Ok;
	 node->Level  = level;
	 node->PeakLevel  = level;
	 numbernodes[level] += 1;    /* increase this for next time */
	 NodeAtLevel[level] = false; /* finalise this node */
	 area++;	       	/* Need to update the parent area also	*/
       }
     }else /* Stable or background regions */	
       area++;
     /* process the neighbors */
     numneighbors = GetNeighbors(p, neighbors);
     for (i=0; i<numneighbors; i++){
       q = neighbors[i];
       if (STATUS[q]==ST_NotAnalyzed){
	 HQueueAdd(P_ORI[q],q);
	 STATUS[q] = ST_InTheQueue;
	 NodeAtLevel[P_ORI[q]] = true;
	 /* The neighbor is greater than p */
	 if (P_ORI[q] > P_ORI[p])
	   {
	     m = P_ORI[q];
	     childarea = 0;
	     childpeaklevel = m;
	     do
	       {
		 m = flood(ORI, P_ORI, m, &childarea,&childpeaklevel);
	       } while (m!=h);
	     area += childarea;
	     peaklevel = (peaklevel<childpeaklevel)? childpeaklevel : peaklevel;
	   }
       }
     }
   }/* end of while loop*/
   
   /* Retrieve the parent and finalise the node*/
   numbernodes[h] = numbernodes[h]+1;
   m = h-1;
   while ((m>=0) && (NodeAtLevel[m]==false))  m--;
   if (m>=0)
     {
       node = Tree + (NumPixelsBelowLevel[h] + numbernodes[h]-1);
       node->Parent = NumPixelsBelowLevel[m] + numbernodes[m];
     } 
   else 
     {
       idx = NumPixelsBelowLevel[h];
       node = Tree + idx;
       node->Parent = idx;
     }
   node->Area += area;
   node->Status = MTS_Ok;
   node->Level = h;
   node->PeakLevel = h;
   NodeAtLevel[h] = false;
   *thisarea = node->Area;
   *thispeaklevel = node->PeakLevel;
   return(m);
} /* flood */


/***********************************************/
/* Filtering Functions                         */
/***********************************************/

typedef long (*kFunc)(long);

long kflatConstant(long greylevel){
  return (long) kbase;
}

long kflatLinear(long greylevel){
  return (long) (kbase*(double)greylevel/100.0);
}


void MaxTreeProcessNode(MaxTree t, long lambda, long idx, kFunc kf, long maxarea)
/* Deletes node idx and possible one or more of its parents. */
{
  long k,parent, diflevel,newlevel;
   
   parent = t[idx].Parent;

   diflevel = t[idx].Level - t[parent].Level;
   newlevel = t[parent].NewLevel + diflevel;
   k = (*kf)(newlevel);
   if ((t[idx].PeakLevel - t[parent].Level> k) && (t[idx].Area>=lambda) &&(t[idx].Area<=maxarea)){
     t[idx].NewLevel = t[parent].NewLevel + diflevel;
     t[idx].kPrime = k;     
   } else {
     if (diflevel>t[parent].kPrime){
       t[idx].NewLevel = t[parent].NewLevel + t[parent].kPrime;
       t[idx].kPrime = 0;     
     } else {
       t[idx].NewLevel = t[parent].NewLevel + diflevel;
       t[idx].kPrime = t[parent].kPrime - diflevel;
     }
   }

} /* MaxTreeProcessNode */

void SetPeakLevels(MaxTree t)
{
  long i, idx,parent;
   int h;

   for ( h= NUMLEVELS-1; h>=0;h-- )
   {
      for (i=0; i<numbernodes[h]; i++)
      {
	 idx = NumPixelsBelowLevel[h] + i;
         parent = t[idx].Parent;
         if (t[parent].PeakLevel < t[idx].PeakLevel)
	   t[parent].PeakLevel = t[idx].PeakLevel;
      }
     
   }
		
} /* SetPeakLevels */


void MaxTreeFilter(greyval *ORI, MaxTree t, long lambda, kFunc k, long maxarea)
{
   long i, idx;
   int h=0;

   SetPeakLevels(t);
   while (numbernodes[h]==0)
     h++;                    /* find root*/
   /*process root*/   

   idx = NumPixelsBelowLevel[h];

   if ((t[idx].Area>=lambda) &&(t[idx].Area<=maxarea)){
     t[idx].kPrime = (*k)(t[idx].Level);

     t[idx].NewLevel = t[idx].Level;  
   } else {
     t[idx].kPrime = 0;
     t[idx].NewLevel = 0;  
   }
    
   h++;
    
   while ( h< NUMLEVELS )
   {
      for (i=0; i<numbernodes[h]; i++)
      {
	 idx = NumPixelsBelowLevel[h] + i;
         MaxTreeProcessNode(t, lambda, idx, k,maxarea);
      }
      h++;
   }
   /* Modify the output image */
   for (i=0; i<ImageSize; i++)	
     ORI[i] = ReadPixel(t, STATUS[i], ORI[i]);
		
} /* MaxTreeFilter */

/* --------------------------------------------------------------- */
/* Notes :	If the attribute threshold lambda is set to a large    */
/*			value, the extensive log reports the parent node index */
/*			as a number out of the node index range for that level.*/
/*			That is because although the node level reduces, the   */
/*			parents of each child nodes above that are different.  */
/* --------------------------------------------------------------- */
 



/***********************************************/
/* Initialisation                              */
/***********************************************/

void InitLevels(void)
{
   int i;

   for (i=0; i<NUMLEVELS; i++)
   {
      numbernodes[i] = 0;
      NodeAtLevel[i] = false;
   }
} /* InitLevels */


void CreateLog(MaxTree t,int hmin, char *fname1, char *fname2)
{
	FILE *fp;
	int l,i;
	long idx,area,level,parent,status, parent_level, parent_node;
	
	fp = fopen("MaxTreeReport.txt","a");
	fprintf(fp,"\n--- Start of Session ---\n");
	fprintf(fp,"Filenames: '%s' - '%s'\n",fname1, fname2);
	fprintf(fp,"Image Size %d\n",ImageSize);
	fprintf(fp,"h_min:%d\n",hmin);

	for (l=NUMLEVELS-1; l>=0; l--)
	{
      for (i=0; i<numbernodes[l]; i++)
      {
		  idx = NumPixelsBelowLevel[l] + i;
		  level  = t[idx].Level;
		  area   = t[idx].Area;
		  parent = t[idx].Parent;
		  /* parent level*/ 
		  parent_level = t[parent].Level;
		  parent_node =  parent - NumPixelsBelowLevel[parent_level];
		  status = t[idx].Status;
		  /*if(status==0)*/
			fprintf(fp,"level: %d, node: %d, area: %d, parent_level: %d, parent_node: %d, status: %d\n",l,i,area,parent_level,parent_node,status);
      }
	}
	fprintf(fp,"--- End of Session ---\n");
	fclose(fp);
}

int main(int argc, char *argv[])
{
  long i, p,plane;
   long area=0,k=0, maxarea;
   int peaklevel;
   long lambda;
   int hmin;
   short buffer;
  struct tms timestruct;
  clock_t start;
  long ticksize=sysconf(_SC_CLK_TCK);
  float museconds;

   if (argc<4)
   {
      printf("Usage: %s <original inputimage> <processed input image> <lambda> [k][maxarea]\n", argv[0]);
      exit(0);
   }
   else lambda = atol(argv[3]);
   FreeImage_Initialise(0);
   
   ReadPGM(argv[1],buffer=1); /* ORI image buffer */
   ReadPGM(argv[2],buffer=2); /* P_ORI  image buffer */
   /*printf("\n   Read Buffers Ready\n");*/
   if (argc>4)
     k=atoi(argv[4]);
   if (argc>5)
     maxarea = atoi(argv[5]);
   else
     maxarea = ImageSize;
   if (maxarea<0)
     maxarea = -(maxarea*ImageSize)/100;
   /* STATUS keeps track of each pixel's labelling */
   /*printf("\n   Levels initialised\n");	*/
    
   kbase = k;

   /*printf("%f\n",kbase);*/
  times(&timestruct);
  start=timestruct.tms_utime;

   for (plane=0; plane<NumPlanes; plane++){
     
     STATUS = calloc((size_t)ImageSize, sizeof(long));
     assert(STATUS!=NULL);
     for (i=0; i<ImageSize; i++)  STATUS[i] = ST_NotAnalyzed;
     InitLevels();
     p = FindHMinPixel(ORI[plane],P_ORI[plane]);
     hmin = P_ORI[plane][p];
     peaklevel = hmin;
     NodeAtLevel[hmin] = true;
     HQueueCreate();
     /*   printf("\n   Hierarchical Queues Ready\n");*/

     HQueueAdd(hmin, p); 
     STATUS[p] = ST_InTheQueue;
     Tree = MaxTreeCreate(ImageSize);
     assert(Tree!=NULL);
     printf("\n   Max-Tree structure Ready\n");
     
     flood(ORI[plane],P_ORI[plane],hmin, &area, &peaklevel);
     printf("\n   MaxTree Structure Completed!\n");

     MaxTreeFilter(ORI[plane],Tree, lambda, kflatConstant,maxarea);
     printf("\n   Area Opening Completed!\n");

     /* Generate a log file */
     CreateLog(Tree,hmin,argv[1],argv[2]);

     free(Tree);
     free(HQueue->Pixels);
     free(STATUS);
   }
  times(&timestruct);
  museconds=1E6*(timestruct.tms_utime-start)/(float) (ticksize);
  printf ("%8d %10.4f\n", lambda, museconds/1000000);
   
   WritePGM("out.tif", ImageWidth, ImageHeight, NumPlanes);
   for(plane=0; plane<NumPlanes; plane++){
     free(ORI[plane]);
     free(P_ORI[plane]);
   }
   free(ORI);
   free(P_ORI);
   FreeImage_DeInitialise();
   return(0);

} /* main */

