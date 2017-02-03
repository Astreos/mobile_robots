//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include "mbs_matrix.h"
#include "math.h"

#include "MBSfun.h"
#include "mbs_data.h"

void mbs_kine_wheel(double Pw[4],double Rw[4][4],
<<<<<<< HEAD
					   double Vw[4],double OMw[4],
					   double tsim,int iwhl,
					   double *rzp, double *anglisp, double *angcambp,
					   double *glissp, double Vct[4], double Rtsol[4][4], double dxF[4])
=======
                       double Vw[4],double OMw[4],
                       MbsData *MBSdata,double tsim,int iwhl,
                       double *rzp, double *anglisp, double *angcambp,
                       double *glissp, double Vct[4], double Rtsol[4][4], double dxF[4])
>>>>>>> dev
{
	double Rtw[4][4], Rsol[4][4], Rtg[4][4], Rttg[4][4];
	double rz, anglis, angcamb, gliss;

	double ex[4], ey[4], eyP[4], ez[4];
	double OMw_w[4], OMwf[4], OMwf_w[4];
	double Vcts[4], Vct_geo[4], Vct_geo_s[4], Vws[4];
	double vrz_I[4], vrz[4], vx1[4];

	double sinf, tg_anglis;
	double Zgnd, Pct[4];

	double z[4]={0,0,0,1};

/* Hypotheses de base

<<<<<<< HEAD
	> le sol est (localement) plan et horizontal
	> le repere de la roue gelee possede un tangage (angle a) et 
                                    un roulis (angcamb) de max pi/4


	Conventions : l'axe de la roue doit etre un axe Y (R2)	
 
	[Rw]  = repère de la roue
	[I]   = repère inertiel
	[Rtg] = repère tangeant à la roue
	[Rsol] = repère au sol aligné à la roue

	[Rw] = Rw*[I]
*/  
	transpose(Rw,Rtw);

/*	Définition des repères */

	//ey = axe de la roue dans In
	ey[1] = Rw[2][1];
	ey[2] = Rw[2][2];
	ey[3] = Rw[2][3];

	//eyP = axe de la roue projeté dans In
	eyP[1] = Rw[2][1];
	eyP[2] = Rw[2][2];
	eyP[3] = 0;
	normalize(eyP,eyP);
	
	
	//ex = vecteur tg à la roue dans le plan dans In
=======
    > le sol est (localement) plan et horizontal
    > le repere de la roue gelee possede un tangage (angle a) et
                                    un roulis (angcamb) de max pi/4


    Conventions : l'axe de la roue doit etre un axe Y (R2)

    [Rw]  = repère de la roue
    [I]   = repère inertiel
    [Rtg] = repère tangeant à la roue
    [Rsol] = repère au sol aligné à la roue

    [Rw] = Rw*[I]
*/
    transpose(Rw,Rtw);

/*  Définition des repères */

    //ey = axe de la roue dans In
    ey[1] = Rw[2][1];
    ey[2] = Rw[2][2];
    ey[3] = Rw[2][3];

    //eyP = axe de la roue projeté dans In
    eyP[1] = Rw[2][1];
    eyP[2] = Rw[2][2];
    eyP[3] = 0;
    normalize(eyP,eyP);


    //ex = vecteur tg à la roue dans le plan dans In
>>>>>>> dev
    cross_product(eyP,z,ex);

	//[Rsol]=Rsol*[I]
	Rsol[1][1] = ex[1];
	Rsol[1][2] = ex[2];
	Rsol[1][3] = ex[3];

	Rsol[2][1] = eyP[1];
	Rsol[2][2] = eyP[2];
	Rsol[2][3] = eyP[3];

	Rsol[3][1] = 0;
	Rsol[3][2] = 0;
	Rsol[3][3] = 1;

	transpose(Rsol,Rtsol);

	//ez = vecteur pointant vers le centre de la roue dans In
    cross_product(ex,ey,ez);
<<<<<<< HEAD
    
	//[Rtg]=Rtg*[I]
	Rtg[1][1] = ex[1];
	Rtg[1][2] = ex[2];
	Rtg[1][3] = ex[3];
=======

    //[Rtg]=Rtg*[I]
    Rtg[1][1] = ex[1];
    Rtg[1][2] = ex[2];
    Rtg[1][3] = ex[3];
>>>>>>> dev

	Rtg[2][1] = ey[1];
	Rtg[2][2] = ey[2];
	Rtg[2][3] = ey[3];

	Rtg[3][1] = ez[1];
	Rtg[3][2] = ez[2];
	Rtg[3][3] = ez[3];

	transpose(Rtg,Rttg);


/*	ANGLE DE CAMBRURE (f) */
//	Calcul de l'angle (f) d'inclinaison de la roue (angcamb) par rapport a la normale au sol

    sinf=Rw[2][3];       	//! ok sur sol horizontal !
    angcamb=asin(sinf);
<<<<<<< HEAD
	*angcambp = angcamb;
    
/*	POINT DE CONTACT + VITESSE */
//	Calcul du rayon de percee (rz) de la roue dans le sol et
//	de la position et vitesse du point de contact 
//	de la roue avec le sol

//Position
	//Hauteur du sol sous le centre de la roue
    Zgnd = 0.0; // user_GroundLevel(Pw[1],Pw[2],MBSdata,tsim,iwhl);
=======
    *angcambp = angcamb;

/*  POINT DE CONTACT + VITESSE */
//  Calcul du rayon de percee (rz) de la roue dans le sol et
//  de la position et vitesse du point de contact
//  de la roue avec le sol

//Position
    //Hauteur du sol sous le centre de la roue
    Zgnd = 0.0; //user_GroundLevel(Pw[1],Pw[2],MBSdata,tsim,iwhl); Not yet implemented
>>>>>>> dev

	//Rayon de percée
    rz = (Pw[3]-Zgnd)/cos(angcamb);
	*rzp = rz;

	//vecteur rayon dans le repere [Rtg]
    vrz[1] = 0;
<<<<<<< HEAD
	vrz[2] = 0;
	vrz[3] = -rz;	
=======
    vrz[2] = 0;
    vrz[3] = -rz;
>>>>>>> dev

	//vecteur rayon dans le repere [I]
	matrix_product(Rttg,vrz,vrz_I);

	//position du point de contact (dans [I])
	vector_sum(Pw,vrz_I,Pct);

    cross_product(OMw,vrz_I,vx1);

<<<<<<< HEAD
	//vitesse du point materiel situe au contact (dans [I])
    vector_sum(Vw,vx1,Vct);			
=======
    //vitesse du point materiel situe au contact (dans [I])
    vector_sum(Vw,vx1,Vct);
>>>>>>> dev

	//vitesse du point materiel situe au contact (dans [Rsol])
	matrix_product(Rsol,Vct,Vcts);


//Point d'application de la force dans [Rw]
	matrix_product(Rw,vrz_I,dxF);


/* GLISSEMENT LATERAL (angliss) */
//	Calcul de l'angle de glissement lateral
//  angle entre la vitesse du point géometrique et le plan de la roue
//Calcul dans le repère [Rsol] avec la vitesse du point géométrique

	//Vitesse du centre de la roue dans le repère [Rsol]
	matrix_product(Rsol,Vw,Vws);

<<<<<<< HEAD
//Vitesse point géométrique: 
	//vecteur vitesse angulaire roue gelée (OMwf)
=======
//Vitesse point géométrique:
    //vecteur vitesse angulaire roue gelée (OMwf)
>>>>>>> dev
    matrix_product(Rw,OMw,OMw_w);
	OMwf_w[1] = OMw_w[1];
	OMwf_w[2] = 0;
	OMwf_w[3] = OMw_w[3];
	matrix_product(Rtw,OMwf_w,OMwf);

	//vitesse d'entrainememt du point geométrique au contact
    cross_product(OMwf,vrz_I,vx1);

<<<<<<< HEAD
	//vitesse du point géométrique situe au contact (dans [I])
    vector_sum(Vw,vx1,Vct_geo);			

	//vitesse du point géométrique situe au contact (dans [Rsol])
	matrix_product(Rsol,Vct_geo,Vct_geo_s);
    
=======
    //vitesse du point géométrique situe au contact (dans [I])
    vector_sum(Vw,vx1,Vct_geo);

    //vitesse du point géométrique situe au contact (dans [Rsol])
    matrix_product(Rsol,Vct_geo,Vct_geo_s);

>>>>>>> dev
//Angle de glissement
    if ((Vct_geo_s[1]>=1e-3) | (Vct_geo_s[1]<=-1e-3))
    {
		tg_anglis = Vct_geo_s[2]/Vct_geo_s[1];
    }
	else
    {
<<<<<<< HEAD
		tg_anglis = 0;
	}
    
=======
        tg_anglis = 0;
    }

>>>>>>> dev

    anglis = atan(tg_anglis);
	*anglisp = anglis;

/* GLISSEMENT */
<<<<<<< HEAD
//	Calcul du glissement longitudinal
//	
//	rapport de la vitesse du point de contact a la vitesse du centre de la
//	roue suivant l'axe de la roue
=======
//  Calcul du glissement longitudinal
//
//  rapport de la vitesse du point de contact a la vitesse du centre de la
//  roue suivant l'axe de la roue
>>>>>>> dev


	//vitesse du point de contact: Vcts(1)
	//vitesse du centre de la roue: Vws(1)
    if ((Vws[1]>=1e-3) | (Vws[1]<=-1e-3))
    {
		gliss = -Vcts[1]/Vws[1];
    }
	else
    {
		gliss = 0;
    }

	*glissp = gliss;
}

