

/**
 *
 * author Quentin Docquier
 *
 */

#include "mbs_data.h"
#include "mbs_project_interface.h"
#include "MBSfun.h"
#include "useful_functions.h"

int mbs_calc_Fruc(double Fruc[], MbsAux *aux, MbsData *s)
{
	int ok, err = 0;
	int i, j;

	int *iquc;
	int nuc;

	double *F;// *Fr_uc;
	double val;
	int ind_x;

	nuc = s->nqu + s->nqc;
	iquc = get_ivec_0(nuc + 1);
	conc_ivec_1(s->qu, s->nqu, s->qc, s->nqc, iquc);

	F = get_dvec_0(s->njoint + 1);

	// ?Q need for a reference to this sequence of calculation (something like a diagram)...

	// 1. driven variables, qc as the user defines it
	if (s->nqc > 0)
	{
		user_DrivenJoints(s, s->tsim);
		for (i = 1; i <= s->nqc; i++)					// Rob convention starts at 1
		{
			if (s->qdd[s->qc[i]] != 0.0)
			{
				s->qdd[s->qc[i]] = 0.0;
				printf(">>MBS>> MBS_equil - user file : non-zero driven accelerations : forced to zero \n");
			}
		}
	}

	// 2. Geometric Constraints, qv as function of qu and qc
    if (s->nqv > 0)
	{

		if (mbs_close_geo(s, aux) >= aux->MAX_NR_ITER)
		{
			err = 501;

			free_ivec_0(iquc);
			free_dvec_0(F);

			return err;
		}

	}

	// 3. Kinematic Constraints, qv_d as function of qu_d and qc_d
	if (s->nqv > 0)
	{
		mbs_close_kin(s, aux);
	}

	// 4. External (and link) and Joint forces
	mbs_calc_force(s);

	// 5.1  Equations of motion (unconstrained MBS)
	//zeros_double_vec(&(s->qdd[1]), s->njoint);		// ?Q ... sécurité en attente d'une fonction symb c(q,qd)
    mbs_invdyna(aux->c, s, s->tsim);			// ?Q futur : phi(q,qd) including the mass matrix. the term is then derived with regards to q and qd !


	for (i = 1; i <= s->njoint; i++)
	{
		F[i] = aux->c[i] - s->Qq[i];
	}

	// 5.2  Equations of motion (reduction for constrained MBS)
	if (s->nqv > 0)	// Fr_uc(i) = F(iquc) + Bvuc'*F(iqv);
	{
		for (i = 1; i <= nuc; i++)
		{
			val = 0.0;
			for (j = 1; j <= s->nqv; j++)
			{
				val += aux->Bvuc[j][i] * F[s->qv[j]];
			}
			Fruc[i] = F[iquc[i]] + val;
		}
	}
	else			// Fr_uc = F(iquc);
	{
		for (i = 1; i <= nuc; i++)
		{
			Fruc[i] = F[iquc[i]];
		}
	}

	free_ivec_0(iquc);
	free_dvec_0(F);
	return err;
}
