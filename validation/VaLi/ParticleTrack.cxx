/*
 * ParticleTrack.cxx
 *
 *  Created on: May 9, 2011
 *      Author: fraenkle
 */

#include "ParticleTrack.h"
#include <math.h>
#include <iostream>

#include <TGraph2D.h>
#include <TCanvas.h>

ParticleTrack::ParticleTrack() {
	// TODO Auto-generated constructor stub

}

ParticleTrack::~ParticleTrack() {
	// TODO Auto-generated destructor stub
	for(int i=0;i<node.size();i++){
		delete node[i];
	}
	node.clear();
}

void ParticleTrack::AddNode(double x, double y, double z, double Ekin, double Edep){

	double *temp = new double[5];
	temp[0] = x;
	temp[1] = y;
	temp[2] = z;
	temp[3] = Ekin;
	temp[4] = Edep;

	node.push_back(temp);

}

double ParticleTrack::GetNetDisplacement(){

        double length = 0.0;
        double dl = 0.0;
	int first_i = 1; // first node index
        int last_i = node.size() - 1; // last node index
                dl = 0.0;
                for(int j=0;j<3;j++){
                        dl += (node[first_i][j]-node[last_i][j])*(node[first_i][j]-node[last_i][j]);
                }
                length += sqrt(dl);
        return length;
}

double ParticleTrack::GetTrackLength(){

	double length = 0.0;
	double dl = 0.0;
	for(int i=1;i<node.size();i++){
		dl = 0.0;
		for(int j=0;j<3;j++){
			dl += (node[i][j]-node[i-1][j])*(node[i][j]-node[i-1][j]);
		}
		length += sqrt(dl);
	}
	return length;
}

double ParticleTrack::GetEnergyDeposit(){

	double energy = 0.0;
	for(int i=0;i<node.size();i++)
		energy += node[i][4];

	return energy;
}

void ParticleTrack::Report(){

	std::cout << "number of nodes:\t" << node.size() << "\n";
	std::cout << "index\tx [mm]\ty [mm]\tz [mm]\tEkin [MeV]\tEdep [MeV]\n";
	for(int i=0;i<node.size();i++){
		std::cout << i+1 << ":\t";
		for(int j=0;j<5;j++){
			std::cout << node[i][j] << "\t";
		}
		std::cout << "\n";
	}
}

void ParticleTrack::PlotTrack(){

	TCanvas *can = new TCanvas();
	can->SetGrid();
	gr = new TGraph(node.size());
	grsec = new TGraph(node.size());

	for(int i=0;i<node.size();i++){
		gr->SetPoint(i,node[i][2],node[i][1]);

		if(node[i][4] > 0.0){
			grsec->SetPoint(i,node[i][2],node[i][1]);
		}
		//std::cout << "filling graph: " << i << "\t" << node[i][0] << "\t" << node[i][1] << "\t" << node[i][2] << "\n";
	}
	gr->SetMarkerStyle(20);
	gr->SetLineColor(kRed);
	gr->SetLineWidth(2.0);

	grsec->SetMarkerStyle(29);
	grsec->SetMarkerColor(kBlue);

	gr->Draw("APL");
	grsec->Draw("P");
}

double ParticleTrack::GetEndPositionX(){

	return node[node.size()-1][0];
}

double ParticleTrack::GetEndPositionY(){

	return node[node.size()-1][1];
}

double ParticleTrack::GetEndPositionZ(){

	return node[node.size()-1][2];
}

double ParticleTrack::GetFirstInteractionX(){

	return node[1][0];
}

double ParticleTrack::GetFirstInteractionY(){

	return node[1][1];
}

double ParticleTrack::GetFirstInteractionZ(){

	return node[1][2];
}

double ParticleTrack::GetStartPositionX(){

	return node[0][0];
}

double ParticleTrack::GetStartPositionY(){

	return node[0][1];
}

double ParticleTrack::GetStartPositionZ(){

	return node[0][2];
}

double ParticleTrack::GetPositionX(int stepNumber){

	if(stepNumber<node.size()){
		return node[stepNumber][0];
	}
	else{
		return 0.0;
	}
}

double ParticleTrack::GetPositionY(int stepNumber){

	if(stepNumber<node.size()){
		return node[stepNumber][1];
	}
	else{
		return 0.0;
	}
}

double ParticleTrack::GetPositionZ(int stepNumber){

	if(stepNumber<node.size()){
		return node[stepNumber][2];
	}
	else{
		return 0.0;
	}
}

double ParticleTrack::GetInitialEnergy(){

	return node[0][3];
}
