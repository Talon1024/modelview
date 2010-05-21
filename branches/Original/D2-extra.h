#ifndef D2EXTRAH
#define D2EXTRAH

/*
//
//  extra.h  ....
//
*/

#pragma pack(1)

#define MAX_D2_TEXTURE	20
#define ID3_HOG 0x464844L   //hog file id


#define D2_TEXTURESET_ALIEN1	0x000
#define D2_TEXTURESET_ALIEN2	0x300
#define D2_TEXTURESET_FIRE		0x600
#define D2_TEXTURESET_GROUPA	0x900
#define D2_TEXTURESET_ICE		0xc00
#define D2_TEXTURESET_WATER		0xf00

#define BM_FLAG_TRANSPARENT        1
#define BM_FLAG_SUPER_TRANSPARENT  2
#define BM_FLAG_NO_LIGHTING        4
#define BM_FLAG_RLE                8
#define BM_FLAG_PAGED_OUT          16
#define BM_FLAG_RLE_BIG            32


/* defines to support robot structures */
#define VCLIP_MAX_FRAMES      30
#define MAX_D2_GUNS           8
#define N_PLAYER_GUNS         8
#define MAX_CONTROLCEN_GUNS   8
#define NDL                   5
#define N_ANIM_STATES	      5
#define MAX_CLIP_FRAMES       50
#define MAX_D2_SUBMODELS         10
#define MAX_D2_ROBOT_TYPES			85
#define MAX_D2_ROBOT_JOINTS		1250
#define MAX_D2_POLYGON_MODELS	200
#define MAX_OBJ_BITMAPS			600
#define	MAX_D2_REACTORS		8
#define	MAX_D2_WEAPON_TYPES		100


typedef signed char    INT8;
typedef signed short   INT16;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef long           FIX;     /* 16 bits int, 16 bits fracion */
typedef short          FIXANG;  /*  8 bits int,  8 bits fraction */
typedef unsigned int   UINTW;
typedef int            INTW;

typedef struct D2_vms_vector {
  FIX x,y,z;
} D2_VMS_VECTOR;

typedef struct D2_vms_angvec {
  FIXANG p,b,h;
} D2_VMS_ANGVEC;

typedef struct D2_vms_matrix {
  D2_VMS_VECTOR rvec,uvec,fvec;
} D2_VMS_MATRIX;

typedef struct {
  UINT16 index;
} D2_BITMAP_INDEX;

typedef struct {
  UINT8       flags;           /*1=lava,2=water,4=force field,8=blue goal, 16=red goal */
  UINT8       pad[3];          /*keep alignment */
  FIX         lighting;        /*how much light this casts */
  FIX         damage;          /*how much damage being against this does (for lava) */
  INT16       eclip_num;       /*the eclip that changes this, or -1 */
  INT16       destroyed;       /*bitmap to show when destroyed, or -1 */
  INT16       slide_u,slide_v; /*slide rates of texture, stored in 8:8 FIX */
} D2_TMAP_INFO;

typedef struct {
  FIX            play_time;  /*total time (in seconds) of clip */
  INT32          num_frames;
  FIX            frame_time; /*time (in seconds) of each frame */
  INT32          flags;
  INT16          sound_num;
  D2_BITMAP_INDEX   frames[VCLIP_MAX_FRAMES];
  FIX            light_value;
} D2_VCLIP;

typedef struct {
  D2_VCLIP  vc;                      /*imbedded vclip */
  FIX    time_left;               /*for sequencing */
  INT32  frame_count;             /*for sequencing */
  INT16  changing_wall_texture;   /*Which element of Textures array to */
                                  /*replace. */
  INT16  changing_object_texture; /*Which element of ObjBitmapPtrs array */
                                  /*to replace. */
  INT32  flags;                   
  INT32  crit_clip;               /*use this clip instead of above one */
                                  /*when mine critical */
  INT32  dest_bm_num;             /*use this bitmap when monitor destroyed */
  INT32  dest_vclip;              /*what vclip to play when exploding */
  INT32  dest_eclip;              /*what eclip to play when exploding */
  FIX    dest_size;               /*3d size of explosion */
  INT32  sound_num;               /*what sound this makes */
  INT32  segnum,sidenum;          /*what seg & side, for one-shot clips */
} D2_ECLIP;

