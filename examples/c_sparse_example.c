/*
 *
 *  This file is part of MUMPS 5.2.1, released
 *  on Fri Jun 14 14:46:05 UTC 2019
 *
 */
/* Example program using the C interface to the 
 * double real arithmetic version of MUMPS, dmumps_c.
 * We solve the system A x = RHS with
 *   A = diag(1 2) and RHS = [1 4]^T
 * Solution is [1 2]^T */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "dmumps_c.h"
#define JOB_INIT -1
#define JOB_END -2
#define USE_COMM_WORLD -987654

#if defined(MAIN_COMP)
/*
 * Some Fortran compilers (COMPAQ fort) define main inside
 * their runtime library while a Fortran program translates
 * to MAIN_ or MAIN__ which is then called from "main". This
 * is annoying because MAIN__ has no arguments and we must
 * define argc/argv arbitrarily !!
 */
int MAIN__();
int MAIN_()
  {
    return MAIN__();
  }

int MAIN__()
{
  int argc=1;
  char * name = "c_example";
  char ** argv ;
#else
int main(int argc, char ** argv)
{
#endif
  DMUMPS_STRUC_C id;
  MUMPS_INT n = 3;
  MUMPS_INT8 nnz = 6;
  MUMPS_INT irn[] = {1, 1, 2, 2, 3, 3};
  MUMPS_INT jcn[] = {1, 2, 1, 2, 2, 3};
  double a[] = { 3, 2, 1, -1, 5, 1};
  double rhs_sparse[] = { 2, -1, 2};
  int irhs_sparse[] = { 1, 2, 3};
  int irhs_ptr[] = {1, 4, 4};
  double rhs[6];


  MUMPS_INT myid, ierr;

  int error = 0;
#if defined(MAIN_COMP)
  argv = &name;
#endif
  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  /* Initialize a MUMPS instance. Use MPI_COMM_WORLD */
  id.comm_fortran=USE_COMM_WORLD;
  id.par=1; id.sym=0;
  id.job=JOB_INIT;
  dmumps_c(&id);

  /* Define the problem on the host */
  if (myid == 0) {
    id.n = n; id.nnz =nnz; id.irn=irn; id.jcn=jcn;
    id.a = a; id.rhs_sparse = rhs_sparse;
    id.irhs_sparse = irhs_sparse; 
    id.irhs_ptr = irhs_ptr; 
    id.nrhs = 2;
    id.lrhs = 3;
    id.nz_rhs = 3;
    id.rhs = rhs;


  } else {
    id.rhs = NULL;
  }
#define ICNTL(I) icntl[(I)-1] /* macro s.t. indices match documentation */
  /* No outputs */
  id.ICNTL(1)=-1; id.ICNTL(2)=-1; id.ICNTL(3)=-1; id.ICNTL(4)=0;
  id.ICNTL(5)=0; id.ICNTL(18)=0; // centrally distributed A
  id.ICNTL(20)=1;
  id.ICNTL(21)=0;

  /* Call the MUMPS package (analyse, factorization and solve). */
  id.job=6;
  dmumps_c(&id);
  if (id.infog[0]<0) {
    printf(" (PROC %d) ERROR RETURN: \tINFOG(1)= %d\n\t\t\t\tINFOG(2)= %d\n",
        myid, id.infog[0], id.infog[1]);
    error = 1;
  }

  /* Terminate instance. */
  id.job=JOB_END;
  dmumps_c(&id);
  if (myid == 0) {
    if (!error) {
      printf("Solution is : (%8.2f  %8.2f)\n (%8.2f  %8.2f)\n (%8.2f  %8.2f)\n", rhs[0], rhs[3], rhs[1], rhs[4], rhs[2], rhs[5]);
    } else {
      printf("An error has occured, please check error code returned by MUMPS.\n");
    }
  }
  ierr = MPI_Finalize();
  return 0;
}
