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

#include <math.h>

#include "Trajectoire.h"

#include <vector>

using namespace boost::posix_time;

void f(double *x, double *f, void *userData)
{
	
}



static double distancePoint(double theta1, double phi1, double theta2, double phi2)
{
	return acos(cos(theta1)*cos(theta2)+sin(theta1)*sin(theta2)*cos(phi2-phi1));
}

static int lastTrajPos = -1;

static double distanceTrajectoire(EtatSysteme& X)
{
	double theta = X.theta();
	double phi = X.phi();
	
	int indiceMin = -1;
	double distMin;
	
	int minTrajPos,maxTrajPos;
	
	if(lastTrajPos==-1)
	{
		minTrajPos=0;
		maxTrajPos=nbPointsTraj;
	}
	else
	{
		minTrajPos = (lastTrajPos - (nbPointsTraj/8) + nbPointsTraj)%nbPointsTraj;
		maxTrajPos = (lastTrajPos + ((3*nbPointsTraj)/8))%nbPointsTraj;
	}
	
	for(int i=minTrajPos ; i!=maxTrajPos ; i = (i+1)%nbPointsTraj)
	{
		double dist = distancePoint(theta,phi,traj[2*i],traj[2*i+1]);
		if(indiceMin==-1 || dist < distMin)
		{
			indiceMin = i;
		}
	}
	
	lastTrajPos = indiceMin;
	
	return distMin;
}

std::vector<CommandeMoteur> Asservissement::calculCommandeOptimale(EtatSysteme& etatInitial, ptime t0)
{
	USING_NAMESPACE_ACADO
	
	EtatSysteme X(7);
	Control u;
	
	DifferentialEquation f;
	
	f << dot(X) == X.derivee(u);
	
	OCP ocp(0.0,HORIZON);
	ocp.subjectTo(f);
	
	//COperator operatorDistance(
	
	ocp.minimizeLagrangeTerm(distanceTrajectoire(X));
	//ocp.subjectTo(AT_START, X == etatInitial);
	ocp.subjectTo( -V_ROTATION_MAX <= u <= V_ROTATION_MAX );
	
	OptimizationAlgorithm algorithm(ocp);
	
	algorithm.set(INTEGRATOR_TYPE, INT_RK45);
	algorithm.set(INTEGRATOR_TOLERANCE, 1e-8);
	algorithm.set(DISCRETIZATION_TYPE, MULTIPLE_SHOOTING);
	algorithm.set(KKT_TOLERANCE, 1e-4);
	
	algorithm.solve();
	
	VariablesGrid controls;
	
	algorithm.getControls(controls);
	
	std::vector<CommandeMoteur> res;
	
	for(int i=0 ; i<controls.getNumPoints() ; i++)
	{
		res.push_back(CommandeMoteur(t0+microseconds(1000000*controls(i,0)),controls(i,1)));
	}
	
	return res;
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