typedef struct {
  FIX    play_time;
  INT16  num_frames;
  INT16  frames[MAX_CLIP_FRAMES];
  INT16  open_sound;
  INT16  close_sound;
  INT16  flags;
  char   filename[13];
  char   pad;
} D2_WCLIP;

/*describes a list of joint positions */
typedef struct {
  INT16  n_joints;
  INT16  offset;
} D2_JOINTLIST;

typedef struct {
  INT32       model_num;               /* which polygon model? */
  D2_VMS_VECTOR  gun_points[MAX_D2_GUNS];    /* where each gun model is */
  UINT8       gun_submodels[MAX_D2_GUNS]; /* which submodel is each gun in? */
  INT16       exp1_vclip_num;
  INT16       exp1_sound_num;
  INT16       exp2_vclip_num;
  INT16       exp2_sound_num;
  INT8        weapon_type;
  INT8        weapon_type2;         /* Secondary weapon number, -1 means none, */
                                    /* otherwise gun #0 fires this weapon. */
  INT8        n_guns;               /* how many different gun positions */
  INT8        contains_id;          /* ID of powerup this robot can contain. */
  INT8        contains_count;       /* Max number of things this instance can */
                                    /* contain. */
  INT8        contains_prob;        /* Probability that this instance will */
                                    /* contain something in N/16 */
  INT8        contains_type;        /* Type of thing contained, robot or */
                                    /* powerup, in bitmaps.tbl, !0=robot, */
                                    /* 0=powerup */
  INT8        kamikaze;             /* !0 means commits suicide when hits you, */
                                    /* strength thereof. 0 means no. */
  INT16       score_value;          /* Score from this robot. */
  INT8        badass;               /* Dies with badass explosion, and strength */
                                    /* thereof, 0 means NO. */
  INT8        energy_drain;         /* Points of energy drained at each */
                                    /* collision. */
  FIX         lighting;             /* should this be here or with polygon */
                                    /* model? */
  FIX         strength;             /* Initial shields of robot */
  FIX         mass;                 /* how heavy is this thing? */
  FIX         drag;                 /* how much drag does it have? */
  FIX         field_of_view[NDL];   /* compare this value with */
                                    /* forward_vector.dot.vector_to_player, */
                                    /* ..if field_of_view <, then robot can */
                                    /* see player */
  FIX         firing_wait[NDL];     /* time in seconds between shots */
  FIX         firing_wait2[NDL];    /* time in seconds between shots */
  FIX         turn_time[NDL];       /* time in seconds to rotate 360 degrees */
                                    /* in a dimension */
  FIX         max_speed[NDL];       /* maximum speed attainable by this robot */
  FIX         circle_distance[NDL]; /* distance at which robot circles player */
  INT8        rapidfire_count[NDL]; /* number of shots fired rapidly */
  INT8        evade_speed[NDL];     /* rate at which robot can evade shots, */
                                    /* 0=none, 4=very fast */
  INT8        cloak_type;           /* 0=never, 1=always, 2=except-when-firing */
  INT8        attack_type;          /* 0=firing, 1=charge (like green guy) */
  UINT8       see_sound;            /* sound robot makes when it first sees the */
                                    /* player */
  UINT8       attack_sound;         /* sound robot makes when it attacks the */
                                    /* player */
  UINT8       claw_sound;           /* sound robot makes as it claws you */
                                    /* (attack_type should be 1) */
  UINT8       taunt_sound;          /* sound robot makes after you die */
  INT8        boss_flag;            /* 0 = not boss, 1 = boss.  Is that? */
  INT8        companion;            /* Companion robot, leads you to things. */
  INT8        smart_blobs;          /* how many smart blobs are emitted when */
                                    /* this guy dies! */
  INT8        energy_blobs;         /* how many smart blobs are emitted when */
                                    /* this guy gets hit by energy weapon! */
  INT8        thief;                /* !0 means this guy can steal when he */
                                    /* collides with you! */
  INT8        pursuit;              /* !0 means pursues player after he goes */
                                    /* around a corner. */
                                    /* ..4 = 4/2 pursue up to 4/2 seconds */
                                    /* after becoming invisible if up to 4 */
                                    /* ..segments away */
  INT8        lightcast;            /* Amount of light cast. 1 is default. */
                                    /* 10 is very large. */
  INT8        death_roll;           /* 0 = dies without death roll. !0 means */
                                    /* does death roll, larger = faster */
                                    /* ..and louder */
  UINT8       flags;                /* misc properties */
                                    /* (boss_flag, companion, thief, & pursuit */
                                    /* probably should also be bits in flags) */
  UINT8       pad[3];               /* alignment */
  UINT8       deathroll_sound;      /* if has deathroll, what sound? */
  UINT8       glow;                 /* apply this light to robot itself. */
                                    /* stored as 4:4 FIXed-point */
  UINT8       behavior;             /* Default behavior. */
  UINT8       aim;                  /* 255 = perfect, less = more likely */
                                    /* to miss.  0 != random, would look stupid. */
                                    /* ..0=45 degree spread.  Specify in */
                                    /* bitmaps.tbl in range 0.0..1.0 */

  D2_JOINTLIST   anim_states[MAX_D2_GUNS+1][N_ANIM_STATES];

  INT32       always_0xabcd;        /* debugging */
} D2_ROBOT_INFO;

