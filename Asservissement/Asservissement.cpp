//
//  Asservissement.cpp
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Asservissement.h"
#include "EtatSysteme.h"
#include "CommandeMoteur.h"

#include <acado_toolkit.hpp>
#include <acado/acado_gnuplot/gnuplot_window.hpp>



std::vector<CommandeMoteur> Asservissement::calculCommandeOptimale(EtatSysteme& etat)
{
	//USING_NAMESPACE_ACADO
	
	
}

/*

int main( ) {
	
    USING_NAMESPACE_ACADO
	
	
    // INTRODUCE THE VARIABLES:
    // -------------------------
    DifferentialState xB;
    DifferentialState xW;
    DifferentialState vB;
    DifferentialState vW;
	
    Disturbance R;
    Control F;
	
    Parameter mB;
    double mW = 50.0;
    double kS = 20000.0;
    double kT = 200000.0;
	
	
    // DEFINE THE DYNAMIC SYSTEM:
    // --------------------------
    DifferentialEquation f;
	
    f << dot(xB) == vB;
    f << dot(xW) == vW;
    f << dot(vB) == ( -kS*xB + kS*xW + F ) / mB;
    f << dot(vW) == (  kS*xB - (kT+kS)*xW + kT*R - F ) / mW;
	
    OutputFcn g;
    g << xB;
    g << 500.0*vB + F;
	
    DynamicSystem dynSys( f,g );
	
	
    // SETUP THE PROCESS:
    // ------------------
    Process myProcess;
	
    myProcess.setDynamicSystem( dynSys,INT_RK45 );
    myProcess.set( ABSOLUTE_TOLERANCE,1.0e-8 );
	
    Vector x0( 4 );
    x0.setZero( );
    x0( 0 ) = 0.01;
	
    myProcess.initializeStartValues( x0 );
    myProcess.setProcessDisturbance( "road.txt" );
	
    myProcess.set( PLOT_RESOLUTION,HIGH );
	
    GnuplotWindow window;
	window.addSubplot( xB, "Body Position [m]" );
	window.addSubplot( xW, "Wheel Position [m]" );
	window.addSubplot( vB, "Body Velocity [m/s]" );
	window.addSubplot( vW, "Wheel Velocity [m/s]" );
	
	window.addSubplot( F,"Damping Force [N]" );
	window.addSubplot( mB,"Body Mass [kg]" );
	window.addSubplot( R, "Road Disturbance" );
	window.addSubplot( g(0),"Output 1" );
	window.addSubplot( g(1),"Output 2" );
	
    myProcess << window;
	
	
    // SIMULATE AND GET THE RESULTS:
    // -----------------------------
    VariablesGrid u( 1,0.0,1.0,6 );
	
    u( 0,0 ) = 10.0;
    u( 1,0 ) = -200.0;
    u( 2,0 ) = 200.0;
    u( 3,0 ) = 0.0;
    u( 4,0 ) = 0.0;
    u( 5,0 ) = 0.0;
	
    Vector p( 1 );
    p(0) = 350.0;
	
    myProcess.init( 0.0 );
    myProcess.run( u,p );
	
	
    VariablesGrid xSim, ySim;
	
    myProcess.getLast( LOG_SIMULATED_DIFFERENTIAL_STATES,xSim );
    xSim.print( "Simulated Differential States" );
	
    myProcess.getLast( LOG_PROCESS_OUTPUT,ySim );
    ySim.print( "Process Output" );
	
    return 0;
} */