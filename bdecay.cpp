//********************************************************************
// Toy Monte Carlo Simulation of the beta decay detector
//
// To run, do "root -l bdecay.cpp"
// Make sure you set up your Xterminal so that the graph can be displayed on your screen
//********************************************************************

// C++ libs
#include <iostream>
#include<stdlib.h>	//rand, srand
#include<cmath>

// ROOT libs
#include<TH1D.h>
#include<TMath.h>
#include<TRandom3>

using namespace std;

void bdecay(){
	gStyle->SetOptStat("nemr");	// Makes statistics box appear automatically in histograms

	// Simulation parameters
	TRandom3 *rand = new TRandom3();	// Generate a random number generator for TRandom3
	const int nevents=5e6;			// Number of events to generate
	const double Pi = 3.14159265;	// Pi
	int ndivisions = 40;		// Number of divisions in energy histograms

	float m_A = 14.00341*931.5;	// Mass (in MeV) of initial particle at rest.
	float m_B = 14.00307*931.5;	// Mass (in MeV) of final particle. 
	float Q= m_A-m_B;	// Q-value of our experiment (Taken from Krane, eq. 9.6)
	float m_e = 0.511;	// Mass (in MeV) of electron
	float m_nu= 0.2e-6;	// Mass (in MEV) of neutrino

	// ROOT Histograms
	TH1D *p_p = new TH1D("p_p", ";p_{p} [MeV];Intensity", ndivisions, 0, Q);	// Momentum histogram for proton
	TH1D *p_e = new TH1D("p_e", ";p_{e} [MeV];Intensity", ndivisions, 0, Q);	// Momentum histogram for electron
	TH1D *p_nu = new TH1D("p_nu",";p_{#nu_{e}} [MeV];Intensity", ndivisions, 0, Q);	// Momentum histogram for neutrino
	TH1D *p_total = new TH1D("p_total", ";p_{total} [MeV];Intensity", ndivisions, 0.5*Q, 1.5*Q);	// Total momentum histogram

	TH1D *E_p = new TH1D("E_p", ";E_{p} [MeV];Intensity", ndivisions, 0, Q);	// Energy histogram for proton
	TH1D *E_e = new TH1D("E_e", ";E_{e} [MeV];Intensity", ndivisions, 0, Q);	// Energy histogram for electron
	TH1D *E_nu = new TH1D("E_nu",";E_{#nu_{e}} [MeV];Intensity", ndivisions, 0, Q);	// Energy histogram for neutrino
	TH1D *E_total = new TH1D("E_total", ";E_{total} [MeV];Intensity", ndivisions, 0.5*Q, 1.5*Q);	// Total energy histogram

	int counter=0;	// Counter for the while loop
	cout << "(Total: 1e" << log10(1.*nevents) << " events)\n";
	cout << "Q = " << Q << " MeV\n";
	while (counter < nevents) {
		//double r1 = rand->Uniform(1);	// Number between 0 and 1
		double r1 = 0;	// We choose it to be 0, as we want to neglect the recoil energy of the nucleus 
		double r2 = rand->Uniform(1);	// Number between 0 and 1
		if (r1+r2 <= 1) {	// We make sure that r1+r2 is not too big so that p_nu is not negative
			// Atribute randomly generated energy to each particle
			float K1 = r1*Q;	// Kinetic energy of proton (we neglect recoil)
			float K2 = r2*Q;	// Kinetic energy of electron
			float K3 = Q*(1-r1-r2);	// Kinetic energy of neutrino

			float p1 = sqrt(2*K1*m_B);	// proton momentum
			float p2 = sqrt(2*K2*m_e);	// electron momentum
			float p3 = sqrt(2*K3*m_nu);	// neutrino momentum

			//double angle1 = rand->Uniform(0,2*Pi);	// Get random angle in radians for proton
			//double angle2 = -1;	// I would like to write an expression that uses conservation of momentum to find angle2 from angle1 and angle3
			//double angle3 = rand->Uniform(0,2*Pi);	// Get random angle in radians for neutrino

			//float E1 = sqrt(pow(m_B,2)+pow(p1,2));	// proton
			//float E3 = sqrt(pow(m_nu,2)+pow(p3,2));	// neutrino
			//float E2 = 1./(2*m_A)*(pow(m_A,2) + pow(m_e,2) - pow(m_B,2) - pow(m_nu,2) - 2*E1*E3 + 2*p1*p3*cos(abs(angle1-angle3)));	//electron, using quadri-impulsion conservation (particle A supposed at rest)

			p_p->Fill(p1);	// Fill histogram p_p using momentum p1
			p_e->Fill(p2);	// Fill histogram p_e using momentum p2
			p_nu->Fill(p3);	// Fill histogram p_nu using momentum p3
			p_total->Fill(p1+p2+p3);	// Fill histogram of total momentum

			E_p->Fill(K1);	// Fill histogram E_p using energy E1
			E_e->Fill(K2);	// Fill histogram E_p using energy E2
			E_nu->Fill(K3);	// Fill histogram E_p using energy E3
			E_total->Fill(K1+K2+K3);	// Fill histogram of total energy

			
			// For execution purposes, acts as a "progress bar"
			if (!(++counter % (nevents/10))) {	// Add 1 to counter and take its modulo
				cout << "Current progress: " << 100.*counter/nevents << "%"<< endl;
			}
		}
	}

	//TCanvas *c0=new TCanvas("p_total","p_total");
	//p_total->SetFillColor(6);	//magenta
	//p_total->Draw();

	TCanvas *c1=new TCanvas("p_p", "p_p");
	p_p->SetFillColor(2);	//red
	p_p->Draw();

	TCanvas *c2=new TCanvas("p_e","p_e");
	p_e->SetFillColor(4);	//blue
	p_e->Draw();

	TCanvas *c3=new TCanvas("p_nu","p_nu");
	p_nu->SetFillColor(3);	//green
	p_nu->Draw();

//	TCanvas *c4=new TCanvas("All_momentum_spectrums","All_momentum_spectrums");
//	p_p->Draw("");
//	p_e->Draw("same");
//	p_nu->Draw("same");

	TCanvas *c00=new TCanvas("E_total","E_total");
	E_total->SetFillColor(8);	//ugly green
	E_total->Draw();

	TCanvas *c5=new TCanvas("E_p", "E_p");
	E_p->SetFillColor(2);	//red
	E_p->Draw();

	TCanvas *c6=new TCanvas("E_e","E_e");
	E_e->SetFillColor(4);	//blue
	E_e->Draw();

	TCanvas *c7=new TCanvas("E_nu","E_nu");
	E_nu->SetFillColor(3);	//green
	E_nu->Draw();

//	TCanvas *c8=new TCanvas("All_energy_spectrums","All_energy_spectrums");
//	E_p->Draw("");
//	E_e->Draw("same");
//	E_nu->Draw("same");
}
