#ifndef CLIENT_CONTENT_HEADER
#define CLIENT_CONTENT_HEADER
struct SOUND
{
	int m_Id;
	const char* m_pFilename;
};
struct SOUNDSET
{
	const char* m_pName;
	int m_NumSounds;
	SOUND *m_aSounds;
	int m_Last;
};
struct IMAGE
{
	const char* m_pName;
	const char* m_pFilename;
	int m_Id;
};
struct SPRITESET
{
	IMAGE* m_pImage;
	int m_Gridx;
	int m_Gridy;
};
struct SPRITE
{
	const char* m_pName;
	SPRITESET* m_pSet;
	int m_X;
	int m_Y;
	int m_W;
	int m_H;
};
struct PICKUPSPEC
{
	const char* m_pName;
	int m_Respawntime;
	int m_Spawndelay;
};
struct ANIM_KEYFRAME
{
	float m_Time;
	float m_X;
	float m_Y;
	float m_Angle;
};
struct ANIM_SEQUENCE
{
	int m_NumFrames;
	ANIM_KEYFRAME *m_aFrames;
};
struct ANIMATION
{
	const char* m_pName;
	ANIM_SEQUENCE m_Body;
	ANIM_SEQUENCE m_BackFoot;
	ANIM_SEQUENCE m_FrontFoot;
	ANIM_SEQUENCE m_Attach;
};
struct WEAPONSPEC
{
	const char* m_pName;
	SPRITE* m_pSpriteBody;
	SPRITE* m_pSpriteCursor;
	SPRITE* m_pSpriteProj;
	int m_NumSpriteMuzzles;
	SPRITE* *m_aSpriteMuzzles;
	int m_VisualSize;
	int m_Firedelay;
	int m_Maxammo;
	int m_Ammoregentime;
	int m_Damage;
	float m_Offsetx;
	float m_Offsety;
	float m_Muzzleoffsetx;
	float m_Muzzleoffsety;
	float m_Muzzleduration;
};
struct WEAPONSPEC_HAMMER
{
	WEAPONSPEC* m_pBase;
};
struct WEAPONSPEC_GUN
{
	WEAPONSPEC* m_pBase;
	float m_Curvature;
	float m_Speed;
	float m_Lifetime;
};
struct WEAPONSPEC_SHOTGUN
{
	WEAPONSPEC* m_pBase;
	float m_Curvature;
	float m_Speed;
	float m_Speeddiff;
	float m_Lifetime;
};
struct WEAPONSPEC_GRENADE
{
	WEAPONSPEC* m_pBase;
	float m_Curvature;
	float m_Speed;
	float m_Lifetime;
};
struct WEAPONSPEC_RIFLE
{
	WEAPONSPEC* m_pBase;
	float m_Reach;
	int m_BounceDelay;
	int m_BounceNum;
	float m_BounceCost;
};
struct WEAPONSPEC_NINJA
{
	WEAPONSPEC* m_pBase;
	int m_Duration;
	int m_Movetime;
	int m_Velocity;
};
struct WEAPONSPECS
{
	WEAPONSPEC_HAMMER m_Hammer;
	WEAPONSPEC_HAMMER m_Gun;
	WEAPONSPEC_SHOTGUN m_Shotgun;
	WEAPONSPEC_GRENADE m_Grenade;
	WEAPONSPEC_RIFLE m_Rifle;
	WEAPONSPEC_NINJA m_Ninja;
	int m_NumId;
	WEAPONSPEC *m_aId;
};
struct DATACONTAINER
{
	int m_NumSounds;
	SOUNDSET *m_aSounds;
	int m_NumImages;
	IMAGE *m_aImages;
	int m_NumPickups;
	PICKUPSPEC *m_aPickups;
	int m_NumSpritesets;
	SPRITESET *m_aSpritesets;
	int m_NumSprites;
	SPRITE *m_aSprites;
	int m_NumAnimations;
	ANIMATION *m_aAnimations;
	WEAPONSPECS m_Weapons;
};
extern DATACONTAINER *g_pData;
enum
{
	IMAGE_NULL=0,
	IMAGE_GAME,
	IMAGE_PARTICLES,
	IMAGE_CURSOR,
	IMAGE_BANNER,
	IMAGE_EMOTICONS,
	IMAGE_BROWSEICONS,
	IMAGE_CONSOLE_BG,
	IMAGE_CONSOLE_BAR,
	NUM_IMAGES
};
enum
{
	ANIM_BASE=0,
	ANIM_IDLE,
	ANIM_INAIR,
	ANIM_WALK,
	ANIM_HAMMER_SWING,
	ANIM_NINJA_SWING,
	NUM_ANIMS
};
enum
{
	SPRITE_PART_SLICE=0,
	SPRITE_PART_BALL,
	SPRITE_PART_SPLAT01,
	SPRITE_PART_SPLAT02,
	SPRITE_PART_SPLAT03,
	SPRITE_PART_SMOKE,
	SPRITE_PART_SHELL,
	SPRITE_PART_EXPL01,
	SPRITE_PART_AIRJUMP,
	SPRITE_HEALTH_FULL,
	SPRITE_HEALTH_EMPTY,
	SPRITE_ARMOR_FULL,
	SPRITE_ARMOR_EMPTY,
	SPRITE_STAR1,
	SPRITE_STAR2,
	SPRITE_STAR3,
	SPRITE_PART1,
	SPRITE_PART2,
	SPRITE_PART3,
	SPRITE_PART4,
	SPRITE_PART5,
	SPRITE_PART6,
	SPRITE_PART7,
	SPRITE_PART8,
	SPRITE_PART9,
	SPRITE_WEAPON_GUN_BODY,
	SPRITE_WEAPON_GUN_CURSOR,
	SPRITE_WEAPON_GUN_PROJ,
	SPRITE_WEAPON_GUN_MUZZLE1,
	SPRITE_WEAPON_GUN_MUZZLE2,
	SPRITE_WEAPON_GUN_MUZZLE3,
	SPRITE_WEAPON_SHOTGUN_BODY,
	SPRITE_WEAPON_SHOTGUN_CURSOR,
	SPRITE_WEAPON_SHOTGUN_PROJ,
	SPRITE_WEAPON_SHOTGUN_MUZZLE1,
	SPRITE_WEAPON_SHOTGUN_MUZZLE2,
	SPRITE_WEAPON_SHOTGUN_MUZZLE3,
	SPRITE_WEAPON_GRENADE_BODY,
	SPRITE_WEAPON_GRENADE_CURSOR,
	SPRITE_WEAPON_GRENADE_PROJ,
	SPRITE_WEAPON_HAMMER_BODY,
	SPRITE_WEAPON_HAMMER_CURSOR,
	SPRITE_WEAPON_HAMMER_PROJ,
	SPRITE_WEAPON_NINJA_BODY,
	SPRITE_WEAPON_NINJA_CURSOR,
	SPRITE_WEAPON_NINJA_PROJ,
	SPRITE_WEAPON_RIFLE_BODY,
	SPRITE_WEAPON_RIFLE_CURSOR,
	SPRITE_WEAPON_RIFLE_PROJ,
	SPRITE_HOOK_CHAIN,
	SPRITE_HOOK_HEAD,
	SPRITE_WEAPON_NINJA_MUZZLE1,
	SPRITE_WEAPON_NINJA_MUZZLE2,
	SPRITE_WEAPON_NINJA_MUZZLE3,
	SPRITE_PICKUP_HEALTH,
	SPRITE_PICKUP_ARMOR,
	SPRITE_PICKUP_WEAPON,
	SPRITE_PICKUP_NINJA,
	SPRITE_FLAG_BLUE,
	SPRITE_FLAG_RED,
	SPRITE_TEE_BODY,
	SPRITE_TEE_BODY_OUTLINE,
	SPRITE_TEE_FOOT,
	SPRITE_TEE_FOOT_OUTLINE,
	SPRITE_TEE_HAND,
	SPRITE_TEE_HAND_OUTLINE,
	SPRITE_TEE_EYE_NORMAL,
	SPRITE_TEE_EYE_ANGRY,
	SPRITE_TEE_EYE_PAIN,
	SPRITE_TEE_EYE_HAPPY,
	SPRITE_TEE_EYE_DEAD,
	SPRITE_TEE_EYE_SURPRISE,
	SPRITE_OOP,
	SPRITE_EXCLAMATION,
	SPRITE_HEARTS,
	SPRITE_DROP,
	SPRITE_DOTDOT,
	SPRITE_MUSIC1,
	SPRITE_MUSIC2,
	SPRITE_GHOST,
	SPRITE_SUSHI,
	SPRITE_SPLATTEE,
	SPRITE_DEVILTEE,
	SPRITE_ZOMG,
	SPRITE_ZZZ,
	SPRITE_BLANK1,
	SPRITE_DEADTEE,
	SPRITE_BLANK2,
	SPRITE_BROWSE_LOCK,
	SPRITE_BROWSE_HEART,
	SPRITE_BROWSE_UNPURE,
	NUM_SPRITES
};
#endif
