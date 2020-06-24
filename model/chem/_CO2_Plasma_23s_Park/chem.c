/*
SPDX-License-Identifier: BSD-2-Clause

Copyright 2020 Bernard Parent

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
   conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
   of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <model/chem/_chem.h>
#include <model/_model.h>
#include <model/thermo/_thermo.h>
#include <model/metrics/_metrics.h>
#include <model/share/chem_share.h>


#define TOWNSEND TRUE
#define TOWNSEND_IMPLICIT FALSE
#define Estarmin 1e-40


/* set all reactions to true except for testing purposes */
const static bool REACTION[46]=
  {
   TRUE, /* reaction 0 */
   TRUE, /* reaction 1 */
   TRUE, /* reaction 2 */
   TRUE, /* reaction 3 */
   TRUE, /* reaction 4 */
   TRUE, /* reaction 5 */
   TRUE, /* reaction 6 */
   TRUE, /* reaction 7 */
   TRUE, /* reaction 8 */
   TRUE, /* reaction 9 */
   TRUE, /* reaction 10 */
   TRUE, /* reaction 11 */
   TRUE, /* reaction 12 */
   TRUE, /* reaction 13 */
   TRUE, /* reaction 14 */
   TRUE, /* reaction 15 */
   TRUE, /* reaction 16 */
   TRUE, /* reaction 17 */
   TRUE, /* reaction 18 */
   TRUE, /* reaction 19 */ 
   TRUE, /* reaction 20 */
   TRUE, /* reaction 21 */
   TRUE, /* reaction 22 */
   TRUE, /* reaction 23 */
   TRUE, /* reaction 24 */
   TRUE, /* reaction 25 */
   TRUE, /* reaction 26 */
   TRUE, /* reaction 27 */
   TRUE, /* reaction 28 */
   TRUE, /* reaction 29 */
   TRUE, /* reaction 30 */
   TRUE, /* reaction 31 */
   TRUE, /* reaction 32 */
   TRUE, /* reaction 33 */
   TRUE, /* reaction 34 */
   TRUE, /* reaction 35 */
   TRUE, /* reaction 36 */
   TRUE, /* reaction 37 */
   TRUE, /* reaction 38 */
   TRUE, /* reaction 39 */
   TRUE, /* reaction 40 */
   TRUE, /* reaction 41 */
   TRUE, /* reaction 42 */
   TRUE, /* reaction 43 */
   TRUE, /* reaction 44 */
   TRUE, /* reaction 45 */
  };