typedef struct {
  INT16       jointnum;
  D2_VMS_ANGVEC  angles;
} D2_JOINTPOS;

typedef struct {
  INT8         render_type;     /* How to draw 0=laser, 1=blob, 2=object */
  INT8         persistent;      /* 0 = dies when it hits something,  */
                                /* = continues (eg, fusion cannon) */
  INT16        model_num;       /* Model num if rendertype==2. */
  INT16        model_num_inner; /* Model num of inner part if rendertype==2. */
  INT8         flash_vclip;     /* What vclip to use for muzzle flash */
  INT8         robot_hit_vclip; /* What vclip for impact with robot */
  INT16        flash_sound;     /* What sound to play when fired */
  INT8         wall_hit_vclip;  /* What vclip for impact with wall */
  INT8         fire_count;      /* Number of bursts fired from EACH GUN */
                                /* per firing. */
                                /* ..For weapons which fire from both */
                                /* sides, 3*fire_count shots will be fired. */
  INT16        robot_hit_sound; /* What sound for impact with robot */
  INT8         ammo_usage;      /* How many units of ammunition it uses. */
  INT8         weapon_vclip;    /* Vclip to render for the weapon, itself. */
  INT16        wall_hit_sound;  /* What sound for impact with wall */
  INT8         destroyable;     /* If !0, this weapon can be destroyed by */
                                /* another weapon. */
  INT8         matter;          /* Flag: set if this object is matter */
                                /* (as opposed to energy) */
  INT8         bounce;          /* 1==always bounces, 2=bounces twice */
  INT8         homing_flag;     /* Set if this weapon can home in on a target. */
  UINT8        speedvar;        /* allowed variance in speed below average, */
                                /* /128: 64 = 50% meaning if speed = 100, */
                                /* ..can be 50..100 */
  UINT8        flags;           /* */
  INT8         flash;           /* Flash effect */
  INT8         afterburner_size;/* Size of blobs in F1_0/16 units, specify */
                                /* in bitmaps.tbl as floating point. */
                                /* ..Player afterburner size = 2.5. */
  INT8         children;        /* ID of weapon to drop if this contains */
                                /* children.  -1 means no children. */
  FIX          energy_usage;    /* How much fuel is consumed to fire this */
                                /* weapon. */
  FIX          fire_wait;          /* Time until this weapon can be fired */
                                   /* again. */
  FIX          multi_damage_scale; /* Scale damage by this amount when */
                                   /* applying to player in multiplayer. */
                                   /* ..F1_0 means no change. */
  D2_BITMAP_INDEX bitmap;          /* Pointer to bitmap if rendertype==0 or 1. */
  FIX          blob_size;       /* Size of blob if blob type */
  FIX          flash_size;      /* How big to draw the flash */
  FIX          impact_size;     /* How big of an impact */
  FIX          strength[NDL];   /* How much damage it can inflict */
  FIX          speed[NDL];      /* How fast it can move, difficulty */
                                /* level based. */
  FIX          mass;            /* How much mass it has */
  FIX          drag;            /* How much drag it has */
  FIX          thrust;                /* How much thrust it has */
  FIX          po_len_to_width_ratio; /* For polyobjects, the ratio of */
                                      /*len/width. (10 maybe?) */
  FIX          light;           /* Amount of light this weapon casts. */
  FIX          lifetime;        /* Lifetime in seconds of this weapon. */
  FIX          damage_radius;   /* Radius of damage caused by weapon, used */
                                /* for missiles (not lasers) to apply */
                                /* ..to damage to things it did not hit */
  D2_BITMAP_INDEX picture;         /* a picture of the weapon for the cockpit */
  D2_BITMAP_INDEX hires_picture;   /* a hires picture of the above */
} D2_WEAPON_INFO;

