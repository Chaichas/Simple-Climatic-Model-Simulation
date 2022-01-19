#include <math.h>
#include <stdio.h>

#define real float

// Solar constant W / m ^ 2
#define S0 1370

// Stefan - Boltzmann constant W / m2 / K4
#define SIGMA 0.00000005670367

// Temperature inertia (in years)
#define THETA 100.0

// Albedo, in this simulation albedo is considered constant
// in reality albedo can become lower with increased temperatures
// due to ice cap melting
#define ALBEDO 0.33

// Initial values

// Simulation starts in 2007
static const real t0 = 2007.0;
// Temperaturature in 2007 in K
static const real T0 = 288.45;
// CO2 concentration in 2007 in ppm
static const real CO20 = 370.0;

// Greenhouse gaz fraction
static real G(real T, real co2)
{
    return 3.35071874e-03 * T + 3.20986702e-05 * co2 - 0.561593690144655;
}

//Puissance Entrante
static real P_in(void){
    return (1-ALBEDO)*(S0/4);
}

//Puissance Sortante
static real P_out(real t, real T){
    real co2= CO20; //Initialisation: 2007
    //real taux= 0.6; //variation de Cco2 en ppm, modele RCP2.6
    //real taux= 3; //variation de Cco2 en ppm, modele RCP6.0
    //real co2_i= co2+taux*(t-t0);
    //printf("Co2: %f", co2_i);
    //printf("\n");
    //printf("t: %f", t);
    //printf("\n");
    return (1-G(T,co2))*SIGMA*pow(T,4);
}

//Difference de Temperature
static real F(real t, real T){
    return (P_in()-P_out(t, T))*t/100;
}

//Integration Temporelle: Euler Explicite
static real euler(real t_final, int steps){
    real T=T0;
    real t=t0;
    
    //t=t+(t_final-t0)/steps;
    T=T+F(t_final,T)*(t_final-t0)/steps;

    return T;
}

static real errors(real t, real steps)
{  
    real m;
        m= euler(t, steps);
    return (fabs(m-288.881466)/288.881466);
}

int main(int argc, char **argv)
{
     //Temperature
    real steps= 65536;
    real t_final= 2107.0;
        
    printf("Simple climate simulation\n");
    FILE *imprim0;
    imprim0=fopen("output.dat","wt");

    for (real t=t0; t<=t_final; t++){
        printf("Temperature T: %f , ",euler(t,steps));
        printf("Year t: %f",t);
        printf("\n");

        errors(t, steps); //fonction errors
        
        fprintf(imprim0,"%f %f \n",t,euler(t,steps));
    }
    fclose(imprim0);

//errors

FILE *imprim;
imprim=fopen("errors.dat","w");

for(real i=16; i<=steps; i*=2){

    printf("errors, steps: %f %f \n",i,errors(t_final, i));
    fprintf(imprim,"%f %f \n",i,errors(t_final, i));
    }
    fclose(imprim);
    }
//-lm apres fichier source utilisant les codes math. preferably at the end