void find_W ( spec_t rhok, double T, double Te, double Tv, double Estar, double Qbeam, spec_t W ) {
  double N[ns];
  double theta,kf;
  long k;
  double R;
  

  /* find properties needed by add_to_W* functions */
  R=1.9872;
  for ( k = 0; k < ns; k++ ) {
    W[k] = 0.0;
    N[k] = rhok[k] / _calM (k ) * 1e-6 * calA;  /* particules/cm^3 */
  }
  Estar = max ( Estarmin, Estar );
  theta = log ( Estar );


  if (REACTION[1])
    add_to_W_2r3p ( specO2, specO2,   specO, specO, specO2,  _kf_Arrhenius(2, 2.3e19, -1.0, 59400.0*R, T) , N, W);
    
  if (REACTION[2])
    add_to_W_2r3p ( specO2, specO,   specO, specO, specO,    _kf_Arrhenius(2, 8.5e19, -1.0, 59400.0*R, T) , N, W);
  
  if (REACTION[3]){
    kf=_kf_Arrhenius(2, 3.0e18, -1.0, 59400.0*R, T);
    add_to_W_2r3p ( specO2, specN,   specO, specO, specN,  kf   , N, W);
    add_to_W_2r3p ( specO2, specN2,   specO, specO, specN2,  kf , N, W);
    add_to_W_2r3p ( specO2, specNO,   specO, specO, specNO,  kf , N, W);
    add_to_W_2r3p ( specO2, specNOplus,   specO, specO, specNOplus,  kf , N, W);
  }
  
  if (REACTION[4])
    add_to_W_2r3p ( specN2, specN2,   specN, specN, specN2,    _kf_Arrhenius(2, 3.8e19, -1.0, 113200.0*R, T) , N, W);

  if (REACTION[5])
    add_to_W_2r3p ( specN2, specN,   specN, specN, specN,    _kf_Arrhenius(2, 1.3e20, -1.0, 113200.0*R, T) , N, W);

  if (REACTION[6]){
    kf=_kf_Arrhenius(2, 1.9e19, -1.0, 113200.0*R, T);
    add_to_W_2r3p ( specN2, specO,   specN, specN, specO,   kf , N, W);
    add_to_W_2r3p ( specN2, specO2,  specN, specN, specO2,   kf , N, W);
    add_to_W_2r3p ( specN2, specNO,  specN, specN, specNO,   kf , N, W);
    add_to_W_2r3p ( specN2, specNOplus,  specN, specN, specNOplus,   kf , N, W);
  }
  
  if (REACTION[7]){
    kf=_kf_Arrhenius(2, 2.4e17, -0.5, 75500.0*R, T);
    add_to_W_2r3p ( specNO, specO,     specN, specO, specO,     kf , N, W);
    add_to_W_2r3p ( specNO, specO2,    specN, specO, specO2,    kf , N, W);
    add_to_W_2r3p ( specNO, specN,     specN, specO, specN,     kf , N, W);
    add_to_W_2r3p ( specNO, specN2,    specN, specO, specN2,    kf , N, W);
    add_to_W_2r3p ( specNO, specNOplus,specN, specO, specNOplus,kf , N, W);
  }

  if (REACTION[8])
    add_to_W_2r2p ( specO, specN2,   specN, specNO,    _kf_Arrhenius(2, 6.8e13, 0.0, 37750.0*R, T) , N, W);

  if (REACTION[9])
    add_to_W_2r2p ( specO, specNO,   specN, specO2,    _kf_Arrhenius(2, 4.3e7, 1.5, 19100.0*R, T) , N, W);

  if (REACTION[10])
    add_to_W_2r2p ( specN, specO,    specNOplus, speceminus,    _kf_Arrhenius(2, 1.3e8, 1.0, 31900.0*R, T) , N, W);

  if (REACTION[11])
    add_to_W_3r2p ( specO, specO, specO2,   specO2, specO2,    _kf_Arrhenius(3, 1.9e16, -0.5, 0.0, T) , N, W);

  if (REACTION[12])
    add_to_W_3r2p ( specO, specO, specO,   specO2, specO,    _kf_Arrhenius(3, 7.1e16, -0.5, 0.0, T) , N, W);

  if (REACTION[13]){
    kf=_kf_Arrhenius(3, 2.5e15, -0.5, 0.0, T);
    add_to_W_3r2p ( specO, specO, specN,   specO2, specN,    kf , N, W);
    add_to_W_3r2p ( specO, specO, specN2,   specO2, specN2,    kf , N, W);
    add_to_W_3r2p ( specO, specO, specNO,   specO2, specNO,    kf , N, W);
    add_to_W_3r2p ( specO, specO, specNOplus,   specO2, specNOplus,    kf , N, W);
  }

  if (REACTION[14])
    add_to_W_3r2p ( specN, specN, specN2,   specN2, specN2,    _kf_Arrhenius(3, 2.0e18, -1.0, 0.0, T) , N, W);

  if (REACTION[15])
    add_to_W_3r2p ( specN, specN, specN,   specN2, specN,    _kf_Arrhenius(3, 7.0e18, -1.0, 0.0, T) , N, W);

  if (REACTION[16]){
    kf=_kf_Arrhenius(3, 1e18, -1.0, 0.0, T);
    add_to_W_3r2p ( specN, specN, specO,   specN2, specO,    kf , N, W);
    add_to_W_3r2p ( specN, specN, specO2,   specN2, specO2,    kf , N, W);
    add_to_W_3r2p ( specN, specN, specNO,   specN2, specNO,    kf , N, W);
    add_to_W_3r2p ( specN, specN, specNOplus,   specN2, specNOplus,    kf , N, W);
  }     

  if (REACTION[17]){
    kf=_kf_Arrhenius(3, 6e16, -0.5, 0.0, T);
    add_to_W_3r2p ( specN, specO, specO,   specNO, specO,    kf , N, W);
    add_to_W_3r2p ( specN, specO, specO2,  specNO, specO2,    kf , N, W);
    add_to_W_3r2p ( specN, specO, specN,   specNO, specN,    kf , N, W);
    add_to_W_3r2p ( specN, specO, specN2,  specNO, specN2,    kf , N, W);
    add_to_W_3r2p ( specN, specO, specNOplus,  specNO, specNOplus,    kf , N, W);
  }     

  if (REACTION[18])
    add_to_W_2r2p ( specN, specNO,   specO, specN2,    _kf_Arrhenius(2, 1.5e13, 0.0, 0.0, T) , N, W);

  if (REACTION[19])
    add_to_W_2r2p ( specN, specO2,   specO, specNO,    _kf_Arrhenius(2, 1.8e8, 1.5, 3020.0*R, T) , N, W);

  if ( TOWNSEND ) {
    
    if (REACTION[20])
      add_to_W_2r3p ( specN2, speceminus,   specN2plus, speceminus, speceminus,   exp ( -0.0105809 * sqr ( theta ) - 2.40411e-75 * pow ( theta, 46.0 ) ), N, W);

    if (REACTION[21])
      add_to_W_2r3p ( specO2, speceminus,   specO2plus, speceminus, speceminus,   exp ( -0.0102785 * sqr ( theta ) - 2.42260e-75 * pow ( theta, 46.0 ) ), N, W);

    if (REACTION[23])
      add_to_W_2r3p ( specCs, speceminus,   specCsplus, speceminus, speceminus,    _kf_Arrhenius(2, 1.6e32, -3.3, 45172.0*R, Te), N, W);
  }

  if (REACTION[24])
    add_to_W_2r2p ( speceminus, specNOplus,   specN, specO,    _kf_Arrhenius(2, 2e19, -1.0, 0.0, Te) , N, W);

  if (REACTION[25])
    add_to_W_2r2p ( speceminus, specO2plus,   specO, specO,    2.0e-7*pow(300.0/Te,0.7) , N, W);

  if (REACTION[26])
    add_to_W_2r2p ( speceminus, specN2plus,   specN, specN,    2.8e-7*pow(300.0/Te,0.5) , N, W);

  if (REACTION[27])
    add_to_W_2r1p ( speceminus, specCsplus,   specCs,   _kf_Arrhenius(2, 7.2e14, -0.67, 0.0, Te)  , N, W);

  if (REACTION[28]){
    kf=3.0e19/sqr(calA);
    add_to_W_3r2p ( speceminus, specCsplus, specO,   specCs, specO,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specCsplus, specO2,  specCs, specO2, kf  , N, W);
    add_to_W_3r2p ( speceminus, specCsplus, specN,   specCs, specN,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specCsplus, specN2,  specCs, specN2, kf  , N, W);
    add_to_W_3r2p ( speceminus, specCsplus, specNO,  specCs, specNO, kf  , N, W);
  }

  if (REACTION[29])
    add_to_W_3r2p ( speceminus, specCsplus, speceminus,  specCs, speceminus,    _kf_Arrhenius(3, 4e39, -4.5, 0.0, Te) , N, W);

  if (REACTION[30])
    add_to_W_2r1p ( speceminus, specO, specOminus,   7.2e8/calA , N, W);

  if (REACTION[31]){
    kf=_kf_Arrhenius(3, 8.5e19, -0.5, 0.0, Te);
    add_to_W_3r2p ( speceminus, specO, specO,   specOminus, specO,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specO, specO2,   specOminus, specO2,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specO, specN,   specOminus, specN,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specO, specN2,   specOminus, specN2,  kf  , N, W);
    add_to_W_3r2p ( speceminus, specO, specNO,   specOminus, specNO,  kf  , N, W);
  }
  
  if (REACTION[33])
    add_to_W_3r2p ( specO2, specO2, speceminus,  specO2, specO2minus,   1.4e-29 * 300.0 / Te * exp ( -600.0 / T ) * exp ( 700.0 * ( Te - T ) / Te / T ), N, W);

  if (REACTION[34])
    add_to_W_3r2p ( specO2, specN2, speceminus,  specN2, specO2minus,   1.07e-31 * sqr ( 300.0 / Te ) * exp ( -70.0 / T ) * exp ( 1500.0 * ( Te - T ) / Te / T ), N, W);

  if (REACTION[35]){
    kf=_kf_Arrhenius(2, 1.4e12, 1.0, 17016.0*R, T);
    add_to_W_2r3p ( specOminus, specO,   specO, speceminus, specO,   kf, N, W);
    add_to_W_2r3p ( specOminus, specO2,   specO, speceminus, specO2,   kf, N, W);
    add_to_W_2r3p ( specOminus, specN,   specO, speceminus, specN,   kf, N, W);
    add_to_W_2r3p ( specOminus, specN2,   specO, speceminus, specN2,   kf, N, W);
    add_to_W_2r3p ( specOminus, specNO,   specO, speceminus, specNO,   kf, N, W);
  }

  if (REACTION[36]){
    kf=2.0e-7 * pow ( 300.0 / T, 0.5 );
    add_to_W_2r2p ( specN2plus, specO2minus,   specO2, specN2,   kf, N, W);
    add_to_W_2r2p ( specO2plus, specO2minus,   specO2, specO2,   kf, N, W);
    add_to_W_2r2p ( specNOplus, specO2minus,   specO2, specNO,   kf, N, W);
  }
  
  if (REACTION[37]){
    kf=2.0e-25 * pow ( 300.0 / T, 2.5 );
    add_to_W_3r3p ( specN2, specN2plus, specO2minus,  specO2, specN2, specN2,   kf, N, W);
    add_to_W_3r3p ( specO2, specN2plus, specO2minus,  specO2, specN2, specO2,   kf, N, W);
    add_to_W_3r3p ( specN,  specN2plus, specO2minus,  specO2, specN2, specN,    kf, N, W);
    add_to_W_3r3p ( specO,  specN2plus, specO2minus,  specO2, specN2, specO,    kf, N, W);
    add_to_W_3r3p ( specNO, specN2plus, specO2minus,  specO2, specN2, specNO,   kf, N, W);
    
    add_to_W_3r3p ( specN2, specO2plus, specO2minus,  specO2, specO2, specN2,   kf, N, W);
    add_to_W_3r3p ( specO2, specO2plus, specO2minus,  specO2, specO2, specO2,   kf, N, W);
    add_to_W_3r3p ( specN,  specO2plus, specO2minus,  specO2, specO2, specN,    kf, N, W);
    add_to_W_3r3p ( specO,  specO2plus, specO2minus,  specO2, specO2, specO,    kf, N, W);
    add_to_W_3r3p ( specNO, specO2plus, specO2minus,  specO2, specO2, specNO,   kf, N, W);
    
    add_to_W_3r3p ( specN2, specNOplus, specO2minus,  specO2, specNO, specN2,   kf, N, W);
    add_to_W_3r3p ( specO2, specNOplus, specO2minus,  specO2, specNO, specO2,   kf, N, W);
    add_to_W_3r3p ( specN,  specNOplus, specO2minus,  specO2, specNO, specN,    kf, N, W);
    add_to_W_3r3p ( specO,  specNOplus, specO2minus,  specO2, specNO, specO,    kf, N, W);
    add_to_W_3r3p ( specNO, specNOplus, specO2minus,  specO2, specNO, specNO,   kf, N, W);
  }

  if (REACTION[38])
    add_to_W_2r3p ( specO2, specO2minus,   specO2, specO2, speceminus,   8.6e-10 * exp ( -6030.0 / T ) * ( 1.0 - exp ( -1570.0 / T ) ), N, W);

  if (REACTION[39])
    add_to_W_2r2p ( specCs, specO2,   specCsplus, specO2minus,    _kf_Arrhenius(2, 1e17, 0.07, 39836.0*R, T) , N, W);

  if (REACTION[40])
    add_to_W_2r2p ( specCs, specO,   specCsplus, specOminus,    _kf_Arrhenius(2, 8.7e16, -0.15, 28177.0*R, T) , N, W);
    
  if (REACTION[41]){
    kf=_kf_Arrhenius(2, 1.2e12, 1.18, 45172.0*R, T);
    add_to_W_2r3p ( specCs, specO,    specCsplus, speceminus, specO,   kf, N, W);    
    add_to_W_2r3p ( specCs, specO2,   specCsplus, speceminus, specO2,  kf, N, W);    
    add_to_W_2r3p ( specCs, specN,    specCsplus, speceminus, specN,   kf, N, W);    
    add_to_W_2r3p ( specCs, specN2,   specCsplus, speceminus, specN2,  kf, N, W);    
    add_to_W_2r3p ( specCs, specNO,   specCsplus, speceminus, specNO,  kf, N, W);    
  }

  if (REACTION[42])
    add_to_W_2r2p ( specCs, specNOplus,   specCsplus, specNO,    _kf_Arrhenius(2, 6.02e13, -0.5, 0.0, T) , N, W);

  if (REACTION[43])
    add_to_W_2r2p ( specCsplus, specO2minus,   specCs, specO2,    2.0e17/calA , N, W);
  
  if (REACTION[44])
    add_to_W_2r2p ( specCsplus, specOminus,   specCs, specO,    _kf_Arrhenius(2, 2.0e19, -0.6, 0.0, T)  , N, W);

  if (REACTION[45])
    add_to_W_2r2p ( specCsplus, specNO,   specCs, specNOplus,    _kf_Arrhenius(2, 8.07e11, 0.0, 61800.0*R, T)  , N, W);
}