typedef struct {
  INT32  vclip_num;
  INT32  hit_sound;
  FIX    size;        /* 3d size of longest dimension */
  FIX    light;       /* amount of light cast by this powerup,  */
                      /* set in bitmaps.tbl */
} D2_POWERUP_TYPE_INFO;

typedef struct {
  INT32      n_models;
  INT32      model_data_size;
  UINT8      *model_data;
  INT32      submodel_ptrs[MAX_D2_SUBMODELS];
  D2_VMS_VECTOR submodel_offsets[MAX_D2_SUBMODELS];
  D2_VMS_VECTOR submodel_norms[MAX_D2_SUBMODELS];   /* norm for sep plane */
  D2_VMS_VECTOR submodel_pnts[MAX_D2_SUBMODELS];    /* point on sep plane */
  FIX        submodel_rads[MAX_D2_SUBMODELS];    /* radius for each submodel */
  UINT8      submodel_parents[MAX_D2_SUBMODELS]; /* what is parent for each  */
                                              /* submodel */
  D2_VMS_VECTOR submodel_mins[MAX_D2_SUBMODELS];
  D2_VMS_VECTOR submodel_maxs[MAX_D2_SUBMODELS];
  D2_VMS_VECTOR mins,maxs;                       /* min,max for whole model */
  FIX        rad;
  UINT8      n_textures;
  UINT16     first_texture;
  UINT8      simpler_model;           /* alternate model with less detail */
                                      /* (0 if none, model_num+1 else) */
} D2_POLYMODEL;

typedef struct {
  UINT32     model_num;
  UINT32     n_guns;
  D2_VMS_VECTOR gun_points[MAX_CONTROLCEN_GUNS];
  D2_VMS_VECTOR gun_dirs[MAX_CONTROLCEN_GUNS];
} D2_REACTOR;

typedef struct D2_PLAYER_SHIP
{
  UINT32     model_num;
  UINT32     expl_vclip_num;
  FIX        mass;
  FIX        drag;
  FIX        max_thrust;
  FIX        reverse_thrust;
  FIX        brakes;
  FIX        wiggle;
  FIX        max_rotthrust;
  D2_VMS_VECTOR gun_points[N_PLAYER_GUNS];
} D2_PLAYER_SHIPtag;




typedef struct D2_PIG_HEADER
{
	INT32 signature;
	INT32 version;
	INT32 num_textures;
} D2_PIG_HEADERtag;

typedef struct D2_PIG_TEXTURE
{
	char name[8];
	UINT8 uxsize;  // bits 0-5 anim frame num, bit 6 abm flag
	UINT8 xsize;   // low 8 bits here, 4 more bits in pad
	UINT8 ysize;   // low 8 bits here, 4 more bits in pad
	UINT8 size2;     // bits 0-3 xsize, bits 4-7 ysize
	UINT8 flag;   // see BM_FLAG_XXX
	UINT8 ave_color;   // average color
	UINT32 offset;
} D2_PIG_TEXTUREtag;


typedef struct D2_BITMAPDATA
{
	INT32 number;
	char name[9];
	BOOL transparent;
	UINT8 ave_color;   // average color
	UINT8  bitmap_array[64*64];
} D2_BITMAPDATAtag;


