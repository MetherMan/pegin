#ifndef __Wind3D_Render_IW3DParticleEx_Header__
#define __Wind3D_Render_IW3DParticleEx_Header__

#define STR_PARTICLEEXFILENAME		"Effect/particle.ptc"
#define STR_PARTICLEEXFILEINFO		"#W3DParticleExInfo File"
#define VERSION_FILEVERSION			100			// * 1.00

#define NUM_PARTICLE_NODE			10
#define NUM_NODEGROUP				 6
#define SIZE_PARTICLE_MAX			 96.0F
#define SIZE_PARTICLE_MIN			 1.0F
#define SPEED_PARTICLE_MAX			 1.0F

struct tagParticleHead{};
typedef CHandle<tagParticleHead>	HPARTICLE;

/**
 * Particles Type
 */
struct SParticleType
{
	DWORD		dwLifeTime;						///< This Paticle Group LifeTime
	DWORD		dwIntervalTime;					///< Interval Time

	int			nNumOneParticles;				///< Number Once Particle
	
	void*		pTexture;						///< Texture
	Vector3		vAccelerate;					///< Accelerate
	float		fGravity;						///< Gravity
	BOOL		bInverse;						///< Inverse

	DWORD		dwOneNodeTime;					///< Node Per Time

	float		afSizes[NUM_PARTICLE_NODE];			///< Node per Size
	float		afSpeeds[NUM_PARTICLE_NODE];		///< Node per Speed
	BYTE		abyColorAlpha[NUM_PARTICLE_NODE];	///< Node per Color the Alpha
	BYTE		abyColorRed[NUM_PARTICLE_NODE];		///< Node per Color the Red
	BYTE		abyColorGreen[NUM_PARTICLE_NODE];	///< Node per Color the Green
	BYTE		abyColorBlue[NUM_PARTICLE_NODE];	///< Node per Color the Blue
	BOOL		abLightMode[NUM_PARTICLE_NODE];		///< Node per LightMode
}; //struct SParticleType

/**
 * 파티클 컴포넌트 인터페이스.
 * @ingroup		Render
 */
class IW3DParticleEx : public IComponent
{
public:
	virtual BOOL			Init( const char* szFileName = NULL ) = 0;
	virtual void			Cleanup() = 0;

	virtual int				SaveFileHead() = 0;
	virtual BOOL			SaveParticleType( 
		const char* szName, 
		SParticleType* psPType,
		const char* szTextureName
		) = 0;
	virtual SParticleType*	LoadParticleType( const char* szLoadName ) = 0;
	virtual SParticleType*	LoadParticleType( const int nCount, char* szName, const int nNameLength ) = 0;

	virtual SParticleType*	GetParticleType( HPARTICLE hParticle ) = 0;
	virtual SParticleType*	FindParticleType( const char* szParticleName ) = 0;

	virtual void			Run( HPARTICLE hParticle ) = 0;
	virtual void			Stop( HPARTICLE hParticle ) = 0;
	virtual void			SetTexture( HPARTICLE hParticle, const char* szFileName ) = 0;

	virtual void			SetParticleFileName( const char* szFileName ) = 0;
	virtual int				CheckFileHead( const char* szFileName ) = 0;

	virtual HPARTICLE		AddParticle( const char* szParticleName, Vector3& vPos, DWORD dwLoop = 0 ) = 0;
	virtual void			DeleteParticle( HPARTICLE hParticle ) = 0;

	virtual void			Render( const DWORD dwTick ) = 0;
};

#endif //__Wind3D_Render_IW3DParticleEx_Header__