#include	"stdafx.h"
#include	"tcf.h"

void	_TCF::barycentric	(Fvector2 &P, float &u, float &v, float &w)
{
	Fvector2 	kV02; kV02.sub(uv[0],uv[2]);
	Fvector2 	kV12; kV12.sub(uv[1],uv[2]);
	Fvector2 	kPV2; kPV2.sub(P,    uv[2]);

	float		fM00 = kV02.dot(kV02);
	float		fM01 = kV02.dot(kV12);
	float		fM11 = kV12.dot(kV12);
	float		fR0  = kV02.dot(kPV2);
	float		fR1  = kV12.dot(kPV2);
	float		fDet = fM00*fM11 - fM01*fM01;

	u			= (fM11*fR0 - fM01*fR1)/fDet;
	v			= (fM00*fR1 - fM01*fR0)/fDet;
	w			= 1.0f - u - v;
}
//	Fvector2			uv	[3];
void	_TCF::read		( IReader	&r )
{
	r.r(uv, sizeof(uv) );
}
void	_TCF::write		( IWriter	&w ) const
{
	w.w(uv, sizeof(uv) );
}