typedef struct D2_MODELBITMAPS
{
	INT32 count;
	D2_BITMAPDATA bitmap[20];
} D2_MODELBITMAPStag;


#define ID_HXM		0x21584d48L     //HMX!
#define ID_HAM		0x214d4148L     //HAM!
#define ID_V_HAM	0x5848414dL     //MAHX       
#define ID_HMEL  	0x4c454d48L     //HMEL

#define D2_VHAM_IDFIRSTMODEL	166;

#define D_EOF           0  //eof
#define D_DEFPOINTS		1	//defpoints
#define D_FLATPOLY		2	//flat-shaded polygon
#define D_TMAPPOLY		3	//texture-mapped polygon
#define D_SORTNORM		4	//sort by normal
#define D_RODBM         5  //rod bitmap
#define D_SUBCALL		   6	//call a subobject
#define D_DEFP_START    7  //defpoints with start
#define D_GLOW			   8	//glow value for next poly

#define D2_VPNT D2_VMS_VECTOR
/*typedef struct D2_VPNT {
  FIX x,y,z;
} D2_VPNTtag;*/

typedef struct D2_PINFO {
  short Corners;
  short Colors;
  short Segment;
  short Polytype;
  D2_VPNT Normal;
  D2_VPNT Bitmap[20];
  short Pointindex[20];
} D2_PINFOtag;


#define MAX_D2_VERTICES	800
#define MAX_D2_POLYGONS	400
#define MAX_D2_SOBJ	10

#define D2_POLYMODEL_TYPE_UNKNOWN		0
#define D2_POLYMODEL_TYPE_ROBOT			1
#define D2_POLYMODEL_TYPE_REACTOR		2
#define D2_POLYMODEL_TYPE_PLAYERSHIP	3
#define D2_POLYMODEL_TYPE_MARKER		4
#define D2_POLYMODEL_TYPE_WEAPON		5
#define D2_POLYMODEL_TYPE_WEAPONINNER	6
#define D2_POLYMODEL_TYPE_TYPESBITS		7

#define D2_POLYMODEL_TYPE_FLAG_LORES		8
#define D2_POLYMODEL_TYPE_FLAG_DESTROYED	16
#define D2_POLYMODEL_TYPE_FLAG_DYING		32

typedef struct D2_JOINTX
{
	BOOL Available;
	D2_JOINTLIST   Anim_states[MAX_D2_GUNS+1][N_ANIM_STATES];
} D2_JOINTXtag;


typedef struct D2_POLYMODELINFO
{
	int Type;
	int GunNum;
	D2_VPNT Gun[MAX_D2_GUNS];
	D2_VPNT GunDirection[MAX_D2_GUNS];
	int GunInSubModel[MAX_D2_GUNS];
	D2_VMS_VECTOR Segment_submodel_mins[MAX_D2_SUBMODELS];
	D2_VMS_VECTOR Segment_submodel_maxs[MAX_D2_SUBMODELS];
	D2_POLYMODEL	dataPolyModel;
	int	offsetPolyModel;
	int offsetModelData;
	int	offsetDyingModel;
	int offsetDestroyedModel;
	int sizeModelData;
	UINT32 dying_modelnum;
	UINT32 destroyed_modelnum;
	D2_JOINTX	Joints;
} D2_POLYMODELINFOtag;

typedef struct D2_MODEL
{
	int			Vcount;
	D2_VPNT		Vpoint[MAX_D2_VERTICES];
	D2_VPNT Vbackup[800];
	short Vsegment[800];
	int			Pcount;
	D2_PINFO	Poly[MAX_D2_POLYGONS];
	int			Scount;
	D2_VPNT		Soffset[MAX_D2_SOBJ];
	D2_VPNT		Sbackup[MAX_D2_SOBJ];
	D2_ROBOT_INFO	RobotInfo;
	D2_POLYMODEL	PolyModel;
	//REACTOR_INFO	ReactorInfo;
	D2_POLYMODELINFO	ExInfo;
	float		MaxXYZ;
	int			PolType;
	int			MyRobot;
	//int			MyReact;
	BOOL		AmLores;
	D2_JOINTX	Joints;
} D2_MODELtag;




#endif