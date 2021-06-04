/*
 *
 *  This file is part of MUMPS 5.4.0, released
 *  on Tue Apr 13 15:26:30 UTC 2021
 *
 *
 *  Copyright 1991-2021 CERFACS, CNRS, ENS Lyon, INP Toulouse, Inria,
 *  Mumps Technologies, University of Bordeaux.
 *
 *  This version of MUMPS is provided to you free of charge. It is
 *  released under the CeCILL-C license 
 *  (see doc/CeCILL-C_V1-en.txt, doc/CeCILL-C_V1-fr.txt, and
 *  https://cecill.info/licences/Licence_CeCILL-C_V1-en.html)
 *
 */
#include<stdio.h>
int main()
{
printf("#if ! defined(MUMPS_INT_H)\n");
printf("#   define MUMPS_INT_H\n");
#if defined(INTSIZE64)
printf("#   define MUMPS_INTSIZE64\n");
#else
printf("#   define MUMPS_INTSIZE32\n");
#endif
printf("#endif\n");
return 0;
}