/* Verify the validity of the dW terms at node i=10,j=10 using the command ./test -r control.wrp -node 10 10 dSchemdU 
 * Make sure to verify the dW terms over a wide range of temperatures and mass fractions 
 * Note that the verification using ./test is done by comparing the analytical expressions to numerical derivatives
 * The numerical derivatives depend strongly on the values given to Uref[] within Cycle()
 */ 

void find_dW_dx ( spec_t rhok, spec_t mu, double T, double Te, double Tv, double Estar, double Qbeam,
                  spec2_t dWdrhok, spec_t dWdT, spec_t dWdTe, spec_t dWdTv, spec_t dWdQbeam ) {
  long k, s;                    
  spec_t N;
  double R,theta,kf,dkfdTe,dkfdT,dkfdTv;
  
  
  R=1.9872;
  /* initialize all derivatives to zero */
  for ( s = 0; s < ns; s++ ) {
    dWdT[s] = 0.0;
    dWdTe[s] = 0.0;
    dWdTv[s] = 0.0;
    dWdQbeam[s] = 0.0;
    for ( k = 0; k < ns; k++ ) {
      dWdrhok[s][k] = 0.0;
    }
  }

  /* find properties needed by add_to_dW* functions in proper units */
  for ( k = 0; k < ns; k++ ) {
    N[k] = rhok[k] / _calM ( k ) * 1e-6 * calA;
  }
  Estar = max ( Estarmin, Estar );
  theta = log ( Estar );



  if (REACTION[1]) {
    kf=_kf_Arrhenius(2, 2.3e19, -1.0, 59400.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 2.3e19, -1.0, 59400.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specO2, specO2,   specO, specO, specO2,  kf , N,  dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
    
  if (REACTION[2]) {
    kf=_kf_Arrhenius(2, 8.5e19, -1.0, 59400.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 8.5e19, -1.0, 59400.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specO2, specO,   specO, specO, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
  
  if (REACTION[3]){
    kf=_kf_Arrhenius(2, 3.0e18, -1.0, 59400.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 3.0e18, -1.0, 59400.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specO2, specN,   specO, specO, specN,  kf   , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specO2, specN2,   specO, specO, specN2,  kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specO2, specNO,   specO, specO, specNO,  kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specO2, specNOplus,   specO, specO, specNOplus,  kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
  
  if (REACTION[4]) {
    kf=_kf_Arrhenius(2, 3.8e19, -1.0, 113200.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 3.8e19, -1.0, 113200.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specN2, specN2,   specN, specN, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[5]) {
    kf=_kf_Arrhenius(2, 1.3e20, -1.0, 113200.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.3e20, -1.0, 113200.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specN2, specN,   specN, specN, specN,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }


  if (REACTION[6]){
    kf=_kf_Arrhenius(2, 1.9e19, -1.0, 113200.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.9e19, -1.0, 113200.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specN2, specO,   specN, specN, specO,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specN2, specO2,  specN, specN, specO2,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specN2, specNO,  specN, specN, specNO,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specN2, specNOplus,  specN, specN, specNOplus,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
  
  if (REACTION[7]){
    kf=_kf_Arrhenius(2, 2.4e17, -0.5, 75500.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 2.4e17, -0.5, 75500.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specNO, specO,     specN, specO, specO,     kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specNO, specO2,    specN, specO, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specNO, specN,     specN, specO, specN,     kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specNO, specN2,    specN, specO, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specNO, specNOplus,specN, specO, specNOplus,kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[8]){
    kf=_kf_Arrhenius(2, 6.8e13, 0.0, 37750.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 6.8e13, 0.0, 37750.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specO, specN2,   specN, specNO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[9]){
    kf=_kf_Arrhenius(2, 4.3e7, 1.5, 19100.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 4.3e7, 1.5, 19100.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specO, specNO,   specN, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[10]){
    kf=_kf_Arrhenius(2, 1.3e8, 1.0, 31900.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.3e8, 1.0, 31900.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specN, specO,    specNOplus, speceminus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[11]){
    kf=_kf_Arrhenius(3, 1.9e16, -0.5, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 1.9e16, -0.5, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_3r2p ( specO, specO, specO2,   specO2, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[12]){
    kf=_kf_Arrhenius(3, 7.1e16, -0.5, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 7.1e16, -0.5, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_3r2p ( specO, specO, specO,   specO2, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[13]){
    kf=_kf_Arrhenius(3, 2.5e15, -0.5, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 2.5e15, -0.5, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;    
    add_to_dW_3r2p ( specO, specO, specN,   specO2, specN,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specO, specO, specN2,   specO2, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specO, specO, specNO,   specO2, specNO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specO, specO, specNOplus,   specO2, specNOplus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[14]){
    kf=_kf_Arrhenius(3, 2.0e18, -1.0, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 2.0e18, -1.0, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;        
    add_to_dW_3r2p ( specN, specN, specN2,   specN2, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[15]){
    kf=_kf_Arrhenius(3, 7.0e18, -1.0, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 7.0e18, -1.0, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;            
    add_to_dW_3r2p ( specN, specN, specN,   specN2, specN,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[16]){
    kf=_kf_Arrhenius(3, 1e18, -1.0, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 1e18, -1.0, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;            
    add_to_dW_3r2p ( specN, specN, specO,   specN2, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specN, specO2,   specN2, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specN, specNO,   specN2, specNO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specN, specNOplus,   specN2, specNOplus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }     

  if (REACTION[17]){
    kf=_kf_Arrhenius(3, 6e16, -0.5, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(3, 6e16, -0.5, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;                
    add_to_dW_3r2p ( specN, specO, specO,   specNO, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specO, specO2,  specNO, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specO, specN,   specNO, specN,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specO, specN2,  specNO, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( specN, specO, specNOplus,  specNO, specNOplus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }     

  if (REACTION[18]){
    kf=_kf_Arrhenius(2, 1.5e13, 0.0, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.5e13, 0.0, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;                
    add_to_dW_2r2p ( specN, specNO,   specO, specN2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }




  if (REACTION[19]){
    kf=_kf_Arrhenius(2, 1.8e8, 1.5, 3020.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.8e8, 1.5, 3020.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;                
    add_to_dW_2r2p ( specN, specO2,   specO, specNO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

    
  if ( TOWNSEND && TOWNSEND_IMPLICIT ) {

    if (REACTION[20]) {
      kf=exp ( -0.0105809 * sqr ( theta ) - 2.40411e-75 * pow ( theta, 46.0 ) );
      dkfdTe = 0.0;
      dkfdT=0.0;
      dkfdTv=0.0;
      add_to_dW_2r3p ( specN2, speceminus,   specN2plus, speceminus, speceminus,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    }
  
    if (REACTION[21]) {
      kf=exp ( -0.0102785 * sqr ( theta ) - 2.42260e-75 * pow ( theta, 46.0 ) );
      dkfdTe = 0.0;
      dkfdT=0.0;
      dkfdTv=0.0;
      add_to_dW_2r3p ( specO2, speceminus,   specO2plus, speceminus, speceminus,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    }

    if (REACTION[23]){
      kf=_kf_Arrhenius(2, 1.6e32, -3.3, 45172.0*R, Te);
      dkfdTe = _dkfdT_Arrhenius(2, 1.6e32, -3.3, 45172.0*R, Te);
      dkfdT=0.0;
      dkfdTv=0.0;      
      add_to_dW_2r3p ( specCs, speceminus,   specCsplus, speceminus, speceminus,    kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    }

  }


  

  if (REACTION[24]){
    kf=_kf_Arrhenius(2, 2e19, -1.0, 0.0, Te);
    dkfdTe=_dkfdT_Arrhenius(2, 2e19, -1.0, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;      
    add_to_dW_2r2p ( speceminus, specNOplus,   specN, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[25]){
    kf=2.0e-7*pow(300.0/Te,0.7);
    dkfdTe=-0.7*2.0e-7*pow(300.0,0.7)*pow(Te,-1.7);
    dkfdT=0.0;
    dkfdTv=0.0;      
    add_to_dW_2r2p ( speceminus, specO2plus,   specO, specO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[26]){
    kf=2.8e-7*pow(300.0/Te,0.5);
    dkfdTe=-0.5*2.8e-7*pow(300.0,0.5)*pow(Te,-1.5);
    dkfdT=0.0;
    dkfdTv=0.0;      
    add_to_dW_2r2p ( speceminus, specN2plus,   specN, specN,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[27]){
    kf=_kf_Arrhenius(2, 7.2e14, -0.67, 0.0, Te);
    dkfdTe=_dkfdT_Arrhenius(2, 7.2e14, -0.67, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;      
    add_to_dW_2r1p ( speceminus, specCsplus,   specCs,   kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[28]){
    kf=3.0e19/sqr(calA);
    dkfdT=0.0;
    dkfdTv=0.0;      
    dkfdTe=0.0;
    add_to_dW_3r2p ( speceminus, specCsplus, specO,   specCs, specO,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specCsplus, specO2,  specCs, specO2, kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specCsplus, specN,   specCs, specN,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specCsplus, specN2,  specCs, specN2, kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specCsplus, specNO,  specCs, specNO, kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[29]){
    kf=_kf_Arrhenius(3, 4e39, -4.5, 0.0, Te);
    dkfdTe=_dkfdT_Arrhenius(3, 4e39, -4.5, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;      
    add_to_dW_3r2p ( speceminus, specCsplus, speceminus,  specCs, speceminus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[30]){
    kf=7.2e8/calA;
    dkfdT=0.0;
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r1p ( speceminus, specO, specOminus,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[31]){
    kf=_kf_Arrhenius(3, 8.5e19, -0.5, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;
    dkfdTe=_dkfdT_Arrhenius(3, 8.5e19, -0.5, 0.0, Te);
    add_to_dW_3r2p ( speceminus, specO, specO,   specOminus, specO,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specO, specO2,   specOminus, specO2,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specO, specN,   specOminus, specN,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specO, specN2,   specOminus, specN2,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r2p ( speceminus, specO, specNO,   specOminus, specNO,  kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }


  if (REACTION[33]) {
    kf=1.4e-29 * 300.0 / Te * exp ( -600.0 / T ) * exp ( 700.0 * ( Te - T ) / Te / T );
    dkfdTe = ( -1.0 / Te + 700.0 / sqr ( Te ) ) * kf;
    dkfdT = ( 600.0 / sqr ( T ) - 700.0 / Te / T - 700.0 * ( Te - T ) / Te / sqr ( T ) ) * kf;
    dkfdTv=0.0;    
    add_to_dW_3r2p ( specO2, specO2, speceminus,  specO2, specO2minus,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
    
  if (REACTION[34]) {
    kf = 1.07e-31 * sqr ( 300.0 / Te ) * exp ( -70.0 / T ) * exp ( 1500.0 * ( Te - T ) / Te / T );
    dkfdTe = ( -2.0 / Te + 1500.0 / sqr ( Te ) ) * kf;
    dkfdT = ( 70.0 / sqr ( T ) - 1500.0 / sqr ( T ) ) * kf;
    dkfdTv=0.0;
    add_to_dW_3r2p ( specO2, specN2, speceminus,  specN2, specO2minus,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[35]){
    kf=_kf_Arrhenius(2, 1.4e12, 1.0, 17016.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.4e12, 1.0, 17016.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specOminus, specO,   specO, speceminus, specO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specOminus, specO2,   specO, speceminus, specO2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specOminus, specN,   specO, speceminus, specN,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specOminus, specN2,   specO, speceminus, specN2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r3p ( specOminus, specNO,   specO, speceminus, specNO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }


  if (REACTION[36]) {
    kf=2.0e-7 * pow ( 300.0 / T, 0.5 );
    dkfdTe = 0.0;
    dkfdT = -0.5 * 2.0e-7 * pow ( T / 300.0, -1.5 ) / 300.0 ;
    dkfdTv=0.0;
    add_to_dW_2r2p ( specN2plus, specO2minus,   specO2, specN2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r2p ( specO2plus, specO2minus,   specO2, specO2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_2r2p ( specNOplus, specO2minus,   specO2, specNO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
  
  if (REACTION[37]) {
    kf=2.0e-25 * pow ( 300.0 / T, 2.5 );
    dkfdTe = 0.0;
    dkfdT = -2.5 * kf / T ;
    dkfdTv=0.0;
    add_to_dW_3r3p ( specO2minus, specO2plus, specO,   specO2, specO2, specO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specN2plus, specO,   specO2, specN2, specO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specNOplus, specO,   specO2, specNO, specO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);

    add_to_dW_3r3p ( specO2minus, specO2plus, specO2,   specO2, specO2, specO2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specN2plus, specO2,   specO2, specN2, specO2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specNOplus, specO2,   specO2, specNO, specO2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);

    add_to_dW_3r3p ( specO2minus, specO2plus, specN,   specO2, specO2, specN,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specN2plus, specN,   specO2, specN2, specN,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specNOplus, specN,   specO2, specNO, specN,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);

    add_to_dW_3r3p ( specO2minus, specO2plus, specN2,   specO2, specO2, specN2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specN2plus, specN2,   specO2, specN2, specN2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specNOplus, specN2,   specO2, specNO, specN2,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);

    add_to_dW_3r3p ( specO2minus, specO2plus, specNO,   specO2, specO2, specNO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specN2plus, specNO,   specO2, specN2, specNO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
    add_to_dW_3r3p ( specO2minus, specNOplus, specNO,   specO2, specNO, specNO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[38]) {
    kf = 8.6e-10 * exp ( -6030.0 / T ) * ( 1.0 - exp ( -1570.0 / T ) );
    dkfdTe = 0.0;
    dkfdT =kf * 6030.0 / sqr ( T ) - 8.6e-10 * exp ( -6030.0 / T ) * exp ( -1570.0 / T ) * 1570.0 / sqr ( T );
    dkfdTv=0.0;
    add_to_dW_2r3p ( specO2, specO2minus,   specO2, specO2, speceminus,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }



  if (REACTION[39]){
    kf=_kf_Arrhenius(2, 1e17, 0.07, 39836.0*R, T);
    dkfdTe = 0.0;
    dkfdT = _dkfdT_Arrhenius(2, 1e17, 0.07, 39836.0*R, T);
    dkfdTv=0.0;
    add_to_dW_2r2p ( specCs, specO2,   specCsplus, specO2minus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[40]){
    kf=_kf_Arrhenius(2, 8.7e16, -0.15, 28177.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 8.7e16, -0.15, 28177.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specCs, specO,   specCsplus, specOminus,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }  
    
  if (REACTION[41]){
    kf=_kf_Arrhenius(2, 1.2e12, 1.18, 45172.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 1.2e12, 1.18, 45172.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r3p ( specCs, specO,    specCsplus, speceminus, specO,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);    
    add_to_dW_2r3p ( specCs, specO2,   specCsplus, speceminus, specO2,  kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);    
    add_to_dW_2r3p ( specCs, specN,    specCsplus, speceminus, specN,   kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);    
    add_to_dW_2r3p ( specCs, specN2,   specCsplus, speceminus, specN2,  kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);    
    add_to_dW_2r3p ( specCs, specNO,   specCsplus, speceminus, specNO,  kf, N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);    
  }

  if (REACTION[42]){
    kf=_kf_Arrhenius(2, 6.02e13, -0.5, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(2, 6.02e13, -0.5, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specCs, specNOplus,   specCsplus, specNO,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[43]){
    kf=2.0e17/calA;
    dkfdT=0.0;
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specCsplus, specO2minus,   specCs, specO2,    kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }
  
  if (REACTION[44]){
    kf=_kf_Arrhenius(2, 2.0e19, -0.6, 0.0, T);
    dkfdT=_dkfdT_Arrhenius(2, 2.0e19, -0.6, 0.0, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specCsplus, specOminus,   specCs, specO,    kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[45]){
    kf=_kf_Arrhenius(2, 8.07e11, 0.0, 61800.0*R, T);
    dkfdT=_dkfdT_Arrhenius(2, 8.07e11, 0.0, 61800.0*R, T);
    dkfdTv=0.0;
    dkfdTe=0.0;
    add_to_dW_2r2p ( specCsplus, specNO,   specCs, specNOplus,    kf  , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  
}



